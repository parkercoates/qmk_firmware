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

/* NOTE:
 * The following curve definitions were generated via the breathing_curves.py
 * script found in this same directory. If making changes to the curves, please
 * do so via the script.
 */

#define d(x) (uint8_t)(255*(x * BREATHING_DEPTH + (1 - BREATHING_DEPTH)) + 0.5)

static const uint8_t breathing_table[BREATHING_STEPS]
#if defined(__AVR__)
PROGMEM
#endif
= {
#if BREATHING_CURVE == 0
    // Half Sine Wave to the Fourth Power:
    d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.001), d(0.001), d(0.002), d(0.004), d(0.005), d(0.007), d(0.010), d(0.013), d(0.017), d(0.022), d(0.028), d(0.034), d(0.042), d(0.051), d(0.061), d(0.072), d(0.084), d(0.098), d(0.113), d(0.129), d(0.147), d(0.166), d(0.187), d(0.209), d(0.232), d(0.256), d(0.282), d(0.309), d(0.336), d(0.365), d(0.395), d(0.425), d(0.456), d(0.488), d(0.520), d(0.552), d(0.584), d(0.616), d(0.648), d(0.679), d(0.710), d(0.740), d(0.769), d(0.797), d(0.823), d(0.849), d(0.872), d(0.895), d(0.915), d(0.933), d(0.949), d(0.964), d(0.975), d(0.985), d(0.992), d(0.997), d(1.000), d(1.000), d(0.997), d(0.992), d(0.985), d(0.975), d(0.964), d(0.949), d(0.933), d(0.915), d(0.895), d(0.872), d(0.849), d(0.823), d(0.797), d(0.769), d(0.740), d(0.710), d(0.679), d(0.648), d(0.616), d(0.584), d(0.552), d(0.520), d(0.488), d(0.456), d(0.425), d(0.395), d(0.365), d(0.336), d(0.309), d(0.282), d(0.256), d(0.232), d(0.209), d(0.187), d(0.166), d(0.147), d(0.129), d(0.113), d(0.098), d(0.084), d(0.072), d(0.061), d(0.051), d(0.042), d(0.034), d(0.028), d(0.022), d(0.017), d(0.013), d(0.010), d(0.007), d(0.005), d(0.004), d(0.002), d(0.001), d(0.001), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000)
#elif BREATHING_CURVE == 1
    // Sinesoidal Wave:
    d(0.000), d(0.001), d(0.002), d(0.005), d(0.010), d(0.015), d(0.022), d(0.030), d(0.039), d(0.049), d(0.060), d(0.072), d(0.086), d(0.100), d(0.115), d(0.131), d(0.149), d(0.167), d(0.186), d(0.205), d(0.225), d(0.246), d(0.268), d(0.290), d(0.313), d(0.336), d(0.360), d(0.384), d(0.408), d(0.432), d(0.457), d(0.481), d(0.506), d(0.531), d(0.556), d(0.580), d(0.604), d(0.628), d(0.652), d(0.676), d(0.698), d(0.721), d(0.743), d(0.764), d(0.785), d(0.805), d(0.824), d(0.842), d(0.860), d(0.877), d(0.893), d(0.907), d(0.921), d(0.934), d(0.946), d(0.956), d(0.966), d(0.974), d(0.982), d(0.988), d(0.993), d(0.996), d(0.999), d(1.000), d(1.000), d(0.999), d(0.996), d(0.993), d(0.988), d(0.982), d(0.974), d(0.966), d(0.956), d(0.946), d(0.934), d(0.921), d(0.907), d(0.893), d(0.877), d(0.860), d(0.842), d(0.824), d(0.805), d(0.785), d(0.764), d(0.743), d(0.721), d(0.698), d(0.676), d(0.652), d(0.628), d(0.604), d(0.580), d(0.556), d(0.531), d(0.506), d(0.481), d(0.457), d(0.432), d(0.408), d(0.384), d(0.360), d(0.336), d(0.313), d(0.290), d(0.268), d(0.246), d(0.225), d(0.205), d(0.186), d(0.167), d(0.149), d(0.131), d(0.115), d(0.100), d(0.086), d(0.072), d(0.060), d(0.049), d(0.039), d(0.030), d(0.022), d(0.015), d(0.010), d(0.005), d(0.002), d(0.001), d(0.000)
