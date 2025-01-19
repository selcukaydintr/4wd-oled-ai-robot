export default {
  install(app) {
    app.config.globalProperties.$encoderSensor = {
      config: {
        type: 'encoder',
        pins: {
          a: null,
          b: null
        },
        ppr: 360 // Pulse per revolution
      },

      async getData() {
        return await fetch('/api/sensors/encoder').then(r => r.json())
      },

      calculateDistance(pulses) {
        const wheelCircumference = Math.PI * this.config.wheelDiameter
        return (pulses / this.config.ppr) * wheelCircumference
      }
    }
  }
} 