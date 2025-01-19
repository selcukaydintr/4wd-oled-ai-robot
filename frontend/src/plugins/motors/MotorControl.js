export default {
  install(app) {
    app.config.globalProperties.$motorControl = {
      async move(speeds) {
        return await fetch('/api/motor/control', {
          method: 'POST',
          body: JSON.stringify(speeds)
        })
      },

      calculateSpeed(direction, speed) {
        const speedMap = {
          forward: { fr: 1, fl: 1, br: 1, bl: 1 },
          backward: { fr: -1, fl: -1, br: -1, bl: -1 },
          left: { fr: 1, fl: -1, br: 1, bl: -1 },
          right: { fr: -1, fl: 1, br: -1, bl: 1 }
        }
        
        return Object.entries(speedMap[direction]).reduce((acc, [motor, mult]) => {
          acc[motor] = speed * mult
          return acc
        }, {})
      }
    }
  }
} 