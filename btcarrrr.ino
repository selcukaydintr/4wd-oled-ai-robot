#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Freenove_WS2812B_RGBLED_Controller.h"

// Sabitler
#define WARNING_MEMORY 300
#define LOW_BATTERY_THRESHOLD 6.8
#define MAX_DISTANCE 200
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

// Pin tanımlamaları
const int PIN_SERVO = 2;
const int PIN_DIRECTION_LEFT = 4;
const int PIN_MOTOR_PWM_LEFT = 6;
const int PIN_DIRECTION_RIGHT = 7;
const int PIN_MOTOR_PWM_RIGHT = 5;
const int PIN_SONIC_TRIG = 13;
const int PIN_SONIC_ECHO = 12;
const int PIN_TRACKING_LEFT = A1;
const int PIN_TRACKING_CENTER = A2;
const int PIN_TRACKING_RIGHT = A3;
const int PIN_BATTERY = A0;
const int PIN_BUZZER = 3;

// Global değişkenler
Servo servo;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Freenove_WS2812B_Controller strip(1, PIN_BUZZER);

volatile bool readSensors = false;
volatile bool checkBatteryFlag = false;
bool errorDetected = false;
float batteryVoltage = 0;
unsigned long lastStripUpdateTime = 0;

// Hata mesajları
const char* const PROGMEM ERROR_MESSAGES[] = {
    "Memory Low",
    "Battery Low",
    "Sensor Error",
    "Motor Error",
    "System Error"
};

// Durum yönetimi
enum RobotState {
    NORMAL,
    WARNING,
    ERROR,
    EMERGENCY
};

class StateManager {
public:
    static RobotState currentState;
    static bool canMove() { return currentState != EMERGENCY; }
    static void updateState();
};

RobotState StateManager::currentState = NORMAL;

// Robot göz sınıfı
class RobotEye {
private:
    Adafruit_SSD1306* display;
    int x, y, radius;
    
public:
    RobotEye(Adafruit_SSD1306* disp, int posX, int posY, int rad) 
        : display(disp), x(posX), y(posY), radius(rad) {}
        
    void draw(bool open) {
        if (open) {
            display->fillCircle(x, y, radius, WHITE);
        } else {
            display->drawCircle(x, y, radius, WHITE);
        }
    }
};

// Sabitler ve tanımlamalar
#define WARNING_MEMORY 300
#define LOW_BATTERY_THRESHOLD 6.5
#define ERROR_BUFFER_SIZE 5
#define ACTION_MOVE 'A'
#define ACTION_STOP 'B'
#define ACTION_RGB 'C'
#define ACTION_BUZZER 'D'
#define ACTION_ULTRASONIC 'E'
#define ACTION_LIGHT_TRACING 'F'
#define ACTION_TRACKING 'G'
#define ACTION_CAR_MODE 'H'
#define ACTION_GET_VOLTAGE 'I'
#define ECHO_OK 'J'
#define ACTION_NONE 'K'
#define MODE_NONE 0
#define MODE_GRAVITY 1
#define MODE_ULTRASONIC 2
#define MODE_TRACKING 3
#define UPLOAD_VOL_TIME 3000
#define COMMANDS_COUNT_MAX 8
#define INTERVAL_CHAR '#'
#define INPUT_BUFFER_SIZE 16
#define COMMAND_BUFFER_SIZE 8

// Pin tanımlamaları
#define PIN_SERVO 2
#define MOTOR_DIRECTION 0
#define PIN_DIRECTION_LEFT 4
#define PIN_DIRECTION_RIGHT 3
#define PIN_MOTOR_PWM_LEFT 6
#define PIN_MOTOR_PWM_RIGHT 5
#define PIN_SONIC_TRIG 7
#define PIN_SONIC_ECHO 8
#define PIN_IRREMOTE_RECV 9
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_BATTERY A0
#define PIN_BUZZER 10
#define PIN_TRACKING_LEFT A1
#define PIN_TRACKING_CENTER A2
#define PIN_TRACKING_RIGHT A3
#define STRIP_SDA A4
#define STRIP_SCL A5
#define PIN_DEBUG_BUTTON 3

// Diğer sabitler
#define MOTOR_PWM_DEAD 10
#define BAT_VOL_STANDARD 7.0
#define STRIP_I2C_ADDRESS 0x20
#define STRIP_LEDS_COUNT 10
#define MAX_NUMBER_OF_DISP_MODES 3
#define MAX_DISTANCE 1000
#define SONIC_TIMEOUT (MAX_DISTANCE * 60)
#define SOUND_VELOCITY 340
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// OLED ve Debug ayarları
#define OLED_NO_SPLASH
#define DISABLE_LOGGING
#define DEBUG_SCREEN_DURATION 3000
#define DEBUG_LINE_HEIGHT 10

// Global değişkenler
volatile bool readSensors = false;
volatile bool checkBatteryFlag = false;
bool errorDetected = false;
float batteryVoltage = 0;
u32 lastUploadVoltageTime;
char inputBuffer[INPUT_BUFFER_SIZE];
bool stringComplete = false;
int paramters[COMMAND_BUFFER_SIZE], paramterCount = 0;
int bleCarMode = MODE_NONE;

// Hata mesajları
const char ERROR_MSG_INIT[] PROGMEM = "System initialization failed";
const char ERROR_MSG_MEMORY[] PROGMEM = "Memory low";
const char ERROR_MSG_BATTERY[] PROGMEM = "Battery low";
const char ERROR_MSG_SENSOR[] PROGMEM = "Sensor error";
const char ERROR_MSG_SYSTEM[] PROGMEM = "System error";

