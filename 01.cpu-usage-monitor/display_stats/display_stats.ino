
// Declaration

class Display 
{
private:
  byte latchPin;
  byte clockPin;
  byte dataPin;

public:
  Display(byte, byte, byte);

  void setValue(byte);
};

// Implementation

Display::Display(byte latchPin, byte clockPin, byte dataPin)
{
  this->latchPin = latchPin;
  this->clockPin = clockPin;
  this->dataPin = dataPin;

  pinMode(this->latchPin, OUTPUT);
  pinMode(this->clockPin, OUTPUT);
  pinMode(this->dataPin, OUTPUT);
}

void Display::setValue(byte value) 
{
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, LSBFIRST, value);  
  digitalWrite(latchPin, HIGH);
}

// Main block

Display display(8, 12 ,11);


void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available() > 0) 
  {
    byte value = Serial.read();
    display.setValue(value);
    Serial.write(value);
  }
}


