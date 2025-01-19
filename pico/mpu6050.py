from machine import Pin, I2C

class MPU6050:
    def __init__(self, i2c, addr=0x68):
        self.i2c = i2c
        self.addr = addr
        self.i2c.writeto_mem(self.addr, 0x6B, bytes([0]))
    
    def get_raw_values(self):
        raw_data = self.i2c.readfrom_mem(self.addr, 0x3B, 14)
        return raw_data
    
    def get_values(self):
        raw = self.get_raw_values()
        return {
            'ax': self.bytes_to_int(raw[0:2]) / 16384.0,
            'ay': self.bytes_to_int(raw[2:4]) / 16384.0,
            'az': self.bytes_to_int(raw[4:6]) / 16384.0,
            'gx': self.bytes_to_int(raw[8:10]) / 131.0,
            'gy': self.bytes_to_int(raw[10:12]) / 131.0,
            'gz': self.bytes_to_int(raw[12:14]) / 131.0
        }
    
    def bytes_to_int(self, bytes_data):
        value = bytes_data[0] << 8 | bytes_data[1]
        if value & (1 << 16 - 1):
            value -= 1 << 16
        return value 