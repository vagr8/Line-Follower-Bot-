#include<TimerOne.h>
#include<Arduino.h>
const int dir_a = 12; // pin motor A direction
const int pwn_a = 3;  // pin motor A output
const int dir_b = 13; // pin motor B direction
const int pwn_b = 11; // pin motor B output

const int s = 14; // pin for switch

const int IRpin[4] = {4, 16, 15, 5}; //pin for IR sensor

const int displayer[4] = {10, 9 , 8, 1}; // pin for 7-segment display 

int black[4] = {400, 500, 400, 580};    // thershold for black. If input value < black[], it's black
int white[4] = {0, 0, 0, 0};            // determine color detected. white = 1, black = 0

int i;
int pv[4] = {0, 0, 0, 0}; // value read from IR sensor
boolean sw = 0;           //switch value 

void show_status(int c); // show 0 1 2 3, depends on current movement
void pc( int a[]);
void Control(int a[]);
void run();
void stop();
void black_or_white(int a[]);
void RightAngleHandler( );

int tcounter = 0; // timer counter 

// function ti countes time.
void ti(){
  tcounter++;
}

void setup(){
  Serial.begin(9600);
  pinMode(dir_a, OUTPUT);
  pinMode(pwn_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  pinMode(pwn_b, OUTPUT);

  pinMode(displayer[0], OUTPUT);
  pinMode(displayer[1], OUTPUT);
  pinMode(displayer[2], OUTPUT);
  pinMode(displayer[3], OUTPUT);

  pinMode(s, INPUT_PULLUP);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(ti);

  digitalWrite(dir_a, 1);
  analogWrite(pwn_a, 0);
  digitalWrite(dir_b, 1);
  analogWrite(pwn_b, 0);

    while(sw == 0)
  {
    sw = !digitalRead(s);
  }

}

void loop(){  
  
    if(tcounter > 10)         // read value every 10 ms 
    {
      tcounter = 0;
      
      // read IR sensor value and put into pv[4]
      for(i = 0; i < 4; i++){    
        pv[i] = analogRead(IRpin[i]);
      }
      pc(pv);                       // function pc controls the movement of 2 motors based on array pv[4]
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
  int c1;

  black_or_white(a);
  Control(white);

}

void black_or_white(int r[]){
  for(i = 0; i < 4; i++){
    if(r[i] < black[i]){
      white[i] = 0;
    }else{
      white[i] = 1;
    }
  }
}

void pid_controll(int p[]){
  int left = p[1];
  int right = p[2];

  if(p[0] == 1 && p[1] == 1 && p[2] == 1 && p[3] == 1){
    stop();
  }else if( p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 0){
    run();
  }else if( p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1){ // left right angle
      digitalWrite(dir_a, 0);
      analogWrite(pwn_a, 120);
      digitalWrite(dir_b, 1);
      analogWrite(pwn_b, 80);
  }else if( p[0] == 1 && p[1] == 0 && p[2] == 0 && p[3] == 0){ // right right angle
      digitalWrite(dir_a, 1);
      analogWrite(pwn_a, 80);
      digitalWrite(dir_b, 0);
      analogWrite(pwn_b, 120);
  }else if( p[0] == 1 && p[3] == 1){
    if( left == 1 ){ // Turn Right, A front B back 
      digitalWrite(dir_a, 1);
      analogWrite(pwn_a, 80);
      digitalWrite(dir_b, 0);
      analogWrite(pwn_b, 120);
      show_status(2);
    }else if( right == 1 ){ // Turn Left,  B front A　back   
      digitalWrite(dir_a, 0);
      analogWrite(pwn_a, 120); 
      digitalWrite(dir_b, 1);
      analogWrite(pwn_b, 80);
      show_status(3);
    }else{
      delay(1000);
      run();
    }  
  }else{
    stop();
  }
}

void run(){
  show_status(1);
  digitalWrite(dir_a, 1);
  analogWrite(pwn_a, 80);
  digitalWrite(dir_b, 1);
  analogWrite(pwn_b, 80);
}

void stop(){
   show_status(0);
   analogWrite(pwn_a, 255);
   analogWrite(pwn_b, 255);
}

void RightAngleHandler(){


}

// STOP = 0; Forward = 1; Turn Right = 2; Turn left = 3;
void show_status( int c ){
 int b[4] = {0, 0, 0, 0};

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
   }

  for(i = 0; i < 4; i++){
    digitalWrite(displayer[i], b[i]);
  }

 }
