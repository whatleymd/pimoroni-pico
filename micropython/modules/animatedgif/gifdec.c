#include "gifdec.h"

// Define MicroPython function objects for GIF methods
static MP_DEFINE_CONST_FUN_OBJ_1(GIF_del_obj, _GIF_del);
static MP_DEFINE_CONST_FUN_OBJ_2(GIF_openRAM_obj, _GIF_openRAM);
static MP_DEFINE_CONST_FUN_OBJ_2(GIF_openFILE_obj, _GIF_openFILE);
static MP_DEFINE_CONST_FUN_OBJ_KW(GIF_decode_obj, 1, _GIF_decode);
static MP_DEFINE_CONST_FUN_OBJ_1(GIF_getWidth_obj, _GIF_getWidth);
static MP_DEFINE_CONST_FUN_OBJ_1(GIF_getHeight_obj, _GIF_getHeight);
static MP_DEFINE_CONST_FUN_OBJ_1(GIF_getLoopCount_obj, _GIF_getLoopCount);
static MP_DEFINE_CONST_FUN_OBJ_KW(GIF_playFrame_obj, 1, _GIF_playFrame);

// Define the dictionary of methods for the GIF class
static const mp_rom_map_elem_t GIF_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&GIF_del_obj) },
    { MP_ROM_QSTR(MP_QSTR_open_RAM), MP_ROM_PTR(&GIF_openRAM_obj) },
    { MP_ROM_QSTR(MP_QSTR_open_file), MP_ROM_PTR(&GIF_openFILE_obj) },
    { MP_ROM_QSTR(MP_QSTR_decode), MP_ROM_PTR(&GIF_decode_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_width), MP_ROM_PTR(&GIF_getWidth_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_height), MP_ROM_PTR(&GIF_getHeight_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_loop_count), MP_ROM_PTR(&GIF_getLoopCount_obj) },
    { MP_ROM_QSTR(MP_QSTR_play_frame), MP_ROM_PTR(&GIF_playFrame_obj) },
};

static MP_DEFINE_CONST_DICT(GIF_locals_dict, GIF_locals_dict_table);

// Define the GIF type
#ifdef MP_DEFINE_CONST_OBJ_TYPE
MP_DEFINE_CONST_OBJ_TYPE(
    GIF_type,
    MP_QSTR_gifdec,
    MP_TYPE_FLAG_NONE,
    make_new, _GIF_make_new,
    locals_dict, (mp_obj_dict_t*)&GIF_locals_dict
);
#else
const mp_obj_type_t GIF_type = {
    { &mp_type_type },
    .name = MP_QSTR_gifdec,
    .make_new = _GIF_make_new,
    .locals_dict = (mp_obj_dict_t*)&GIF_locals_dict,
};
#endif

// Define the module globals
static const mp_map_elem_t GIF_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_gifdec) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_GIF), (mp_obj_t)&GIF_type },
    { MP_ROM_QSTR(MP_QSTR_GIF_SCALE_FULL), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_GIF_SCALE_HALF), MP_ROM_INT(2) },
    { MP_ROM_QSTR(MP_QSTR_GIF_SCALE_QUARTER), MP_ROM_INT(4) },
    { MP_ROM_QSTR(MP_QSTR_GIF_SCALE_EIGHTH), MP_ROM_INT(8) },
};

static MP_DEFINE_CONST_DICT(mp_module_GIF_globals, GIF_globals_table);

const mp_obj_module_t GIF_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_GIF_globals,
};

// Register the module
#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_gifdec, GIF_user_cmodule, MODULE_GIFDEC_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_gifdec, GIF_user_cmodule);
#endif
