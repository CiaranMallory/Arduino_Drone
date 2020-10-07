#include <IRremote.h>

int RECV_PIN = 7;
int trigPin = 2;
int echoPin = 4;

int distance;
long duration;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Starts Remote Receiver
  // Motor pins
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // Distance Sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
   if (irrecv.decode(&results)){
    //Serial.println(results.value, HEX);
    irrecv.resume();
     
      digitalWrite(trigPin, LOW); // Checks Sensor is LOW
      delayMicroseconds(2);

      digitalWrite(trigPin, HIGH); // Sends out waves
      delayMicroseconds(10); // For 10uS
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH); // Echo returns to receiver
      distance = duration*0.034/2; // Calculate distance using value returned by echo(reflection)

      //Serial.print("Distance: "); // Display distance in serial monitor
      //Serial.println(distance);
    
     
     
     // Horizontal Movements
     // Forward
     if (results.value == 0xFD50AF) // If forward button is pressed
      {
       // Back Motors
        analogWrite(9, 255);
        analogWrite(5, 255);
       // Front Motors
        analogWrite(3, 191);
        analogWrite(10, 191);
        // Back motors are made turn faster than front motors by controlling the duty cycle on the pins
      }
     
     // Reverse
     if (results.value == 0xFD10EF)
      {
        analogWrite(9, 191);
        analogWrite(5, 191);
       
        analogWrite(3, 255);
        analogWrite(10, 255);
      }
     
     // Right
     if (results.value == 0xFD609F) // If turn right button is pressed
      {
       // Right Motors
        analogWrite(3, 191);
        analogWrite(5, 191);
       // Left Motors
        analogWrite(9, 255);
        analogWrite(10, 255);
        // Left motors are made turn faster than right motors by controlling the duty cycle on the pins 
      }
     
     // Left
     if (results.value == 0xFD20DF) // If turn left button is pressed
      {
        analogWrite(3, 255);
        analogWrite(5, 255);

        analogWrite(9, 191);
        analogWrite(10, 191);
        // Right motors are made turn faster than left motors by controlling the duty cycle on the pins
      }
     
     
     // Vertical Movements
     // Downwards
     if (results.value == 0xFD906F) // If descend button is pressed
     {
        analogWrite(9, 191); // Slow pin duty cycle(motor speed)  
        analogWrite(5, 191);
        analogWrite(3, 191);
        analogWrite(10, 191);
        delay(3000); // Wait 3s
        analogWrite(9, 127); // Slow pin duty cycle(motor speed) even more
        analogWrite(5, 127);
        analogWrite(3, 127);
        analogWrite(10, 127);   
     }
     
     // Upwards
     if (results.value == 0xFD807F)
     {
        analogWrite(9, 127);
        analogWrite(5, 127);
        analogWrite(3, 127);
        analogWrite(10, 127);
        delay(3000);
        analogWrite(9, 191);
        analogWrite(5, 191);
        analogWrite(3, 191);
        analogWrite(10, 191);
     }
     
     
     
     // Landing algorithm
     // Descent speed is set according to distance from the ground
     if (results.value == 0xFD40BF) // If land button is pressed
     {
      // If distance is greater than 100 values decrease at 64 duty cycle(motor speed)
      // Descend fast
      if (distance > 100){
        analogWrite(3, 64);
        analogWrite(5, 64);
        analogWrite(9, 64);
        analogWrite(10, 64);
      }
      // If distance is between values decrease at 127 duty cycle(motor speed)
      // Descend moderately
      if (distance < 100 && distance > 75){
        analogWrite(3, 127);
        analogWrite(5, 127);
        analogWrite(9, 127);
        analogWrite(10, 127);
      }
      // If distance is between values decrease at 191 duty cycle(motor speed)
      // Descend slowly
      if (distance < 75 && distance > 25){
        analogWrite(3, 191);
        analogWrite(5, 191);
        analogWrite(9, 191);
        analogWrite(10, 191);
      }
      // If distance is less than 25 slowly descend and eventually stop motors
      if (distance < 25){
        analogWrite(3, 220);
        analogWrite(5, 220);
        analogWrite(9, 220);
        analogWrite(10, 220);
        delay(3000);
        analogWrite(3, 0);
        analogWrite(5, 0);
        analogWrite(9, 0);
        analogWrite(10, 0);
      }
     }
   }
}
