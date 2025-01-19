import machine
import json
import time

class PicoCommunication:
    def __init__(self):
        self.uart = machine.UART(1, baudrate=115200, timeout=1000)
        self.command_queue = []
        self.last_command_time = time.ticks_ms()
        self.retry_count = 0
        self.MAX_RETRIES = 3
        
    def send_command(self, command):
        try:
            # Komut sıklığını kontrol et
            if time.ticks_diff(time.ticks_ms(), self.last_command_time) < 50:
                self.command_queue.append(command)
                return False
                
            encoded_command = json.dumps(command).encode()
            self.uart.write(encoded_command)
            self.last_command_time = time.ticks_ms()
            return True
            
        except Exception as e:
            print(f"Komut gönderme hatası: {e}")
            if self.retry_count < self.MAX_RETRIES:
                self.retry_count += 1
                return self.send_command(command)
            return False
            
    def process_queue(self):
        while self.command_queue:
            if self.send_command(self.command_queue[0]):
                self.command_queue.pop(0)
        
    def read_sensor_data(self):
        if self.uart.any():
            try:
                return json.loads(self.uart.readline())
            except:
                return None 