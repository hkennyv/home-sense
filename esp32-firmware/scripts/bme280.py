"""
bme280.py
author(s): khuynh
description: a script to test and explore the bme280 chip. it can be used to
prototype data or validate hardware.
"""
from smbus2 import SMBus

def concat_20bit(blist):
    return (blist[0] & 0x0007) + (blist[1] << 8) + (blist[2] << 16)

BME_ADDR = 0x76

with SMBus(1) as bus:
    # read the ID register of the chip, should output 0x60
    bme280_id = bus.read_byte_data(BME_ADDR, 0xD0)
    print(format(bme280_id, '#04X'))

    # burst read pressure and temp
    #  pressure - 0xF7, 0xF8, 0xF9
    #  temp - 0xFA, 0xFB, 0xFC
    res = bus.read_i2c_block_data(BME_ADDR, 0xF7, 6)
    pressure = concat_20bit(res[0:3])
    temp = concat_20bit(res[3:6])

    print(f'{pressure=} {temp=}')
