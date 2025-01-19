export default {
  install(app) {
    app.config.globalProperties.$oledDisplay = {
      // Ekran yapılandırması
      config: {
        left: {
          address: 0x3C,
          width: 128,
          height: 64,
          animations: [],
          menu: {
            enabled: true,
            items: []
          }
        },
        right: {
          address: 0x3D,
          width: 128,
          height: 64,
          animations: [],
          menu: {
            enabled: true,
            items: []
          }
        }
      },

      // Animasyon şablonları
      eyeAnimations: {
        normal: { frames: ['(o o)', '(- -)'], interval: 3000 },
        happy: { frames: ['(^ ^)', '(˘ ˘)'], interval: 500 },
        sad: { frames: ['(; ;)', '(, ,)'], interval: 1000 },
        angry: { frames: ['(> <)', '(- -)'], interval: 300 },
        sleep: { frames: ['(- -)'], interval: 1000 }
      },

      async updateConfig(config) {
        return await fetch('/api/displays/oled/config', {
          method: 'POST',
          body: JSON.stringify(config)
        })
      },

      async setAnimation(side, animation) {
        return await fetch(`/api/displays/oled/${side}/animation`, {
          method: 'POST',
          body: JSON.stringify(animation)
        })
      }
    }
  }
} 