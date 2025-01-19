import { createApp } from 'vue'
import App from './App.vue'
import RobotPlugins from './plugins'
import store from './store'
import router from './router'

const app = createApp(App)

// Plugin'leri yükle
app.use(RobotPlugins)

// Store ve Router'ı yükle
app.use(store)
app.use(router)

// Uygulamayı başlat
app.mount('#app') 