# 4WD OLED AI Robot Dokümantasyonu

## İçindekiler
- [Proje Genel Bakış](#proje-genel-bakış)
- [Sistem Mimarisi](#sistem-mimarisi)
- [Donanım Bileşenleri](#donanım-bileşenleri)
- [Yazılım Bileşenleri](#yazılım-bileşenleri)
- [Proje Yapısı](#proje-yapısı)
- [Kurulum Kılavuzu](#kurulum-kılavuzu)
- [Geliştirici Kılavuzu](#geliştirici-kılavuzu)
- [Katkıda Bulunma](#katkıda-bulunma)
- [Lisans ve İletişim](#lisans-ve-i̇letişim)

## Proje Genel Bakış

4WD OLED AI Robot, ESP32-CAM ve Raspberry Pico kullanarak gerçek zamanlı kontrol sağlayan, TinyML tabanlı otonom bir araç projesidir. Robot, çeşitli sensörler ve motor sürücüleri kullanarak çevresel etkileşim ve makine öğrenimi algoritmaları ile otonom hareket kabiliyetine sahiptir.

### Temel Özellikler
- ESP32-CAM ve Raspberry Pico ile gerçek zamanlı kontrol
- Çevresel farkındalık için çoklu sensör entegrasyonu
- Mobil uygulama kontrol arayüzü
- Gelişmiş öğrenme algoritmaları
- SLAM (Eşzamanlı Konumlandırma ve Haritalama) yeteneği

## Sistem Mimarisi

### 1. Web Arayüzü (Vue.js & JavaScript)
- **Gösterge Paneli**: Genel robot durumu ve temel kontroller
- **Motor Paneli**: Motor kontrolü ve izleme
- **Sensör Yapılandırma**: Sensör kurulumu ve kalibrasyonu
- **Robot Tasarımı**: Robot yapılandırma arayüzü
- **SLAM Arayüzü**: Haritalama ve navigasyon kontrolleri

### 2. Arka Uç (Python)
- Motor kontrol modülü
- Sensör veri işleme
- SLAM uygulaması
- Gerçek zamanlı iletişim için WebSocket sunucusu
- REST API uç noktaları

### 3. Donanım Kontrolü (C++)
- Arduino/mikrodenetleyici yazılımı
- Motor kontrol sistemleri
- OLED ekran yönetimi
- Sensör entegrasyonları

## Donanım Bileşenleri

### Sensörler
1. **MPU6050 6 Eksen İvmeölçer ve Jiroskop**
   - Amaç: Hareket ve yönelim tespiti
   - Kullanım: Denge kontrolü ve navigasyon

2. **L298 Mini DC ve Step Motor Sürücü**
   - Amaç: Motor kontrolü
   - Kullanım: Robot hareket kontrolü

3. **OLED Ekran (SSD1306)**
   - Amaç: Durum gösterimi
   - Kullanım: Çalışma durumu ve sensör verilerinin görselleştirilmesi

4. **Ek Sensörler**
   - Titreşim sensörü
   - Işık sensörü (Fotoresistör)
   - Temas anahtarı
   - Eğim anahtarı
   - 3 renkli LED modülü

### Güç Yönetimi
- **TP4056 LiPo Şarj Modülü**
  - Amaç: Pil şarjı
  - Kullanım: Güç kaynağı yönetimi

## Yazılım Bileşenleri

### Dizin Yapısı
```
proje-kökdizini/
├── esp32cam/           # ESP32-CAM Python dosyaları
│   └── main.py
├── frontend/          # Vue.js önyüz dosyaları
│   └── src/
├── future/           # Gelecek geliştirme dosyaları
├── pico/            # Raspberry Pico dosyaları
│   └── main.py
├── src/             # C/C++ kaynak dosyaları
├── tools/           # Yardımcı araçlar
│   └── esp32_uploader.py
├── web/             # Web ile ilgili dosyalar
├── @vue_cli.json
└── package.json
```

## Kurulum Kılavuzu

### Ön Gereksinimler
- Node.js ve npm
- Python 3.x
- Arduino IDE
- Gerekli donanım bileşenleri

### Kurulum Adımları
1. **Projeyi Klonlama**
   ```bash
   git clone <repo_link>
   cd <proje_dizini>
   ```

2. **Bağımlılıkların Kurulumu**
   ```bash
   npm install
   ```

3. **Projeyi Başlatma**
   ```bash
   npm run serve
   ```

4. **Donanım Kurulumu**
   - Elektronik bileşenlerin bağlantılarını yapın
   - Güç kaynağını kontrol edin
   - Sensörleri kalibre edin

## Geliştirici Kılavuzu

### Sistem Güvenliği Önerileri
- Kullanıcı kimlik doğrulama sistemi
- API güvenlik katmanı
- WebSocket bağlantılarının şifrelenmesi

### Arayüz İyileştirmeleri
- Gerçek zamanlı veri akışının optimizasyonu
- Hata yakalama sisteminin geliştirilmesi
- Duyarlı tasarımın iyileştirilmesi

### Sensör Entegrasyonu
- Yeni sensör tipleri için modüler yapı
- Sensör veri işleme geliştirmeleri
- Otomatik kalibrasyon sistemi

## Katkıda Bulunma

1. Projeyi forklayın
2. Yeni bir dal oluşturun: `git checkout -b özellik/yeni-özellik`
3. Değişikliklerinizi commit edin: `git commit -m "Yeni özellik eklendi"`
4. Dalınıza push yapın: `git push origin özellik/yeni-özellik`
5. Bir Pull Request oluşturun

## Lisans ve İletişim

### Lisans
Bu proje MIT Lisansı altında lisanslanmıştır.

### İletişim
- **Proje Sahibi**: Selçuk Aydın
- **E-posta**: selcukaydintr@gmail.com
- **Sorunlar**: [GitHub Issues](https://github.com/selcukaydintr/4wd-oled-ai-robot/issues)
