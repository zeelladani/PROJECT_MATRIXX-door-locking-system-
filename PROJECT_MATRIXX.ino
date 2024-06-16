#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

// Define the size of the keypad
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the keymap
char keys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
  
// Define pin connections for the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Create the OLED display object
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const String password1 = "2769"; // Define the first correct password
const String password2 = "9335"; // Define the second correct password
String inputPassword = ""; // Store the input password

const int greenLEDPin = 10; // Pin for the green LED
const int redLEDPin = 11;   // Pin for the red LED

void setup() {
  Serial.begin(9600); // Initialize the serial communication

  pinMode(greenLEDPin, OUTPUT); // Set the green LED pin as output
  pinMode(redLEDPin, OUTPUT);   // Set the red LED pin as output

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is a common I2C address for SSD1306
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("PSWD");
  display.display();
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    if (key == '#') { // Enter key
      display.clearDisplay();
      if (inputPassword == password1 || inputPassword == password2) {
        digitalWrite(greenLEDPin, HIGH); // Turn on green LED
        digitalWrite(redLEDPin, LOW);    // Ensure red LED is off
        display.setTextSize(2);
        display.setCursor(23, 23);
        display.print("Granted");
        Serial.println("Access Granted");
        display.display();
        delay(1000); // Keep LED on for 1 second
        digitalWrite(greenLEDPin, LOW); // Turn off green LED after 1 second
      } else {
        display.setTextSize(2);
        display.setCursor(25, 27);
        display.print("Denied");
        Serial.println("Access Denied");
        display.display();
        
        for (int i = 0; i < 5; i++) { // Blink red LED 3 times (700ms on, 700ms off)
          digitalWrite(redLEDPin, HIGH); // Turn on red LED
          delay(350); // Wait 700ms
          digitalWrite(redLEDPin, LOW); // Turn off red LED
          delay(200); // Wait 700ms
        }
      }
      display.setTextSize(1);
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("PSWD");
      inputPassword = ""; // Reset input
    } else if (key == '*') { // Clear key
      inputPassword = "";
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("PSWD");
      digitalWrite(greenLEDPin, LOW); // Turn off green LED
      digitalWrite(redLEDPin, LOW);   // Turn off red LED
    } else {
      inputPassword += key;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("PSWD");
      display.setCursor(0, 10); // Move to the second line
      display.print(inputPassword);
    }
    display.display();
  }
}
