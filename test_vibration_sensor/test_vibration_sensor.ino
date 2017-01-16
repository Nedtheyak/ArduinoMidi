const int PIEZO_PIN = A0; // Piezo output
int delayTime = 15;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoADC = analogRead(PIEZO_PIN);
  float piezoV = piezoADC;
  Serial.println(piezoV); // Print the voltage.
  delay(delayTime);
}
