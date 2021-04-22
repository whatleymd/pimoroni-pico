#include "breakout_as7262.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// BreakoutAS7262 Class
////////////////////////////////////////////////////////////////////////////////////////////////////

/***** Methods *****/
MP_DEFINE_CONST_FUN_OBJ_1(BreakoutAS7262_reset_obj, BreakoutAS7262_reset);
//MP_DEFINE_CONST_FUN_OBJ_1(BreakoutAS7262_device_type_obj, BreakoutAS7262_device_type); //TODO
//MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_hardware_version_obj, 2, BreakoutAS7262_hardware_version); //TODO
MP_DEFINE_CONST_FUN_OBJ_1(BreakoutAS7262_firmware_version_obj, BreakoutAS7262_firmware_version);
MP_DEFINE_CONST_FUN_OBJ_1(BreakoutAS7262_read_obj, BreakoutAS7262_read);
MP_DEFINE_CONST_FUN_OBJ_1(BreakoutAS7262_temperature_obj, BreakoutAS7262_temperature);
MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_set_gain_obj, 1, BreakoutAS7262_set_gain);
MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_set_measurement_mode_obj, 1, BreakoutAS7262_set_measurement_mode);
MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_set_indicator_current_obj, 1, BreakoutAS7262_set_indicator_current);
MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_set_illumination_current_obj, 1, BreakoutAS7262_set_illumination_current);
MP_DEFINE_CONST_FUN_OBJ_KW(BreakoutAS7262_set_leds_obj, 1, BreakoutAS7262_set_leds);

/***** Binding of Methods *****/
STATIC const mp_rom_map_elem_t BreakoutAS7262_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&BreakoutAS7262_reset_obj) },
    //{ MP_ROM_QSTR(MP_QSTR_device_type), MP_ROM_PTR(&BreakoutAS7262_device_type_obj) },
    //{ MP_ROM_QSTR(MP_QSTR_hardware_version), MP_ROM_PTR(&BreakoutAS7262_hardware_version_obj) },
    { MP_ROM_QSTR(MP_QSTR_firmware_version), MP_ROM_PTR(&BreakoutAS7262_firmware_version_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&BreakoutAS7262_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_temperature), MP_ROM_PTR(&BreakoutAS7262_temperature_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_gain), MP_ROM_PTR(&BreakoutAS7262_set_gain_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_measurement_mode), MP_ROM_PTR(&BreakoutAS7262_set_measurement_mode_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_indicator_current), MP_ROM_PTR(&BreakoutAS7262_set_indicator_current_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_illumination_current), MP_ROM_PTR(&BreakoutAS7262_set_illumination_current_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_leds), MP_ROM_PTR(&BreakoutAS7262_set_leds_obj) },
};
STATIC MP_DEFINE_CONST_DICT(BreakoutAS7262_locals_dict, BreakoutAS7262_locals_dict_table);

/***** Class Definition *****/
const mp_obj_type_t breakout_as7262_BreakoutAS7262_type = {
    { &mp_type_type },
    .name = MP_QSTR_breakout_matrix11x7,
    .print = BreakoutAS7262_print,
    .make_new = BreakoutAS7262_make_new,
    .locals_dict = (mp_obj_dict_t*)&BreakoutAS7262_locals_dict,
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// breakout_as7262 Module
////////////////////////////////////////////////////////////////////////////////////////////////////

/***** Globals Table *****/
STATIC const mp_map_elem_t breakout_as7262_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_breakout_as7262) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_BreakoutAS7262), (mp_obj_t)&breakout_as7262_BreakoutAS7262_type },
};
STATIC MP_DEFINE_CONST_DICT(mp_module_breakout_as7262_globals, breakout_as7262_globals_table);

/***** Module Definition *****/
const mp_obj_module_t breakout_as7262_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_breakout_as7262_globals,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
MP_REGISTER_MODULE(MP_QSTR_breakout_as7262, breakout_as7262_user_cmodule, MODULE_BREAKOUT_AS7262_ENABLED);
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////