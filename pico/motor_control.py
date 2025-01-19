from machine import Pin, PWM

class MotorController:
    def __init__(self):
        # Motor PWM pinleri (EN pinleri)
        self.pwm_fr = PWM(Pin(0))  # Ön Sağ
        self.pwm_fl = PWM(Pin(1))  # Ön Sol
        self.pwm_br = PWM(Pin(2))  # Arka Sağ
        self.pwm_bl = PWM(Pin(3))  # Arka Sol
        
        # Motor yön pinleri
        self.dir_fr1 = Pin(4, Pin.OUT)  # Ön Sağ IN1
        self.dir_fr2 = Pin(5, Pin.OUT)  # Ön Sağ IN2
        self.dir_fl1 = Pin(6, Pin.OUT)  # Ön Sol IN1
        self.dir_fl2 = Pin(7, Pin.OUT)  # Ön Sol IN2
        self.dir_br1 = Pin(8, Pin.OUT)  # Arka Sağ IN1
        self.dir_br2 = Pin(9, Pin.OUT)  # Arka Sağ IN2
        self.dir_bl1 = Pin(10, Pin.OUT)  # Arka Sol IN1
        self.dir_bl2 = Pin(11, Pin.OUT)  # Arka Sol IN2
        
        # PWM frekansını ayarla (Sarı T motorlar için optimize edilmiş)
        self.pwm_freq = 1000
        self.setup_pwm()
        
        # Motor durumları
        self.current_speeds = {
            'fr': 0, 'fl': 0, 'br': 0, 'bl': 0
        }
        
    def setup_pwm(self):
        for pwm in [self.pwm_fr, self.pwm_fl, self.pwm_br, self.pwm_bl]:
            pwm.freq(self.pwm_freq)
            pwm.duty_u16(0)  # Başlangıçta motorları durdur
            
    def set_motor_direction(self, dir1, dir2, forward):
        if forward:
            dir1.value(1)
            dir2.value(0)
        else:
            dir1.value(0)
            dir2.value(1)
            
    def set_motor_speed(self, pwm, speed):
        # Speed: -100 ile 100 arası
        abs_speed = abs(speed)
        pwm_value = int((abs_speed / 100.0) * 65535)
        pwm.duty_u16(pwm_value)
        
    def control_motor(self, motor, speed):
        # Motor seçimi ve kontrol
        if motor == 'fr':
            self.set_motor_direction(self.dir_fr1, self.dir_fr2, speed >= 0)
            self.set_motor_speed(self.pwm_fr, speed)
        elif motor == 'fl':
            self.set_motor_direction(self.dir_fl1, self.dir_fl2, speed >= 0)
            self.set_motor_speed(self.pwm_fl, speed)
        elif motor == 'br':
            self.set_motor_direction(self.dir_br1, self.dir_br2, speed >= 0)
            self.set_motor_speed(self.pwm_br, speed)
        elif motor == 'bl':
            self.set_motor_direction(self.dir_bl1, self.dir_bl2, speed >= 0)
            self.set_motor_speed(self.pwm_bl, speed)
            
        self.current_speeds[motor] = speed
        
    def move(self, speeds):
        """
        speeds: Dictionary containing speeds for each motor
        Example: {'fr': 50, 'fl': 50, 'br': 50, 'bl': 50}
        """
        for motor, speed in speeds.items():
            self.control_motor(motor, speed)
            
    def stop(self):
        """Tüm motorları durdur"""
        for motor in ['fr', 'fl', 'br', 'bl']:
            self.control_motor(motor, 0)
            
    def move_forward(self, speed=50):
        self.move({'fr': speed, 'fl': speed, 'br': speed, 'bl': speed})
        
    def move_backward(self, speed=50):
        self.move({'fr': -speed, 'fl': -speed, 'br': -speed, 'bl': -speed})
        
    def turn_left(self, speed=50):
        self.move({'fr': speed, 'fl': -speed, 'br': speed, 'bl': -speed})
        
    def turn_right(self, speed=50):
        self.move({'fr': -speed, 'fl': speed, 'br': -speed, 'bl': speed})
        
    def rotate_clockwise(self, speed=50):
        self.move({'fr': -speed, 'fl': speed, 'br': -speed, 'bl': speed})
        
    def rotate_counterclockwise(self, speed=50):
        self.move({'fr': speed, 'fl': -speed, 'br': speed, 'bl': -speed})
        
    def get_motor_status(self):
        return {
            'speeds': self.current_speeds,
            'pwm_freq': self.pwm_freq
        } 