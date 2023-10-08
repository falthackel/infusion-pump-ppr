const int pwmPin = 9;  // Pin yang terhubung ke MOSFET
const int pwmChannel = 0; // Channel PWM (0-15)
const int pwmResolution = 8; // Resolusi PWM (8-bit)
int pwmStep = 15; // Nilai pertambahan pwm
int pwmValue = 15;    // Nilai PWM (0-255) untuk mengatur kecepatan

void setup() {
  // Inisialisasi PWM
  ledcSetup(pwmChannel, 1000, pwmResolution); // Frekuensi 1000 Hz
  ledcAttachPin(pwmPin, pwmChannel);
  Serial.begin(115200);// initialize serial motor
}

void loop() {
  // Set nilai PWM untuk mengatur kecepatan peristaltik pump
  ledcWrite(pwmChannel, pwmValue);

  // Tambahkan logika pengendalian sesuai kebutuhan Anda di sini
    Serial.print("Speed: ");
    Serial.println(pwmValue);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
  pwmValue = pwmValue + pwmStep;
  // See video you for details.
  if (pwmValue <= 0 || pwmValue >= 255) {
    pwmStep = -pwmStep;
  }  

  delay(1000);  // Delay untuk mengatur frekuensi perubahan PWM
}