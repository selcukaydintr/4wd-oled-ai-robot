const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
  transpileDependencies: true,
  // Derleme çıktısını küçült
  productionSourceMap: false,
  // Çıktıyı ESP32'nin www klasörüne yönlendir
  outputDir: '../esp32cam/www',
  // Dosya boyutlarını küçült
  chainWebpack: config => {
    config.optimization.minimize(true)
    config.optimization.splitChunks({
      chunks: 'all',
      minSize: 10000,
      maxSize: 250000
    })
  }
}) 