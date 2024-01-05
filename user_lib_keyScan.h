#ifndef USER_LIB_KEY_SCAN_H
#define USER_LIB_KEY_SCAN_H

#include "stdint.h"

typedef union
{
    enum
    {
        KEY_IS_NOT_PRESSED = 0,
        KEY_IS_PRESSED_ONCE = 1,
        KEY_IS_PRESSED_CONTINIOUS = 2,
    } enums;
    int32_t pad;
} KEY_STATE_UNION;

typedef struct
{
    int32_t key_active_level;
    int32_t key_pressed_cont;
    int32_t cnt_threshold_pressed_once;
    int32_t cnt_threshold_pressed_continious;
    KEY_STATE_UNION keystate;
    int32_t keyEvent_flag;
    int32_t keyScan_freq;
} KEY_STATE_STR;

extern void USER_LIB_KEYSCAN_void_key_str_init(KEY_STATE_STR *str, int32_t key_active_level, int32_t keyScan_freq);
extern int32_t USER_LIB_KEYSCAN_int32_t_keyScan(int32_t input, KEY_STATE_STR *str);

#endif