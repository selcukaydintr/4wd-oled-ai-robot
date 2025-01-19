<template>
  <div class="robot-design">
    <div class="row">
      <!-- Robot Tasarım Görünümü -->
      <div class="col-md-8">
        <div class="card">
          <div class="card-header">
            <h5>Robot Tasarım Görünümü</h5>
          </div>
          <div class="card-body">
            <div class="robot-layout">
              <!-- Robot şeması -->
              <div class="robot-schema" ref="robotSchema">
                <!-- 4 tekerlek -->
                <div class="wheel front-left"></div>
                <div class="wheel front-right"></div>
                <div class="wheel back-left"></div>
                <div class="wheel back-right"></div>
                
                <!-- Sensörler (dinamik olarak oluşturulacak) -->
                <div v-for="sensor in activeSensors" 
                     :key="sensor.id"
                     :class="['sensor', sensor.type]"
                     :style="getSensorPosition(sensor)"
                     @click="selectSensor(sensor)">
                  <span class="sensor-label">{{ sensor.name }}</span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Sensör Konfigürasyonu -->
      <div class="col-md-4">
        <div class="card">
          <div class="card-header">
            <h5>Sensör Yönetimi</h5>
          </div>
          <div class="card-body">
            <!-- Yeni Sensör Ekleme -->
            <div class="mb-3">
              <label>Sensör Tipi</label>
              <select v-model="newSensor.type" class="form-select">
                <option value="ultrasonic">Ultrasonik Sensör</option>
                <option value="imu">IMU Sensör</option>
                <option value="lidar">LIDAR</option>
                <option value="camera">Kamera</option>
                <option value="encoder">Enkoder</option>
              </select>
            </div>
            
            <!-- Seçili Sensör Ayarları -->
            <div v-if="selectedSensor" class="sensor-settings">
              <h6>Sensör Ayarları</h6>
              <div class="mb-2">
                <label>İsim</label>
                <input v-model="selectedSensor.name" class="form-control">
              </div>
              <div class="mb-2">
                <label>Pozisyon (derece)</label>
                <input v-model="selectedSensor.angle" 
                       type="number" 
                       class="form-control"
                       min="0" 
                       max="359">
              </div>
              <div class="mb-2">
                <label>Mesafe (cm)</label>
                <input v-model="selectedSensor.distance" 
                       type="number" 
                       class="form-control">
              </div>
              <button @click="updateSensor" class="btn btn-primary me-2">
                Güncelle
              </button>
              <button @click="removeSensor" class="btn btn-danger">
                Kaldır
              </button>
            </div>
          </div>
        </div>

        <!-- Motor Konfigürasyonu -->
        <div class="card mt-3">
          <div class="card-header">
            <h5>4WD Motor Ayarları</h5>
          </div>
          <div class="card-body">
            <div class="mb-2">
              <label>Motor Tipi</label>
              <select v-model="motorConfig.type" class="form-select">
                <option value="dc">DC Motor</option>
                <option value="stepper">Step Motor</option>
                <option value="servo">Servo Motor</option>
              </select>
            </div>
            <div class="mb-2">
              <label>PWM Frekansı (Hz)</label>
              <input v-model="motorConfig.pwmFreq" 
                     type="number" 
                     class="form-control">
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- SLAM Konfigürasyonu -->
    <div class="row mt-3">
      <div class="col-12">
        <div class="card">
          <div class="card-header">
            <h5>SLAM Konfigürasyonu</h5>
          </div>
          <div class="card-body">
            <div class="row">
              <div class="col-md-6">
                <h6>Haritalama Ayarları</h6>
                <div class="mb-2">
                  <label>Grid Boyutu (cm)</label>
                  <input v-model="slamConfig.gridSize" 
                         type="number" 
                         class="form-control">
                </div>
                <div class="mb-2">
                  <label>Tarama Hızı (Hz)</label>
                  <input v-model="slamConfig.scanRate" 
                         type="number" 
                         class="form-control">
                </div>
              </div>
              <div class="col-md-6">
                <h6>Sensör Entegrasyonu</h6>
                <div v-for="sensor in activeSensors" 
                     :key="sensor.id" 
                     class="form-check">
                  <input type="checkbox" 
                         class="form-check-input"
                         v-model="sensor.useInSlam">
                  <label class="form-check-label">
                    {{ sensor.name }} ile SLAM
                  </label>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      activeSensors: [],
      selectedSensor: null,
      newSensor: {
        type: 'ultrasonic',
        name: '',
        angle: 0,
        distance: 10
      },
      motorConfig: {
        type: 'dc',
        pwmFreq: 1000
      },
      slamConfig: {
        gridSize: 5,
        scanRate: 10
      }
    }
  },
  methods: {
    getSensorPosition(sensor) {
      // Sensör pozisyonunu hesapla
      const angle = sensor.angle * (Math.PI / 180)
      const x = 50 + sensor.distance * Math.cos(angle)
      const y = 50 + sensor.distance * Math.sin(angle)
      return {
        left: `${x}%`,
        top: `${y}%`
      }
    },
    selectSensor(sensor) {
      this.selectedSensor = sensor
    },
    updateSensor() {
      // Sensör güncelleme
    },
    removeSensor() {
      // Sensör kaldırma
    }
  }
}
</script>

<style scoped>
.robot-schema {
  position: relative;
  width: 100%;
  height: 400px;
  border: 2px solid #ccc;
  border-radius: 10px;
}

.sensor {
  position: absolute;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  cursor: pointer;
}

.wheel {
  position: absolute;
  width: 30px;
  height: 60px;
  background: #333;
}

.sensor-label {
  position: absolute;
  font-size: 12px;
  white-space: nowrap;
}
</style> 