const char* const PROGMEM ERROR_MESSAGES[] = {
    ERROR_MSG_INIT,
    ERROR_MSG_MEMORY,
    ERROR_MSG_BATTERY,
    ERROR_MSG_SENSOR,
    ERROR_MSG_SYSTEM
};

// Buzzer için yeni tanımlamalar
#define NOTE_C4  262  // Do
#define NOTE_D4  294  // Re
#define NOTE_E4  330  // Mi
#define NOTE_F4  349  // Fa
#define NOTE_G4  392  // Sol
#define NOTE_A4  440  // La
#define NOTE_B4  494  // Si
#define NOTE_C5  523  // Üst Do
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

// Ses önceliği yönetimi için
struct SoundPriority {
  static const int ALARM_PRIORITY = 1;
  static const int EMOTIONAL_PRIORITY = 2;
  static const int NORMAL_PRIORITY = 3;
};

class SoundManager {
  private:
    static int currentPriority;
    static unsigned long lastSoundTime;
    
  public:
    static bool canPlaySound(int priority) {
      if (priority <= currentPriority) {
        currentPriority = priority;
        lastSoundTime = millis();
        return true;
      }
      return false;
    }
    
    static void update() {
      if (millis() - lastSoundTime > 1000) {
        currentPriority = SoundPriority::NORMAL_PRIORITY;
      }
    }
};

// Buzzer fonksiyonları
void playTone(unsigned int frequency, unsigned long duration) {
  unsigned long period = 1000000L / frequency;
  unsigned long elapsed = 0;
  unsigned long startTime = micros();
  
  while (elapsed < duration * 1000) {
    digitalWrite(PIN_BUZZER, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(PIN_BUZZER, LOW);
    delayMicroseconds(period / 2);
    elapsed = micros() - startTime;
  }
}

void playMelody() {
  // Başlangıç melodisi - Do Re Mi Fa Sol
  int notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4};
  int durations[] = {200, 200, 200, 200, 400};
  
  for (int i = 0; i < 5; i++) {
    playTone(notes[i], durations[i]);
    delay(50);
  }
}

void alarmMode(uint8_t mode) {
  if (SoundManager::canPlaySound(SoundPriority::ALARM_PRIORITY)) {
    switch(mode) {
      case 1: // Acil durum alarmı
        for(int i = 0; i < 3; i++) {
          playTone(NOTE_A4, 500);
          delay(200);
        }
        break;
        
      case 2: // Düşük pil uyarısı
        for(int i = 0; i < 2; i++) {
          playTone(NOTE_C4, 300);
          delay(500);
        }
        break;
        
      case 3: // Başlangıç melodisi
        playMelody();
        break;
        
      case 4: // Engel algılama uyarısı
        playTone(NOTE_G4, 200);
        delay(100);
        break;
        
      case 5: // Görev tamamlama
        playTone(NOTE_C5, 200);
        delay(100);
        playTone(NOTE_G4, 400);
        break;
    }
  }
}

// Duygusal melodiler için yeni fonksiyon
void playEmotionalSound(int expression) {
  if (SoundManager::canPlaySound(SoundPriority::EMOTIONAL_PRIORITY)) {
    static int lastExpression = -1;
    
    // Sadece ifade değiştiğinde çal
    if (lastExpression != expression) {
      switch(expression) {
        case 1: // Mutlu
          playTone(NOTE_C5, 100);
          playTone(NOTE_E5, 100);
          playTone(NOTE_G5, 200);
          break;
          
        case 2: // Üzgün
          playTone(NOTE_G4, 200);
          playTone(NOTE_E4, 200);
          playTone(NOTE_C4, 300);
          break;
          
        case 3: // Şaşkın
          playTone(NOTE_C5, 50);
          playTone(NOTE_C4, 50);
          playTone(NOTE_C5, 50);
          break;
          
        case 0: // Normal
          playTone(NOTE_C4, 100);
          playTone(NOTE_E4, 100);
          break;
      }
      lastExpression = expression;
    }
  }
}

class RobotEye {
  private:
    Adafruit_SSD1306* display;
    int centerX;
    int centerY;
    int radius;
    int pupilX;
    int pupilY;
    int pupilRadius;
    int expression;  // 0: normal, 1: mutlu, 2: üzgün, 3: şaşkın
    
  public:
    RobotEye(Adafruit_SSD1306* disp, int x, int y, int r) {
      display = disp;
      centerX = x;
      centerY = y;
      radius = r;
      pupilX = x;
      pupilY = y;
      pupilRadius = r / 3;
      expression = 0;
    }
    
    void setExpression(int exp) {
      expression = exp;
    }
    
    void lookDirection(int angle) {
      // Açıyı radyana çevir
      float angleRad = angle * PI / 180.0;
      int maxOffset = radius - pupilRadius;
      
      // Yeni göz bebeği pozisyonunu hesapla
      pupilX = centerX + (int)(cos(angleRad) * maxOffset * 0.5);
      pupilY = centerY - (int)(sin(angleRad) * maxOffset * 0.5);
      
      draw();
    }
    
