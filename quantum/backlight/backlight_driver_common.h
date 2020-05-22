#pragma once

void backlight_pins_init(void);
void backlight_pins_on(void);
void backlight_pins_off(void);

void breathing_task(void);

uint16_t cie_lightness(uint16_t v);

#if defined(BACKLIGHT_BREATHING)

#define BREATHING_STEPS 128
uint8_t breathing_curve(uint8_t index);

#endif
