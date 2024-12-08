//Required to be on top for Blynk 
//Get from Blynk project template
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

//Including Config File
#include "esp8266D1mini_config.h"

//Function to check conection
void connection_status(){

  // Check if connected to Blynk server
  if (Blynk.connected() && !isConnected) {
    isConnected = true;
    //Serial.println("ESP8266 is connected to Blynk!");
    analogWrite(status_led, 30);
  }
  else if (!Blynk.connected() && isConnected){
    isConnected = false;
    analogWrite(status_led, 0);
  }

}

void checkSwitchState() {
  Blynk.syncVirtual(V1);  // Sync the state of V1 periodically
}

void mode_detect(){

  int button_state = digitalRead(mode_button);
  //Serial.print("button_state = "); Serial.println(button_state);
  //Serial.print("mode = "); Serial.println(modes[0]);

  if(button_state == LOW){ //mode button is pressed
    if(!button_pressed){ //if button wasn't previously pressed
      button_pressed = true;
      button_press_start = millis(); //Record the time when button is pressed
    }
    else{
      //Check if button has pressed for required duration
      if (millis() - button_press_start >= press_duration){
        if(!button_toggled){
          // Toggle the mode
          modes[0] = (modes[0] == 0) ? 1 : 0;
          button_toggled = true; // Ensure the mode is only toggled once per press duration
          delay(100);
        }
      }
    } 
  }
  else{ //button is not pressed
    if(button_pressed){ //If button wasn't pressed
      button_pressed = false;
      button_toggled = false;
    }
  }
}

void mode_update(){

  if (modes[0] == 0){
    //Serial.println("LDR");
    Blynk.virtualWrite(V0, "LDR");
  }
  else if (modes[0] == 1){
    //Serial.println("ONLINE");
    Blynk.virtualWrite(V0, "ONLINE");
  }
}

// BLYNK_WRITE function to handle virtual pin V1 changes
BLYNK_WRITE(V1){
  // Check if modes[0] = 1 before processing the Blynk data | modes[0] = 1 means online mode
  if (modes[0] == 1){
    if (param.asInt() == 1){
      digitalWrite(realy_p, HIGH);  // Turn relay ON
    }
    // If modes[0] is not 1, ignore the Blynk input.
    else{
      digitalWrite(realy_p, LOW);   // Turn relay OFF
    }
  }

}

void ldr_toggle(int pin){

  int x = digitalRead(pin);
  //Serial.println(x);

  if(modes[0] == 1){
    
  }
  else{
    if( modes[0] == 0 && x == 1){
      digitalWrite(realy_p, HIGH);
    }
    else if( modes[0] == 0 && x == 0){
      digitalWrite(realy_p, LOW);
    }
  }

} 

void setup() {
  // Start the Serial Monitor for debugging
  Serial.begin(9600);
  delay(1000);
  Serial.print(version);

  // Setting up pinMode
  pinMode(power_led, OUTPUT); analogWrite(power_led, 30);
  pinMode(status_led, OUTPUT); analogWrite(status_led, 0);

  pinMode(realy_p, OUTPUT);
  pinMode(ldr_p, INPUT_PULLUP);

  pinMode(mode_button, INPUT_PULLUP);
  //pinMode(mode_button, INPUT);

  // Initializing Blynk
  Blynk.begin(auth, ssid, pass); 
  // Check every 1 second
  timer.setInterval(1000L, checkSwitchState);  

}

void loop() {

  Blynk.run();
  timer.run();    // Timer handler
  connection_status();
  mode_detect();
  mode_update();
  ldr_toggle(ldr_p);

  // periodically sync the switch state
  //Blynk.syncVirtual(V1); 

}