    void draw() {
      // Dış daire (göz)
      display->drawCircle(centerX, centerY, radius, WHITE);
      
      // İfadeye göre göz şeklini değiştir
      switch(expression) {
        case 1: // Mutlu
          display->fillCircle(pupilX, pupilY, pupilRadius, WHITE);
          display->drawPixel(pupilX, pupilY-2, BLACK); // Göz parıltısı
          display->drawPixel(pupilX+1, pupilY-1, BLACK); // Ekstra parıltı
          break;
          
        case 2: // Üzgün
          display->fillCircle(pupilX, pupilY, pupilRadius, WHITE);
          display->drawPixel(pupilX-2, pupilY+2, BLACK); // Göz yaşı efekti
          display->drawLine(pupilX-2, pupilY+2, pupilX-2, pupilY+4, WHITE); // Göz yaşı çizgisi
          break;
          
        case 3: // Şaşkın
          display->drawCircle(pupilX, pupilY, pupilRadius+2, WHITE); // Büyük göz bebeği
          display->fillCircle(pupilX, pupilY, pupilRadius, WHITE);
          display->drawPixel(pupilX+1, pupilY-1, BLACK); // Şaşkın parıltı
          break;
          
        default: // Normal
          display->fillCircle(pupilX, pupilY, pupilRadius, WHITE);
          break;
      }
    }
};

RobotEye leftEye(&display, 45, 35, 15);   // Sol göz: daha büyük ve merkeze yakın
RobotEye rightEye(&display, 85, 35, 15);  // Sağ göz: daha büyük ve merkeze yakın

void drawDualRadarBars(int width, int height, float distance) {
  int barWidth = 48;
  
  // Arka planı siyah yap
  display.fillRect(0, 0, width, height, BLACK);
  
  // Sol bar
  display.fillRect(0, 8, barWidth, height-8, WHITE);
  for(int x = 8; x < barWidth; x += 8) {
    display.drawFastVLine(x, 8, height-8, BLACK);
  }
  
  // Sol bar dolumu
  static float lastDistance = 0;
  float movement = distance - lastDistance;
  float movementPercentage = abs(movement) / MAX_DISTANCE * 100;
  
  int emptyWidth = (int)(barWidth * (movementPercentage / 100));
  emptyWidth = min(max(emptyWidth, 0), barWidth);
  display.fillRect(0, 8, emptyWidth, height-8, BLACK);
  
  // Sağ bar
  int rightStart = width - barWidth;
  display.fillRect(rightStart, 8, barWidth, height-8, WHITE);
  for(int x = rightStart + 8; x < width; x += 8) {
    display.drawFastVLine(x, 8, height-8, BLACK);
  }
  
  // Sağ bar dolumu
  display.fillRect(width - emptyWidth, 8, emptyWidth, height-8, BLACK);
  
  // Mesafeyi ortaya yazdır
  char distStr[10];
  sprintf(distStr, "%.0f", distance);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(distStr, 0, 0, &x1, &y1, &w, &h);
  int textX = (SCREEN_WIDTH - w) / 2;
  display.setTextColor(WHITE);
  display.setCursor(textX, 9);
  display.print(distStr);
  
  // Mesafeye göre ifadeyi belirle
  int expression = 0;
  if (distance < OA_OBSTACLE_DISTANCE_LOW) {
    expression = 2; // Üzgün - çok yakın engel
    // Üzgün kaşlar - aşağı eğimli
    display.drawLine(20, 6, 35, 2, WHITE);  // Sol kaş
    display.drawLine(width-35, 2, width-20, 6, WHITE);  // Sağ kaş
  } 
  else if (distance < OA_OBSTACLE_DISTANCE) {
    expression = 3; // Şaşkın - yakın engel
    // Şaşkın kaşlar - yukarı eğimli
    display.drawLine(20, 2, 35, 4, WHITE);  // Sol kaş
    display.drawLine(width-35, 4, width-20, 2, WHITE);  // Sağ kaş
  }
  else if (distance > MAX_DISTANCE * 0.8) {
    expression = 1; // Mutlu - yol açık
    // Mutlu kaşlar - hafif yukarı eğimli
    display.drawLine(20, 4, 35, 2, WHITE);  // Sol kaş
    display.drawLine(width-35, 2, width-20, 4, WHITE);  // Sağ kaş
  }
  else {
    expression = 0; // Normal
    // Normal kaşlar - düz
    display.drawLine(20, 3, 35, 3, WHITE);  // Sol kaş
    display.drawLine(width-35, 3, width-20, 3, WHITE);  // Sağ kaş
  }
  
  // Göz ifadelerini güncelle
  leftEye.setExpression(expression);
  rightEye.setExpression(expression);
  
  // Duygusal sesi çal
  playEmotionalSound(expression);
}

u8 colorPos = 0;
u8 colorStep = 50;
u8 stripDisplayMode = 1;
u8 currentLedIndex = 0;
u16 stripDisplayDelay = 100;
u32 lastStripUpdateTime = 0;
Freenove_WS2812B_Controller strip(STRIP_I2C_ADDRESS, STRIP_LEDS_COUNT, TYPE_GRB);

// Nesneler
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Statik üye tanımlamaları (global alanda, sınıf tanımlamalarından sonra)
int SoundManager::currentPriority = SoundPriority::NORMAL_PRIORITY;
unsigned long SoundManager::lastSoundTime = 0;

// F() makrosu ile string'leri program belleğinde sakla
Serial.println(F("System initialization failed"));

// Sabit değerleri PROGMEM'e taşı
const PROGMEM uint8_t SERVO_ANGLES[] = {45, 90, 135, 180};
const PROGMEM uint16_t MOTOR_SPEEDS[] = {100, 150, 180, 200, 255};
const PROGMEM char* ERROR_MESSAGES[] = {
    "NO ERROR",
    "MEMORY ERROR",
    "SENSOR ERROR",
    "BATTERY ERROR",
    "MOTOR ERROR"
};

