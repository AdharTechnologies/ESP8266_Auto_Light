/*
version_history
version 0 | 2024/09/05
blynk connect, set mode, detect button mode

version 1 | 2024/09/05
blynk connect, set mode, detect button mode flag system implemented

version 4 | 2024/09/08
Integrated with Blynk. button to set LDR mode and online mode.
blynk switch to turn ON/OFF light

problem: Blynk Write function is triggered only when there is a change
need to implement timer

version 5 | 2024/09/08
full syncing done. above problem solved
LDR mode
Online mode

*/

//Importing Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// for WiFi
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Your WiFi Name";
char pass[] = "Your WiFi Password";
bool isConnected = false; // Flag to check connection status

//Defining pins
#define power_led 14 //D5 
#define status_led 12 //D6
#define mode_button 2 //D4   Do not use D8 it cannot be pulled UP
#define realy_p 4 //D2 
#define ldr_p 13 //D7

// Time constants
const unsigned long press_duration = 1000; // 2 seconds
unsigned long button_press_start = 0;
bool button_pressed = false;
bool button_toggled = false;

//Array to hold modes
int modes[1] = {}; // 0 means LDR, 1 means online

//Array to hold messegaes
char s_buffer[50];

//Defining variables for timer
unsigned long curr_t, diff_t;
unsigned long prev_t = 0; 
bool last_button_state = HIGH; // Track previous button state

// Create a BlynkTimer object
BlynkTimer timer;

String version = "Auto Light ESP8266 D1 mini | version 1";