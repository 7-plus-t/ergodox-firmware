/* ----------------------------------------------------------------------------
 * ergoDOX : layout : default LED control
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012 Ben Blazak <benblazak.dev@gmail.com>
 * Released under The MIT License (MIT) (see "license.md")
 * Project located at <https://github.com/benblazak/ergodox-firmware>
 * ------------------------------------------------------------------------- */


#ifndef KEYBOARD__ERGODOX__LAYOUT__DEFAULT__LED_CONTROL_h
	#define KEYBOARD__ERGODOX__LAYOUT__DEFAULT__LED_CONTROL_h

	// --------------------------------------------------------------------

	/*
	 * state and delay macros
	 */

	#ifndef kb_led_state_power_on
	#define kb_led_state_power_on() do {				\
			_kb_led_all_set_percent(MAKEFILE_LED_BRIGHTNESS/10); \
			_kb_led_all_on();				\
            _delay_ms(333);					\
            _kb_led_all_off();	\
			} while(0)
	#endif

	// note: need to delay for a total of ~1 second
	#ifndef kb_led_delay_usb_init
	#define kb_led_delay_usb_init() do {				\
			_kb_led_1_set_percent(MAKEFILE_LED_BRIGHTNESS);	\
            _kb_led_1_on(); \
			_delay_ms(333);					\
			_kb_led_2_set_percent(MAKEFILE_LED_BRIGHTNESS);	\
            _kb_led_2_on(); \
			_delay_ms(333);					\
			_kb_led_3_set_percent(MAKEFILE_LED_BRIGHTNESS);	\
            _kb_led_3_on(); \
			_delay_ms(333);					\
            _kb_led_3_off(); \
			_delay_ms(333);					\
            _kb_led_2_off(); \
			_delay_ms(333);					\
            _kb_led_1_off(); \
			_delay_ms(333);					\
			} while(0)
	#endif

	#ifndef kb_led_state_ready
	#define kb_led_state_ready() do {				\
			_kb_led_all_off();	\
            _delay_ms(333);	\
            _kb_led_6_on();	\
            _delay_ms(333);	\
            _kb_led_6_off();	\
			} while(0)
	#endif


	/*
	 * logical LED macros
	 * - unused macros should be defined to nothing
	 *   - they all are here, because they really need to be specified in
	 *     the layout specific file
	 */

	#ifndef kb_led_num_on
	#define kb_led_num_on()
	#endif
	#ifndef kb_led_num_off
	#define kb_led_num_off()
	#endif
	#ifndef kb_led_caps_on
	#define kb_led_caps_on()
	#endif
	#ifndef kb_led_caps_off
	#define kb_led_caps_off()
	#endif
	#ifndef kb_led_scroll_on
	#define kb_led_scroll_on()
	#endif
	#ifndef kb_led_scroll_off
	#define kb_led_scroll_off()
	#endif
	#ifndef kb_led_compose_on
	#define kb_led_compose_on()
	#endif
	#ifndef kb_led_compose_off
	#define kb_led_compose_off()
	#endif
	#ifndef kb_led_kana_on
	#define kb_led_kana_on()
	#endif
	#ifndef kb_led_kana_off
	#define kb_led_kana_off()
	#endif
    #ifndef kb_led_layer1_on
    #define kb_led_layer1_on()
    #endif
    #ifndef kb_led_layer1_off
    #define kb_led_layer1_off()
    #endif
    #ifndef kb_led_layer2_on
    #define kb_led_layer2_on()
    #endif
    #ifndef kb_led_layer2_off
    #define kb_led_layer2_off()
    #endif
    #ifndef kb_led_layer3_on
    #define kb_led_layer3_on()
    #endif
    #ifndef kb_led_layer3_off
    #define kb_led_layer3_off()
    #endif
    #ifndef kb_led_layer4_on
    #define kb_led_layer4_on()
    #endif
    #ifndef kb_led_layer4_off
    #define kb_led_layer4_off()
    #endif
    #ifndef kb_led_layer5_on
    #define kb_led_layer5_on()
    #endif
    #ifndef kb_led_layer5_off
    #define kb_led_layer5_off()
    #endif
    #ifndef kb_led_layer6_on
    #define kb_led_layer6_on()
    #endif
    #ifndef kb_led_layer6_off
    #define kb_led_layer6_off()
    #endif
    #ifndef kb_led_layer7_on
    #define kb_led_layer7_on()
    #endif
    #ifndef kb_led_layer7_off
    #define kb_led_layer7_off()
    #endif
    #ifndef kb_led_layer8_on
    #define kb_led_layer8_on()
    #endif
    #ifndef kb_led_layer8_off
    #define kb_led_layer8_off()
    #endif
    #ifndef kb_led_layer9_on
    #define kb_led_layer9_on()
    #endif
    #ifndef kb_led_layer9_off
    #define kb_led_layer9_off()
    #endif
    #ifndef kb_led_layer10_on
    #define kb_led_layer10_on()
    #endif
    #ifndef kb_led_layer10_off
    #define kb_led_layer10_off()
    #endif


#endif

