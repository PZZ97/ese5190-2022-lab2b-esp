## The Accelerometer Detector

The project used a ADXL335 module to measure the acceleration and shows on the OLED screen through I2C. In the demo below, the screen shows the acceleration it detected in x,y,z axis. When it was placed flat, the z axis had the largest value due to the gravity. While changing the direction of the module, the value also changed according to gravity direction. 

![467103838760dce5f29fc3de45db987d_AdobeExpress](https://user-images.githubusercontent.com/43904091/202276012-3916f1e0-ced6-4a70-9eb4-73c4bfdc5b59.gif)

Here is the code block. The ADXL335 returned the acceleration components by analog voltage level range from -3v to +3v, then it was read by the AD module in QT PY2040, and sent to the OLED through I2C port.

```python
import board
import digitalio
import adafruit_ssd1305
import terminalio
import displayio
from adafruit_display_text import label
import adafruit_displayio_ssd1305
import busio
import time
import adafruit_mpu6050
from analogio import AnalogIn
import math

def ad2g(value):
    return math.floor((value/109)-300)/100

'''
i2c1 = board.STEMMA_I2C()   # uses board.SCL and board.SDA
#i2c2 = busio.I2C(board.SCL, board.SDA)  # uses board.SCL and board.SDA
oled_reset = digitalio.DigitalInOut(board.D4)
display = adafruit_ssd1305.SSD1305_I2C(128, 64, i2c1, addr=0x3D, reset=oled_reset)
#mpu = adafruit_mpu6050.MPU6050(i2c1,0x68)
display.fill(0)
display.show()
# Set a pixel in the origin 0,0 position.
display.pixel(0, 0, 10)
# Set a pixel in the middle 64, 16 position.
display.pixel(0, 25, 20)
# Set a pixel in the opposite 127, 31 position.
display.pixel(0, 50, 30)
display.println("hello world")
display.show()
x = AnalogIn(board.A0)
y = AnalogIn(board.A1)
z = AnalogIn(board.A2)
while(1):
    print(x.value)
    time.sleep(0.1)
'''
displayio.release_displays()
# Reset is usedfor both SPI and I2C
oled_reset = board.D7
# Use for I2C
i2c = board.I2C()
display_bus = displayio.I2CDisplay(i2c, device_address=0x3d, reset=oled_reset)
WIDTH = 128
HEIGHT = 64 # Change to 32 if needed
BORDER = 8
FONTSCALE = 1
display = adafruit_displayio_ssd1305.SSD1305(display_bus, width=WIDTH,
height=HEIGHT)
# Make the display context
splash = displayio.Group()
display.show(splash)
color_bitmap = displayio.Bitmap(display.width, display.height, 1)
color_palette = displayio.Palette(1)
color_palette[0] = 0x000000
bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
splash.append(bg_sprite)
# get the data
z = AnalogIn(board.A0)
y = AnalogIn(board.A1)
x = AnalogIn(board.A2)
# Draw a label

while True:
    text = 'x,y,z acceleration:\n'+str(ad2g(x.value))+'\n'+ str(ad2g(y.value))+'\n'+str(ad2g(z.value))
    text_area = label.Label(terminalio.FONT, text=text, color=0xFFFFFF)
    text_width = text_area.bounding_box[2] * FONTSCALE
    text_group = displayio.Group(
    scale=FONTSCALE,
    x=display.width // 2 - text_width // 2,
    y=1,
    )
    text_group.append(text_area) # Subgroup for text scaling
    splash.pop()
    splash.append(text_group)


```