// String yerine F() makrosu kullan
#define DEBUG(msg) Serial.println(F(msg))

// Buffer paylaşımı
union {
    char commandBuffer[COMMAND_BUFFER_SIZE];
    uint8_t displayBuffer[128];  // DISPLAY_BUFFER_SIZE
    uint8_t sensorData[8];
} sharedBuffer;

// Timer interrupt için
ISR(TIMER1_COMPA_vect) {
    static uint8_t counter = 0;
    counter++;
    
    // Her 50ms'de bir
    if(counter % 50 == 0) {
        readSensors = true;
    }
    
    // Her 1000ms'de bir
    if(counter % 1000 == 0) {
        checkBatteryFlag = true;
        counter = 0;
    }
}

void setupTimer() {
    cli();  // Interrupts'ı devre dışı bırak
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 250;  // 1ms için
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);  // 64 prescaler
    TIMSK1 |= (1 << OCIE1A);
    sei();  // Interrupts'ı etkinleştir
}

class MemoryGuard {
private:
    static const uint16_t CRITICAL_MEMORY = 150;
    static const uint16_t WARNING_MEMORY = 300;
    
public:
    static bool allocateMemory(size_t size) {
        uint16_t freeRam = getFreeMemory();
        if(freeRam - size < CRITICAL_MEMORY) {
            enterEmergencyMode();
            return false;
        }
        if(freeRam - size < WARNING_MEMORY) {
            reduceFunctionality();
        }
        return true;
    }
    
    static void reduceFunctionality() {
        disableNonEssentialFeatures();
        clearBuffers();
        collectGarbage();
    }
    
    static void monitorMemory() {
        static uint32_t lastCheck = 0;
        if(millis() - lastCheck > 1000) {
            if(getFreeMemory() < WARNING_MEMORY) {
                reduceFunctionality();
            }
            lastCheck = millis();
        }
    }
};

class WatchdogManager {
private:
    static const uint32_t WATCHDOG_INTERVAL = 100;
    static uint32_t lastReset;
    static uint8_t resetCount;
    static const uint8_t MAX_RESET_COUNT = 3;
    
public:
    static void init() {
        wdt_enable(WDTO_250MS);
        lastReset = millis();
        resetCount = 0;
    }
    
    static void update() {
        if (millis() - lastReset > WATCHDOG_INTERVAL) {
            if (isSystemHealthy()) {
                wdt_reset();
                lastReset = millis();
                resetCount = 0;
            } else {
                resetCount++;
                if (resetCount > MAX_RESET_COUNT) {
                    enterSafeMode();
                }
            }
        }
    }
    
    static bool isSystemHealthy() {
        return (getFreeMemory() > WARNING_MEMORY &&
                batteryVoltage > LOW_BATTERY_THRESHOLD &&
                !errorDetected &&
                StateManager::getCurrentState() != EMERGENCY);
    }
};

class CriticalSection {
private:
    static bool inCriticalSection;
    
public:
    static void enter() {
        cli();  // Interrupts'ı devre dışı bırak
        inCriticalSection = true;
    }
    
    static void exit() {
        inCriticalSection = false;
        sei();  // Interrupts'ı etkinleştir
    }
    
    static bool isInCriticalSection() {
        return inCriticalSection;
    }
};

// Kullanımı:
void updateCriticalData() {
    CriticalSection::enter();
    // Kritik işlemler
    CriticalSection::exit();
}

class LoopOptimizer {
    static void beginLoop() {
        lastLoopTime = micros();
    }
    
    static void endLoop() {
        uint32_t duration = micros() - lastLoopTime;
        if(duration > MAX_LOOP_TIME) {
            optimizeExecution();
        }
    }
};

class SensorValidator {
    static bool validateReading(float value) {
        return (value >= minValid && value <= maxValid);
    }
};

void recoverFromError() {
    resetSystem();
    enterSafeMode();
    logError();
}

bool CriticalSection::inCriticalSection = false;
uint32_t WatchdogManager::lastReset = 0;
uint8_t WatchdogManager::resetCount = 0;

// 1. Task Scheduler
class TaskScheduler {
private:
    static const uint8_t MAX_TASKS = 8;
    
    struct Task {
        void (*function)();
        uint16_t interval;
        uint32_t lastRun;
        bool enabled;
    };
    
    static Task tasks[MAX_TASKS];
    static uint8_t taskCount;
    
public:
    static void addTask(void (*func)(), uint16_t interval) {
        if(taskCount < MAX_TASKS) {
            tasks[taskCount].function = func;
            tasks[taskCount].interval = interval;
            tasks[taskCount].lastRun = 0;
            tasks[taskCount].enabled = true;
            taskCount++;
        }
    }
    
    static void update() {
        uint32_t now = millis();
        for(uint8_t i = 0; i < taskCount; i++) {
            if(tasks[i].enabled && (now - tasks[i].lastRun >= tasks[i].interval)) {
                tasks[i].function();
                tasks[i].lastRun = now;
            }
        }
    }
};

// 2. Non-Blocking İşlemler
class NonBlockingOperations {
private:
    static uint32_t servoMoveStart;
    static uint32_t ledUpdateStart;
    static uint8_t currentOperation;
    
public:
    static void updateServo(uint8_t angle) {
        if(millis() - servoMoveStart >= 100) {
            servo.write(angle);
            servoMoveStart = millis();
        }
    }
    
