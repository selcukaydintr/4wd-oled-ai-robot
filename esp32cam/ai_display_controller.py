from display_controller import OLEDController
import tflite_micro as tflite
import speech
import json
import time

class AIDisplayController:
    def __init__(self):
        self.oled = OLEDController()
        self.emotion_interpreter = None
        self.voice_interpreter = None
        self.tts_engine = None
        
        self.load_models()
        self.init_tts()
        
    def load_models(self):
        try:
            # Duygu tanıma modeli
            with open('models/emotion_model.tflite', 'rb') as f:
                self.emotion_interpreter = tflite.Interpreter(model_content=f.read())
                self.emotion_interpreter.allocate_tensors()
                
            # Ses komut modeli
            with open('models/voice_model.tflite', 'rb') as f:
                self.voice_interpreter = tflite.Interpreter(model_content=f.read())
                self.voice_interpreter.allocate_tensors()
                
        except Exception as e:
            print(f"Model yükleme hatası: {e}")
            
    def init_tts(self):
        try:
            self.tts_engine = speech.TTS(
                language='tr',
                speaker='female',
                speed=1.0
            )
        except Exception as e:
            print(f"TTS başlatma hatası: {e}")
            
    def process_emotion(self, image):
        if not self.emotion_interpreter:
            return None
            
        # Görüntüyü işle
        input_details = self.emotion_interpreter.get_input_details()
        output_details = self.emotion_interpreter.get_output_details()
        
        # Görüntüyü modele uygun formata dönüştür
        processed_image = self.preprocess_image(image)
        self.emotion_interpreter.set_tensor(input_details[0]['index'], processed_image)
        
        # Çıkarım yap
        self.emotion_interpreter.invoke()
        emotion = self.emotion_interpreter.get_tensor(output_details[0]['index'])
        
        return self.process_emotion_result(emotion)
        
    def process_voice(self, audio):
        if not self.voice_interpreter:
            return None
            
        # Ses verisini işle
        input_details = self.voice_interpreter.get_input_details()
        output_details = self.voice_interpreter.get_output_details()
        
        # Ses verisini modele uygun formata dönüştür
        processed_audio = self.preprocess_audio(audio)
        self.voice_interpreter.set_tensor(input_details[0]['index'], processed_audio)
        
        # Çıkarım yap
        self.voice_interpreter.invoke()
        command = self.voice_interpreter.get_tensor(output_details[0]['index'])
        
        return self.process_voice_result(command)
        
    def update_display_with_ai(self, emotion=None, command=None):
        # Duygu durumuna göre göz ifadesini güncelle
        if emotion:
            self.oled.set_emotion_animation(emotion)
            
        # Ses komutuna göre menü veya mesaj göster
        if command:
            self.oled.show_command_response(command)
            self.speak_response(command)
            
    def speak_response(self, command):
        responses = {
            'merhaba': 'Merhaba, nasıl yardımcı olabilirim?',
            'dur': 'Tamam, duruyorum',
            'başla': 'Başlıyorum',
            'dans': 'Dans etmeye başlıyorum'
        }
        
        if command in responses:
            self.tts_engine.speak(responses[command])
            
    def run_ai_loop(self):
        while True:
            try:
                # Kameradan görüntü al
                image = self.capture_image()
                emotion = self.process_emotion(image)
                
                # Mikrofondan ses al
                audio = self.capture_audio()
                command = self.process_voice(audio)
                
                # Ekranı ve sesi güncelle
                self.update_display_with_ai(emotion, command)
                
                time.sleep(0.1)
                
            except Exception as e:
                print(f"AI döngü hatası: {e}")
                time.sleep(1) 