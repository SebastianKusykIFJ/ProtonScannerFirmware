// digital pins 0...1 - reserved for UART
//inputs from drivers - operation status:
const int statusX1 = 2;
const int statusX2 = 3;
const int statusY1 = 4;
const int statusY2 = 5;
//encoder 0 mark inputs:
const int enkoderX = 6;
const int enkoderY = 7;
//outputs connected to GRBL limit switches:
int forwardX = 8;
int forwardY = 9;
//input for keys:
const int buttonUp = 10;
const int buttonDown = 11;
const int buttonLeft = 12;
const int buttonRight=13;

volatile int previous_statusX1=1;
volatile int previous_statusX2=1;
volatile int previous_statusY1=1;
volatile int previous_statusY2=1;
volatile int previousEnkoderXState=1;//TODO: SPRAWDZ CZY MA BYC 1 CZY 0
volatile int previousEnkoderYState=1;
volatile int previousButtonsState=15;

volatile int statusX1State;
volatile int statusX2State;
volatile int statusY1State;
volatile int statusY2State;
volatile int enkoderXState;
volatile int enkoderYState;
volatile int buttonsState;

volatile char ZeroSearch = '0';

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(statusX1, INPUT_PULLUP);
  pinMode(statusX2, INPUT_PULLUP);
  pinMode(statusY1, INPUT_PULLUP);
  pinMode(statusY2, INPUT_PULLUP);  
  pinMode(enkoderX, INPUT_PULLUP);
  pinMode(enkoderY, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);  
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP); 
}

void loop() {
  int SendStatusFlag=0;
  //read serial and turn ZeroSearch:
  if(Serial.available()) {
    char data_rcvd = Serial.read();   // read one byte from serial buffer and save to data_rcvd
    if(data_rcvd == '1') ZeroSearch='1'; //forward X and Y axis
    if(data_rcvd == '0') ZeroSearch='0'; //forward no axis
    if(data_rcvd == 'x') ZeroSearch='x'; //forward only X
    if(data_rcvd == 'y') ZeroSearch='y'; //forward only Y
    if(data_rcvd == '?') SendStatusFlag=1; //status will be sent in this turn even if no inputs changed
  }
  // read the input pin:
  statusX1State = digitalRead(statusX1);
  statusX2State = digitalRead(statusX2);
  statusY1State = digitalRead(statusY1);
  statusY2State = digitalRead(statusY2);
  enkoderXState = digitalRead(enkoderX);
  enkoderYState = digitalRead(enkoderY);
  buttonsState = digitalRead(buttonUp)*8+digitalRead(buttonDown)*4+digitalRead(buttonLeft)*2+digitalRead(buttonRight);

  //forward encoder zero channels:
  if(ZeroSearch=='1'){
    pinMode(forwardX, OUTPUT);
    pinMode(forwardY, OUTPUT);
    digitalWrite(forwardX, enkoderXState);
    digitalWrite(forwardY, enkoderYState);
  } else if(ZeroSearch=='x'){
    pinMode(forwardX, OUTPUT);
    pinMode(forwardY, INPUT);
    digitalWrite(forwardX, enkoderXState);
  } else if(ZeroSearch=='y'){
    pinMode(forwardX, INPUT);
    pinMode(forwardY, OUTPUT);
    digitalWrite(forwardY, enkoderYState);
  } else{
    pinMode(forwardX, INPUT);
    pinMode(forwardY, INPUT);
  }

  if(SendStatusFlag==1 ||
  statusX1State!=previous_statusX1 || statusX2State!=previous_statusX2 ||
  statusY1State!=previous_statusY1 || statusY2State!=previous_statusY2 ||
  enkoderXState!=previousEnkoderXState || enkoderYState!=previousEnkoderYState ||
  buttonsState!=previousButtonsState){
    Serial.print(statusX1State);
    Serial.print(statusX2State);
    Serial.print(statusY1State);
    Serial.print(statusY2State);
    Serial.print(enkoderXState);
    Serial.print(enkoderYState);
    //if (ZeroSearch==1) Serial.print('Z');
    //else Serial.print('z');
    Serial.print(ZeroSearch);
    Serial.print(buttonsState, HEX);
    Serial.print("\n");

    previous_statusX1=statusX1State;
    previous_statusX2=statusX2State;
    previous_statusY1=statusY1State;
    previous_statusY2=statusY2State;
    previousEnkoderXState=enkoderXState;
    previousEnkoderYState=enkoderYState;
    previousButtonsState=buttonsState;
  }
  delay(1);
}
