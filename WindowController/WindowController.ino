#include "DHT.h"
#include "Servo.h"

#define DHTPin 2
#define DHTType DHT11
#define ServoPin 9 // Must be a PWM Pin

// Celsius
#define ThresholdTemp 27.0
#define HystTemp 0.5

// Interval between each temperature sample
// Must be at least 1000ms for DHT11
#define RefreshInterval_ms 2000

// The angle in degrees the servo needs
// to turn to fully open the window
#define ActuationAngle 90

DHT dht(DHTPin, DHTType);
Servo servo;

void setup() {
    Serial.begin(9600);
    servo.attach(ServoPin);
    dht.begin();
    delay(2000);
}

void loop() {
    // Assuming window is always closed on startup
    static bool windowOpen = false;

    float temp = dht.readTemperature();
    if (!isnan(temp)) {
        Serial.println(temp);
        // Close window when temperature below ThresholdTemp - HystTemp
        if (windowOpen && temp < ThresholdTemp - HystTemp) {
            Serial.println("Closing window");
            closeWindow();
            windowOpen = false;
        }
        // Open window when temperature above ThresholdTemp
        else if (windowOpen && temp > ThresholdTemp) {
            Serial.println("Opening window");
            openWindow();
            windowOpen = true;
        }
    }

    delay(RefreshInterval_ms);
}

void openWindow() {
    rampServo(0, ActuationAngle, 1);
}
void closeWindow() {
    rampServo(ActuationAngle, 0, -1);
}

void rampServo(int initial, int final, int step) {
    for (int i=initial; i!=final; i+=step) {
        servo.write(i);
        // Assuming 15ms is enough for the servo to move 1 degree
        delay(15 * abs(step));
    }
}
