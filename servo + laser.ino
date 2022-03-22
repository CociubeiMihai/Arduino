#include <Servo.h>  // add servo library
 
Servo myservo0;  // create servo object to control a servo
Servo myservo1;  // create servo object to control a servo
 
int potpin0 = 0;  // analog pin used to connect the potentiometer
int potpin1 = 1;  // analog pin used to connect the potentiometer
int val_pot0;    // variable to read the value from the analog pin
int val_pot1;    // variable to read the value from the analog pin

int led = 13;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
bool startFlag = 0;

unsigned long time_now = 0;
int period = 10000;

 
void setup() {
  myservo0.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo1.attach(8);  // attaches the servo on pin 8 to the servo object

  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600); // Interfata Serial 0, pentru PC
  Serial1.begin(9600); // Interfata Serial 1, pentru Bluetooth
  
}
 
void loop() {
    if (Serial1.available()){ // Citire de pe Bluetooth, trimite la PC
    //Serial.write(Serial1.read());
    char data= Serial1.read(); 
    switch(data)
    {
      case 's':startFlag = 1; Serial1.write("Security system is now working\n");break;
      case 'o':startFlag = 0; Serial1.write("Security system stopped working\n");digitalWrite(led, LOW);break;
      default : break;
    }
  }
  
  if (Serial.available()) // Citire de la PC, trimite la Bluetooth
    Serial1.write(Serial.read());
  if(startFlag){
    val_pot0 = analogRead(potpin0);            // reads the value of the potentiometer (value between 0 and 1023)
    val_pot1 = analogRead(potpin1);            // reads the value of the potentiometer (value between 0 and 1023)
  
    
    val_pot1 = map(val_pot1, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    val_pot0 = map(val_pot0, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    
    myservo1.write(val_pot1);                  // sets the servo position according to the scaled value
    myservo0.write(val_pot0);                  // sets the servo position according to the scaled value
    delay(15);                           // waits for the servo to get there
  
  
    val = digitalRead(sensor);   // read sensor value
    if (val == HIGH) {           // check if the sensor is HIGH
      if (state == LOW) {
        time_now = millis();
        digitalWrite(led, HIGH);   // turn LED ON
        Serial.println("Motion detected!"); 
        Serial1.println("Motion detected!"); 
        state = HIGH;       // update variable state to HIGH
      }
    } 
    if (state == HIGH){
      if(millis()- time_now >= period){
            state = LOW;
            time_now = millis();
            Serial.println("Motion stopped");
            Serial1.println("Motion stopped");
            digitalWrite(led, LOW); // turn LED OFF
        }
    }
  }
  
}
