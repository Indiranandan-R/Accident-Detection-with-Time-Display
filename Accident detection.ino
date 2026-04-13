#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

MPU6050 mpu;
RTC_DS3231 rtc;

int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};
MyData data;

int lcdAddress = 0x27;
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

int relayPin = 7;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1)
      ;
  }
  // Updated to November 6, 2025 at 12:00:00 (noon)
  rtc.adjust(DateTime(2025, 11, 7, 9, 0, 0));

  lcd.init();
  lcd.backlight();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.println("System Started");
  delay(1000);
}

void loop() {
  DateTime now = rtc.now();
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  data.X = map(ax, -19000, 17000, 0, 255);
  data.Y = map(ay, -17000, 17000, 0, 255);
  data.Z = map(az, -18000, 17000, 0, 255);

  lcd.setCursor(0, 0);
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print(" ");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());

  lcd.setCursor(0, 1);
  lcd.print("X:");
  lcd.print(data.X);
  lcd.print(" Y:");
  lcd.print(data.Y);
  lcd.print(" Z:");
  lcd.print(data.Z);

  if (abs(ax) > 15000 || abs(ay) > 15000 || abs(az) > 15000) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }

  delay(500);
}