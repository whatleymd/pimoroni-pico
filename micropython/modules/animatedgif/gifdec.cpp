#include "libraries/animatedgif/AnimatedGIF.h"
#include "micropython/modules/util.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

using namespace pimoroni;

extern "C" {
#include "gifdec.h"
#include "micropython/modules/picographics/picographics.h"
#include "py/stream.h"
#include "py/reader.h"
#include "extmod/vfs.h"

typedef struct _ModPicoGraphics_obj_t {
    mp_obj_base_t base;
    PicoGraphics *graphics;
    DisplayDriver *display;
    void *buffer;
} ModPicoGraphics_obj_t;

typedef struct _GIF_obj_t {
    mp_obj_base_t base;
    AnimatedGIF *gif;
    void *dither_buffer;
    mp_obj_t file;
    mp_buffer_info_t buf;
    ModPicoGraphics_obj_t *graphics;
    int width;
    int height;
    int loop_count;
} _GIF_obj_t;

uint8_t current_flags = 0;

enum FLAGS : uint8_t {
    FLAG_NO_DITHER = 1u
};

// Callbacks for GIF decoding
void *gifdec_open_callback(const char *filename, int32_t *size) {
    mp_obj_t fn = mp_obj_new_str(filename, (mp_uint_t)strlen(filename));
    mp_obj_t args[2] = { fn, MP_OBJ_NEW_QSTR(MP_QSTR_r) };
    mp_obj_t stat = mp_vfs_stat(fn);
    mp_obj_tuple_t *tuple = MP_OBJ_TO_PTR2(stat, mp_obj_tuple_t);
    *size = mp_obj_get_int(tuple->items[6]);
    mp_obj_t fhandle = mp_vfs_open(MP_ARRAY_SIZE(args), &args[0], (mp_map_t *)&mp_const_empty_map);
    return (void *)fhandle;
}

void gifdec_close_callback(void *handle) {
    mp_stream_close((mp_obj_t)handle);
}

int32_t gifdec_read_callback(GIFFILE *gif, uint8_t *p, int32_t c) {
    mp_obj_t fhandle = gif->fHandle;
    int error;
    return mp_stream_read_exactly(fhandle, p, c, &error);
}

int32_t gifdec_seek_callback(GIFFILE *gif, int32_t p) {
    mp_obj_t fhandle = gif->fHandle;
    struct mp_stream_seek_t seek_s;
    seek_s.offset = p;
    seek_s.whence = SEEK_SET;
    const mp_stream_p_t *stream_p = mp_get_stream(fhandle);
    int error;
    mp_uint_t res = stream_p->ioctl(fhandle, MP_STREAM_SEEK, (mp_uint_t)(uintptr_t)&seek_s, &error);
    if (res == MP_STREAM_ERROR) {
        mp_raise_OSError(error);
    }
    return seek_s.offset;
}

void GIFDraw(GIFDRAW *pDraw) {
#ifdef mp_event_handle_nowait
mp_event_handle_nowait();
#endif
    PicoGraphics *current_graphics = (PicoGraphics *)pDraw->pUser;
    // Implement drawing logic for GIF frames here, similar to JPEG example
    // ...

    return 1;
}

// Helper function to open GIF
void gifdec_open_helper(_GIF_obj_t *self) {
    int result = -1;
    if(mp_obj_is_str(self->file)){
        GET_STR_DATA_LEN(self->file, str, str_len);
        result = self->gif->open((const char*)str, gifdec_open_callback, gifdec_close_callback, gifdec_read_callback, gifdec_seek_callback, GIFDraw);
    } else {
        mp_get_buffer_raise(self->file, &self->buf, MP_BUFFER_READ);
        result = self->gif->open((uint8_t *)self->buf.buf, self->buf.len, GIFDraw);
    }
    if(result != 1) mp_raise_msg(&mp_type_RuntimeError, "GIF: could not read file/buffer.");
}

// MicroPython binding for creating a new GIF object
mp_obj_t _GIF_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_picographics };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_picographics, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    if(!MP_OBJ_IS_TYPE(args[ARG_picographics].u_obj, &ModPicoGraphics_type)) mp_raise_ValueError(MP_ERROR_TEXT("PicoGraphics Object Required"));
    _GIF_obj_t *self = mp_obj_malloc_with_finaliser(_GIF_obj_t, &GIF_type);
    self->gif = m_new_class(AnimatedGIF);
    self->graphics = (ModPicoGraphics_obj_t *)MP_OBJ_TO_PTR(args[ARG_picographics].u_obj);
    return self;
}

// MicroPython binding for deleting a GIF object
mp_obj_t _GIF_del(mp_obj_t self_in) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    self->gif->close();
    return mp_const_none;
}

// MicroPython binding for opening a GIF from a file
mp_obj_t _GIF_openFILE(mp_obj_t self_in, mp_obj_t filename) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    self->file = filename;
    gifdec_open_helper(self);
    self->width = self->gif->getCanvasWidth();
    self->height = self->gif->getCanvasHeight();
    self->loop_count = self->gif->getLoopCount();
    self->gif->close();
    return mp_const_true;
}

// MicroPython binding for opening a GIF from RAM
mp_obj_t _GIF_openRAM(mp_obj_t self_in, mp_obj_t buffer) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    self->file = buffer;
    gifdec_open_helper(self);
    self->width = self->gif->getCanvasWidth();
    self->height = self->gif->getCanvasHeight();
    self->loop_count = self->gif->getLoopCount();
    self->gif->close();
    return mp_const_true;
}

// MicroPython binding for decoding a GIF frame
mp_obj_t _GIF_decode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_x, ARG_y, ARG_scale, ARG_dither };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_x, MP_ARG_INT, {.u_int = 0}  },
        { MP_QSTR_y, MP_ARG_INT, {.u_int = 0}  },
        { MP_QSTR_scale, MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_dither, MP_ARG_OBJ, {.u_obj = mp_const_true} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, _GIF_obj_t);
    int x = args[ARG_x].u_int;
    int y = args[ARG_y].u_int;
    int scale = args[ARG_scale].u_int;
    current_flags = args[ARG_dither].u_obj == mp_const_false ? FLAG_NO_DITHER : 0;
    gifdec_open_helper(self);
    self->gif->playFrame(true, NULL, self->graphics->graphics);
    current_flags = 0;
    self->gif->close();
    return mp_const_true;
}

// MicroPython binding for getting the width of a GIF
mp_obj_t _GIF_getWidth(mp_obj_t self_in) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    return mp_obj_new_int(self->width);
}

// MicroPython binding for getting the height of a GIF
mp_obj_t _GIF_getHeight(mp_obj_t self_in) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    return mp_obj_new_int(self->height);
}

// MicroPython binding for getting the loop count of a GIF
mp_obj_t _GIF_getLoopCount(mp_obj_t self_in) {
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(self_in, _GIF_obj_t);
    return mp_obj_new_int(self->loop_count);
}

// MicroPython binding for playing a GIF frame
mp_obj_t _GIF_playFrame(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_sync };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_sync, MP_ARG_OBJ, {.u_obj = mp_const_true} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    _GIF_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, _GIF_obj_t);
    bool sync = args[ARG_sync].u_obj == mp_const_true;
    int delayMilliseconds;
    int result = self->gif->playFrame(sync, &delayMilliseconds, self->graphics->graphics);
    return mp_obj_new_int(result);
}

}
