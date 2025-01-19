from machine import Pin, UART, SPI, WDT
import camera
import network
import socket
import json
import gc
import asyncio
from microdot import Microdot, send_file
from microdot.websocket import with_websocket
from storage import Storage
from multimodal_ml import MultimodalTinyML
from communication import RobotCommunication
from web_server import WebServer
from robot_controller import RobotController
from wifi_manager import WiFiManager
import time

class RobotSystem:
    def __init__(self):
        self.plugins = {}
        self.uart = UART(2, baudrate=115200)  # UART2 için
        self.setup_camera()
        self.setup_wifi()
        self.app = Microdot()
        self.setup_routes()
        self.storage = Storage()
        self.ml_system = MultimodalTinyML()
        self.communication = RobotCommunication()
        self.controller = RobotController()
        self.web_server = WebServer(self)
        self.wdt = WDT(timeout=30000)  # 30 sn
        self.running = True
        
        try:
            self.storage = Storage()
            self.ml_system = MultimodalTinyML()
            self.communication = RobotCommunication()
            self.controller = RobotController()
            self.web_server = WebServer(self)
        except Exception as e:
            print(f"Başlatma hatası: {e}")
            self.running = False
            
    def setup_camera(self):
        camera.init(0, format=camera.JPEG)
        camera.quality(10)  # Düşük bellek kullanımı için
        camera.framesize(camera.FRAME_VGA)
    
    def setup_wifi(self):
        self.wlan = network.WLAN(network.STA_IF)
        self.wlan.active(True)
        # WiFi ayarlarını config.json'dan oku
        with open('config.json') as f:
            config = json.load(f)
        self.wlan.connect(config['wifi_ssid'], config['wifi_password'])
    
    def setup_routes(self):
        @self.app.route('/')
        def index(request):
            return send_file('www/index.html')
            
        @self.app.route('/api/camera')
        def get_frame(request):
            buf = camera.capture()
            return send_file(buf, content_type='image/jpeg')
            
        @self.app.route('/ws')
        @with_websocket
        def handle_websocket(request, ws):
            while True:
                data = ws.receive()
                if data:
                    # UART üzerinden Pico'ya komut gönder
                    self.uart.write(f"{data}\n")
                
                # Pico'dan gelen verileri kontrol et
                if self.uart.any():
                    response = self.uart.readline()
                    ws.send(response)
                    
                gc.collect()  # Bellek yönetimi
    
    async def main_loop(self):
        while self.running:
            try:
                # Bellek yönetimi
                gc.collect()
                
                # Sistemleri kontrol et
                await self.check_systems()
                
                # Watchdog'u resetle
                self.wdt.feed()
                
                await asyncio.sleep(0.1)
                
            except Exception as e:
                print(f"Ana döngü hatası: {e}")
                
    async def check_systems(self):
        # Tüm alt sistemlerin durumunu kontrol et
        if not self.communication.is_connected():
            await self.communication.reconnect()
            
        if self.storage.needs_cleanup():
            self.storage.cleanup_old_data()
    
    def run(self):
        self.app.run(port=80)

def main():
    # WiFi yöneticisini başlat
    wifi = WiFiManager()
    
    # Önce WiFi ağına bağlanmayı dene
    if not wifi.connect_wifi():
        # Bağlantı başarısız olursa AP modunu başlat
        wifi.start_ap()
    
    # Web sunucusunu başlat
    server = WebServer()
    server.start()
    
    while True:
        try:
            # Ana döngü
            time.sleep(1)
        except Exception as e:
            print(f"Hata: {e}")
            machine.reset()

if __name__ == '__main__':
    main() 