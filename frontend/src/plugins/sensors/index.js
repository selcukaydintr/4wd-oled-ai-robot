import UltrasonicSensor from './UltrasonicSensor'
import IMUSensor from './IMUSensor'
import EncoderSensor from './EncoderSensor'
import CameraSensor from './CameraSensor'
import LidarSensor from './LidarSensor'

export default {
  install(app) {
    app.use(UltrasonicSensor)
    app.use(IMUSensor)
    app.use(EncoderSensor)
    app.use(CameraSensor)
    app.use(LidarSensor)
  }
} 