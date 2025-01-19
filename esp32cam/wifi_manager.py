import network
import time
import json

class WiFiManager:
    def __init__(self):
        self.ap = network.WLAN(network.AP_IF)
        self.sta = network.WLAN(network.STA_IF)
        self.load_config()
        
    def load_config(self):
        try:
            with open('wifi_config.json', 'r') as f:
                self.config = json.load(f)
        except:
            # Varsayılan yapılandırma
            self.config = {
                'ap_ssid': 'RobotAP',
                'ap_password': '12345678',
                'ap_channel': 1,
                'sta_ssid': '',
                'sta_password': ''
            }
            self.save_config()
            
    def save_config(self):
        with open('wifi_config.json', 'w') as f:
            json.dump(self.config, f)
            
    def start_ap(self):
        """Access Point modunu başlat"""
        self.ap.active(True)
        self.ap.config(
            essid=self.config['ap_ssid'],
            password=self.config['ap_password'],
            channel=self.config['ap_channel']
        )
        print(f"Access Point başlatıldı: {self.config['ap_ssid']}")
        print(f"IP Adresi: {self.ap.ifconfig()[0]}")
        
    def connect_wifi(self):
        """WiFi ağına bağlan"""
        if self.config['sta_ssid']:
            self.sta.active(True)
            self.sta.connect(
                self.config['sta_ssid'],
                self.config['sta_password']
            )
            # Bağlantıyı bekle
            for _ in range(20):
                if self.sta.isconnected():
                    print(f"WiFi'ya bağlandı: {self.config['sta_ssid']}")
                    print(f"IP Adresi: {self.sta.ifconfig()[0]}")
                    return True
                time.sleep(1)
            print("WiFi bağlantısı başarısız")
        return False
        
    def get_ip(self):
        """Aktif IP adresini döndür"""
        if self.sta.isconnected():
            return self.sta.ifconfig()[0]
        return self.ap.ifconfig()[0] 