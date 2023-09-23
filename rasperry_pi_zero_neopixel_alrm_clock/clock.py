import datetime
import board
import neopixel
pixel_pin = board.D18
num_pixels = 30
ORDER = neopixel.GRB
pixels = neopixel.NeoPixel(
    pixel_pin, num_pixels, brightness=0.5, auto_write=False, pixel_order=ORDER
)


def rainbow_cycle(wait):
    for j in range(255):
        for i in range(num_pixels):
            pixel_index = (i * 256 // num_pixels) + j
            pixels[i] = wheel(pixel_index & 255)
        pixels.show()
        time.sleep(wait)


def display_7_segment(color, digit, index):
    NEO_OFF = (0, 0, 0)
    if digit == 0:
        pixels[index] = NEO_OFF
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = color
        pixels[index + 4] = color
        pixels[index + 5] = color
        pixels[index + 6] = color
        return
    if digit == 1:
        pixels[index] = NEO_OFF
        pixels[index + 1] = color
        pixels[index + 2] = NEO_OFF
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = NEO_OFF
        pixels[index + 5] = NEO_OFF
        pixels[index + 6] = color
        return
    if digit == 2:
        pixels[index] = color
        pixels[index + 1] = NEO_OFF
        pixels[index + 2] = color
        pixels[index + 3] = color
        pixels[index + 4] = NEO_OFF
        pixels[index + 5] = color
        pixels[index + 6] = color
        return
    if digit == 3:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = NEO_OFF
        pixels[index + 5] = color
        pixels[index + 6] = color
        return
    if digit == 4:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = NEO_OFF
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = color
        pixels[index + 5] = NEO_OFF
        pixels[index + 6] = color
        return
    if digit == 5:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = color
        pixels[index + 5] = color
        pixels[index + 6] = NEO_OFF
        return
    if digit == 6:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = color
        pixels[index + 4] = color
        pixels[index + 5] = color
        pixels[index + 6] = NEO_OFF
        return
    if digit == 7:
        pixels[index] = NEO_OFF
        pixels[index + 1] = color
        pixels[index + 2] = NEO_OFF
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = NEO_OFF
        pixels[index + 5] = color
        pixels[index + 6] = color
        return
    if digit == 8:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = color
        pixels[index + 4] = color
        pixels[index + 5] = color
        pixels[index + 6] = color
        return
    if digit == 9:
        pixels[index] = color
        pixels[index + 1] = color
        pixels[index + 2] = color
        pixels[index + 3] = NEO_OFF
        pixels[index + 4] = color
        pixels[index + 5] = color
        pixels[index + 6] = color
        return


t = datetime.datetime.now()
hour = t.hour
hour1 = hour // 10
hour2 = hour % 10
minute = t.minute
minute1 = minute // 10
minute2 = minute % 10

color = (0, 0, 255)

display_7_segment(color, hour1, 0)
display_7_segment(color, hour2, 7)
if t.second % 2 == 0:
    pixels[14] = color
    pixels[15] = color
else:
    pixels[14] = (0, 0, 0)
    pixels[15] = (0, 0, 0)
display_7_segment(color, minute1, 16)
display_7_segment(color, minute2, 23)
pixels.show()
