# Generates the breathing curves found in backlight_driver_common.c

from math import cos, sin, pi

def print_curve(name, values) :
    valueString = ', '.join(str(i) for i in values)
    print(f'\n    // {name}:\n    {valueString}')

print_curve('Half Sine Wave to the Fourth Power',
            (int(sin(x/128.0*pi)**4*255) for x in range(128)))

print_curve('Sinesoidal Wave',
            (int(round(255*(cos((1+2*x/127)*pi)/2+0.5))) for x in range(128)))

print_curve('Sawtooth Wave',
            (int(round(255*x/127)) for x in range(128)))

print_curve('Triangular Wave',
            (int(round(255*min(x,128-x)/64)) for x in range(128)))

print_curve('Trapezoidal Wave',
            (int(round(255*min(1,max(0,min(x,128-x)/32-0.5)))) for x in range(128)))
