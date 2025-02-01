#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
Servo myservo;

const int IR_SENSOR1 = 2;  // First IR sensor (entrance)
const int IR_SENSOR2 = 3;  // Second IR sensor (exit)
const int SERVO_PIN = 5;   // Servo motor pin

int totalSlots = 4;  // Total number of parking slots
int availableSlots = 4;  // Initially all slots are available

void setup() {
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

  myservo.attach(SERVO_PIN);  // Attaches the servo on pin 5 to the servo object
  myservo.write(90);  // Set servo to closed position (adjust angle as needed)

  pinMode(IR_SENSOR1, INPUT);
  pinMode(IR_SENSOR2, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Car Parking");
  lcd.setCursor(0, 1);
  lcd.print("System");
  delay(2000);
  updateLCD();
}

void loop() {
  if (digitalRead(IR_SENSOR1) == LOW) {
    if (availableSlots > 0) {
      // Car entering
      myservo.write(0);  // Open barrier (adjust angle as needed)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome!");
      lcd.setCursor(0, 1);
      lcd.print("Slot Available");
      delay(2000);  // Wait for car to pass
      availableSlots--;
      updateLCD();
    } else {
      // Parking is full
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorry!");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full");
      delay(2000);
      updateLCD();
    }
  }

  if (digitalRead(IR_SENSOR2) == LOW && availableSlots < totalSlots) {
    // Car exiting
    myservo.write(0);  // Open barrier (adjust angle as needed)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thank You!");
    lcd.setCursor(0, 1);
    lcd.print("Visit Again");
    delay(2000);  // Wait for car to pass
    availableSlots++;
    updateLCD();
  }

  myservo.write(90);  // Close barrier (adjust angle as needed)

  // Check if parking is full
  if (availableSlots == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Parking Full");
    lcd.setCursor(0, 1);
    lcd.print("Please Wait");
    delay(2000);
    updateLCD();
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Available Slots:");
  lcd.setCursor(0, 1);
  lcd.print(availableSlots);
} 