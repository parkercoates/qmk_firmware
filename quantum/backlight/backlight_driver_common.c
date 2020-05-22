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
