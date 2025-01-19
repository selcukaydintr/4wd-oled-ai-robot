<template>
  <div class="oled-config">
    <div class="row">
      <!-- Sol OLED -->
      <div class="col-md-6">
        <div class="card">
          <div class="card-header">
            <h5>Sol OLED Ekran</h5>
          </div>
          <div class="card-body">
            <!-- Ekran Önizleme -->
            <div class="oled-preview" :style="previewStyle">
              <canvas ref="leftPreview" width="128" height="64"></canvas>
            </div>
            
            <!-- Göz Animasyonu Seçimi -->
            <div class="form-group mt-3">
              <label>Göz Animasyonu</label>
              <select v-model="leftEye.animation" class="form-select">
                <option v-for="(anim, name) in eyeAnimations" 
                        :key="name" 
                        :value="name">
                  {{ name }}
                </option>
              </select>
            </div>
            
            <!-- I2C Adres Ayarı -->
            <div class="form-group mt-3">
              <label>I2C Adresi</label>
              <input v-model="leftEye.address" 
                     type="text" 
                     class="form-control"
                     placeholder="0x3C">
            </div>
            
            <!-- Menü Ayarları -->
            <div class="form-group mt-3">
              <label>Menü Öğeleri</label>
              <div v-for="(item, index) in leftEye.menu.items" 
                   :key="index" 
                   class="menu-item">
                <input v-model="item.text" 
                       type="text" 
                       class="form-control">
                <button @click="removeMenuItem('left', index)" 
                        class="btn btn-danger btn-sm">
                  Sil
                </button>
              </div>
              <button @click="addMenuItem('left')" 
                      class="btn btn-primary btn-sm mt-2">
                Menü Öğesi Ekle
              </button>
            </div>
          </div>
        </div>
      </div>

      <!-- Sağ OLED -->
      <div class="col-md-6">
        <!-- Sol OLED ile aynı yapıda -->
        <div class="card">
          <div class="card-header">
            <h5>Sağ OLED Ekran</h5>
          </div>
          <div class="card-body">
            <!-- Benzer içerik -->
          </div>
        </div>
      </div>
    </div>

    <!-- Genel Ayarlar -->
    <div class="row mt-4">
      <div class="col-12">
        <div class="card">
          <div class="card-header">
            <h5>Genel Ayarlar</h5>
          </div>
          <div class="card-body">
            <div class="form-group">
              <label>Animasyon Hızı</label>
              <input v-model.number="globalSettings.animationSpeed" 
                     type="range" 
                     class="form-range" 
                     min="100" 
                     max="5000">
            </div>
            <div class="form-group mt-3">
              <label>Menü Geçiş Süresi (ms)</label>
              <input v-model.number="globalSettings.menuTransitionTime" 
                     type="number" 
                     class="form-control">
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Kaydet Butonu -->
    <div class="row mt-4">
      <div class="col-12">
        <button @click="saveConfig" 
                class="btn btn-primary btn-lg w-100">
          Ayarları Kaydet
        </button>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'OLEDConfig',
  data() {
    return {
      leftEye: {
        animation: 'normal',
        address: '0x3C',
        menu: {
          enabled: true,
          items: []
        }
      },
      rightEye: {
        animation: 'normal',
        address: '0x3D',
        menu: {
          enabled: true,
          items: []
        }
      },
      globalSettings: {
        animationSpeed: 1000,
        menuTransitionTime: 3000
      },
      eyeAnimations: this.$oledDisplay.eyeAnimations
    }
  },
  methods: {
    addMenuItem(side) {
      const eye = side === 'left' ? this.leftEye : this.rightEye
      eye.menu.items.push({ text: 'Yeni Öğe' })
    },
    removeMenuItem(side, index) {
      const eye = side === 'left' ? this.leftEye : this.rightEye
      eye.menu.items.splice(index, 1)
    },
    async saveConfig() {
      try {
        await this.$oledDisplay.updateConfig({
          left: this.leftEye,
          right: this.rightEye,
          global: this.globalSettings
        })
        this.$toast.success('Ayarlar kaydedildi')
      } catch (error) {
        this.$toast.error('Ayarlar kaydedilemedi')
      }
    },
    updatePreview() {
      // Canvas üzerinde OLED önizleme
      const leftCtx = this.$refs.leftPreview.getContext('2d')
      // Önizleme çizimi...
    }
  },
  mounted() {
    this.updatePreview()
  }
}
</script>

<style scoped>
.oled-preview {
  background: #000;
  padding: 10px;
  border-radius: 5px;
}

.menu-item {
  display: flex;
  gap: 10px;
  margin-bottom: 5px;
}

canvas {
  background: #000;
  border: 1px solid #333;
}
</style> 