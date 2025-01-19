import os
import serial
import time

class ESP32Uploader:
    def __init__(self, port='/dev/ttyUSB0', baud_rate=115200):
        self.port = port
        self.baud_rate = baud_rate
        
    def prepare_device(self):
        # ESP32'yi flash moduna al
        print("ESP32 flash moduna alınıyor...")
        # GPIO0'ı GND'ye bağlayın ve reset'e basın
        input("GPIO0'ı GND'ye bağlayın, reset'e basın ve Enter'a basın...")
        
    def upload_files(self, files_dir):
        try:
            # Dosyaları yükle
            with serial.Serial(self.port, self.baud_rate) as ser:
                for root, _, files in os.walk(files_dir):
                    for file in files:
                        if file.endswith('.py'):
                            self.upload_file(ser, os.path.join(root, file))

            print("ESP32-CAM'a dosyalar başarıyla yüklendi!")
            
        except Exception as e:
            print(f"Yükleme hatası: {e}")
            
    def upload_file(self, ser, file_path):
        # Dosya yükleme fonksiyonu
        print(f"Yükleniyor: {file_path}")
        with open(file_path, 'r') as file:
            content = file.read()
            # MicroPython dosyasını yükleme
            ser.write(content.encode())
            time.sleep(0.1)  # Yavaşlatma
        print(f"Dosya {file_path} başarıyla yüklendi!")
