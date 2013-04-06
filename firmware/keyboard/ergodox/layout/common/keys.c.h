/* ----------------------------------------------------------------------------
 * Copyright (c) 2013 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "doc/licenses/MIT.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**                                                                 description
 * This file implements and extends the definitions in ".../lib/layout/keys.h"
 * and extends the definitions in ".../lib/layout/key-functions.h"
 *
 * Meant to be included *only* by the layout using it.
 *
 * TODO: put a note about where to look for more information if people are
 * trying to learn more about how to make key functions (probably, the usb,
 * key_functions, and keys headers; and others?)
 */

// TODO: implement chords (as in, for a chorded key layout)
// TODO: implement sticky keys

#ifndef ERGODOX_FIRMWARE__KEYBOARD__ERGODOX__LAYOUT__COMMON__KEYS__C__H
#define ERGODOX_FIRMWARE__KEYBOARD__ERGODOX__LAYOUT__COMMON__KEYS__C__H
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


#include "./definitions.h"

// ----------------------------------------------------------------------------

/**                                                        macros/P/description
 * Expand `name` into the corresponding "press" function name
 */
#define  P(name)  keys__press__##name

/**                                                        macros/R/description
 * Expand `name` into the corresponding "release" function name
 */
#define  R(name)  keys__release__##name

/**                                                        macros/K/description
 * Expand into a "key" suitable for putting into the layout matrix
 */
#define  K(name)  { &P(name), &R(name) }

/**                                                       macros/KF/description
 * Expand `name` into the corresponding "key_functions" function name
 */
#define  KF(name)  key_functions__##name

// ----------------------------------------------------------------------------

/**                                            macros/KEYS__DEFAULT/description
 * Define the functions for a default key (i.e. a normal key that presses and
 * releases a keycode as you'd expect)
 *
 * Needed by ".../lib/layout/keys.h"
 */
#define  KEYS__DEFAULT(name, value)             \
    void P(name) (void) { KF(press)(value); }   \
    void R(name) (void) { KF(release)(value); }

/**                                            macros/KEYS__SHIFTED/description
 * Define the functions for a "shifted" key (i.e. a key that sends a "shift"
 * along with the keycode)
 *
 * Needed by ".../lib/layout/keys.h"
 */
#define  KEYS__SHIFTED(name, value)                         \
    void P(name) (void) { KF(press)(KEY__LeftShift);        \
                          KF(press)(value); }               \
    void R(name) (void) { KF(release)(value);               \
                          KF(release)(KEY__LeftShift); }

/**                                    macros/KEYS__LAYER__PUSH_POP/description
 * Define the functions for a layer push-pop key (i.e. a layer shift key).
 *
 * Naming Convention:
 * - Example: In the name `lpupo1l1`, we have the following:
 *     - `l`  : this is a layer key
 *     - `pu` : it pushes the layer element onto the stack on "press"
 *     - `po` : it pops the layer element out of the stack on "release"
 *     - `1`  : it pushes and pops the layer element with `layer_id` = 1
 *     - `l`  : (separate the `layer_id` from the `layer_number`)
 *     - `1`  : it pushes a layer element with `layer_number` = 1
 *
 * - The first and second number do not have to be the same (that is, the
 *   `layer_id` and `layer_number` can be different; there may be situations
 *   where you want or need this).
 *
 * - Only one of `pu` or `po` is necessary.  A key with only `pu` should *only*
 *   push the layer onto the stack, not pop anything out of it.  A key with
 *   only `po` should *only* pop the layer out of the stack.
 *
 * Notes:
 * - To save space, if you define a push-pop function, the push (only) and pop
 *   (only) functions may be defined as follows (using the example `lpupo1l1`
 *   from above):
 *
 *       #define  keys__press__lpu1l1    P(lpupo1l1)
 *       #define  keys__release__lpu1l1  P(nop)
 *       #define  keys__press__lpo1l1    R(lpupo1l1)
 *       #define  keys__release__lpo1l1  P(nop)
 *
 * - It is recommended (as a general rule) to allocate `layer_id`s as follows:
 *     - 0-9   : leave alone (for use in default layer key definitions)
 *     - 10-19 : for custom things like 'numPush' and 'numPop'
 */
