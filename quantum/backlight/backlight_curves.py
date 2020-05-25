# Generates the breathing curves found in backlight_driver_common.c

from math import cos, sin, pi

def print_curve(name, values) :
    valueString = ', '.join(f'd({i:.3f})' for i in values)
    print(f'\n    // {name}:\n    {valueString}')

print_curve('Half Sine Wave to the Fourth Power',
            (sin(pi*x/127)**4 for x in range(128)))

print_curve('Sinesoidal Wave',
            (cos(pi*(1+2*x/127))/2+0.5 for x in range(128)))

print_curve('Sawtooth Wave',
            (x/127 for x in range(128)))

print_curve('Triangular Wave',
            (min(x,128-x)/64 for x in range(128)))

print_curve('Trapezoidal Wave',
            (min(1,max(0,min(x,128-x)/32-0.5)) for x in range(128)))
