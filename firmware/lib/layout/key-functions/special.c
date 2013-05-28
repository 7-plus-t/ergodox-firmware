/* ----------------------------------------------------------------------------
 * Copyright (c) 2012, 2013 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "doc/licenses/MIT.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**                                                                 description
 * Implements the "special" section of "../key-functions.h"
 *
 * Notes:
 * - If the USB keyboard modifier state functions turn out to be something that
 *   would be generally useful, the functionality should be reimplemented in
 *   ".../firmware/lib/usb" (and removed from here).  For now I'm leaving them
 *   here because it seems better not to encourage messing with modifiers as a
 *   special group of keys, except for special purposes.
 */


#include <stdbool.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "../../../../firmware/lib/usb.h"
#include "../../../../firmware/lib/usb/usage-page/keyboard.h"

// ----------------------------------------------------------------------------

/**                                         types/_modifier_state_t/description
 * A struct representing the state of the keyboard modifier keys
 */
struct _modifier_state_t {
    bool left_control  : 1;
    bool left_shift    : 1;
    bool left_alt      : 1;
    bool left_gui      : 1;
    bool right_control : 1;
    bool right_shift   : 1;
    bool right_alt     : 1;
    bool right_gui     : 1;
};

// ----------------------------------------------------------------------------

/**                                  functions/_read_modifier_state/description
 * Return the state of the modifier keys
 *
 * Returns:
 * - success: A `_modifier_state_t`
 */
static struct _modifier_state_t _read_modifier_state(void) {
    return (struct _modifier_state_t) {
        .left_control  = usb__kb__read_key( KEYBOARD__LeftControl  ),
        .left_shift    = usb__kb__read_key( KEYBOARD__LeftShift    ),
        .left_alt      = usb__kb__read_key( KEYBOARD__LeftAlt      ),
        .left_gui      = usb__kb__read_key( KEYBOARD__LeftGUI      ),
        .right_control = usb__kb__read_key( KEYBOARD__RightControl ),
        .right_shift   = usb__kb__read_key( KEYBOARD__RightShift   ),
        .right_alt     = usb__kb__read_key( KEYBOARD__RightAlt     ),
        .right_gui     = usb__kb__read_key( KEYBOARD__RightGUI     ),
    };
}

/**                                   functions/_set_modifier_state/description
 * Set the state of the modifier keys to `state`
 *
 * Arguments:
 * - `state`: A `_modifier_state_t`
 */
static void _set_modifier_state(struct _modifier_state_t state) {
    usb__kb__set_key( state.left_control  , KEYBOARD__LeftControl  );
    usb__kb__set_key( state.left_shift    , KEYBOARD__LeftShift    );
    usb__kb__set_key( state.left_alt      , KEYBOARD__LeftAlt      );
    usb__kb__set_key( state.left_gui      , KEYBOARD__LeftGUI      );
    usb__kb__set_key( state.right_control , KEYBOARD__RightControl );
    usb__kb__set_key( state.right_shift   , KEYBOARD__RightShift   );
    usb__kb__set_key( state.right_alt     , KEYBOARD__RightAlt     );
    usb__kb__set_key( state.right_gui     , KEYBOARD__RightGUI     );

    usb__kb__send_report();
}

/**                                       functions/_send_hex_digit/description
 * Press then release the keycode corresponding to the character (0-9 A-F)
 * representing the low 4 bits of `digit` in base 16
 *
 * Warnings:
 * - Drops the high bit, but *does not do bounds checking on the value*
 *
 * Arguments:
 * - `digit`: A `uint8_t` who's low 4 bits represents the character to send
 */
static void _send_hex_digit(uint8_t digit) {
    digit &= 0x0F;

    if      (digit == 0) digit  = KEYBOARD__0_RightParenthesis;
    else if (digit < 10) digit += KEYBOARD__1_Exclamation-1;
    else                 digit += KEYBOARD__a_A-10;

    usb__kb__set_key(true, digit);
    usb__kb__send_report();
    usb__kb__set_key(false, digit);
    usb__kb__send_report();
}

