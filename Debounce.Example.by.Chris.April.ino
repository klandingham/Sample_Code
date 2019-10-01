/*-------------------------------------------------------------
  Debounce pushbutton code created for the following functions:
  1.  Single press
  2.  Quick double press
  3.  Long press

  Aim is to get this working in an array
  By Chris April 2015
*///-------------------------------------------------------------

const int num = 2;                     // number of switches and corresponding led indicators
const int led[] = {8, 7};              // leds to arduino pin
const int button[] = {4, 2};           // push buttons connected to ground and Arduino pins as numbered
unsigned long previousMillis = 0;      // used in pushbutton timinig
unsigned long currentMillis = 0;       // used in pushbutton timinig
unsigned long pressStartTime = 0;      // used in pushbutton double click recognition
unsigned long pressEndTime = 0;        // used in pushbutton double click recognition
unsigned long doubleClickMax = 150;    // max time allowed between "double clicks"
unsigned long buttonPressLong = 1000;  // min time required for long press delay
const int debounce = 50;               // to stop mulitiple false triggering
int switchStage = 1;                   // variable used to find position in loops
int buttonState[] = {1, 1};            // HIGH or LOW
//------------------------------------
void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < num; i++) {
    pinMode(button[num], INPUT_PULLUP);
    pinMode(led[num], OUTPUT);
  }
}
//-------------------------------------

void loop() {
  buttonUpdate();
  // Call other functions here
}

//--------------------------------------
void buttonUpdate () {
  currentMillis = millis();

  for (int i = 0; i < num; i++) {
    Serial.println("1st for");
    buttonState[num] = digitalRead(button[num]);
  }

  // record the time of the button first press
  if (switchStage == 1 && buttonState[num] == LOW && currentMillis - previousMillis > debounce) {
    pressStartTime = millis();
    previousMillis = currentMillis;
    switchStage = 2;
  }

  // record the time of the button release
  if (switchStage == 2 && buttonState[num] == HIGH && currentMillis - previousMillis > debounce) {
    pressEndTime = millis();
    currentMillis = millis();
    previousMillis = currentMillis;
    switchStage = 3;
  }

  // check if conditions met for a long button hold duration
  if (switchStage == 3 && pressEndTime - pressStartTime > buttonPressLong) {
    clickLong();
    switchStage = 1;
    pressStartTime = 0;
    pressEndTime = 0;
    previousMillis = currentMillis;
  }

  // check if conditions met for a short button hold duration
  if (switchStage == 3 && pressEndTime - pressStartTime < buttonPressLong) {
    switchStage = 4;  // condition satisfied, so lets check if double click
  }

  // check if the button is quickly pressed again for a "double click".
  if (switchStage == 4 && millis() - previousMillis < doubleClickMax && buttonState[num] == LOW && currentMillis - previousMillis > debounce) { // double click satisfied
    clickDouble();
    switchStage = 1;
    pressStartTime = 0;
    pressEndTime = 0;
    previousMillis;
    currentMillis;
  }

  if (switchStage == 4 && millis() - previousMillis > doubleClickMax && buttonState[num] == HIGH && currentMillis - previousMillis > debounce) { // single click satisfied
    clickQuick();
    switchStage = 1;
    pressStartTime = 0;
    pressEndTime = 0;
    previousMillis = currentMillis;
  }
} // end void buttonUpdate

//--Three operations below for testing only
//-------------------------------------
void clickQuick() {
  for (int i = 0; i < num; i++) {
    digitalWrite(led[num], HIGH);
    delay(20);
    digitalWrite(led[num], LOW);
  }
}

//-------------------------------------
void clickLong() {
  for (int i = 0; i < num; i++) {
    digitalWrite(led[num], HIGH);
    delay(2000);
    digitalWrite(led[num], LOW);
  }
}

//-------------------------------------
void clickDouble () {
  for (int i = 0; i < num; i++) {
    delay(1000);
    digitalWrite(led[num], HIGH);
    delay(20);
    digitalWrite(led[num], LOW);
    delay(500);
    digitalWrite(led[num], HIGH);
    delay(20);
    digitalWrite(led[num], LOW);
  }
}
