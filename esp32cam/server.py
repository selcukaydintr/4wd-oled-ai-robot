from microdot import Microdot, send_file, Response
import json

class WebServer:
    def __init__(self, storage):
        self.app = Microdot()
        self.storage = storage
        self.setup_routes()
    
    def setup_routes(self):
        @self.app.route('/')
        def index(request):
            return send_file('/www/index.html')
        
        @self.app.route('/static/<path:path>')
        def static(request, path):
            return send_file(f'/www/{path}')
        
        @self.app.route('/api/config', methods=['GET', 'POST'])
        def config(request):
            if request.method == 'GET':
                return Response(
                    json.dumps(self.storage.read_config()),
                    headers={'Content-Type': 'application/json'}
                )
            else:
                new_config = request.json
                self.storage.save_config(new_config)
                return {'status': 'ok'}
    
    def run(self):
        self.app.run(port=80) 