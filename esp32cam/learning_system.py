class AdaptiveLearning:
    def __init__(self):
        self.memory = []
        self.interaction_history = []
        self.learning_model = None
        
    def learn_from_interaction(self, state, action, reward):
        # Pekiştirmeli öğrenme
        self.memory.append({
            'state': state,
            'action': action,
            'reward': reward,
            'timestamp': time.time()
        })
        self.update_model()
    
    def learn_from_human(self, interaction_data):
        # İnsan etkileşiminden öğrenme
        self.interaction_history.append(interaction_data)
        self.adapt_behavior()
        
    def update_model(self):
        # Online öğrenme için model güncelleme
        if len(self.memory) >= MIN_SAMPLES:
            self.train_incremental() 