export default {
  install(app) {
    app.config.globalProperties.$ultrasonicSensor = {
      config: {
        type: 'ultrasonic',
        pins: {
          trigger: null,
          echo: null
        },
        position: {
          x: 0,
          y: 0,
          angle: 0
        }
      },

      async getData() {
        return await fetch('/api/sensors/ultrasonic').then(r => r.json())
      },

      async configure(config) {
        return await fetch('/api/sensors/ultrasonic/config', {
          method: 'POST',
          body: JSON.stringify(config)
        })
      }
    }
  }
} 