    static void updateLEDs() {
        if(millis() - ledUpdateStart >= 50) {
            strip.show();
            ledUpdateStart = millis();
        }
    }
};

// 3. İşlem Önceliklendirme
class PriorityManager {
public:
    enum Priority {
        PRIORITY_CRITICAL = 0,   // Her loop'ta
        PRIORITY_HIGH = 1,       // Her 10ms
        PRIORITY_MEDIUM = 2,     // Her 50ms
        PRIORITY_LOW = 3         // Her 100ms
    };
    
    static void processByPriority() {
        static uint32_t lastCheck[4] = {0,0,0,0};
        uint32_t now = millis();
        
        // Kritik işlemler
        processMotors();
        checkSafety();
        
        // Yüksek öncelikli
        if(now - lastCheck[PRIORITY_HIGH] >= 10) {
            updateSensors();
            lastCheck[PRIORITY_HIGH] = now;
        }
        
        // Orta öncelikli
        if(now - lastCheck[PRIORITY_MEDIUM] >= 50) {
            updateDisplay();
            lastCheck[PRIORITY_MEDIUM] = now;
        }
        
        // Düşük öncelikli
        if(now - lastCheck[PRIORITY_LOW] >= 100) {
            updateLEDs();
            checkBattery();
            lastCheck[PRIORITY_LOW] = now;
        }
    }
};

// 4. Loop Süresi Kontrolü
class LoopTimeManager {
private:
    static const uint32_t TARGET_LOOP_TIME = 20000; // 20ms
    static uint32_t lastLoopTime;
    
public:
    static void begin() {
        lastLoopTime = micros();
    }
    
    static void end() {
        uint32_t elapsed = micros() - lastLoopTime;
        if(elapsed < TARGET_LOOP_TIME) {
            delayMicroseconds(TARGET_LOOP_TIME - elapsed);
        }
        lastLoopTime = micros();
    }
};

void setupTasks() {
    TaskScheduler::addTask(updateSensors, 50);    // Her 50ms
    TaskScheduler::addTask(updateDisplay, 100);   // Her 100ms
    TaskScheduler::addTask(checkBattery, 1000);   // Her 1s
    TaskScheduler::addTask(updateLEDs, 50);       // Her 50ms
    TaskScheduler::addTask(checkSafety, 10);      // Her 10ms
}

void setup() {
    // Seri haberleşme başlatma
    Serial.begin(115200);
    
    // I2C başlatma
    Wire.begin();
    Wire.setClock(100000);
    
    // Pin modlarını ayarlama
    pinMode(PIN_SERVO, OUTPUT);
    pinMode(PIN_DIRECTION_LEFT, OUTPUT);
    pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT);
    pinMode(PIN_DIRECTION_RIGHT, OUTPUT);
    pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT);
    pinMode(PIN_SONIC_TRIG, OUTPUT);
    pinMode(PIN_SONIC_ECHO, INPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_TRACKING_LEFT, INPUT);
    pinMode(PIN_TRACKING_CENTER, INPUT);
    pinMode(PIN_TRACKING_RIGHT, INPUT);
    
    // Servo başlatma
    servo.attach(PIN_SERVO);
    servo.write(90);
    
    // Display başlatma
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    display.clearDisplay();
    display.display();
    
    // LED strip başlatma
    if (!strip.begin()) {
        Serial.println(F("LED strip initialization failed"));
        return;
    }
    strip.show();
    
    // Timer ve Watchdog başlatma
    setupTimer();
    WatchdogManager::init();
}

void loop() {
    // Watchdog güncelleme
    WatchdogManager::update();
    
    // Sensör okuma
    if (readSensors) {
        updateSensors();
        readSensors = false;
    }
    
    // Batarya kontrolü
    if (checkBatteryFlag) {
        checkBattery();
        checkBatteryFlag = false;
    }
    
    // Komut işleme
    if (stringComplete) {
        processCommand();
        stringComplete = false;
    }
    
    // Durum güncelleme
    StateManager::updateState();
    
    // Ekran güncelleme
    DisplayManager::update();
    
    // LED güncelleme
    if (millis() - lastStripUpdateTime > 50) {
        strip.show();
        lastStripUpdateTime = millis();
    }
}

// 2. Veri Tipi Optimizasyonu
class OptimizedVariables {
    uint8_t  distance;    // int yerine uint8_t (1 byte)
    uint8_t  speed;      // int yerine uint8_t (1 byte)
    uint8_t  angle;      // int yerine uint8_t (1 byte)
    uint16_t voltage;    // float yerine uint16_t (2 byte)
};

// 3. Statik Bellek Havuzu
class MemoryPool {
private:
    static uint8_t pool[64];  // Sabit boyutlu havuz
    static uint8_t used;
    
public:
    static void* allocate(uint8_t size) {
        if (used + size <= 64) {
            void* ptr = &pool[used];
            used += size;
            return ptr;
        }
        return nullptr;
    }
    
    static void reset() {
        used = 0;
    }
};

// 4. String Optimizasyonu
#define MAX_MESSAGE_LENGTH 16
struct CompactMessage {
    char msg[MAX_MESSAGE_LENGTH];
    uint8_t len;
    
    void set(const char* str) {
        len = strlen(str);
        if(len > MAX_MESSAGE_LENGTH-1) len = MAX_MESSAGE_LENGTH-1;
        memcpy(msg, str, len);
        msg[len] = 0;
    }
};

