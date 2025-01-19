import os
from machine import SDCard
import json

class Storage:
    def __init__(self):
        try:
            self.sd = SDCard(slot=2)  # ESP32-CAM SD kart slotu
            os.mount(self.sd, '/sd')
            self.root = '/sd'
        except:
            print("SD kart bulunamadı, dahili depolama kullanılıyor")
            self.root = '/'
    
    def read_config(self):
        try:
            with open(f'{self.root}/config.json', 'r') as f:
                return json.load(f)
        except:
            # Varsayılan config değerleri
            default_config = {
                'wifi_ssid': 'default_ssid',
                'wifi_password': 'default_password'
            }
            # Dosya yoksa oluştur
            self.save_config(default_config)
            return default_config
    
    def save_config(self, config):
        with open(f'{self.root}/config.json', 'w') as f:
            json.dump(config, f)
    
    def list_files(self, path):
        full_path = f'{self.root}/{path}'
        try:
            return os.listdir(full_path)
        except:
            return [] 