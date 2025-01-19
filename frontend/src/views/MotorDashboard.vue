<template>
  <div class="motor-dashboard">
    <!-- Pin Yapılandırması -->
    <div class="card mb-4">
      <div class="card-header">
        <h5>Motor Pin Yapılandırması</h5>
      </div>
      <div class="card-body">
        <div class="row">
          <div v-for="motor in motors" :key="motor.id" class="col-md-3">
            <div class="motor-config mb-3">
              <h6>{{ motor.name }}</h6>
              <div class="form-group">
                <label>PWM Pin (EN)</label>
                <select v-model="motor.pwm_pin" class="form-select">
                  <option v-for="pin in availablePins" :key="pin" :value="pin">
                    GPIO{{ pin }}
                  </option>
                </select>
              </div>
              <div class="form-group">
                <label>Yön Pin 1 (IN1)</label>
                <select v-model="motor.dir_pin1" class="form-select">
                  <option v-for="pin in availablePins" :key="pin" :value="pin">
                    GPIO{{ pin }}
                  </option>
                </select>
              </div>
              <div class="form-group">
                <label>Yön Pin 2 (IN2)</label>
                <select v-model="motor.dir_pin2" class="form-select">
                  <option v-for="pin in availablePins" :key="pin" :value="pin">
                    GPIO{{ pin }}
                  </option>
                </select>
              </div>
            </div>
          </div>
        </div>
        <button @click="saveMotorConfig" class="btn btn-primary">
          Pin Yapılandırmasını Kaydet
        </button>
      </div>
    </div>

    <!-- Motor Kontrol Paneli -->
    <div class="card">
      <div class="card-header">
        <h5>Motor Kontrol Paneli</h5>
      </div>
      <div class="card-body">
        <!-- PWM Frekans Ayarı -->
        <div class="row mb-4">
          <div class="col-md-4">
            <label>PWM Frekansı (Hz)</label>
            <input v-model.number="pwmFrequency" 
                   type="number" 
                   class="form-control"
                   min="100" 
                   max="5000">
          </div>
        </div>

        <!-- Bireysel Motor Kontrolleri -->
        <div class="row mb-4">
          <div v-for="motor in motors" :key="motor.id" class="col-md-3">
            <div class="motor-control">
              <h6>{{ motor.name }}</h6>
              <div class="speed-slider">
                <input type="range" 
                       v-model.number="motor.speed"
                       class="form-range" 
                       min="-100" 
                       max="100">
                <div class="speed-value">
                  Hız: {{ motor.speed }}%
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Hareket Kontrolleri -->
        <div class="movement-controls">
          <div class="row mb-3">
            <div class="col text-center">
              <button @click="moveForward" class="btn btn-primary">
                İleri
              </button>
            </div>
          </div>
          <div class="row mb-3">
            <div class="col text-center">
              <button @click="turnLeft" class="btn btn-primary me-2">
                Sol
              </button>
              <button @click="stop" class="btn btn-danger me-2">
                Dur
              </button>
              <button @click="turnRight" class="btn btn-primary">
                Sağ
              </button>
            </div>
          </div>
          <div class="row">
            <div class="col text-center">
              <button @click="moveBackward" class="btn btn-primary">
                Geri
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'MotorDashboard',
  data() {
    return {
      motors: [
        { id: 'fr', name: 'Ön Sağ', pwm_pin: 0, dir_pin1: 4, dir_pin2: 5, speed: 0 },
        { id: 'fl', name: 'Ön Sol', pwm_pin: 1, dir_pin1: 6, dir_pin2: 7, speed: 0 },
        { id: 'br', name: 'Arka Sağ', pwm_pin: 2, dir_pin1: 8, dir_pin2: 9, speed: 0 },
        { id: 'bl', name: 'Arka Sol', pwm_pin: 3, dir_pin1: 10, dir_pin2: 11, speed: 0 }
      ],
      pwmFrequency: 1000,
      availablePins: Array.from({length: 28}, (_, i) => i), // GPIO 0-27
      defaultSpeed: 50
    }
  },
  methods: {
    async saveMotorConfig() {
      try {
        await this.$store.dispatch('saveMotorConfig', {
          motors: this.motors,
          pwmFrequency: this.pwmFrequency
        })
        this.$toast.success('Motor yapılandırması kaydedildi')
      } catch (error) {
        this.$toast.error('Yapılandırma kaydedilemedi')
      }
    },
    async moveForward() {
      const speeds = this.motors.reduce((acc, motor) => {
        acc[motor.id] = this.defaultSpeed
        return acc
      }, {})
      await this.sendMotorCommand(speeds)
    },
    async moveBackward() {
      const speeds = this.motors.reduce((acc, motor) => {
        acc[motor.id] = -this.defaultSpeed
        return acc
      }, {})
      await this.sendMotorCommand(speeds)
    },
    async turnLeft() {
      await this.sendMotorCommand({
        fr: this.defaultSpeed,
        fl: -this.defaultSpeed,
        br: this.defaultSpeed,
        bl: -this.defaultSpeed
      })
    },
    async turnRight() {
      await this.sendMotorCommand({
        fr: -this.defaultSpeed,
        fl: this.defaultSpeed,
        br: -this.defaultSpeed,
        bl: this.defaultSpeed
      })
    },
    async stop() {
      const speeds = this.motors.reduce((acc, motor) => {
        acc[motor.id] = 0
        return acc
      }, {})
      await this.sendMotorCommand(speeds)
    },
    async sendMotorCommand(speeds) {
      try {
        await this.$store.dispatch('sendMotorCommand', speeds)
      } catch (error) {
        this.$toast.error('Motor komutu gönderilemedi')
      }
    }
  },
  watch: {
    'motors': {
      deep: true,
      handler(newValue) {
        // Her motor hızı değiştiğinde otomatik güncelle
        const speeds = newValue.reduce((acc, motor) => {
          acc[motor.id] = motor.speed
          return acc
        }, {})
        this.sendMotorCommand(speeds)
      }
    }
  }
}
</script>

<style scoped>
.motor-dashboard {
  padding: 20px;
}

.speed-slider {
  padding: 10px;
  border: 1px solid #ddd;
  border-radius: 5px;
  margin-bottom: 10px;
}

.speed-value {
  text-align: center;
  font-weight: bold;
}

.movement-controls {
  max-width: 300px;
  margin: 0 auto;
}

.motor-config {
  border: 1px solid #eee;
  padding: 10px;
  border-radius: 5px;
}
</style> 