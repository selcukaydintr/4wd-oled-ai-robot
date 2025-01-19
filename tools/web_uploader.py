import os
import shutil
from esp32_uploader import ESP32Uploader

class WebUploader:
    def __init__(self, esp32_port='/dev/ttyUSB0'):
        self.esp32 = ESP32Uploader(port=esp32_port)
        
    def upload_web_files(self):
        # Vue projesini derle
        os.system('cd frontend && npm run build')
        
        # Derlenen dosyaları ESP32'ye kopyala
        web_dir = 'frontend/dist'
        esp32_web_dir = 'esp32cam/www'
        
        # Klasörü temizle ve kopyala
        if os.path.exists(esp32_web_dir):
            shutil.rmtree(esp32_web_dir)
        shutil.copytree(web_dir, esp32_web_dir)
        
        # ESP32'ye yükle
        self.esp32.prepare_device()
        self.esp32.upload_files('esp32cam')
        
if __name__ == '__main__':
    uploader = WebUploader()
    uploader.upload_web_files() 