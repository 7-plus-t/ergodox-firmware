/* ----------------------------------------------------------------------------
 * ergoDOX : layout : DEFAULT : exports
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (MIT) (see "license.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */


#ifndef KEYBOARD__ERGODOX__LAYOUT__DEFAULT_h
	#define KEYBOARD__ERGODOX__LAYOUT__DEFAULT_h

	#include "../controller.h"

	// --------------------------------------------------------------------

	#define kb_led_layer1_on()      _kb_led_1_on()
	#define kb_led_layer1_off()     _kb_led_1_off()
	#define kb_led_layer2_on()      _kb_led_2_on()
	#define kb_led_layer2_off()     _kb_led_2_off()
    #define kb_led_layer3_on()      _kb_led_3_on()
    #define kb_led_layer3_off()     _kb_led_3_off()
	#define kb_led_caps_on()        _kb_led_6_on()
	#define kb_led_caps_on()        _kb_led_6_off()

	// --------------------------------------------------------------------

	#include "./default--led-control.h"
	#include "./default--matrix-control.h"

#endif

