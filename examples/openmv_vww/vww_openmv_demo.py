# Run this script in OpenMV IDE
import sensor, image, time
import cexample, lcd
from pyb import LED

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QQCIF)   # Set frame size to QVGA (160x120)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create as clock object to track the FPS.
lcd.init() # Initialize the lcd screen.

while(True):
    last = time.time_ns()
    img = sensor.snapshot()         # Take a picture and return the image.
    person = cexample.VWW(img)
    lcd.display(img,2,2,1.4,2)# Take a picture and display the image.
    if person == 1:
        red_led.on()
        green_led.off()
    else:
        red_led.off()
        green_led.on()

    complete = time.time_ns()
    print('person=', person, 'time=', (complete-last)/1000000, 'ms')

