# Esp32-Cam + Raspberry Pico Robot

## Proje Hakkında
Bu proje, ESP32-CAM ve Raspberry Pico kullanarak bir robot geliştirmeyi amaçlamaktadır. Proje, çeşitli sensörler ve motor sürücüler kullanarak robotun kontrolünü sağlamaktadır.

## Dosya Yapısı

### 1. Ana Dizinin İçeriği
- **@vue_cli.json**: Vue.js geliştirme arayüzü için yapılandırma dosyası.
- **donanım listesi.txt**: Projede kullanılan donanım bileşenlerinin listesi.
- **package.json**: Projede kullanılan bağımlılıkların listesi.
- **esp32cam/**: ESP32-CAM ile ilgili Python dosyaları.
- **frontend/**: Vue.js tabanlı ön yüz dosyaları.
- **future/**: Gelecek geliştirmeler için Python dosyaları.
- **pico/**: Raspberry Pico ile ilgili Python dosyaları.
- **src/**: C/C++ kaynak dosyaları.
- **tools/**: Araçlar ve yardımcı dosyalar.
- **web/**: Web ile ilgili dosyalar.

### 2. Önemli Dosyalar
- **esp32cam/main.py**: Projenin ana giriş noktası.
- **frontend/src/**: Vue.js bileşenlerinin bulunduğu dizin.
- **pico/main.py**: Raspberry Pico için ana dosya.
- **tools/esp32_uploader.py**: ESP32'ye dosya yüklemek için kullanılan araç.

## Sensörler ve Bileşenler

### 1. MPU6050 6 Eksen İvme ve Gyro Sensörü
- **Açıklama**: Bu sensör, robotun hareketini ve yönünü algılamak için kullanılır. Hem ivmeölçer hem de jiroskop içerir.
- **Kullanım**: Robotun dengesini sağlamak ve yönlendirme yapmak için verileri kullanır.

### 2. L298 Mini DC ve Step Motor Sürücü Modülü
- **Açıklama**: Motorları kontrol etmek için kullanılan bir sürücü modülüdür.
- **Kullanım**: Robotun hareketini sağlamak için motorları sürer.

### 3. OLED Ekran (SSD1306)
- **Açıklama**: Robotun durumunu göstermek için kullanılan bir ekran.
- **Kullanım**: Robotun çalışma durumu ve sensör verilerini görüntülemek için kullanılır.

### 4. TP4056 Lipo Şarj Cihazı
- **Açıklama**: Lipo pilleri şarj etmek için kullanılan bir modül.
- **Kullanım**: Robotun enerji ihtiyacını karşılamak için kullanılır.

### 5. Diğer Sensörler
- **Vibrasyon Sensörü**: Robotun titreşimlerini algılamak için kullanılır.
- **Fotoresistor**: Işık seviyelerini algılamak için kullanılır.
- **Temas Anahtarı**: Robotun fiziksel etkileşimlerini algılamak için kullanılır.
- **Eğim Anahtarı**: Robotun eğim durumunu algılamak için kullanılır.
- **3 Renkli LED Modülü**: Robotun durumunu göstermek için kullanılır.

## Özellikler
- **Gerçek Zamanlı Kontrol**: Robot, ESP32-CAM ve Raspberry Pico kullanarak gerçek zamanlı kontrol sağlar.
- **Çeşitli Sensörler**: Proje, farklı sensörlerle donatılmıştır, bu da robotun çevresini algılamasını sağlar.
- **Mobil Uygulama Desteği**: Robot, mobil uygulama ile kontrol edilebilir.
- **Gelişmiş Öğrenme Algoritmaları**: Robot, makine öğrenimi algoritmaları kullanarak öğrenme yeteneğine sahiptir.

## Proje Özeti
Bu proje, TinyML tabanlı 4WD otonom bir araçtır. Robot, çeşitli sensörler ve motor sürücüleri kullanarak çevresini algılayabilir ve otonom olarak hareket edebilir. Proje, makine öğrenimi algoritmaları kullanarak robotun öğrenme yeteneğini artırmayı hedefler.

## Kurulum
1. Gerekli bağımlılıkları yüklemek için `npm install` komutunu çalıştırın.
2. Projeyi başlatmak için `npm run serve` komutunu kullanın.

## Kullanım
1. Projeyi klonlayın: `git clone <repo_link>`
2. Proje dizinine gidin: `cd <project_directory>`
3. Gerekli bağımlılıkları yükleyin: `npm install`
4. Projeyi başlatın: `npm run serve`
5. Robotu kontrol etmek için mobil uygulamayı kullanın.

## Katkıda Bulunma
Katkıda bulunmak isterseniz, lütfen şu adımları izleyin:
1. Depoyu fork edin.
2. Yeni bir dal oluşturun: `git checkout -b <feature_branch>`
3. Değişikliklerinizi yapın ve commit edin: `git commit -m "Yeni özellik ekle"`
4. Dalınızı gönderin: `git push origin <feature_branch>`
5. Bir pull isteği oluşturun.

## İletişim
Herhangi bir sorunuz veya öneriniz varsa, lütfen benimle iletişime geçin: [Selçuk Aydın](mailto:selcukaydintr@gmail.com)

## Katkıda Bulunanlar
- [Selçuk Aydın] - Proje sahibi

## Lisans
Bu proje MIT lisansı altında lisanslanmıştır.
