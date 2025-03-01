class SevenSegmentDisplay {
private:
    // Pin definitions for the 7-segment display
    int button = 2;
    int pina = 3;    
    int pinb = 4;    
    int pinc = 5;   
    int pind = 6;    
    int pine = 7;    
    int pinf = 8;    
    int ping = 9;   
    int pindp = 10;  

    int currentValue = 0;
    
    // Turn all segments off
    void clearLED() {
        digitalWrite(pina, LOW);
        digitalWrite(pinb, LOW);
        digitalWrite(pinc, LOW);
        digitalWrite(pind, LOW);
        digitalWrite(pine, LOW);
        digitalWrite(pinf, LOW);
        digitalWrite(ping, LOW);
        digitalWrite(pindp, LOW);
    }

    // Write a number (0-9) to the display
    void write(int num) {
        bool W, X, Y, Z;
        bool a, b, c, d, e, f, g;

        // Extract binary bits from the number
        Z = num & 1;
        num = num >> 1;
        Y = num & 1;
        num = num >> 1;
        X = num & 1;
        num = num >> 1;
        W = num & 1;

        // K-mapping for segment control (a-g)
        a = W + Y + X * Z + !X * !Z;
        b = !X + !Y * !Z + Y * Z;
        c = X + !Y + Z;
        d = W + !X * !Z + !X * Y + Y * !Z + X * !Y * Z;
        e = !X * !Z + Y * !Z;
        f = W + X * !Y + X * !Z + !Y * !Z;
        g = W + X * !Y + !X * Y + Y * !Z;

        // Set segment pins
        digitalWrite(pina, a);
        digitalWrite(pinb, b);
        digitalWrite(pinc, c);
        digitalWrite(pind, d);
        digitalWrite(pine, e);
        digitalWrite(pinf, f);
        digitalWrite(ping, g);
    }

public:
    
    SevenSegmentDisplay() {
        pinMode(pina, OUTPUT);
        pinMode(pinb, OUTPUT);
        pinMode(pinc, OUTPUT);
        pinMode(pind, OUTPUT);
        pinMode(pine, OUTPUT);
        pinMode(pinf, OUTPUT);
        pinMode(ping, OUTPUT);
        pinMode(pindp, OUTPUT);

        pinMode(button, INPUT_PULLUP);
        
        clearLED(); // Turn off all segments initially
    }

    // Function to display a number based on the rules
    void displayNumber(int num) {
        clearLED(); // Clear the display before updating

        if (num > 19) {
            // Turn on the DOT and turn off all other segments
            digitalWrite(pindp, HIGH);
        } else if (num < 0) {
            // Turn off the display
            clearLED();
        } else if (num >= 0 && num <= 9) {
            // Display the number (0-9)
            write(num);
        } else if (num >= 10 && num <= 19) {
            // Display the ones place and turn on the DOT
            write(num % 10); // Display the ones place
            digitalWrite(pindp, HIGH); // Turn on the DOT
        }
    }

    void checkButton(){
      bool buttonState = digitalRead(button);
      if(buttonState == LOW){
        currentValue = (currentValue + 1) % 10;
        displayNumber(currentValue);
        delay(200);
      }
    }
};

SevenSegmentDisplay display;

void setup() {
     
}

void loop() {
  display.checkButton();
}
