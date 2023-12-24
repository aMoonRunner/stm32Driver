#include "user_lib_keyScan.h"

void USER_LIB_KEYSCAN_void_key_str_init(KEY_STATE_STR *str)
{
  str->keystate.pad = 0;
  str->keyEvent_flag = 0;
  str->key_pressed_cont = 0;
  str->key_active_level = 0;
  str->keyScan_freq = 1000;
}

int32_t USER_LIB_KEYSCAN_int32_t_keyScan(int32_t input, KEY_STATE_STR *str)
{

  if (input == str->key_active_level)
  {
    str->key_pressed_cont++;
    if (str->key_pressed_cont >= 100)
    {
      str->key_pressed_cont = 100;
      str->keyEvent_flag = 1;
      str->keystate.enum_keyState = KEY_IS_PRESSED_CONTINIOUS;
    }
    else if (str->key_pressed_cont >= 10)
    {
      str->keyEvent_flag = 1;
      str->keystate.enum_keyState = KEY_IS_PRESSED_ONCE;
    }
    else
    {
      str->keystate.enum_keyState = KEY_IS_NOT_PRESSED;
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
    str->keystate.enum_keyState = KEY_IS_NOT_PRESSED;
  }

  return str->keystate.enum_keyState;
}