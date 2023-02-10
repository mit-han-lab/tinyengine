# This example shows how to invoke training or inference function calls of tinyengine.
import cexample
import lcd
import sensor

sensor.reset()  # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)  # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.LCD)  # Set frame size to QVGA 160x128
lcd.init()  # Initialize the lcd screen.

while True:
    img = sensor.snapshot()  # Take a picture and return the image.

    ret = cexample.person_detection(img, 0.15)
    lcd.display(img)  # Display the image.
