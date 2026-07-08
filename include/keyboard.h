#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct key_event {
    uint8_t scancode;
    uint8_t pressed;
    uint8_t repeat;
};

void init_keyboard(void);
uint8_t get_key_state(uint8_t scancode);
char read_char(void);
struct key_event read_event(void);

#define SC_ESC          0x01
#define SC_1            0x02
#define SC_2            0x03
#define SC_3            0x04
#define SC_4            0x05
#define SC_5            0x06
#define SC_6            0x07
#define SC_7            0x08
#define SC_8            0x09
#define SC_9            0x0A
#define SC_0            0x0B
#define SC_MINUS        0x0C
#define SC_EQUALS       0x0D
#define SC_BACKSPACE    0x0E
#define SC_TAB          0x0F
#define SC_Q            0x10
#define SC_W            0x11
#define SC_E            0x12
#define SC_R            0x13
#define SC_T            0x14
#define SC_Y            0x15
#define SC_U            0x16
#define SC_I            0x17
#define SC_O            0x18
#define SC_P            0x19
#define SC_LBRACKET     0x1A
#define SC_RBRACKET     0x1B
#define SC_ENTER        0x1C
#define SC_LCTRL        0x1D
#define SC_A            0x1E
#define SC_S            0x1F
#define SC_D            0x20
#define SC_F            0x21
#define SC_G            0x22
#define SC_H            0x23
#define SC_J            0x24
#define SC_K            0x25
#define SC_L            0x26
#define SC_SEMICOLON    0x27
#define SC_QUOTE        0x28
#define SC_BACKTICK     0x29
#define SC_LSHIFT       0x2A
#define SC_BACKSLASH    0x2B
#define SC_Z            0x2C
#define SC_X            0x2D
#define SC_C            0x2E
#define SC_V            0x2F
#define SC_B            0x30
#define SC_N            0x31
#define SC_M            0x32
#define SC_COMMA        0x33
#define SC_PERIOD       0x34
#define SC_SLASH        0x35
#define SC_RSHIFT       0x36
#define SC_PRINTSCREEN  0x37
#define SC_LALT         0x38
#define SC_SPACE        0x39
#define SC_CAPSLOCK     0x3A
#define SC_F1           0x3B
#define SC_F2           0x3C
#define SC_F3           0x3D
#define SC_F4           0x3E
#define SC_F5           0x3F
#define SC_F6           0x40
#define SC_F7           0x41
#define SC_F8           0x42
#define SC_F9           0x43
#define SC_F10          0x44
#define SC_NUMLOCK      0x45
#define SC_SCROLLLOCK   0x46
#define SC_HOME         0x47
#define SC_UP           0x48
#define SC_PGUP         0x49
#define SC_MINUS_KP     0x4A
#define SC_LEFT         0x4B
#define SC_CENTER_KP    0x4C
#define SC_RIGHT        0x4D
#define SC_PLUS_KP      0x4E
#define SC_END          0x4F
#define SC_DOWN         0x50
#define SC_PGDN         0x51
#define SC_INS          0x52
#define SC_DEL          0x53

#define SC_EXT_HOME     (0x80 | 0x47)
#define SC_EXT_UP       (0x80 | 0x48)
#define SC_EXT_PGUP     (0x80 | 0x49)
#define SC_EXT_LEFT     (0x80 | 0x4B)
#define SC_EXT_RIGHT    (0x80 | 0x4D)
#define SC_EXT_END      (0x80 | 0x4F)
#define SC_EXT_DOWN     (0x80 | 0x50)
#define SC_EXT_PGDN     (0x80 | 0x51)
#define SC_EXT_INS      (0x80 | 0x52)
#define SC_EXT_DEL      (0x80 | 0x53)
#define SC_EXT_ENTER    (0x80 | 0x1C)
#define SC_EXT_SLASH    (0x80 | 0x35)
#define SC_EXT_RCTRL    (0x80 | 0x1D)
#define SC_EXT_RALT     (0x80 | 0x38)
#define SC_EXT_LWIN     (0x80 | 0x5B)
#define SC_EXT_RWIN     (0x80 | 0x5C)
#define SC_EXT_MENU     (0x80 | 0x5D)

#define KEY_HOME        0x80
#define KEY_UP          0x81
#define KEY_PGUP        0x82
#define KEY_LEFT        0x83
#define KEY_RIGHT       0x84
#define KEY_END         0x85
#define KEY_DOWN        0x86
#define KEY_PGDN        0x87
#define KEY_INS         0x88
#define KEY_DEL         0x89
#define KEY_PRTSCR      0x8A

#ifdef __cplusplus
}
#endif

#endif