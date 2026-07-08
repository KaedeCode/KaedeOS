#include <stdint.h>
#include <ports.h>
#include <isr.h>
#include <pic.h>
#include <stddef.h>

#define MOD_SHIFT       1
#define MOD_CAPS        2
#define MOD_ALT         4
#define MOD_CTRL        8
#define MOD_NUMLOCK     16
#define MOD_SCROLLLOCK  32
#define MOD_ALTGR       64
#define MOD_WIN         128
#define MOD_META        256

#define RING_SIZE       256
#define EVENT_QUEUE_SIZE 64

struct key_event {
    uint8_t scancode;
    uint8_t pressed;
    uint8_t repeat;
};

static uint8_t key_state[256];
static char ring_buffer[RING_SIZE];
static volatile uint16_t ring_head = 0;
static volatile uint16_t ring_tail = 0;
static struct key_event event_queue[EVENT_QUEUE_SIZE];
static volatile uint16_t ev_head = 0;
static volatile uint16_t ev_tail = 0;
static uint32_t modifier_keys = 0;
static volatile uint8_t extended = 0;

static uint8_t ascii_map[128] = { 0 };
static uint8_t ascii_map_shift[128] = { 0 };
static uint8_t ascii_map_caps[128] = { 0 };
static uint8_t ascii_map_shift_caps[128] = { 0 };
static uint8_t ascii_map_ext[128] = { 0 };

