#include <Servo.h>

#define SERVO_PIN 9  
#define BUTTON_PIN 8  
#define POT_PIN A0  

int leds[] = {2, 3,6, 7};  
Servo myServo;
int timerDelay;

void setup() {
    myServo.attach(SERVO_PIN);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    for (int i = 0; i < 6; i++) {
        pinMode(leds[i], OUTPUT);
    }
    
    myServo.write(0);  // Start at 0 degrees
    Serial.begin(9600);
}

void loop() {
    // Read potentiometer and set delay dynamically
    timerDelay = map(analogRead(POT_PIN), 0, 1023, 500, 3000);
    Serial.print("Timer Delay: ");
    Serial.println(timerDelay);

    // LEDs turn ON one by one
    for (int i = 0; i < 6; i++) {
        digitalWrite(leds[i], HIGH);
        delay(timerDelay);

        // If button is pressed, reset immediately
        if (digitalRead(BUTTON_PIN) == LOW) {
            manualReset();
            return;
        }
    }

    // LEDs turn OFF one by one
    for (int i = 5; i >= 0; i--) {
        digitalWrite(leds[i], LOW);
        delay(timerDelay);

        // If button is pressed, reset immediately
        if (digitalRead(BUTTON_PIN) == LOW) {
            manualReset();
            return;
        }
    }

    // Flip hourglass (Move servo to 180° and back)
    Serial.println("Flipping Hourglass...");
    myServo.write(180);
    delay(1000);
    myServo.write(0);
    delay(1000);

    // Manual Reset using Button
    if (digitalRead(BUTTON_PIN) == LOW) {
        manualReset();
    }
}

// Function to manually reset everything using the button
void manualReset() {
    Serial.println("Manual Reset Activated!");
    
    // Turn off all LEDs
    for (int i = 0; i < 6; i++) {
        digitalWrite(leds[i], LOW);
    }

    // Reset Servo
    myServo.write(0);
    
    // Wait for button release (to avoid multiple triggers)
    while (digitalRead(BUTTON_PIN) == LOW);
}
