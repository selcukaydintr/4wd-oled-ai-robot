const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
  transpileDependencies: true,
  
  // API proxy ayarları
  devServer: {
    proxy: {
      '/api': {
        target: 'http://192.168.4.1',  // ESP32-CAM IP adresi
        ws: true,
        changeOrigin: true
      }
    }
  },
  
  // Build ayarları
  outputDir: '../esp32cam/www',  // ESP32'ye yüklenecek klasör
  
  // Asset ayarları
  assetsDir: 'static',
  
  // Üretim optimizasyonları
  productionSourceMap: false,
  
  // PWA ayarları
  pwa: {
    name: 'Robot Control',
    themeColor: '#4DBA87'
  }
}) 