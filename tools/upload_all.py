from esp32_uploader import ESP32Uploader
from pico_uploader import PicoUploader
import os
import argparse

def main():
    parser = argparse.ArgumentParser(description='Robot dosyalarını yükle')
    parser.add_argument('--esp32-port', default='/dev/ttyUSB0',
                      help='ESP32-CAM seri port')
    parser.add_argument('--esp32-files', default='esp32cam',
                      help='ESP32-CAM dosyaları klasörü')
    parser.add_argument('--pico-files', default='pico',
                      help='Pico dosyaları klasörü')
    
    args = parser.parse_args()
    
    # ESP32-CAM'a yükle
    print("ESP32-CAM'a dosyalar yükleniyor...")
    esp32 = ESP32Uploader(port=args.esp32_port)
    esp32.prepare_device()
    esp32.upload_files(args.esp32_files)
    
    # Pico'ya yükle
    print("\nPico'ya dosyalar yükleniyor...")
    pico = PicoUploader()
    pico.upload_files(args.pico_files)

if __name__ == '__main__':
    main() 