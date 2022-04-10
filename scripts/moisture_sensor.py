import time

import board

from adafruit_seesaw.seesaw import Seesaw

i2c_bus = board.I2C()

ss = Seesaw(i2c_bus, addr=0x36)

def getMoisture():
    return ss.moisture_read()

def getTemp():
    return ss.get_temp()

with open("tmp/moisture.dat", "w") as moist_f:
    moist_raw = getMoisture()
    moist_f.writelines("{}\n".format(moist_raw))

    moist_temp = getTemp()
    moist_f.writelines("{}\n".format(moist_temp))