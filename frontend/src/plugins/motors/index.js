import MotorConfig from './MotorConfig'
import MotorControl from './MotorControl'

export default {
  install(app) {
    app.use(MotorConfig)
    app.use(MotorControl)
  }
} 