static void init_maps(void) {
    ascii_map[0x01] = 0x1B;
    ascii_map[0x0E] = 0x08;
    ascii_map[0x1C] = 0x0A;
    ascii_map[0x0F] = 0x09;
    ascii_map[0x39] = ' ';
    ascii_map[0x29] = '`';
    ascii_map[0x02] = '1';
    ascii_map[0x03] = '2';
    ascii_map[0x04] = '3';
    ascii_map[0x05] = '4';
    ascii_map[0x06] = '5';
    ascii_map[0x07] = '6';
    ascii_map[0x08] = '7';
    ascii_map[0x09] = '8';
    ascii_map[0x0A] = '9';
    ascii_map[0x0B] = '0';
    ascii_map[0x0C] = '-';
    ascii_map[0x0D] = '=';
    ascii_map[0x10] = 'q';
    ascii_map[0x11] = 'w';
    ascii_map[0x12] = 'e';
    ascii_map[0x13] = 'r';
    ascii_map[0x14] = 't';
    ascii_map[0x15] = 'y';
    ascii_map[0x16] = 'u';
    ascii_map[0x17] = 'i';
    ascii_map[0x18] = 'o';
    ascii_map[0x19] = 'p';
    ascii_map[0x1A] = '[';
    ascii_map[0x1B] = ']';
    ascii_map[0x1E] = 'a';
    ascii_map[0x1F] = 's';
    ascii_map[0x20] = 'd';
    ascii_map[0x21] = 'f';
    ascii_map[0x22] = 'g';
    ascii_map[0x23] = 'h';
    ascii_map[0x24] = 'j';
    ascii_map[0x25] = 'k';
    ascii_map[0x26] = 'l';
    ascii_map[0x27] = ';';
    ascii_map[0x28] = '\'';
    ascii_map[0x2B] = '\\';
    ascii_map[0x2C] = 'z';
    ascii_map[0x2D] = 'x';
    ascii_map[0x2E] = 'c';
    ascii_map[0x2F] = 'v';
    ascii_map[0x30] = 'b';
    ascii_map[0x31] = 'n';
    ascii_map[0x32] = 'm';
    ascii_map[0x33] = ',';
    ascii_map[0x34] = '.';
    ascii_map[0x35] = '/';
    ascii_map[0x37] = 0x8A;

    ascii_map_shift[0x01] = 0x1B;
    ascii_map_shift[0x0E] = 0x08;
    ascii_map_shift[0x1C] = 0x0A;
    ascii_map_shift[0x0F] = 0x09;
    ascii_map_shift[0x39] = ' ';
    ascii_map_shift[0x29] = '~';
    ascii_map_shift[0x02] = '!';
    ascii_map_shift[0x03] = '@';
    ascii_map_shift[0x04] = '#';
    ascii_map_shift[0x05] = '$';
    ascii_map_shift[0x06] = '%';
    ascii_map_shift[0x07] = '^';
    ascii_map_shift[0x08] = '&';
    ascii_map_shift[0x09] = '*';
    ascii_map_shift[0x0A] = '(';
    ascii_map_shift[0x0B] = ')';
    ascii_map_shift[0x0C] = '_';
    ascii_map_shift[0x0D] = '+';
    ascii_map_shift[0x10] = 'Q';
    ascii_map_shift[0x11] = 'W';
    ascii_map_shift[0x12] = 'E';
    ascii_map_shift[0x13] = 'R';
    ascii_map_shift[0x14] = 'T';
    ascii_map_shift[0x15] = 'Y';
    ascii_map_shift[0x16] = 'U';
    ascii_map_shift[0x17] = 'I';
    ascii_map_shift[0x18] = 'O';
    ascii_map_shift[0x19] = 'P';
    ascii_map_shift[0x1A] = '{';
    ascii_map_shift[0x1B] = '}';
    ascii_map_shift[0x1E] = 'A';
    ascii_map_shift[0x1F] = 'S';
    ascii_map_shift[0x20] = 'D';
    ascii_map_shift[0x21] = 'F';
    ascii_map_shift[0x22] = 'G';
    ascii_map_shift[0x23] = 'H';
    ascii_map_shift[0x24] = 'J';
    ascii_map_shift[0x25] = 'K';
    ascii_map_shift[0x26] = 'L';
    ascii_map_shift[0x27] = ':';
    ascii_map_shift[0x28] = '"';
    ascii_map_shift[0x2B] = '|';
    ascii_map_shift[0x2C] = 'Z';
    ascii_map_shift[0x2D] = 'X';
    ascii_map_shift[0x2E] = 'C';
    ascii_map_shift[0x2F] = 'V';
    ascii_map_shift[0x30] = 'B';
    ascii_map_shift[0x31] = 'N';
    ascii_map_shift[0x32] = 'M';
    ascii_map_shift[0x33] = '<';
    ascii_map_shift[0x34] = '>';
    ascii_map_shift[0x35] = '?';
    ascii_map_shift[0x37] = 0x8A;

    for (int i = 0; i < 128; i++) ascii_map_caps[i] = ascii_map[i];
    ascii_map_caps[0x10] = 'Q'; ascii_map_caps[0x11] = 'W';
    ascii_map_caps[0x12] = 'E'; ascii_map_caps[0x13] = 'R';
    ascii_map_caps[0x14] = 'T'; ascii_map_caps[0x15] = 'Y';
    ascii_map_caps[0x16] = 'U'; ascii_map_caps[0x17] = 'I';
    ascii_map_caps[0x18] = 'O'; ascii_map_caps[0x19] = 'P';
    ascii_map_caps[0x1E] = 'A'; ascii_map_caps[0x1F] = 'S';
    ascii_map_caps[0x20] = 'D'; ascii_map_caps[0x21] = 'F';
    ascii_map_caps[0x22] = 'G'; ascii_map_caps[0x23] = 'H';
    ascii_map_caps[0x24] = 'J'; ascii_map_caps[0x25] = 'K';
    ascii_map_caps[0x26] = 'L';
    ascii_map_caps[0x2C] = 'Z'; ascii_map_caps[0x2D] = 'X';
    ascii_map_caps[0x2E] = 'C'; ascii_map_caps[0x2F] = 'V';
    ascii_map_caps[0x30] = 'B'; ascii_map_caps[0x31] = 'N';
    ascii_map_caps[0x32] = 'M';

    for (int i = 0; i < 128; i++) ascii_map_shift_caps[i] = ascii_map_shift[i];
    ascii_map_shift_caps[0x10] = 'q'; ascii_map_shift_caps[0x11] = 'w';
    ascii_map_shift_caps[0x12] = 'e'; ascii_map_shift_caps[0x13] = 'r';
    ascii_map_shift_caps[0x14] = 't'; ascii_map_shift_caps[0x15] = 'y';
    ascii_map_shift_caps[0x16] = 'u'; ascii_map_shift_caps[0x17] = 'i';
    ascii_map_shift_caps[0x18] = 'o'; ascii_map_shift_caps[0x19] = 'p';
    ascii_map_shift_caps[0x1E] = 'a'; ascii_map_shift_caps[0x1F] = 's';
    ascii_map_shift_caps[0x20] = 'd'; ascii_map_shift_caps[0x21] = 'f';
    ascii_map_shift_caps[0x22] = 'g'; ascii_map_shift_caps[0x23] = 'h';
    ascii_map_shift_caps[0x24] = 'j'; ascii_map_shift_caps[0x25] = 'k';
    ascii_map_shift_caps[0x26] = 'l';
    ascii_map_shift_caps[0x2C] = 'z'; ascii_map_shift_caps[0x2D] = 'x';
    ascii_map_shift_caps[0x2E] = 'c'; ascii_map_shift_caps[0x2F] = 'v';
    ascii_map_shift_caps[0x30] = 'b'; ascii_map_shift_caps[0x31] = 'n';
    ascii_map_shift_caps[0x32] = 'm';

    ascii_map_ext[0x47] = 0x80;
    ascii_map_ext[0x48] = 0x81;
    ascii_map_ext[0x49] = 0x82;
    ascii_map_ext[0x4B] = 0x83;
    ascii_map_ext[0x4D] = 0x84;
    ascii_map_ext[0x4F] = 0x85;
    ascii_map_ext[0x50] = 0x86;
    ascii_map_ext[0x51] = 0x87;
    ascii_map_ext[0x52] = 0x88;
    ascii_map_ext[0x53] = 0x89;
    ascii_map_ext[0x1C] = 0x0A;
    ascii_map_ext[0x35] = '/';
}

