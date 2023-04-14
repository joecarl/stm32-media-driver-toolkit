#ifndef MDT_USB_INPUT_H
#define MDT_USB_INPUT_H

#include <stdbool.h>
#if __has_include("usbh_hid.h")
#include "usbh_def.h"
#define USB_SUPPORT_ENABLED
#endif

#include "usb_input_events.h"

typedef struct {
	uint8_t state;
	uint8_t lctrl;
	uint8_t lshift;
	uint8_t lalt;
	uint8_t lgui;
	uint8_t rctrl;
	uint8_t rshift;
	uint8_t ralt;
	uint8_t rgui;
	uint8_t keys[6];
} MDT_USB_INPUT_KeybdInfo;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t buttons[3];
} MDT_USB_INPUT_MouseInfo;

#define MDT_KEY_NONE                               0x00
#define MDT_KEY_ERRORROLLOVER                      0x01
#define MDT_KEY_POSTFAIL                           0x02
#define MDT_KEY_ERRORUNDEFINED                     0x03
#define MDT_KEY_A                                  0x04
#define MDT_KEY_B                                  0x05
#define MDT_KEY_C                                  0x06
#define MDT_KEY_D                                  0x07
#define MDT_KEY_E                                  0x08
#define MDT_KEY_F                                  0x09
#define MDT_KEY_G                                  0x0A
#define MDT_KEY_H                                  0x0B
#define MDT_KEY_I                                  0x0C
#define MDT_KEY_J                                  0x0D
#define MDT_KEY_K                                  0x0E
#define MDT_KEY_L                                  0x0F
#define MDT_KEY_M                                  0x10
#define MDT_KEY_N                                  0x11
#define MDT_KEY_O                                  0x12
#define MDT_KEY_P                                  0x13
#define MDT_KEY_Q                                  0x14
#define MDT_KEY_R                                  0x15
#define MDT_KEY_S                                  0x16
#define MDT_KEY_T                                  0x17
#define MDT_KEY_U                                  0x18
#define MDT_KEY_V                                  0x19
#define MDT_KEY_W                                  0x1A
#define MDT_KEY_X                                  0x1B
#define MDT_KEY_Y                                  0x1C
#define MDT_KEY_Z                                  0x1D
#define MDT_KEY_1_EXCLAMATION_MARK                 0x1E
#define MDT_KEY_2_AT                               0x1F
#define MDT_KEY_3_NUMBER_SIGN                      0x20
#define MDT_KEY_4_DOLLAR                           0x21
#define MDT_KEY_5_PERCENT                          0x22
#define MDT_KEY_6_CARET                            0x23
#define MDT_KEY_7_AMPERSAND                        0x24
#define MDT_KEY_8_ASTERISK                         0x25
#define MDT_KEY_9_OPARENTHESIS                     0x26
#define MDT_KEY_0_CPARENTHESIS                     0x27

#define MDT_KEY_1                                  0x1E
#define MDT_KEY_2                                  0x1F
#define MDT_KEY_3                                  0x20
#define MDT_KEY_4                                  0x21
#define MDT_KEY_5                                  0x22
#define MDT_KEY_6                                  0x23
#define MDT_KEY_7                                  0x24
#define MDT_KEY_8                                  0x25
#define MDT_KEY_9                                  0x26
#define MDT_KEY_0                                  0x27

