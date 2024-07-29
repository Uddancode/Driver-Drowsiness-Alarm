#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90640.h>

// Define pins for buzzer
#define BUZZER_PIN 9

// Initialize display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize thermal camera
Adafruit_MLX90640 mlx;
float frame[32*24];

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize display
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Initialize thermal camera
  if (!mlx.begin(MLX90640_I2C_ADDR, &Wire)) {
    Serial.println("MLX90640 not found!");
    while (1);
  }
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_8_HZ);
}

void loop() {
  // Capture thermal image
  if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed to read frame");
    return;
  }

  // Process the frame to detect drowsiness (this is a placeholder for the actual processing)
  bool isDrowsy = detectDrowsiness(frame);

  // Display the result on the OLED
  display.clearDisplay();
  display.setCursor(0,0);
  if (isDrowsy) {
    display.println("Driver is drowsy!");
    triggerBeep();
  } else {
    display.println("Driver is alert.");
  }
  display.display();

  // Delay before capturing the next frame
  delay(1000);
}

bool detectDrowsiness(float* frame) {
  // Placeholder for actual drowsiness detection logic
  // In a real implementation, this would involve running the frame through a pre-trained model
  // Here, we'll just simulate drowsiness detection for demonstration purposes
  float averageTemp = 0;
  for (int i = 0; i < 32 * 24; i++) {
    averageTemp += frame[i];
  }
  averageTemp /= (32 * 24);

  // Simulate drowsiness detection based on temperature (this is just a placeholder)
  return averageTemp > 30.0;
}

void triggerBeep() {
  // Turn on the buzzer
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000); // Beep for 1 second
  // Turn off the buzzer
  digitalWrite(BUZZER_PIN, LOW);
}