#define ERROR_MSG_1 1
const char* getErrorMessage(uint8_t code) {
    return (char*)pgm_read_word(&ERROR_MESSAGES[code]);
}

void runDiagnostics() {
    testMemory();     // Bellek testi
    testSensors();    // Sensör testi
    testMotors();     // Motor testi
    testTiming();     // Zamanlama testi
}

void logSystemStatus() {
    logMemoryUsage();    // Bellek kullanımı
    logTaskTimings();    // Görev zamanlamaları
    logErrorCounts();    // Hata sayıları
}

void systemCalibration() {
    calibrateMotors();
    calibrateSensors();
    calibrateServos();
}

void calibrateMotors() {
    // Ölü bölge tespiti
    for(int i = 0; i < 255; i++) {
        motorRun(i, i);
        if(detectMovement()) {
            MOTOR_DEAD_ZONE = i;
            break;
        }
    }
    
    // Hız dengesi
    for(int speed = 0; speed <= 255; speed += 50) {
        motorRun(speed, speed);
        measureSpeedDifference();
        adjustMotorBalance();
    }
}

void calibrateSensors() {
    // Ultrasonik sensör
    float sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += getSonar();
        delay(50);
    }
    SONAR_OFFSET = KNOWN_DISTANCE - (sum / 10);
    
    // Çizgi sensörleri
    calibrateLineSensors();
}

void calibrateServos() {
    // Merkez pozisyon bulma
    servo.write(90);
    delay(1000);
    int actualPosition = measureServoPosition();
    SERVO_CENTER_OFFSET = 90 - actualPosition;
    
    // Hareket limitleri
    findServoLimits();
}

// EEPROM adresleri için sabitler
#define EEPROM_ADDR_MOTOR_DEAD_ZONE 0
#define EEPROM_ADDR_MOTOR_DIRECTION 1
#define EEPROM_ADDR_SERVO_CENTER 2
#define EEPROM_ADDR_SCAN_ANGLE 3
#define EEPROM_ADDR_OBSTACLE_DIST 4
#define EEPROM_ADDR_FORWARD_SPEED 5
#define EEPROM_ADDR_TURN_SPEED 6
#define EEPROM_ADDR_VOLTAGE_COMP 7

// Ayarları EEPROM'dan yükle
void loadSettingsFromEEPROM() {
  MOTOR_PWM_DEAD = EEPROM.read(EEPROM_ADDR_MOTOR_DEAD_ZONE);
  MOTOR_DIRECTION = EEPROM.read(EEPROM_ADDR_MOTOR_DIRECTION);
  OA_SERVO_CENTER = EEPROM.read(EEPROM_ADDR_SERVO_CENTER);
  OA_SCAN_ANGLE_INTERVAL = EEPROM.read(EEPROM_ADDR_SCAN_ANGLE);
  OA_OBSTACLE_DISTANCE = EEPROM.read(EEPROM_ADDR_OBSTACLE_DIST);
  TK_FORWARD_SPEED = EEPROM.read(EEPROM_ADDR_FORWARD_SPEED);
  TK_TURN_SPEED_LV4 = EEPROM.read(EEPROM_ADDR_TURN_SPEED);
  TK_SPEED_OFFSET_PER_V = EEPROM.read(EEPROM_ADDR_VOLTAGE_COMP);
}

// Ayarları EEPROM'a kaydet
void saveSettingsToEEPROM() {
  EEPROM.write(EEPROM_ADDR_MOTOR_DEAD_ZONE, MOTOR_PWM_DEAD);
  EEPROM.write(EEPROM_ADDR_MOTOR_DIRECTION, MOTOR_DIRECTION);
  EEPROM.write(EEPROM_ADDR_SERVO_CENTER, OA_SERVO_CENTER);
  EEPROM.write(EEPROM_ADDR_SCAN_ANGLE, OA_SCAN_ANGLE_INTERVAL);
  EEPROM.write(EEPROM_ADDR_OBSTACLE_DIST, OA_OBSTACLE_DISTANCE);
  EEPROM.write(EEPROM_ADDR_FORWARD_SPEED, TK_FORWARD_SPEED);
  EEPROM.write(EEPROM_ADDR_TURN_SPEED, TK_TURN_SPEED_LV4);
  EEPROM.write(EEPROM_ADDR_VOLTAGE_COMP, TK_SPEED_OFFSET_PER_V);
}

// Varsayılan ayarları yükle
void loadDefaultSettings() {
  MOTOR_PWM_DEAD = 10;
  MOTOR_DIRECTION = 0;
  OA_SERVO_CENTER = 90;
  OA_SCAN_ANGLE_INTERVAL = 50;
  OA_OBSTACLE_DISTANCE = 40;
  TK_FORWARD_SPEED = 90;
  TK_TURN_SPEED_LV4 = 160;
  TK_SPEED_OFFSET_PER_V = 30;
}

// Bluetooth'dan gelen ayar komutlarını işle
void processSettingsCommand() {
  if (inputBuffer[0] == 'S') {  // Settings komutu
    int index = 1;
    MOTOR_PWM_DEAD = paramters[index++];
    MOTOR_DIRECTION = paramters[index++];
    OA_SERVO_CENTER = paramters[index++];
    OA_SCAN_ANGLE_INTERVAL = paramters[index++];
    OA_OBSTACLE_DISTANCE = paramters[index++];
    TK_FORWARD_SPEED = paramters[index++];
    TK_TURN_SPEED_LV4 = paramters[index++];
    TK_SPEED_OFFSET_PER_V = paramters[index];
  }
  else if (inputBuffer[0] == 'E') {  // EEPROM komutu
    saveSettingsToEEPROM();
  }
}

