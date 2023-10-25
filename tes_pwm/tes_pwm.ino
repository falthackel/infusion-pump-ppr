const int pwmPin = 0;  // Pin yang terhubung ke MOSFET
const int pwmChannel = 0; // Channel PWM (0-15)
const int pwmResolution = 10; // Resolusi PWM (10-bit)1
const int PushButton1 = 15;// Assign Pushbutton naik di 15
const int PushButton2 = 2;// Assign Pushbutton turun di 2
int pwmStep = 10; // Nilai pertambahan pwm
int pwmValue = 642;    // Nilai PWM (0-255) untuk mengatur kecepatan

void setup() {
  // Inisialisasi PWM
  ledcSetup(pwmChannel, 1000, pwmResolution); // Frekuensi 1000 Hz
  ledcAttachPin(pwmPin, pwmChannel);
  Serial.begin(115200);// initialize serial motor
  pinMode (PushButton1, INPUT); //Declare PushButton as Input
  pinMode (PushButton2, INPUT); //Declare PushButton as Input
}

void loop() {
  // Set nilai PWM untuk mengatur kecepatan peristaltik pump
  ledcWrite(pwmChannel, pwmValue);
  int Push_button_state1 = digitalRead(PushButton1);
  int Push_button_state2 = digitalRead(PushButton2);
  // Tambahkan logika pengendalian sesuai kebutuhan Anda di sini
  Serial.print("Button Naik: ");
  Serial.println(Push_button_state1);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
  Serial.print("Button Turun: ");
  Serial.println(Push_button_state2);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
  Serial.print("Speed: ");
  Serial.println(pwmValue);// print mSpeed value on Serial monitor (click on Tools->Serial Monitor)
  if (Push_button_state1 == HIGH){
    if ((pwmValue >= 1023) || (pwmValue + pwmStep >=1023)){
      pwmValue = 1023;
    }
    else if (pwmValue + pwmStep < 642){
      pwmValue = 642;
    }
    else{
    pwmValue = pwmValue + pwmStep;
    }
  }

  if (Push_button_state2 == HIGH){
    if ((pwmValue < 642) || (pwmValue - pwmStep < 642)){
      pwmValue = 0;
    }
    else{
    pwmValue = pwmValue - pwmStep;
    }
  }

  delay(250);  // Delay untuk mengatur frekuensi perubahan PWM
}
