#include <Arduino.h>
#include <LCD812L-B.h>

// Define the pins connected to the LCD
#define CLK_PIN 2
#define DATA_PIN 3
#define HKS_PIN 4

LCD821LB lcd;

void setup() {
  // Initialize the LCD
  lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
  
  // Clear the display
  lcd.clear();
}

void loop() {
  // Print a string
  lcd.print("HELLO");
  delay(2000);

  // Print an integer
  lcd.print((signed long long)1234567890);
  delay(2000);

  // Print a double
  lcd.print(3.14159);
  delay(2000);
  
  // Print a right-padded string
  lcd.print("PAD", true);
  delay(2000);
}