#define  KEYS__LAYER__PUSH_POP(ID, LAYER)                                   \
    void P(l##ID##pupo##LAYER) (void) { layer_stack__push(ID, LAYER); }     \
    void R(l##ID##pupo##LAYER) (void) { layer_stack__pop_id(ID); }

/**                               macros/(group) layer : number pad/description
 * Define function for pushing and popping the number pad (namely `numPush`,
 * `numPop`, and `numPuPo`)
 *
 * Members:
 * - `KEYS__LAYER__NUM_PU_PO`
 * - `KEYS__LAYER__NUM_PUSH`
 * - `KEYS__LAYER__NUM_POP`
 *
 * These macros are meant to be used (if necessary) in the layout file, since
 * they need to know the layer on which the number pad has been placed.
 */
#define  KEYS__LAYER__NUM_PU_PO(ID, LAYER)                          \
    void P(numPuPo) (void) { layer_stack__push(ID, LAYER);          \
                             KF(press)(KEY__LockingNumLock);        \
                             usb__kb__send_report();                \
                             KF(release)(KEY__LockingNumLock);      \
                             usb__kb__send_report(); }              \
    void R(numPuPo) (void) { layer_stack__pop_id(ID);               \
                             KF(press)(KEY__LockingNumLock);        \
                             usb__kb__send_report();                \
                             KF(release)(KEY__LockingNumLock);      \
                             usb__kb__send_report(); }

#define  KEYS__LAYER__NUM_PUSH(LAYER, ID)                           \
    void P(numPush) (void) { layer_stack__push(ID, LAYER);          \
                             KF(press)(KEY__LockingNumLock); }      \
    void R(numPush) (void) { KF(release)(KEY__LockingNumLock); }

#define  KEYS__LAYER__NUM_POP(LAYER, ID)                            \
    void P(numPop) (void) { layer_stack__pop_id(ID);                \
                            KF(press)(KEY__LockingNumLock); }       \
    void R(numPop) (void) { KF(release)(KEY__LockingNumLock); }

// ----------------------------------------------------------------------------

/**                           functions/key__functions__2_keys_caps/description
 * Press the given keycode, and also press "capslock" if this is the second
 * consecutive time this function has been called with `pressed == true`.
 *
 * Notes:
 * - Meant to be used with the left and right "shift" keys.
 */
void KF(2_keys_capslock)(bool pressed, uint8_t keycode) {
    static counter = 0;
    if (pressed) {
        counter++;
        KF(press)(keycode);
    }
    if (counter == 2 && pressed) {
        KF(toggle_capslock)();
    }
    if (!pressed) {
        counter--;
        KF(release)(keycode);
    }
}

// ----------------------------------------------------------------------------

// --- default key definitions ------------------------------------------------

#include "../../../../firmware/lib/layout/keys.h"


// --- special meaning --------------------------------------------------------

/**                                                     keys/transp/description
 * transparent
 *
 * This key signals to the firmware (specifically the
 * `kb__layout__exec_key_location()` function) that it should look for what key
 * to "press" or "release" by going down the layer-stack until it finds a
 * non-transparent key at the same position.
 *
 * Notes:
 * - Keys may be half transparent; that is, the "press" part of a key may be
 *   transparent while the "release" part isn't, or vice versa.  I expect this
 *   to be fairly uncommon though.
 */
#define  keys__press__transp    NULL
#define  keys__release__transp  NULL

/**                                                        keys/nop/desctiption
 * no operation
 *
 * This key does nothing (and is not transparent).
 */
void P(nop) (void) {}
void R(nop) (void) {}


// --- special keycode --------------------------------------------------------

KEYS__DEFAULT( power,   KEY__Power      );
KEYS__DEFAULT( volumeU, KEY__VolumeUp   );
KEYS__DEFAULT( volumeD, KEY__VolumeDown );
KEYS__DEFAULT( mute,    KEY__Mute       );


// --- special function -------------------------------------------------------

/**                                                  keys/shL2kcaps/description
 * left shift + toggle capslock (if both shifts are pressed)
 *
 * This key always generates a left shift.  If the `shR2kcaps` is pressed at
 * the same time, "capslock" will be toggled.
 */
void P(shL2kcaps) (void) { KF(2_keys_capslock)(true, KEY__LeftShift); }
void R(shL2kcaps) (void) { KF(2_keys_capslock)(false, KEY__LeftShift); }

/**                                                  keys/shR2kcaps/description
 * right shift + toggle capslock (if both shifts are pressed)
 *
 * This key always generates a right shift.  If the `shL2kcaps` is pressed at
 * the same time, "capslock" will be toggled.
 */
void P(shR2kcaps) (void) { KF(2_keys_capslock)(true, KEY__RightShift); }
void R(shR2kcaps) (void) { KF(2_keys_capslock)(false, KEY__RightShift); }

/**                                                      keys/btldr/description
 * jump to the bootloader
 *
 * This prepares the Teensy to load a new firmware.  If you press this without
 * meaning to, you must turn your keyboard off then on again (usually by
 * unplugging it, then plugging it back in)
 */
void P(btldr) (void) { KF(jump_to_bootloader)(); }
void R(btldr) (void) {}


// ----------------------------------------------------------------------------
// --- layer ------------------------------------------------------------------

// note: these are just some default layer key definitions; no need to stick to
// them if they're inconvenient

KEYS__LAYER__PUSH_POP(0, 0);
#define  keys__press__lpu0l0    P(lpupo0l0)
#define  keys__release__lpu0l0  P(nop)
#define  keys__press__lpo0l0    R(lpupo0l0)
#define  keys__release__lpo0l0  P(nop)

KEYS__LAYER__PUSH_POP(1, 1);
#define  keys__press__lpu1l1    P(lpupo1l1)
#define  keys__release__lpu1l1  P(nop)
#define  keys__press__lpo1l1    R(lpupo1l1)
#define  keys__release__lpo1l1  P(nop)

KEYS__LAYER__PUSH_POP(2, 2);
#define  keys__press__lpu2l2    P(lpupo2l2)
#define  keys__release__lpu2l2  P(nop)
#define  keys__press__lpo2l2    R(lpupo2l2)
#define  keys__release__lpo2l2  P(nop)

KEYS__LAYER__PUSH_POP(3, 3);
#define  keys__press__lpu3l3    P(lpupo3l3)
#define  keys__release__lpu3l3  P(nop)
#define  keys__press__lpo3l3    R(lpupo3l3)
#define  keys__release__lpo3l3  P(nop)

KEYS__LAYER__PUSH_POP(4, 4);
#define  keys__press__lpu4l4    P(lpupo4l4)
#define  keys__release__lpu4l4  P(nop)
#define  keys__press__lpo4l4    R(lpupo4l4)
#define  keys__release__lpo4l4  P(nop)

KEYS__LAYER__PUSH_POP(5, 5);
#define  keys__press__lpu5l5    P(lpupo5l5)
#define  keys__release__lpu5l5  P(nop)
#define  keys__press__lpo5l5    R(lpupo5l5)
#define  keys__release__lpo5l5  P(nop)

KEYS__LAYER__PUSH_POP(6, 6);
#define  keys__press__lpu6l6    P(lpupo6l6)
#define  keys__release__lpu6l6  P(nop)
#define  keys__press__lpo6l6    R(lpupo6l6)
#define  keys__release__lpo6l6  P(nop)

KEYS__LAYER__PUSH_POP(7, 7);
#define  keys__press__lpu7l7    P(lpupo7l7)
#define  keys__release__lpu7l7  P(nop)
#define  keys__press__lpo7l7    R(lpupo7l7)
#define  keys__release__lpo7l7  P(nop)


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#endif  // ERGODOX_FIRMWARE__KEYBOARD__ERGODOX__LAYOUT__COMMON__KEYS__C__H

