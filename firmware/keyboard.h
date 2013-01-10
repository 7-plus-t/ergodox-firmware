/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "license.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**
 * - description: |
 *     The keyboard interface, and related definitions
 *
 *     Prefix: `kb__`
 */


#ifndef ERGODOX_FIRMWARE__FIRMWARE__KEYBOARD__H
#define ERGODOX_FIRMWARE__FIRMWARE__KEYBOARD__H
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


#include <stdbool.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#include "../firmware/lib/layout/key-functions.h"

// ----------------------------------------------------------------------------

#if MAKE__KEYBOARD == 'ergodox'

    #define  KB__ROWS      6
    #define  KB__COLUMNS  14

#endif

// ----------------------------------------------------------------------------

typedef struct {
    kf__function_pointer_t  press_function;
    uint16_t                press_value;
    kf__function_pointer_t  release_function;
    uint16_t                release_value;
} kb__key_t;

// ----------------------------------------------------------------------------

// controller
int8_t kb__init          (void);
int8_t kb__update_matrix (bool matrix[KB__ROWS][KB__COLUMNS]);

// LED
void kb__led__on  (int8_t led);
void kb__led__off (int8_t led);
void kb__led__set (int8_t led, float n);
// -------
void kb__led__all_on  (void);
void kb__led__all_off (void);
void kb__led__all_set (float n);
// -------
void kb__led__state__power_on (void);
void kb__led__state__ready    (void);
void kb__led__delay__usb_init (void);

// layout
void kb__led__logical_on  (char led);
void kb__led__logical_off (char led);
// -------
void kb__layout__exec_key ( bool   pressed,
                            int8_t layer,
                            int8_t row,
                            int8_t column );


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#endif  // ERGODOX_FIRMWARE__FIRMWARE__KEYBOARD__H


// TODO: rewrite, coz of changes
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # documentation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-
  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # macros ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~--

   - macro:
       name: '`KB__ROWS`'
       values:
         - { name: '`6`', description: for the ErgoDox }
  
   - macro:
       name: '`KB__COLUMNS`'
       values:
         - { name: '`14`', description: for the ErgoDox }

  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # typedefs ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   - typedef:
       type: '`struct`'
       name: '`kb__key_t`'
       description: The type of each key in the layout matrix.
       values:
         - type: '`uint8_t`'
           name: '`value`'
           description:
             The value passed to the appropriate function when the key changes
             state.  The function may do as it wishes with this value, but it
             will most commonly be a keycode to send to the host.
         - type: '`kf__function_pointer_t`'
           name: '`press`'
           description:
             The function to call when the key changes from 'released' to
             'pressed'.
         - type: '`kf__function_pointer_t`'
           name: '`release`'
           description:
             The function to call when the key changes from 'pressed' to
             'released'.

  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  # controller ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-

   - function:
       name: '`kb__init`'
       description: Initialize the keyboard.
       return value:
         type: '`int8_t`'
         values:
           - { name: '`0`', description: success }
           - { name: '[other]', description: error }
       notes:
         - Should be called exactly once by `main()` before entering the run
           loop.

   - function:
       name: '`kb__update_matrix`'
       description:
         Update the given matrix to the current state of the keyboard.
       arguments:
         - type: '`bool[][]`'
           name: '`matrix`'
           description: A pointer to the matrix to update.
           notes:
             - Matrix dimensions are `[KB__ROWS][KB__COLUMNS]`.
       return value:
         type: '`int8_t`'
         values:
           - { name: '`0`', description: success }
           - { name: '[other]', description: error }

  # LED ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~--

   - .ignore:
     
     - &kb__led___int8_t__led
       type: '`int8_t`'
       name: '`led`'
       description:
         The number of the LED to set.  Should an integer between 1 and 5
         inclusive; behavior is undefined otherwise.
       notes:
         - For US keyboards, likely only LEDs 1 through 3 will be present.  But
           the function should handle 4 through 5 gracefully anyway.
  
     - &kb__led___float__n
       type: '`float`'
       name: '`n`'
       description:
         The percent of the highest possible brightness at which to set the
         LED.  Should be a float between 0 and 1 inclusive; behavior is
         undefined otherwise.

  # ~~~~~~~

   - function:
       name: '`kb__led__on`'
       description: Set the given LED 'on'.
       arguments:
         - *kb__led___int8_t__led
  
   - function:
       name: '`kb__led__off`'
       description: Set the given LED 'off'.
       arguments:
         - *kb__led___int8_t__led
  
   - function:
       name: '`kb__led__set`'
       description: Set the given LED's brightness (by percent).
       arguments:
         - *kb__led___int8_t__led
         - *kb__led___float__n

  # ~~~~~~~

   - function:
       name: '`kb__led__all_on`'
       description: Set all the LEDs 'on'.
  
   - function:
       name: '`kb__led__all_off`'
       description: Set all the LEDs 'off'.
  
   - function:
       name: '`kb__led__all_set`'
       description: Set all the LEDs brightness (by percent).
       arguments:
         - *kb__led___float__n

  # ~~~~~~~

   - function:
       name: '`kb__led__state__power_on`'
       description:
         Set the LEDs to the state that indicates the keyboard just powered on.
  
   - function:
       name: '`kb__led__state__ready`'
       description:
         Set the LEDs to the state that indicates the keyboard is ready to
         receive keystrokes.
  
   - function:
       name: '`kb__led__delay__usb_init`'
       description:
         Delay for a total of ~1 second, to allow the host to load drivers and
         such.

  # layout ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~--

   - .ignore:
  
     - &kb__led__logical___arguments
         - type: '`char`'
           name: '`led`'
           values:
             - { name: '`N`', description: numlock }
             - { name: '`C`', description: capslock }
             - { name: '`S`', description: scroll lock }
             - { name: '`O`', description: compose }
             - { name: '`K`', description: kana }

  # ~~~~~~~

   - function:
       name: '`kb__led__logical_on`'
       description:
         Set LEDs on based on their meaning (e.g. 'numlock', 'capslock').
       arguments: *kb__led__logical___arguments
  
   - function:
       name: '`kb__led__logical_off`'
       description:
         Set LEDs off based on their meaning (e.g. 'numlock', 'capslock').
       arguments: *kb__led__logical___arguments

  # ~~~~~~~

   - function:
       name: '`kb__layout__get_key`'
       description:
         Look up and return the contents of the layout matrix at the given
         point.
       arguments:
         - { type: '`uint8_t`', name: '`layer`' }
         - { type: '`uint8_t`', name: '`row`' }
         - { type: '`uint8_t`', name: '`column`' }
       return value:
         type: '`kb__key_t`'

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ **/

