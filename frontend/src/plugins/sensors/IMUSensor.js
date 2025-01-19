export default {
  install(app) {
    app.config.globalProperties.$imuSensor = {
      config: {
        type: 'imu',
        pins: {
          sda: null,
          scl: null
        },
        address: 0x68
      },

      async getData() {
        return await fetch('/api/sensors/imu').then(r => r.json())
      },

      async calibrate() {
        return await fetch('/api/sensors/imu/calibrate', { method: 'POST' })
      }
    }
  }
} 