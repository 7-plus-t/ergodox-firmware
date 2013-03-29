/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (see "doc/licenses/MIT.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */

/**                                                                 description
 * Some generally useful key-functions, and related definitions
 *
 * Prefix: `kf__`
 */


#ifndef ERGODOX_FIRMWARE__FIRMWARE__LIB__LAYOUT__KEY_FUNCTIONS__H
#define ERGODOX_FIRMWARE__FIRMWARE__LIB__LAYOUT__KEY_FUNCTIONS__H
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------

typedef  void (*kf__function_pointer_t)(uint16_t value);

// ----------------------------------------------------------------------------

// basic
void kf__press          (uint16_t keycode);
void kf__release        (uint16_t keycode);
void kf__toggle         (uint16_t keycode);
void kf__layer__push    (uint16_t id__layer);
void kf__layer__pop     (uint16_t id__ignore);
// void kf__macro__sram    (uint16_t pointer);  // TODO
void kf__macro__progmem (uint16_t pointer);
// void kf__macro__eeprom  (uint16_t pointer);  // TODO

// TODO: chording
//
// - consider making a 'kf__chord(uint16_t pointer)' that takes a pointer to an
//   array containing whatever's needed to specify [ which group of keys it
//   belongs to; whether to increment or decrement the counter for that group;
//   the threshold for action for that group; the action (possibly a
//   layer/row/col tuple for a key to call exec on, or possible just a row/col,
//   and a layer shift some other way, ...) ]
//
// - create a auto-resizing array ('flex_array'?) for use with this function,
//   and probably with the the layer stack functions as well.  IDs can be
//   treated as array indices (the burden being on the user not to use indices
//   that are too large...)
//
// TODO: kf__macro__eeprom
// - this should probably go into its own little place in 'lib'; it'll need a
//   function to write the macro to memory, code to keep track of what's
//   written in the eeprom, and stuff like that
//
// TODO: kf__macro__sram
// - is this necessary?  will it be confusing (if we already have
//   kf__macro__eeprom)?  we should probably remove it.
//
// TODO: rewrite layouts to reflect
// - kf__two_keys_capslock() -> kf__toggle_capslock()
// - the removal of kf__send()

// device
void kf__jump_to_bootloader (uint16_t ignore);

// special
void kf__toggle_capslock (uint16_t ignore);


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#endif  // ERGODOX_FIRMWARE__FIRMWARE__LIB__LAYOUT__KEY_FUNCTIONS__H



// ============================================================================
// === documentation ==========================================================
// ============================================================================


// ----------------------------------------------------------------------------
// typedefs -------------------------------------------------------------------
// ----------------------------------------------------------------------------

// === kf__function_pointer_t ===
/**                                 typedefs/kf__function_pointer_t/description
 * The pointer type for all key-functions
 *
 * Notes:
 * - All key-functions must have the same type so that we can easily store
 *   pointers to them, and call them using the pointers without knowing which
 *   function specifically is being pointed to.
 */


// ----------------------------------------------------------------------------
// functions ------------------------------------------------------------------
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// basic ----------------------------------------------------------------------

// === kf__press() ===
/**                                             functions/kf__press/description
 * Generate a normal keypress (and send the USB report).
 *
 * Arguments:
 * - `keycode`: The keycode to "press"
 */

// === kf__release() ===
/**                                           functions/kf__release/description
 * Generate a normal keyrelease (and send the USB report).
 *
 * Arguments:
 * - `keycode`: The keycode to "release"
 */

// === kf__toggle() ===
/**                                            functions/kf__toggle/description
 * Toggle the key pressed or unpressed.
 *
 * Arguments:
 * - `keycode`: The keycode to "toggle"
 */

// === kf__layer__push() ===
/**                                       functions/kf__layer__push/description
 * If no other layer element with the given ID exists, push a layer element
 * with the given ID and layer number to the top of the stack.  If a layer
 * element with the given ID does already exist, update that element with the
 * given layer number.
 *
 * Arguments:
 * - `id__layer`:
 *     - `id`: The high byte of this `uint16_t` argument.  The ID of the layer
 *       element to push.
 *     - `layer`: The low byte of this `uint16_t` argument.  The layer number
 *       of the layer to push.
 */

// === kf__layer__pop() ===
/**                                        functions/kf__layer__pop/description
 * Pop the layer element with the given ID out of the layer stack (no matter
 * where it is, but without touching the order of any of the other elements),
 * if it exists.
 *
 * Arguments:
 * - `id__layer`:
 *     - `id`: The high byte of this `uint16_t` argument.  The ID of the layer
 *       element to pop.
 *     - `ignore`: The low byte of this `uint16_t` argument.  To be ignored.
 */

// === kf__macro__sram() ===
/**                                       functions/kf__macro__sram/description
 * Execute the macro at the given location in SRAM.
 *
 * Arguments:
 * - `pointer`: A pointer to the location in SRAM where the macro begins
 *
 * Notes:
 * - Macros are arrays of `uint16_t`s with the following format:
 *     - `[0]`: A count of how many ("function pointer", "argument") pairs
 *       follow
 *     - `[1]`: A pointer to a key-function
 *     - `[2]`: The argument to be passed to the preceding key-function
 *     - ...
 */

// === kf__macro__progmem() ===
/**                                    functions/kf__macro__progmem/description
 * Execute the macro at the given location in PROGMEM.
 *
 * Arguments:
 * - `pointer`: A pointer to the location in PROGMEM where the macro begins
 *
 * Notes:
 * - Macros are arrays of `uint16_t`s with the following format:
 *     - `[0]`: A count of how many ("function pointer", "argument") pairs
 *       follow
 *     - `[1]`: A pointer to a key-function
 *     - `[2]`: The argument to be passed to the preceding key-function
 *     - ...
 */

// === kf__macro__eeprom() ===
/**                                     functions/kf__macro__eeprom/description
 * Execute the macro at the given location in the EEPROM.
 *
 * Arguments:
 * - `pointer`: A pointer to the location in the EEPROM where the macro begins
 *
 * Notes:
 * - Macros are arrays of `uint16_t`s with the following format:
 *     - `[0]`: A count of how many ("function pointer", "argument") pairs
 *       follow
 *     - `[1]`: A pointer to a key-function
 *     - `[2]`: The argument to be passed to the preceding key-function
 *     - ...
 */


// ----------------------------------------------------------------------------
// device ---------------------------------------------------------------------

// === kf__jump_to_bootloader() ===
/**                                functions/kf__jump_to_bootloader/description
 * For reflashing the controller.
 *
 * Arguments:
 * - `ignore`: [ignore]
 */


// ----------------------------------------------------------------------------
// special --------------------------------------------------------------------

// === kf__toggle_capslock() ===
/**                                   functions/kf__toggle_capslock/description
 * Toggle the state of 'capslock'
 *
 * Arguments:
 * - `ignore`: [ignore]
 *
 * Notes:
 * - This requires special handling because neither of the shift keys may be
 *   active when 'capslock' is pressed, or it will not register properly.  This
 *   function disables both shift keys, toggles 'capslock', and then restores
 *   the state of both shift keys before exiting.
 */

