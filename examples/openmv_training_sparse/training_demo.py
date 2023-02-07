# This example shows how to invoke to training or inference function calls of tinyengine.
import cexample
import lcd
import sensor
from pyb import Pin

sensor.reset()  # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)  # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.B128X128)  # Set frame size to QVGA (128x128)
lcd.init()  # Initialize the lcd screen.


# class 1: green
pin4 = Pin("P4", Pin.IN, Pin.PULL_UP)
# class 0: red
pin1 = Pin("P1", Pin.IN, Pin.PULL_UP)

while True:
    img = sensor.snapshot()  # Take a picture and return the image.

    pin4_value = pin4.value()
    pin1_value = pin1.value()
    if pin4_value == 0:
        ret = cexample.train_demo(img, 0)
        print("train class 0")
    elif pin1_value == 0:
        ret = cexample.train_demo(img, 1)
        print("train class 1")
    else:
        ret = cexample.train_demo(img, -1)
    lcd.display(img)  # Display the image.
