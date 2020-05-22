#include "quantum.h"
#include "backlight.h"
#include "backlight_driver_common.h"

#if !defined(BACKLIGHT_PIN) && !defined(BACKLIGHT_PINS)
#    error "Backlight pin/pins not defined. Please configure."
#endif

#if defined(BACKLIGHT_PINS)
static const pin_t backlight_pins[] = BACKLIGHT_PINS;
#    ifndef BACKLIGHT_LED_COUNT
#        define BACKLIGHT_LED_COUNT (sizeof(backlight_pins) / sizeof(pin_t))
#    endif

#    define FOR_EACH_LED(x)                                 \
        for (uint8_t i = 0; i < BACKLIGHT_LED_COUNT; i++) { \
            pin_t backlight_pin = backlight_pins[i];        \
            { x }                                           \
        }
#else
// we support only one backlight pin
static const pin_t backlight_pin = BACKLIGHT_PIN;
#    define FOR_EACH_LED(x) x
#endif

static inline void backlight_on(pin_t backlight_pin) {
#if BACKLIGHT_ON_STATE == 0
    writePinLow(backlight_pin);
#else
    writePinHigh(backlight_pin);
#endif
}

static inline void backlight_off(pin_t backlight_pin) {
#if BACKLIGHT_ON_STATE == 0
    writePinHigh(backlight_pin);
#else
    writePinLow(backlight_pin);
#endif
}

void backlight_pins_init(void) {
    // Setup backlight pin as output and output to off state.
    FOR_EACH_LED(setPinOutput(backlight_pin); backlight_off(backlight_pin);)
}

void backlight_pins_on(void) { FOR_EACH_LED(backlight_on(backlight_pin);) }

void backlight_pins_off(void) { FOR_EACH_LED(backlight_off(backlight_pin);) }

// See http://jared.geek.nz/2013/feb/linear-led-pwm
uint16_t cie_lightness(uint16_t v) {
    if (v <= 5243)     // if below 8% of max
        return v / 9;  // same as dividing by 900%
    else {
        uint32_t y = (((uint32_t)v + 10486) << 8) / (10486 + 0xFFFFUL);  // add 16% of max and compare
        // to get a useful result with integer division, we shift left in the expression above
        // and revert what we've done again after squaring.
        y = y * y * y >> 8;
        if (y > 0xFFFFUL)  // prevent overflow
            return 0xFFFFU;
        else
            return (uint16_t)y;
    }
}

#if defined(BACKLIGHT_BREATHING)

/* To generate breathing curve in python:
 * from math import sin, pi; [int(sin(x/128.0*pi)**4*255) for x in range(128)]
 */
static const uint8_t breathing_table[BREATHING_STEPS]
#if defined(__AVR__)
PROGMEM
#endif
= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 17, 20, 24, 28, 32, 36, 41, 46, 51, 57, 63, 70, 76, 83, 91, 98, 106, 113, 121, 129, 138, 146, 154, 162, 170, 178, 185, 193, 200, 207, 213, 220, 225, 231, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254, 253, 252, 250, 247, 244, 240, 235, 231, 225, 220, 213, 207, 200, 193, 185, 178, 170, 162, 154, 146, 138, 129, 121, 113, 106, 98, 91, 83, 76, 70, 63, 57, 51, 46, 41, 36, 32, 28, 24, 20, 17, 15, 12, 10, 8, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t breathing_curve(uint8_t index)
{
#if defined(__AVR__)
    return pgm_read_byte(&breathing_table[index]);
#else
    return breathing_table[index];
#endif
}


#endif
