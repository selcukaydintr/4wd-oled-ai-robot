import MotorPlugin from './motors'
import SensorPlugin from './sensors'

export default {
  install(app) {
    // Motor plugin'ini yükle
    app.use(MotorPlugin)
    // Sensör plugin'ini yükle
    app.use(SensorPlugin)
  }
} 