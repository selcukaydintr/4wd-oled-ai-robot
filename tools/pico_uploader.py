import shutil
import os
import time

class PicoUploader:
    def __init__(self):
        self.pico_path = self.find_pico()
        
    def find_pico(self):
        """Bağlı Pico'yu bul"""
        # Windows için
        if os.name == 'nt':
            for drive in range(ord('D'), ord('Z')+1):
                path = f'{chr(drive)}:\\'
                if os.path.exists(path + 'INDEX.HTM'):
                    return path
                    
        # Linux/Mac için
        else:
            # Genellikle /media/username/RPI-RP2 şeklinde
            base_paths = ['/media', '/mnt']
            for base in base_paths:
                for path in os.listdir(base):
                    full_path = os.path.join(base, path)
                    if 'RPI-RP2' in path:
                        return full_path
                        
        raise Exception("Pico bulunamadı!")
        
    def upload_files(self, files_dir):
        try:
            if not self.pico_path:
                raise Exception("Pico bağlı değil!")
                
            print(f"Pico bulundu: {self.pico_path}")
            
            # Dosyaları kopyala
            for root, _, files in os.walk(files_dir):
                for file in files:
                    if file.endswith('.py'):
                        src = os.path.join(root, file)
                        dst = os.path.join(self.pico_path, file)
                        shutil.copy2(src, dst)
                        print(f"{file} yüklendi")
                        
            print("Pico'ya dosyalar başarıyla yüklendi!")
            
        except Exception as e:
            print(f"Yükleme hatası: {e}") 