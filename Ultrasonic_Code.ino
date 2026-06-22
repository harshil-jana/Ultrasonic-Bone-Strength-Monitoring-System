#include <Wire.h> // I2C communication library
#include <LiquidCrystal_I2C.h> // LCD I2C library
// Initialize LCD: I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Pin definitions
int txPin = 9; // Digital pin 9 drives the ultrasonic transmitter
int rxPin = A0; // Analog pin A0 reads the amplified receiver signal
// Threshold for bone strength classification
// Value determined through experimental calibration
int threshold = 250;
// =================================================
// SETUP FUNCTION - Runs once at power-on/reset
// =================================================
void setup() {
 pinMode(txPin, OUTPUT); // Set TX pin as output
 Serial.begin(9600); // Initialize serial for debugging
 lcd.init(); // Initialize the LCD
 lcd.backlight(); // Turn on LCD backlight
 // Display startup message
 lcd.print("Bone Detector");
 delay(2000); // Show for 2 seconds
 lcd.clear(); // Clear for main display
}
// =================================================
// LOOP FUNCTION - Repeats continuously
// =================================================
void loop() {
 // --- PHASE 1: ULTRASONIC PULSE TRANSMISSION ---
 // Generate 800 cycles of ~40 kHz square wave
 // Each cycle = 20 microseconds (10 HIGH + 10 LOW)
 // Total burst duration ≈ 16 ms
 for (int i = 0; i < 800; i++) {
 digitalWrite(txPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(txPin, LOW);
 delayMicroseconds(10);
 }


 // Read 25 samples with 5 ms delay between readings
 // Averaging improves stability and reduces noise
 int sum = 0;
 for (int i = 0; i < 25; i++) {
 sum += analogRead(rxPin); // Read ADC value (0–1023)
 delay(5);
 }

 int value = sum / 25;

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Signal:");
 lcd.print(value);
 lcd.setCursor(0, 1);

 if (value > 700) {
 lcd.print("No Signal"); // Saturation / noise
 }
 else if (value > threshold) {
 lcd.print("Bone: STRONG"); // Strong transmission
 }
 else {
 lcd.print("Bone: WEAK"); // Weak transmission
 }
 // Serial output for debugging
 Serial.print("ADC Value: ");
 Serial.println(value);
 delay(400); // Delay before next cycle
}