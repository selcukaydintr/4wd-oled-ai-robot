export default {
  install(app) {
    app.config.globalProperties.$motorConfig = {
      validatePins(config) {
        // Pin çakışma kontrolü
        const usedPins = new Set()
        for (const motor of config.motors) {
          const pins = [motor.pwm_pin, motor.dir_pin1, motor.dir_pin2]
          for (const pin of pins) {
            if (usedPins.has(pin)) {
              throw new Error(`Pin ${pin} birden fazla kez kullanılmış`)
            }
            usedPins.add(pin)
          }
        }
        return true
      },

      async saveConfig(config) {
        if (this.validatePins(config)) {
          return await fetch('/api/motor/config', {
            method: 'POST',
            body: JSON.stringify(config)
          })
        }
      },

      getAvailablePins() {
        return Array.from({length: 28}, (_, i) => i)
      }
    }
  }
} 