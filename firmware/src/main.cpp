#include <Arduino.h>

#define TOGGLE_PIN 3
#define LED_PIN 8               // onboard blue LED, active low
#define PERIOD_MS 60 * 1000     // overall cycle time (60 s)
#define PULSE_MS 300              // how long each button press (LOW) lasts
#define PULSE_SPACING_MS 600      // time between the two presses
#define LED_PERIOD_MS 2000        // blink period
#define LED_ON_MS 1000            // 50% duty cycle

static void press() {
    Serial.printf("[%lu] Toggling units button\n", millis() / 1000);
    pinMode(TOGGLE_PIN, OUTPUT);
    digitalWrite(TOGGLE_PIN, LOW);
    delay(PULSE_MS);
    pinMode(TOGGLE_PIN, INPUT); // release to hi-Z
}

void setup() {
    Serial.begin(115200);
    pinMode(TOGGLE_PIN, INPUT); // start tri-stated (hi-Z)
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // LED off
    Serial.printf("[%lu] Startup complete\n", millis() / 1000);
}

void loop() {
    static unsigned long cycleStart = 0;
    static int pressCount = 0;
    static bool ledOn = false;
    static unsigned long ledStart = 0;

    unsigned long now = millis();

    // double button press at start of each period
    if (pressCount == 0 || (pressCount == 1 && now - cycleStart >= PULSE_SPACING_MS)) {
        press();
        pressCount++;
        if (pressCount == 1) {
            cycleStart = now;
        }
    } else if (pressCount >= 2 && now - cycleStart >= PERIOD_MS) {
        pressCount = 0;
    }

    // heartbeat LED blink
    if (now - ledStart >= (ledOn ? LED_ON_MS : LED_PERIOD_MS - LED_ON_MS)) {
        ledOn = !ledOn;
        ledStart = now;
        digitalWrite(LED_PIN, ledOn ? LOW : HIGH); // active low
    }
}