#elif BREATHING_CURVE == 2
    // Sawtooth Wave:
    d(0.000), d(0.008), d(0.016), d(0.024), d(0.031), d(0.039), d(0.047), d(0.055), d(0.063), d(0.071), d(0.079), d(0.087), d(0.094), d(0.102), d(0.110), d(0.118), d(0.126), d(0.134), d(0.142), d(0.150), d(0.157), d(0.165), d(0.173), d(0.181), d(0.189), d(0.197), d(0.205), d(0.213), d(0.220), d(0.228), d(0.236), d(0.244), d(0.252), d(0.260), d(0.268), d(0.276), d(0.283), d(0.291), d(0.299), d(0.307), d(0.315), d(0.323), d(0.331), d(0.339), d(0.346), d(0.354), d(0.362), d(0.370), d(0.378), d(0.386), d(0.394), d(0.402), d(0.409), d(0.417), d(0.425), d(0.433), d(0.441), d(0.449), d(0.457), d(0.465), d(0.472), d(0.480), d(0.488), d(0.496), d(0.504), d(0.512), d(0.520), d(0.528), d(0.535), d(0.543), d(0.551), d(0.559), d(0.567), d(0.575), d(0.583), d(0.591), d(0.598), d(0.606), d(0.614), d(0.622), d(0.630), d(0.638), d(0.646), d(0.654), d(0.661), d(0.669), d(0.677), d(0.685), d(0.693), d(0.701), d(0.709), d(0.717), d(0.724), d(0.732), d(0.740), d(0.748), d(0.756), d(0.764), d(0.772), d(0.780), d(0.787), d(0.795), d(0.803), d(0.811), d(0.819), d(0.827), d(0.835), d(0.843), d(0.850), d(0.858), d(0.866), d(0.874), d(0.882), d(0.890), d(0.898), d(0.906), d(0.913), d(0.921), d(0.929), d(0.937), d(0.945), d(0.953), d(0.961), d(0.969), d(0.976), d(0.984), d(0.992), d(1.000)
#elif BREATHING_CURVE == 3
    // Triangular Wave:
    d(0.000), d(0.016), d(0.031), d(0.047), d(0.062), d(0.078), d(0.094), d(0.109), d(0.125), d(0.141), d(0.156), d(0.172), d(0.188), d(0.203), d(0.219), d(0.234), d(0.250), d(0.266), d(0.281), d(0.297), d(0.312), d(0.328), d(0.344), d(0.359), d(0.375), d(0.391), d(0.406), d(0.422), d(0.438), d(0.453), d(0.469), d(0.484), d(0.500), d(0.516), d(0.531), d(0.547), d(0.562), d(0.578), d(0.594), d(0.609), d(0.625), d(0.641), d(0.656), d(0.672), d(0.688), d(0.703), d(0.719), d(0.734), d(0.750), d(0.766), d(0.781), d(0.797), d(0.812), d(0.828), d(0.844), d(0.859), d(0.875), d(0.891), d(0.906), d(0.922), d(0.938), d(0.953), d(0.969), d(0.984), d(1.000), d(0.984), d(0.969), d(0.953), d(0.938), d(0.922), d(0.906), d(0.891), d(0.875), d(0.859), d(0.844), d(0.828), d(0.812), d(0.797), d(0.781), d(0.766), d(0.750), d(0.734), d(0.719), d(0.703), d(0.688), d(0.672), d(0.656), d(0.641), d(0.625), d(0.609), d(0.594), d(0.578), d(0.562), d(0.547), d(0.531), d(0.516), d(0.500), d(0.484), d(0.469), d(0.453), d(0.438), d(0.422), d(0.406), d(0.391), d(0.375), d(0.359), d(0.344), d(0.328), d(0.312), d(0.297), d(0.281), d(0.266), d(0.250), d(0.234), d(0.219), d(0.203), d(0.188), d(0.172), d(0.156), d(0.141), d(0.125), d(0.109), d(0.094), d(0.078), d(0.062), d(0.047), d(0.031), d(0.016)
#elif BREATHING_CURVE == 4
    // Trapezoidal Wave:
    d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.031), d(0.062), d(0.094), d(0.125), d(0.156), d(0.188), d(0.219), d(0.250), d(0.281), d(0.312), d(0.344), d(0.375), d(0.406), d(0.438), d(0.469), d(0.500), d(0.531), d(0.562), d(0.594), d(0.625), d(0.656), d(0.688), d(0.719), d(0.750), d(0.781), d(0.812), d(0.844), d(0.875), d(0.906), d(0.938), d(0.969), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(1.000), d(0.969), d(0.938), d(0.906), d(0.875), d(0.844), d(0.812), d(0.781), d(0.750), d(0.719), d(0.688), d(0.656), d(0.625), d(0.594), d(0.562), d(0.531), d(0.500), d(0.469), d(0.438), d(0.406), d(0.375), d(0.344), d(0.312), d(0.281), d(0.250), d(0.219), d(0.188), d(0.156), d(0.125), d(0.094), d(0.062), d(0.031), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000), d(0.000)
#else
#    error "Unrecognized BREATHING_CURVE value!"
#endif
};

uint8_t breathing_curve(uint8_t index)
{
#if defined(__AVR__)
    return pgm_read_byte(&breathing_table[index]);
#else
    return breathing_table[index];
#endif
}


#endif
