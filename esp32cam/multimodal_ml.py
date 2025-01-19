class MultimodalTinyML:
    def __init__(self):
        self.models = {
            'vision': None,
            'audio': None,
            'motion': None
        }
        self.fusion_model = None
        
    def load_models(self):
        # Her modalite için model yükleme
        self.models['vision'] = self.load_tflite_model('models/vision_model.tflite')
        self.models['audio'] = self.load_tflite_model('models/audio_model.tflite')
        self.models['motion'] = self.load_tflite_model('models/motion_model.tflite')
        
        # Füzyon modeli yükleme
        self.fusion_model = self.load_tflite_model('models/fusion_model.tflite')
    
    def process_multimodal_input(self, image, audio, motion):
        # Her modaliteden tahmin al
        vision_pred = self.predict(self.models['vision'], image)
        audio_pred = self.predict(self.models['audio'], audio)
        motion_pred = self.predict(self.models['motion'], motion)
        
        # Füzyon için tahminleri birleştir
        fusion_input = np.concatenate([vision_pred, audio_pred, motion_pred])
        
        # Son tahmini yap
        return self.predict(self.fusion_model, fusion_input) 