// Statik üye tanımlamaları (global alanda, sınıf tanımlamalarından sonra)
int SoundManager::currentPriority = SoundPriority::NORMAL_PRIORITY;
unsigned long SoundManager::lastSoundTime = 0;
uint8_t TaskScheduler::taskCount = 0;
Task TaskScheduler::tasks[TaskScheduler::MAX_TASKS];
uint32_t NonBlockingOperations::servoMoveStart = 0;
uint32_t NonBlockingOperations::ledUpdateStart = 0;
uint8_t NonBlockingOperations::currentOperation = 0;
uint32_t LoopTimeManager::lastLoopTime = 0;

// Eksik fonksiyon tanımlamaları
int getFreeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void enterEmergencyMode() {
  StateManager::currentState = EMERGENCY;
  strip.setAllPixels(255, 0, 0);  // Kırmızı uyarı
  strip.show();
  playEmotionalSound(2);  // Üzgün ses
}

void disableNonEssentialFeatures() {
  stripDisplayMode = 0;  // LED animasyonları kapat
  isBuzzered = false;    // Buzzer'ı sustur
}

void clearBuffers() {
  memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
  paramterCount = 0;
}

void collectGarbage() {
  // Arduino'da manual garbage collection yok
  // Sadece değişkenleri sıfırlayabiliriz
  stringComplete = false;
}

void enterSafeMode() {
  // Temel güvenlik moduna geç
  servo.write(90);  // Servo'yu merkeze al
  analogWrite(PIN_MOTOR_PWM_LEFT, 0);
  analogWrite(PIN_MOTOR_PWM_RIGHT, 0);
  strip.setAllPixels(0, 0, 0);
  strip.show();
  
  // Acil durum sesi çal
  tone(PIN_BUZZER, 1000, 1000);
}

void optimizeExecution() {
  disableNonEssentialFeatures();
  clearBuffers();
}

// PROGMEM kullanımı için string sabitleri
const char PROGMEM MSG_LOW_BATTERY[] = "Low Battery!";
const char PROGMEM MSG_ERROR[] = "Error Detected";
const char PROGMEM MSG_SAFE[] = "Safe Mode";
const char PROGMEM MSG_WARNING[] = "Warning";

// Setup fonksiyonunda bellek optimizasyonu
void setup() {
    // Watchdog'u başlat
    WatchdogManager::init();
    
    // I2C hızını düşür (daha stabil çalışma için)
    Wire.setClock(100000);
    
    // Buffer boyutunu kontrol et
    if (!MemoryGuard::allocateMemory(INPUT_BUFFER_SIZE)) {
        enterSafeMode();
        return;
    }
    
    // Diğer setup işlemleri...
    {{ ... }}
}

// Loop fonksiyonunda optimizasyon
void loop() {
    LoopOptimizer::beginLoop();
    
    // Bellek kontrolü
    MemoryGuard::monitorMemory();
    
    // Watchdog güncelleme
    WatchdogManager::update();
    
    // Ana loop işlemleri...
    {{ ... }}
    
    LoopOptimizer::endLoop();
}

// Forward declarations
void processMotors();
void checkSafety();
void updateSensors();
void updateDisplay();
void updateLEDs();
void checkBattery();

// Global değişkenler
float distance[3];  // Ultrasonik sensör mesafeleri
uint32_t lastLoopTime = 0;

// Eksik fonksiyon tanımlamaları
void processMotors() {
    // Motor kontrol kodları
}

void checkSafety() {
    // Güvenlik kontrolleri
    StateManager::updateState();
    if (!StateManager::canMove()) {
        enterSafeMode();
    }
}

void updateSensors() {
    // Sensör okumalarını güncelle
}

void updateDisplay() {
    DisplayManager::update();
}

void checkBattery() {
    // Batarya kontrolü
    batteryVoltage = analogRead(PIN_BATTERY) * 5.0 * 3 / 1024;
}

// Sabitler ve tanımlamalar
#define WARNING_MEMORY 300
#define LOW_BATTERY_THRESHOLD 6.5
#define ERROR_BUFFER_SIZE 5

// Global değişkenler
volatile bool readSensors = false;
volatile bool checkBatteryFlag = false;
bool errorDetected = false;

class StateManager {
private:
    static RobotState currentState;
public:
    static RobotState getCurrentState() {
        return currentState;
    }
    {{ ... }}
};

class PriorityManager {
public:
    enum Priority {
        PRIORITY_CRITICAL = 0,
        PRIORITY_HIGH = 1,
        PRIORITY_MEDIUM = 2,
        PRIORITY_LOW = 3
    };
    {{ ... }}
};

class SensorValidator {
private:
    static const float minValid = 0.0;
    static const float maxValid = 100.0;
public:
    static bool validateReading(float value) {
        return (value >= minValid && value <= maxValid);
    }
};

// Timer interrupt handler
ISR(TIMER1_COMPA_vect) {
    static uint8_t counter = 0;
    counter++;
    
    // Her 100ms'de bir sensör okuması
    if (counter % 10 == 0) {
        readSensors = true;
    }
    
    // Her 1 saniyede bir batarya kontrolü
    if (counter >= 100) {
        checkBatteryFlag = true;
        counter = 0;
    }
}

