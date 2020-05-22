#pragma once

void backlight_pins_init(void);
void backlight_pins_on(void);
void backlight_pins_off(void);

void breathing_task(void);

uint16_t cie_lightness(uint16_t v);
