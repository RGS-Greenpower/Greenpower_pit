#include <SendOnlySoftwareSerial.h>
//#include <LiquidCrystal.h>

float speed;
float current;
float av_current;
float voltage;
float battery_temp;
float motor_temp;
float lap_time;
String s_speed;
String s_current;
String s_av_current;
String s_voltage;
String s_battery_temp;
String s_motor_temp;
char c_speed[3];
char c_current[3];
char c_av_current[3];
char c_voltage[3];
char c_battery_temp[3];
char c_motor_temp[3];
char buffer[16];
String raw;
String send;
String display;
const int lap_pin = 2;
int lap_last;
SendOnlySoftwareSerial lcd_data = SendOnlySoftwareSerial(5);
//LiquidCrystal lcd_data(8, 7, 6, 5, 4, 3);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial1.begin(4800);
  while (!Serial1) {
    ;
  }
  //default values
  speed = 0;
  current = 0;
  av_current = 0;
  voltage = 0;
  battery_temp = 0;
  motor_temp = 0;
  lap_time = 0;
  lap_last = -1;
  //lcd_data.begin(20, 4);
  pinMode(lap_pin, INPUT);
  digitalWrite(lap_pin, HIGH);
}

void loop() {
  //get serial data from computer and send it to car
  if (Serial.available() > 0) {
    Serial1.print(Serial.read());
  }
  //get serial data from car and format it correctly
  if (Serial1.available() > 0) {
    Serial1.readBytes(buffer, 15);
    buffer[15] = 0;
    raw = String(buffer);
    s_speed = raw.substring(0, 3);
    s_current = raw.substring(3, 6);
    s_av_current = raw.substring(6, 9);
    s_voltage = raw.substring(9, 12);
    s_battery_temp = raw.substring(12, 15);
    s_motor_temp = raw.substring(15, 18);
    s_speed.toCharArray(c_speed, 3);
    s_current.toCharArray(c_current, 3);
    s_av_current.toCharArray(c_av_current, 3);
    s_voltage.toCharArray(c_voltage, 3);
    s_battery_temp.toCharArray(c_battery_temp, 3);
    s_motor_temp.toCharArray(c_motor_temp, 3);
    speed = float(int(c_speed)) / 10;
    current = float(int(c_current)) / 10;
    av_current = float(int(c_av_current)) / 10;
    voltage = float(int(c_voltage)) / 10;
    battery_temp = float(int(c_battery_temp)) / 10;
    motor_temp = float(int(c_motor_temp)) / 10;
    //print values on LCD
  }

  if (digitalRead(lap_pin) == LOW) {
    if (lap_last >= 0) {
      lap_time = (millis() - lap_last) / 60000;
    }
    lap_last = millis();
  }

  send = String(int(speed * 10)) + String(int(current * 10)) + String(int(av_current * 10)) + String(int(voltage * 10)) + String(int(battery_temp * 10)) + String(int(motor_temp * 10)) + String(int(lap_time * 10));
  Serial.println(send);

  display = String("Speed: ") + String(speed) + String(" mph, Current: ") + String(current) + String(" A, Av Current: ") + String(av_current) + String(" A, Voltage: ") + String(voltage) + String(" V, Lap Time: ") + String(lap_time) +String(" mins.");
  lcd_data.println(display);

  /*
  lcd_data.setCursor(0, 0);
  lcd_data.print("Speed: ");
  lcd_data.print(speed);
  lcd_data.print("mph");
  lcd_data.setCursor(0, 1);
  lcd_data.print("Current: ");
  lcd_data.print(current);
  lcd_data.print("A");
  lcd_data.setCursor(0, 2);
  lcd_data.print("Voltage: ");
  lcd_data.print(voltage);
  lcd_data.print("V");
  lcd_data.setCursor(0, 3);
  lcd_data.print("Bat: ");
  lcd_data.print(battery_temp);
  lcd_data.print("Mot: ");
  lcd_data.print(motor_temp);
  */
}