// WatchdogManager sınıfını güncelleme
class WatchdogManager {
private:
    static const uint32_t WATCHDOG_INTERVAL = 100;
    static uint32_t lastReset;
    static uint8_t resetCount;
    static const uint8_t MAX_RESET_COUNT = 3;
    
public:
    static void init() {
        wdt_enable(WDTO_250MS);
        lastReset = millis();
        resetCount = 0;
    }
    
    static void update() {
        if (millis() - lastReset > WATCHDOG_INTERVAL) {
            if (isSystemHealthy()) {
                wdt_reset();
                lastReset = millis();
                resetCount = 0;
            } else {
                resetCount++;
                if (resetCount > MAX_RESET_COUNT) {
                    enterSafeMode();
                }
            }
        }
    }
    
    static bool isSystemHealthy() {
        extern const uint16_t WARNING_MEMORY;
        return (getFreeMemory() > WARNING_MEMORY &&
                batteryVoltage > LOW_BATTERY_THRESHOLD &&
                !errorDetected &&
                StateManager::getCurrentState() != EMERGENCY);
    }
};

// Setup fonksiyonunu güncelleme
void setup() {
    // Seri haberleşmeyi başlat
    Serial.begin(115200);
    while (!Serial) {
        ; // Seri port hazır olana kadar bekle
    }
    
    // Sistem başlatma
    if (!initializeSystem()) {
        Serial.println(F("System initialization failed"));
        enterSafeMode();
        return;
    }
    
    // Timer setup
    setupTimer();
    
    // Watchdog başlatma
    WatchdogManager::init();
}

// Yardımcı fonksiyonlar
bool initializeSystem() {
    Wire.begin();
    Wire.setClock(100000);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        return false;
    }
    
    if (!strip.begin()) {
        return false;
    }
    
    return true;
}

void updateSensors() {
    // Sensör okumalarını güncelle
    int sonarDistance = getSonarDistance();
    int trackingLeft = analogRead(PIN_TRACKING_LEFT);
    int trackingCenter = analogRead(PIN_TRACKING_CENTER);
    int trackingRight = analogRead(PIN_TRACKING_RIGHT);
    
    // Sensör verilerini işle
    processSensorData(sonarDistance, trackingLeft, trackingCenter, trackingRight);
}

int getSonarDistance() {
    digitalWrite(PIN_SONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONIC_TRIG, LOW);
    
    return pulseIn(PIN_SONIC_ECHO, HIGH) / 58;
}

void checkBattery() {
    int rawValue = analogRead(PIN_BATTERY);
    batteryVoltage = rawValue * (5.0 / 1023.0) * 3;
    
    if (batteryVoltage < LOW_BATTERY_THRESHOLD) {
        enterEmergencyMode();
    }
}

void motorRun(int leftSpeed, int rightSpeed) {
    // Sol motor
    digitalWrite(PIN_DIRECTION_LEFT, leftSpeed >= 0 ? HIGH : LOW);
    analogWrite(PIN_MOTOR_PWM_LEFT, abs(leftSpeed));
    
    // Sağ motor
    digitalWrite(PIN_DIRECTION_RIGHT, rightSpeed >= 0 ? HIGH : LOW);
    analogWrite(PIN_MOTOR_PWM_RIGHT, abs(rightSpeed));
}

void processCommand() {
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'F': motorRun(100, 100); break;  // İleri
            case 'B': motorRun(-100, -100); break; // Geri
            case 'L': motorRun(-100, 100); break;  // Sol
            case 'R': motorRun(100, -100); break;  // Sağ
            case 'S': motorRun(0, 0); break;       // Dur
            default: break;
        }
    }
}

void setupTimer() {
    // Timer1 ayarları
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 15624;  // 1Hz için
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect) {
    readSensors = true;
    checkBatteryFlag = true;
}

void processSensorData(int sonarDistance, int trackingLeft, int trackingCenter, int trackingRight) {
    // Mesafe kontrolü
    if (sonarDistance < 10) {  // 10cm'den yakın engel
        motorRun(0, 0);  // Dur
        strip.setAllPixels(255, 0, 0);  // Kırmızı uyarı
        strip.show();
    }
    
    // Hat takip sensörleri
    bool onLine = (trackingCenter > 500);  // Hat üzerinde mi?
    bool leftTurn = (trackingLeft > 500);  // Sol dönüş gerekli mi?
    bool rightTurn = (trackingRight > 500);  // Sağ dönüş gerekli mi?
    
    adjustMovement(onLine, leftTurn, rightTurn);
}

void adjustMovement(bool onLine, bool leftTurn, bool rightTurn) {
    if (!onLine) {
        if (leftTurn) {
            motorRun(-80, 80);  // Sola dön
        }
        else if (rightTurn) {
            motorRun(80, -80);  // Sağa dön
        }
    }
    else {
        motorRun(100, 100);  // Düz git
    }
}

void enterEmergencyMode() {
    StateManager::currentState = EMERGENCY;
    motorRun(0, 0);  // Motorları durdur
    strip.setAllPixels(255, 0, 0);  // Kırmızı uyarı
    strip.show();
    
    // Acil durum sesi çal
    tone(PIN_BUZZER, 1000, 1000);
}

int getFreeMemory() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void StateManager::updateState() {
    if (errorDetected) {
        currentState = ERROR;
    }
    else if (batteryVoltage < LOW_BATTERY_THRESHOLD) {
        currentState = WARNING;
    }
    else if (getFreeMemory() < WARNING_MEMORY) {
        currentState = WARNING;
    }
    else {
        currentState = NORMAL;
    }
}