// ----------------------------------------------------------------------------

void key_functions__toggle_capslock (uint16_t ignore) {
    struct _modifier_state_t state = _read_modifier_state();
    // -------
    struct _modifier_state_t temp_state = state;
    temp_state.left_shift = false;
    temp_state.right_shift = false;
    // -------
    _set_modifier_state(temp_state);

    // toggle capslock
    usb__kb__set_key(true,  KEYBOARD__CapsLock);
    usb__kb__send_report();
    usb__kb__set_key(false, KEYBOARD__CapsLock);
    usb__kb__send_report();

    _set_modifier_state(state);
}

/**                  functions/key_functions__send_unicode_sequence/description
 * Implementation notes:
 *
 * - We use `uint8_t` instead of `char` when iterating over `string` because
 *   the signedness of `char` is implementation defined (and, actually, signed
 *   by default with avr-gcc, which is not what we want if we're going to be
 *   doing bitwise operations and comparisons).
 *
 * - We assume, for the most part, that the string is valid modified (i.e.
 *   null-terminated) UTF-8.  This should be a fairly safe assumption, since
 *   all PROGMEM strings should be generated by the compiler :)
 *
 * - UTF-8 character format
 *
 *     ----------------------------------------------------------------------
 *      code points      avail. bits  byte 1    byte 2    byte 3    byte 4
 *      ---------------  -----------  --------  --------  --------  --------
 *      0x0000 - 0x007F           7   0xxxxxxx
 *      0x0080 - 0x07FF          11   110xxxxx  10xxxxxx
 *      0x0800 - 0xFFFF          16   1110xxxx  10xxxxxx  10xxxxxx
 *      0x010000 - 0x10FFFF      21   11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
 *     ----------------------------------------------------------------------
 */
void key_functions__send_unicode_sequence (const char * string) {
    struct _modifier_state_t state = _read_modifier_state();
    _set_modifier_state( (struct _modifier_state_t){} );

    uint8_t  c;       // for storing the current byte of the character
    uint16_t c_full;  // for storing the full character

    // send string
    for (c = pgm_read_byte(string); c; c = pgm_read_byte(++string)) {

        // get character
        if (c >> 7 == 0b0) {
            // a 1-byte utf-8 character
            c_full = c & 0x7F;

        } else if (c >> 5 == 0b110) {
            // beginning of a 2-byte utf-8 character
            // assume the string is valid
            c_full  = (c & 0x1F) <<  6; c = pgm_read_byte(++string);
            c_full |= (c & 0x3F) <<  0;

        } else if (c >> 4 == 0b1110) {
            // beginning of a 3-byte utf-8 character
            // assume the string is valid
            c_full  = (c & 0x0F) << 12; c = pgm_read_byte(++string);
            c_full |= (c & 0x3F) <<  6; c = pgm_read_byte(++string);
            c_full |= (c & 0x3F) <<  0;

        } else if ((c >> 3) == 0b11110) {
            // beginning of a 4-byte utf-8 character
            // this character is too long, we can't send it
            // skip this byte, and the next 3
            string += 3;
            continue;

        } else {
            // ran across some invalid utf-8
            // ignore it, try again next time
            continue;
        }

        // send start sequence
        usb__kb__set_key(true,  KEYBOARD__LeftAlt   ); usb__kb__send_report();
        usb__kb__set_key(true,  KEYBOARD__Equal_Plus); usb__kb__send_report();
        usb__kb__set_key(false, KEYBOARD__Equal_Plus); usb__kb__send_report();

        // send character
        _send_hex_digit( (c_full >> 12)       );
        _send_hex_digit( (c_full >>  8) & 0xF );
        _send_hex_digit( (c_full >>  4) & 0xF );
        _send_hex_digit( (c_full      ) & 0xF );

        // send end sequence
        usb__kb__set_key(false, KEYBOARD__LeftAlt); usb__kb__send_report();
    }

    _set_modifier_state(state);
}

