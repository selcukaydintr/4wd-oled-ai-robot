export default {
  install(app) {
    app.config.globalProperties.$robotAI = {
      // TinyML modelleri
      models: {
        emotion: {
          enabled: true,
          threshold: 0.7,
          emotions: ['mutlu', 'üzgün', 'kızgın', 'normal', 'şaşkın']
        },
        objectDetection: {
          enabled: true,
          threshold: 0.6,
          objects: ['insan', 'kedi', 'köpek', 'top']
        },
        voiceCommand: {
          enabled: true,
          threshold: 0.8,
          commands: ['merhaba', 'dur', 'başla', 'dans']
        }
      },

      // TTS ayarları
      tts: {
        enabled: true,
        voice: 'tr-TR-Standard-A',
        speed: 1.0,
        pitch: 1.0,
        responses: {
          greeting: 'Merhaba, nasılsın?',
          farewell: 'Görüşürüz!',
          error: 'Üzgünüm, bir hata oluştu'
        }
      },

      // API metodları
      async updateAIConfig(config) {
        return await fetch('/api/ai/config', {
          method: 'POST',
          body: JSON.stringify(config)
        })
      },

      async speak(text) {
        return await fetch('/api/tts/speak', {
          method: 'POST',
          body: JSON.stringify({ text })
        })
      }
    }
  }
} 