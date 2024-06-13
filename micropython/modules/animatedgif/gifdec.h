#include "py/runtime.h"
#include "py/objstr.h"

extern const mp_obj_type_t GIF_type;

extern uint8_t current_flags;

extern mp_obj_t _GIF_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
extern mp_obj_t _GIF_del(mp_obj_t self_in);
extern mp_obj_t _GIF_openRAM(mp_obj_t self_in, mp_obj_t buffer);
extern mp_obj_t _GIF_openFILE(mp_obj_t self_in, mp_obj_t filename);
extern mp_obj_t _GIF_decode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
extern mp_obj_t _GIF_getWidth(mp_obj_t self_in);
extern mp_obj_t _GIF_getHeight(mp_obj_t self_in);
extern mp_obj_t _GIF_getLoopCount(mp_obj_t self_in);
extern mp_obj_t _GIF_playFrame(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
