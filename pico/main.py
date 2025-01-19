from machine import Pin, PWM, UART, I2C
import time
import json
from motor_control import MotorController
from sensor_manager import SensorManager

class PicoController:
    def __init__(self):
        self.motors = MotorController()
        self.sensors = SensorManager()
        self.uart = UART(0, baudrate=115200)
        
    def process_command(self, command):
        if command['type'] == 'move':
            self.motors.move(command['left'], command['right'])
        elif command['type'] == 'sensor_request':
            self.send_sensor_data()
            
    def send_sensor_data(self):
        data = self.sensors.get_sensor_data()
        self.uart.write(json.dumps(data).encode())

if __name__ == '__main__':
    controller = PicoController()
    while True:
        if controller.uart.any():
            cmd = controller.uart.readline().decode('utf-8').strip()
            try:
                cmd = json.loads(cmd)
                controller.process_command(cmd)
            except:
                print("Geçersiz komut")
        time.sleep(0.1)  # CPU yükünü azalt