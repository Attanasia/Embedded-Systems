#include <RTClib.h>
#include <dht_nonblocking.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 2
#define BUTTON_C_PIN 3 //Button for Celsius
#define BUTTON_K_PIN 4 //Button for Kelvin
#define BUTTON_F_PIN 5 //Button for Fahrenheit

DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
RTC_DS1307 rtc;
float temperature = 0.0;
float humidity = 0.0;

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    
bool lastButtonStateC = HIGH;         
bool buttonStateC = HIGH;          
bool lastButtonStateK = HIGH;         
bool buttonStateK = HIGH;  
bool lastButtonStateF = HIGH;         
bool buttonStateF = HIGH;     

int selectedUnit = 0;  // 0 = Celsius, 1 = Fahrenheit, 2 = Kelvin

bool reading_available(float *temperature, float *humidity) {
    static unsigned long timestamp = millis(); 
    // Check if 3 seconds have passed since the last reading
    if (millis() - timestamp > 3000ul) {  
        if (dht_sensor.measure(temperature, humidity)) {  // Get new reading
            timestamp = millis();  // Update timestamp
            return true;
        }
    }
    return false;
}

void setup() {
  
  Serial.begin(9600);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);
  pinMode(BUTTON_K_PIN, INPUT_PULLUP);
  pinMode(BUTTON_F_PIN, INPUT_PULLUP);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("No RTC component detected");
    while (1);  // Stop if RTC is not connected
  }
}

void loop() {
    // Handle button press for Celsius
    int readingC = digitalRead(BUTTON_C_PIN);
    if (readingC != lastButtonStateC) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingC == LOW && buttonStateC == HIGH) {
            selectedUnit = 0;  // Set unit to Celsius
            Serial.println("Switched to Celsius");
            buttonStateC = LOW;
        } else if (readingC == HIGH && buttonStateC == LOW) {
            buttonStateC = HIGH;
        }
    }
    lastButtonStateC = readingC;

    // Handle button press for Kelvin
    int readingK = digitalRead(BUTTON_K_PIN);
    if (readingK != lastButtonStateK) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingK == LOW && buttonStateK == HIGH) {
            selectedUnit = 2;  // Set unit to Kelvin
            Serial.println("Switched to Kelvin");
            buttonStateK = LOW;
        } else if (readingK == HIGH && buttonStateK == LOW) {
            buttonStateK = HIGH;
        }
    }
    lastButtonStateK = readingK;

    // Handle button press for Fahrenheit
    int readingF = digitalRead(BUTTON_F_PIN);
    if (readingF != lastButtonStateF) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingF == LOW && buttonStateF == HIGH) {
            selectedUnit = 1;  // Set unit to Fahrenheit
            Serial.println("Switched to Fahrenheit");
            buttonStateF = LOW;
        } else if (readingF == HIGH && buttonStateF == LOW) {
            buttonStateF = HIGH;
        }
    }
    lastButtonStateF = readingF;

    // Retrieve the current date and time from the RTC
    DateTime now = rtc.now();

    // Check if new DHT11 data is available
    if (reading_available(&temperature, &humidity)) {
        // Print the full date and time
        Serial.print("Date: ");
        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);

        Serial.print(" Time: ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);

        // Convert and print temperature based on selected unit
        Serial.print(", Temperature=");
        float displayedTemp = temperature;

        if (selectedUnit == 1) {  // Fahrenheit
            displayedTemp = (temperature * 9.0 / 5.0) + 32.0;
            Serial.print(displayedTemp, 1);
            Serial.print("°F");
        } else if (selectedUnit == 2) {  // Kelvin
            displayedTemp = temperature + 273.15;
            Serial.print(displayedTemp, 1);
            Serial.print("K");
        } else {  // Celsius
            Serial.print(displayedTemp, 1);
            Serial.print("°C");
        }

        Serial.print(", Humidity=");
        Serial.print(humidity, 1);
        Serial.println("%");
    }

    delay(100);  // Wait for next cycle
}
