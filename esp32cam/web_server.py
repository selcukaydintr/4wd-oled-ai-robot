from microdot import Microdot, send_file
import json
import time
import gc

class WebServer:
    def __init__(self):
        self.app = Microdot()
        self.setup_routes()
        
    def setup_routes(self):
        @self.app.before_request
        def before_request(request):
            # Rate limiting
            self.request_count += 1
            if self.request_count > 1000:  # Max 1000 istek/dakika
                return {'error': 'Too many requests'}, 429
                
            # Periyodik temizlik
            if time.time() - self.last_cleanup > 60:
                gc.collect()
                self.request_count = 0
                self.last_cleanup = time.time()
                
        @self.app.errorhandler(Exception)
        def handle_error(error):
            print(f"Server hatası: {error}")
            return {'error': str(error)}, 500
            
        @self.app.route('/')
        def index(request):
            return send_file('www/index.html')
            
        @self.app.route('/static/<path:path>')
        def static(request, path):
            return send_file(f'www/static/{path}')
            
        @self.app.route('/api/wifi/config', methods=['GET', 'POST'])
        def wifi_config(request):
            if request.method == 'POST':
                config = request.json
                # WiFi yapılandırmasını güncelle
                with open('wifi_config.json', 'w') as f:
                    json.dump(config, f)
                return {'status': 'success'}
            else:
                with open('wifi_config.json', 'r') as f:
                    return json.load(f)
            
        @self.app.route('/api/robot/config', methods=['POST'])
        def config(request):
            config = request.json
            self.robot.storage.save_config(config)
            return {'status': 'success'}
            
        @self.app.route('/api/sensors/data')
        def sensor_data(request):
            return self.robot.get_sensor_data()
        
    def start(self):
        self.app.run(port=80) 