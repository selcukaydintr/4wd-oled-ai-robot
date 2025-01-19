from machine import I2C, Pin
from ssd1306 import SSD1306_I2C
import json
import time

class OLEDController:
    def __init__(self):
        # I2C başlat
        self.i2c = I2C(0, scl=Pin(22), sda=Pin(21))
        
        # OLED ekranları başlat
        self.displays = {
            'left': SSD1306_I2C(128, 64, self.i2c, addr=0x3C),
            'right': SSD1306_I2C(128, 64, self.i2c, addr=0x3D)
        }
        
        # Animasyon durumları
        self.animations = {
            'left': {'current': 'normal', 'frame': 0},
            'right': {'current': 'normal', 'frame': 0}
        }
        
        # Menü durumları
        self.menus = {
            'left': {'items': [], 'current': 0},
            'right': {'items': [], 'current': 0}
        }
        
    def update_display(self, side):
        display = self.displays[side]
        animation = self.animations[side]
        
        # Ekranı temizle
        display.fill(0)
        
        # Animasyonu çiz
        self.draw_animation(display, animation)
        
        # Menüyü çiz
        if self.menus[side]['items']:
            self.draw_menu(display, side)
            
        # Ekranı güncelle
        display.show()
        
    def draw_animation(self, display, animation):
        # Animasyon çizimi
        frames = self.get_animation_frames(animation['current'])
        frame = frames[animation['frame']]
        
        # Frame'i ekrana çiz
        display.text(frame, 0, 0, 1)
        
    def draw_menu(self, display, side):
        menu = self.menus[side]
        current_item = menu['items'][menu['current']]
        
        # Menü öğesini ekrana çiz
        display.text(current_item, 0, 32, 1)
        
    def update_config(self, config):
        # Yapılandırmayı güncelle
        for side in ['left', 'right']:
            if side in config:
                self.displays[side].addr = int(config[side]['address'], 16)
                self.menus[side]['items'] = config[side]['menu']['items']
                
    def run_animation_loop(self):
        while True:
            for side in ['left', 'right']:
                self.update_display(side)
            time.sleep(0.1) 