#define MDT_KEY_ENTER                              0x28
#define MDT_KEY_ESCAPE                             0x29
#define MDT_KEY_BACKSPACE                          0x2A
#define MDT_KEY_TAB                                0x2B
#define MDT_KEY_SPACEBAR                           0x2C
#define MDT_KEY_MINUS_UNDERSCORE                   0x2D
#define MDT_KEY_EQUAL_PLUS                         0x2E
#define MDT_KEY_OBRACKET_AND_OBRACE                0x2F
#define MDT_KEY_CBRACKET_AND_CBRACE                0x30
#define MDT_KEY_BACKSLASH_VERTICAL_BAR             0x31
#define MDT_KEY_NONUS_NUMBER_SIGN_TILDE            0x32
#define MDT_KEY_SEMICOLON_COLON                    0x33
#define MDT_KEY_SINGLE_AND_DOUBLE_QUOTE            0x34
#define MDT_KEY_GRAVE_ACCENT_AND_TILDE             0x35
#define MDT_KEY_COMMA_AND_LESS                     0x36
#define MDT_KEY_DOT_GREATER                        0x37
#define MDT_KEY_SLASH_QUESTION                     0x38
#define MDT_KEY_CAPS_LOCK                          0x39
#define MDT_KEY_F1                                 0x3A
#define MDT_KEY_F2                                 0x3B
#define MDT_KEY_F3                                 0x3C
#define MDT_KEY_F4                                 0x3D
#define MDT_KEY_F5                                 0x3E
#define MDT_KEY_F6                                 0x3F
#define MDT_KEY_F7                                 0x40
#define MDT_KEY_F8                                 0x41
#define MDT_KEY_F9                                 0x42
#define MDT_KEY_F10                                0x43
#define MDT_KEY_F11                                0x44
#define MDT_KEY_F12                                0x45
#define MDT_KEY_PRINTSCREEN                        0x46
#define MDT_KEY_SCROLL_LOCK                        0x47
#define MDT_KEY_PAUSE                              0x48
#define MDT_KEY_INSERT                             0x49
#define MDT_KEY_HOME                               0x4A
#define MDT_KEY_PAGEUP                             0x4B
#define MDT_KEY_DELETE                             0x4C
#define MDT_KEY_END1                               0x4D
#define MDT_KEY_PAGEDOWN                           0x4E
#define MDT_KEY_RIGHTARROW                         0x4F
#define MDT_KEY_LEFTARROW                          0x50
#define MDT_KEY_DOWNARROW                          0x51
#define MDT_KEY_UPARROW                            0x52
#define MDT_KEY_KEYPAD_NUM_LOCK_AND_CLEAR          0x53
#define MDT_KEY_KEYPAD_SLASH                       0x54
#define MDT_KEY_KEYPAD_ASTERIKS                    0x55
#define MDT_KEY_KEYPAD_MINUS                       0x56
#define MDT_KEY_KEYPAD_PLUS                        0x57
#define MDT_KEY_KEYPAD_ENTER                       0x58
#define MDT_KEY_KEYPAD_1_END                       0x59
#define MDT_KEY_KEYPAD_2_DOWN_ARROW                0x5A
#define MDT_KEY_KEYPAD_3_PAGEDN                    0x5B
#define MDT_KEY_KEYPAD_4_LEFT_ARROW                0x5C
#define MDT_KEY_KEYPAD_5                           0x5D
#define MDT_KEY_KEYPAD_6_RIGHT_ARROW               0x5E
#define MDT_KEY_KEYPAD_7_HOME                      0x5F
#define MDT_KEY_KEYPAD_8_UP_ARROW                  0x60
#define MDT_KEY_KEYPAD_9_PAGEUP                    0x61
#define MDT_KEY_KEYPAD_0_INSERT                    0x62
#define MDT_KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE    0x63
#define MDT_KEY_NONUS_BACK_SLASH_VERTICAL_BAR      0x64
#define MDT_KEY_APPLICATION                        0x65
#define MDT_KEY_POWER                              0x66
#define MDT_KEY_KEYPAD_EQUAL                       0x67
#define MDT_KEY_F13                                0x68
#define MDT_KEY_F14                                0x69
#define MDT_KEY_F15                                0x6A
#define MDT_KEY_F16                                0x6B
#define MDT_KEY_F17                                0x6C
#define MDT_KEY_F18                                0x6D
#define MDT_KEY_F19                                0x6E
#define MDT_KEY_F20                                0x6F
#define MDT_KEY_F21                                0x70
#define MDT_KEY_F22                                0x71
#define MDT_KEY_F23                                0x72
#define MDT_KEY_F24                                0x73
#define MDT_KEY_EXECUTE                            0x74
#define MDT_KEY_HELP                               0x75
#define MDT_KEY_MENU                               0x76
#define MDT_KEY_SELECT                             0x77
#define MDT_KEY_STOP                               0x78
#define MDT_KEY_AGAIN                              0x79
#define MDT_KEY_UNDO                               0x7A
#define MDT_KEY_CUT                                0x7B
#define MDT_KEY_COPY                               0x7C
#define MDT_KEY_PASTE                              0x7D
#define MDT_KEY_FIND                               0x7E
#define MDT_KEY_MUTE                               0x7F
#define MDT_KEY_VOLUME_UP                          0x80
#define MDT_KEY_VOLUME_DOWN                        0x81
#define MDT_KEY_LOCKING_CAPS_LOCK                  0x82
#define MDT_KEY_LOCKING_NUM_LOCK                   0x83
#define MDT_KEY_LOCKING_SCROLL_LOCK                0x84
#define MDT_KEY_KEYPAD_COMMA                       0x85
#define MDT_KEY_KEYPAD_EQUAL_SIGN                  0x86
#define MDT_KEY_INTERNATIONAL1                     0x87
#define MDT_KEY_INTERNATIONAL2                     0x88
#define MDT_KEY_INTERNATIONAL3                     0x89
#define MDT_KEY_INTERNATIONAL4                     0x8A
#define MDT_KEY_INTERNATIONAL5                     0x8B
#define MDT_KEY_INTERNATIONAL6                     0x8C
#define MDT_KEY_INTERNATIONAL7                     0x8D
#define MDT_KEY_INTERNATIONAL8                     0x8E
#define MDT_KEY_INTERNATIONAL9                     0x8F
#define MDT_KEY_LANG1                              0x90
#define MDT_KEY_LANG2                              0x91
#define MDT_KEY_LANG3                              0x92
#define MDT_KEY_LANG4                              0x93
#define MDT_KEY_LANG5                              0x94
#define MDT_KEY_LANG6                              0x95
#define MDT_KEY_LANG7                              0x96
#define MDT_KEY_LANG8                              0x97
#define MDT_KEY_LANG9                              0x98
#define MDT_KEY_ALTERNATE_ERASE                    0x99
#define MDT_KEY_SYSREQ                             0x9A
#define MDT_KEY_CANCEL                             0x9B
#define MDT_KEY_CLEAR                              0x9C
#define MDT_KEY_PRIOR                              0x9D
#define MDT_KEY_RETURN                             0x9E
#define MDT_KEY_SEPARATOR                          0x9F
#define MDT_KEY_OUT                                0xA0
#define MDT_KEY_OPER                               0xA1
#define MDT_KEY_CLEAR_AGAIN                        0xA2
#define MDT_KEY_CRSEL                              0xA3
#define MDT_KEY_EXSEL                              0xA4
#define MDT_KEY_KEYPAD_00                          0xB0
#define MDT_KEY_KEYPAD_000                         0xB1
#define MDT_KEY_THOUSANDS_SEPARATOR                0xB2
#define MDT_KEY_DECIMAL_SEPARATOR                  0xB3
#define MDT_KEY_CURRENCY_UNIT                      0xB4
#define MDT_KEY_CURRENCY_SUB_UNIT                  0xB5
#define MDT_KEY_KEYPAD_OPARENTHESIS                0xB6
#define MDT_KEY_KEYPAD_CPARENTHESIS                0xB7
#define MDT_KEY_KEYPAD_OBRACE                      0xB8
#define MDT_KEY_KEYPAD_CBRACE                      0xB9
#define MDT_KEY_KEYPAD_TAB                         0xBA
#define MDT_KEY_KEYPAD_BACKSPACE                   0xBB
#define MDT_KEY_KEYPAD_A                           0xBC
#define MDT_KEY_KEYPAD_B                           0xBD
#define MDT_KEY_KEYPAD_C                           0xBE
#define MDT_KEY_KEYPAD_D                           0xBF
#define MDT_KEY_KEYPAD_E                           0xC0
#define MDT_KEY_KEYPAD_F                           0xC1
#define MDT_KEY_KEYPAD_XOR                         0xC2
#define MDT_KEY_KEYPAD_CARET                       0xC3
#define MDT_KEY_KEYPAD_PERCENT                     0xC4
#define MDT_KEY_KEYPAD_LESS                        0xC5
#define MDT_KEY_KEYPAD_GREATER                     0xC6
#define MDT_KEY_KEYPAD_AMPERSAND                   0xC7
#define MDT_KEY_KEYPAD_LOGICAL_AND                 0xC8
#define MDT_KEY_KEYPAD_VERTICAL_BAR                0xC9
#define MDT_KEY_KEYPAD_LOGIACL_OR                  0xCA
#define MDT_KEY_KEYPAD_COLON                       0xCB
#define MDT_KEY_KEYPAD_NUMBER_SIGN                 0xCC
#define MDT_KEY_KEYPAD_SPACE                       0xCD
#define MDT_KEY_KEYPAD_AT                          0xCE
#define MDT_KEY_KEYPAD_EXCLAMATION_MARK            0xCF
#define MDT_KEY_KEYPAD_MEMORY_STORE                0xD0
#define MDT_KEY_KEYPAD_MEMORY_RECALL               0xD1
#define MDT_KEY_KEYPAD_MEMORY_CLEAR                0xD2
#define MDT_KEY_KEYPAD_MEMORY_ADD                  0xD3
#define MDT_KEY_KEYPAD_MEMORY_SUBTRACT             0xD4
#define MDT_KEY_KEYPAD_MEMORY_MULTIPLY             0xD5
#define MDT_KEY_KEYPAD_MEMORY_DIVIDE               0xD6
#define MDT_KEY_KEYPAD_PLUSMINUS                   0xD7
#define MDT_KEY_KEYPAD_CLEAR                       0xD8
#define MDT_KEY_KEYPAD_CLEAR_ENTRY                 0xD9
#define MDT_KEY_KEYPAD_BINARY                      0xDA
#define MDT_KEY_KEYPAD_OCTAL                       0xDB
#define MDT_KEY_KEYPAD_DECIMAL                     0xDC
#define MDT_KEY_KEYPAD_HEXADECIMAL                 0xDD
#define MDT_KEY_LEFTCONTROL                        0xE0
#define MDT_KEY_LEFTSHIFT                          0xE1
#define MDT_KEY_LEFTALT                            0xE2
#define MDT_KEY_LEFT_GUI                           0xE3
#define MDT_KEY_RIGHTCONTROL                       0xE4
#define MDT_KEY_RIGHTSHIFT                         0xE5
#define MDT_KEY_RIGHTALT                           0xE6
#define MDT_KEY_RIGHT_GUI                          0xE7


