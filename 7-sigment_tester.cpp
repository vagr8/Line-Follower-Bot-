#include<TimerOne.h>
#include<Arduino.h>

const int ledPin[4] = {4, 5, 6, 7}; // Suppose LED pin = 2 4 6 7

const int s = 14; // pin for switch

const int IRpin[4] = {4, 16, 15, 5}; //pin for IR sensor

const int displayer[4] = {10, 8, 9, 1}; // pin for 7-segment display 

int black[4] = {400, 480, 400, 100};    // thershold for black. If input value < black[], it's black
int white[4] = {0, 0, 0, 0};            // determine color detected. white = 1, black = 0

int i;
int pv[4] = {0, 0, 0, 0}; // value read from IR sensor
boolean sw = 0;           //switch value 
int LEDtimer = 0;
int LEDcount = 0;

void show_status(int c); // show 0 1 2 3, depends on current movement
void pc( int a[]);
void black_or_white(int a[]);
void ledDisplay();
void reset(int a[]);
void Control(int a[]);
void run(int a );

int tcounter = 0; // timer counter 
int ledconter = 0; // ledcounter

// function ti countes time.
void ti(){
  tcounter++;
}

void setup(){
  Serial.begin(9600);

//set displayer pin and LED pin
  for(i = 0; i < 4; i++){
    pinMode(displayer[i], OUTPUT);
    pinMode(ledPin[i], OUTPUT);
  }

  pinMode(s, INPUT_PULLUP);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ti);

    while(sw == 0)
  {
    sw = !digitalRead(s);
  }
}

void loop(){  
  
    if(tcounter > 10)         // read value every 10 ms 
    {
      tcounter = 0;
      
      LEDtimer++;
      if(LEDtimer > 10){ // Change LED bulb every 10*10 = 100 ms
        LEDtimer = 0;
        ledDisplay();
      }

      // read IR sensor value and put into pv[4]
      for(i = 0; i < 4; i++){    
        pv[i] = analogRead(IRpin[i]);
      }
      pc(pv);  // function pc controls the movement of 2 motors based on array pv[4]
    }

    for(i = 0; i < 4; i++ ){
      Serial.print(pv[i]);
      Serial.print( " " );
    }
    for(i = 0; i < 4; i++ ){
     Serial.print(white[i]);
      Serial.print( " " );
    }

  Serial.print( "\n");
  }

// pc will detertmine current status detected: right; left; right angle
void pc(int a[]){

  black_or_white(a);
  Control(white);
}

void black_or_white(int r[]){
  int i;
  for(i = 0; i < 4; i++){
    if(r[i] < black[i]){
      white[i] = 0;
    }else{
      white[i] = 1;
    }
  }
}

void Control(int p[]){

  if(p[0] == 1 && p[3] == 1){//  White on Both Sides
    if( p[1] == 1 && p[2] == 1){ // All White
      show_status(1);  // Forword
    }else if( p[1] == 0){ //  turn left, speed: A < B 
      show_status(3);  // Left turn
    } else if( p[2] == 0){ // turn right, speed: A > B
      show_status(2);  // Right turn OK
    }else{
      show_status(4);
    }
  }else if( p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 0){ // All black
      show_status(1); // run
  }else if( p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1){ // left right angle
      show_status(3);
  }else if( p[0] == 1 && p[1] == 0 && p[2] == 0 && p[3] == 0){ // right right angle
      show_status(2);
  }else{
     show_status(2);
  }
}

void run(int a){
  show_status(1);
}

void stop(){
   show_status(0);
}

void RightAngleHandler(){
  // noting, just in case
}

// STOP = 0; Forward = 1; Turn Right = 2; Turn left = 3;
void show_status( int c ){
 int b[4] = {0, 0, 0, 0};
 int i;

 switch(c){

    case 0: // STOP
     b[0] = 0;
     b[1] = 0;
     b[2] = 0;
     b[3] = 0;
     break;

    case 1: // Forward
     b[0] = 1;
     b[1] = 0;
     b[2] = 0;
     b[3] = 0; 
     break;

    case 2: // Turn Right
     b[0] = 0;
     b[1] = 1;
     b[2] = 0;
     b[3] = 0;    
     break;

    case 3: // Turn Left
     b[0] = 1;
     b[1] = 1;
     b[2] = 0;
     b[3] = 0;
     break;

     case 4: // Except Situation
     b[0] = 0;
     b[1] = 0;
     b[2] = 1;
     b[3] = 0;
     break;
   }

  for(i = 0; i < 4; i++){
    digitalWrite(displayer[i], b[i]);
  }
 }

 // Masami thinks he like to take shower, but I don't agree with him.

 void ledDisplay(){
   int i;
   int bit[4];

   LEDcount++;
   if(LEDcount > 5){
     LEDcount = 0;
      reset(bit);
   }

  switch(LEDcount){
    case 0:
    break;

    case 1: 
    bit[0] = 1;
    break;

    case 2: 
    bit[1] = 1;
    break;
    
    case 3: 
    bit[2] = 1;
    break;
    
    case 4: 
    bit[3] = 1;
    break;
  }

  for(i = 0; i < 4; i++){
    digitalWrite( ledPin[i], !bit[i] );
  }  
 }

 void reset(int a[]){
   int i;

  a[i] = 1;
  for(i = 1; i<4 ;i++){
    a[i] = 0;
  }
 }

 // FIFO Round Robin WOFQ and someting else