static uint8_t* get_ascii_table(void) {
    int shift = (modifier_keys & MOD_SHIFT) ? 1 : 0;
    int caps  = (modifier_keys & MOD_CAPS)  ? 1 : 0;
    if (shift && caps) return ascii_map_shift_caps;
    if (shift)         return ascii_map_shift;
    if (caps)          return ascii_map_caps;
    return ascii_map;
}

static void update_modifiers(uint8_t sc, uint8_t pressed) {
    switch(sc) {
        case 0x2A: case 0x36:
            if (pressed) modifier_keys |= MOD_SHIFT;
            else modifier_keys &= ~MOD_SHIFT;
            break;
        case 0x3A:
            if (pressed) modifier_keys ^= MOD_CAPS;
            break;
        case 0x1D:
            if (pressed) modifier_keys |= MOD_CTRL;
            else modifier_keys &= ~MOD_CTRL;
            break;
        case 0x38:
            if (pressed) modifier_keys |= MOD_ALT;
            else modifier_keys &= ~MOD_ALT;
            break;
        case 0x45:
            if (pressed) modifier_keys ^= MOD_NUMLOCK;
            break;
        case 0x46:
            if (pressed) modifier_keys ^= MOD_SCROLLLOCK;
            break;
        case 0x5B: case 0x5C:
            if (pressed) modifier_keys |= MOD_WIN;
            else modifier_keys &= ~MOD_WIN;
            break;
        default:
            break;
    }
}

uint8_t get_key_state(uint8_t scancode) {
    return key_state[scancode];
}

char read_char(void) {
    if (ring_head == ring_tail)
        return 0;
    char c = ring_buffer[ring_tail];
    ring_tail = (ring_tail + 1) % RING_SIZE;
    return c;
}

struct key_event read_event(void) {
    struct key_event empty = {0,0,0};
    if (ev_head == ev_tail)
        return empty;
    struct key_event e = event_queue[ev_tail];
    ev_tail = (ev_tail + 1) % EVENT_QUEUE_SIZE;
    return e;
}

void keyboard_handler(RegFrame *frame) {
    (void)frame;
    uint8_t scancode = inb(0x60);

    if (scancode == 0xE0) {
        extended = 1;
        outb(0x20, 0x20);
        return;
    }

    uint8_t pressed = !(scancode & 0x80);
    scancode &= 0x7F;

    if (extended) {
        scancode |= 0x80;
        extended = 0;
    }

    uint8_t repeat = 0;
    if (pressed && key_state[scancode] == 1) {
        repeat = 1;
    }

    key_state[scancode] = pressed;

    if (scancode == 0x38 && (scancode & 0x80)) {
        if (pressed) modifier_keys |= MOD_ALTGR;
        else modifier_keys &= ~MOD_ALTGR;
    } else if (scancode == 0x1D && (scancode & 0x80)) {
        if (pressed) modifier_keys |= MOD_CTRL;
        else modifier_keys &= ~MOD_CTRL;
    } else {
        update_modifiers(scancode, pressed);
    }

    int ev_next = (ev_head + 1) % EVENT_QUEUE_SIZE;
    if (ev_next != ev_tail) {
        event_queue[ev_head].scancode = scancode;
        event_queue[ev_head].pressed = pressed;
        event_queue[ev_head].repeat = repeat;
        ev_head = ev_next;
    }

    if (pressed && !repeat) {
        uint8_t* table;
        if (scancode & 0x80) {
            table = ascii_map_ext;
        } else {
            table = get_ascii_table();
        }
        uint8_t ch = table[scancode & 0x7F];
        if (ch != 0) {
            int next = (ring_head + 1) % RING_SIZE;
            if (next != ring_tail) {
                ring_buffer[ring_head] = ch;
                ring_head = next;
            }
        }
    }

    outb(0x20, 0x20);
}

void init_keyboard(void) {
    init_maps();

    for (int i = 0; i < 256; i++) key_state[i] = 0;
    ring_head = ring_tail = 0;
    ev_head = ev_tail = 0;
    modifier_keys = 0;
    extended = 0;

    enable_pic(1);
    register_isr(0x21, keyboard_handler);
}