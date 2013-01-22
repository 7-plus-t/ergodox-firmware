# ----------------------------------------------------------------------------
# Global options
# ----------------------------------------------------------------------------
# Copyright (c) 2013 Ben Blazak <benblazak.dev@gmail.com>
# Released under The MIT License (see "doc/license.md")
# Project located at <https://github.com/benblazak/ergodox-firmware>
# ----------------------------------------------------------------------------

# TODO

ifeq($(MAKE__KEYBOARD),ergodox)
	# TODO: move this part of the options to keyboard/ergodox/options.mk

	MCU := atmega32u4
	# processor type (for the teensy 2.0)

	F_CPU := 16000000
	# processor speed, in Hz

	MAKE__LED_BRIGHTNESS := 0.5
	# a multiplier, with 1 being the max

	MAKE__DEBOUNCE_TIME := 5
	# in milliseconds; 5ms should be good for cherry mx switches (per the
	# keyswitch spec)

	MAKE__TWI_FREQ := 400000
	# TWI frequency, in Hz; should be no greater than 400000 (400kHz) (per
	# the Teensy datasheet sec 20.1); we want it as fast as possible


	# .....................................................................
	# DRIVE_ROWS and DRIVE_COLUMNS
	# .....................................................................
	# Select which set of pins will drive (alternate between hi-Z and drive
	# low) and which will be inputs (hi-Z)
	#
	#
	# Notes
	#
	# - You must set exactly one of each 'TEENSY' variable, and one of each
	#   'MCP23018' variable, to '1', and the other must be set to '0'
	#
	# - If you are using internal diodes (inside the key switches), set
	#   MAKE__TEENSY__DRIVE_COLUMNS := 1
	#   MAKE__MCP23018__DRIVE_ROWS  := 1
	#
	# - If the diode cathode is towards the square solder pad, set
	#   MAKE__TEENSY__DRIVE_COLUMNS   := 1
	#   MAKE__MCP23018__DRIVE_COLUMNS := 1
	#
	# - If the diode cathode is towards the circular solder pad, set
	#   MAKE__TEENSY__DRIVE_ROWS   := 1
	#   MAKE__MCP23018__DRIVE_ROWS := 1
	# .....................................................................

	# set variables
	MAKE__TEENSY__DRIVE_ROWS    := 0
	MAKE__TEENSY__DRIVE_COLUMNS := 1
	MAKE__MCP23018__DRIVE_ROWS    := 0
	MAKE__MCP23018__DRIVE_COLUMNS := 1

	# check variables
	ERROR := "see the 'DRIVE_ROWS and DRIVE_COLUMNS section in 'options.mk'"
	# --- teensy
	ifeq($(MAKE__TEENSY__DRIVE_ROWS),1)
		ifneq($(MAKE__TEENSY__DRIVE_COLUMNS),0)
			$(error $(ERROR))
		endif
	else
		ifeq($(MAKE__TEENSY__DRIVE_ROWS),0)
			ifneq($(MAKE__TEENSY__DRIVE_COLUMNS),1)
				$(error $(ERROR))
			endif
		else
			$(error $(ERROR))
		endif
	endif
	# --- mcp23018
	ifeq($(MAKE__MCP23018__DRIVE_ROWS),1)
		ifneq($(MAKE__MCP23018__DRIVE_COLUMNS),0)
			$(error $(ERROR))
		endif
	else
		ifeq($(MAKE__MCP23018__DRIVE_ROWS),0)
			ifneq($(MAKE__MCP23018__DRIVE_COLUMNS),1)
				$(error $(ERROR))
			endif
		else
			$(error $(ERROR))
		endif
	endif
	# .....................................................................
	# .....................................................................

endif