#ifdef USB_SUPPORT_ENABLED


void MDT_USB_INPUT_Init();

bool MDT_USB_INPUT_IsKbdKeyPressed(uint8_t key);

MDT_USB_INPUT_KeybdInfo* MDT_USB_INPUT_GetKeybdInfo();

MDT_USB_INPUT_MouseInfo* MDT_USB_INPUT_GetMouseInfo();

uint16_t MDT_USB_INPUT_GetEventCount();

USBH_HandleTypeDef* MDT_USB_INPUT_GetHandle();

USBH_StatusTypeDef MDT_USB_INPUT_GetLastCode();

const char* MDT_USB_INPUT_GetState();

void MDT_USB_INPUT_EVENTS_SetQueue(MDT_EVENT_QUEUE*);

void MDT_USB_INPUT_EVENTS_UnsetQueue();


#else 


#define MDT_USB_INPUT_Init()

#define MDT_USB_INPUT_GetState() "USB_DISABLED"

#define MDT_USB_INPUT_GetKeybdInfo() (NULL)

#define MDT_USB_INPUT_GetMouseInfo() (NULL)

#define MDT_USB_INPUT_GetEventCount() (0)

#define MDT_USB_INPUT_GetHandle() (NULL)

#define  MDT_USB_INPUT_GetLastCode() (0)

#define MDT_USB_INPUT_IsKbdKeyPressed(k) (false)

#define MDT_USB_INPUT_EVENTS_SetQueue(k)

#define MDT_USB_INPUT_EVENTS_UnsetQueue()


#endif

#endif