<template>
  <div class="ai-config">
    <!-- TinyML Ayarları -->
    <div class="card mb-4">
      <div class="card-header">
        <h5>TinyML Model Ayarları</h5>
      </div>
      <div class="card-body">
        <!-- Duygu Tanıma -->
        <div class="form-group">
          <h6>Duygu Tanıma</h6>
          <div class="form-check">
            <input v-model="config.models.emotion.enabled"
                   type="checkbox"
                   class="form-check-input">
            <label class="form-check-label">Aktif</label>
          </div>
          <div class="mt-2">
            <label>Hassasiyet</label>
            <input v-model.number="config.models.emotion.threshold"
                   type="range"
                   class="form-range"
                   min="0"
                   max="1"
                   step="0.1">
          </div>
        </div>

        <!-- Nesne Tanıma -->
        <div class="form-group mt-3">
          <h6>Nesne Tanıma</h6>
          <div class="form-check">
            <input v-model="config.models.objectDetection.enabled"
                   type="checkbox"
                   class="form-check-input">
            <label class="form-check-label">Aktif</label>
          </div>
          <div class="mt-2">
            <label>Hassasiyet</label>
            <input v-model.number="config.models.objectDetection.threshold"
                   type="range"
                   class="form-range"
                   min="0"
                   max="1"
                   step="0.1">
          </div>
        </div>

        <!-- Ses Komutları -->
        <div class="form-group mt-3">
          <h6>Ses Komutları</h6>
          <div class="form-check">
            <input v-model="config.models.voiceCommand.enabled"
                   type="checkbox"
                   class="form-check-input">
            <label class="form-check-label">Aktif</label>
          </div>
          <div class="mt-2">
            <label>Hassasiyet</label>
            <input v-model.number="config.models.voiceCommand.threshold"
                   type="range"
                   class="form-range"
                   min="0"
                   max="1"
                   step="0.1">
          </div>
        </div>
      </div>
    </div>

    <!-- TTS Ayarları -->
    <div class="card mb-4">
      <div class="card-header">
        <h5>TTS Ayarları</h5>
      </div>
      <div class="card-body">
        <div class="form-check mb-3">
          <input v-model="config.tts.enabled"
                 type="checkbox"
                 class="form-check-input">
          <label class="form-check-label">TTS Aktif</label>
        </div>

        <div class="form-group">
          <label>Ses</label>
          <select v-model="config.tts.voice" class="form-select">
            <option value="tr-TR-Standard-A">Kadın Ses</option>
            <option value="tr-TR-Standard-B">Erkek Ses</option>
          </select>
        </div>

        <div class="form-group mt-3">
          <label>Konuşma Hızı</label>
          <input v-model.number="config.tts.speed"
                 type="range"
                 class="form-range"
                 min="0.5"
                 max="2"
                 step="0.1">
        </div>

        <div class="form-group mt-3">
          <label>Ses Tonu</label>
          <input v-model.number="config.tts.pitch"
                 type="range"
                 class="form-range"
                 min="0.5"
                 max="2"
                 step="0.1">
        </div>
      </div>
    </div>

    <!-- Kaydet Butonu -->
    <button @click="saveConfig"
            class="btn btn-primary btn-lg w-100">
      Ayarları Kaydet
    </button>
  </div>
</template>

<script>
export default {
  name: 'AIConfig',
  data() {
    return {
      config: this.$robotAI.$options
    }
  },
  methods: {
    async saveConfig() {
      try {
        await this.$robotAI.updateAIConfig(this.config)
        this.$toast.success('AI ayarları kaydedildi')
      } catch (error) {
        this.$toast.error('AI ayarları kaydedilemedi')
      }
    }
  }
}
</script> 