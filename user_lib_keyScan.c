#include "user_lib_keyScan.h"

void USER_LIB_KEYSCAN_void_key_str_init(KEY_STATE_STR *str, int32_t key_active_level, int32_t keyScan_freq)
{
  str->keystate.pad = 0;
  str->keyEvent_flag = 0;
  str->key_pressed_cont = 0;
  str->key_active_level = key_active_level;
  str->keyScan_freq = keyScan_freq;
  str->cnt_threshold_pressed_once = keyScan_freq / 100;      // 10ms为短按
  str->cnt_threshold_pressed_continious = keyScan_freq / 10; // 100ms为长按
}

int32_t USER_LIB_KEYSCAN_int32_t_keyScan(int32_t input, KEY_STATE_STR *str)
{
  if (input == str->key_active_level)
  {
    str->key_pressed_cont++;
    if (str->key_pressed_cont >= str->cnt_threshold_pressed_once)
    {
      str->key_pressed_cont = str->cnt_threshold_pressed_once;
      str->keyEvent_flag = 1;
      str->keystate.enums = KEY_IS_PRESSED_CONTINIOUS;
    }
    else if (str->key_pressed_cont >= str->cnt_threshold_pressed_once)
    {
      str->keyEvent_flag = 1;
      str->keystate.enums = KEY_IS_PRESSED_ONCE;
    }
    else
    {
      str->keystate.enums = KEY_IS_NOT_PRESSED;
    }
  }
  else if (str->keyEvent_flag == 1)
  {
    // 按键事件没有得到响应，即使松手，按键仍为按下的状态
    // 只是按键计数器归零
    str->key_pressed_cont = 0;
  }
  else
  {
    str->key_pressed_cont = 0;
    str->keystate.enums = KEY_IS_NOT_PRESSED;
  }
  return str->keystate.enums;
}