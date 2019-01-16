#include<TimerOne.h> 
#include<Arduino.h> 

const int dir_a = 13; // pin motor A direction; assume 1= forward; 0 = backward 
const int pwn_a = 11; // pin motor A output ; 0 = MaxSpeed, 255 = STOP 
const int dir_b = 12; // pin motor B direction 
const int pwn_b = 3; // pin motor B output 

// const int ledPin[4] = {4, 5, 6, 7}; // LED bulb pin = 4 5 6 7; digital write 

const int s = 14; // pin for switch 

const int IRpin[4] = {4, 16, 15, 5}; //pin for IR sensor; analog write 

const int displayer[4] = {1, 8, 9, 10}; // pin for 7-segment display 

const int d[4][4] = {
  {0,0,0,0},
  {1,0,0,0},
  {0,1,0,0},
  {1,1,0,0},
};

int black[4] = {250, 300, 350, 450}; // thershold for black. If input value < black[], it's black 
int white[4] = {0, 0, 0, 0}; // determine color detected. white = 1, black = 0 
int display = 0; 

int i; 
int sp = 0;    // speed control for straight line
int pv[4] = {0, 0, 0, 0}; // value read from IR sensor 
boolean sw = 0; //switch value 
int LEDtimer = 0; 
int curve[2] = {0, 0};   // gives the sensoring curvity
// int LEDcount = 0; 

void show_status(int c); // show 0 1 2 3, depends on current movement 
void pc( int a[]);       // transfer signal to black/ white and pass to control funciton
void Control(int a[]);   // conntrol the movement
void run(int a);         // run with speed 'a'
void stop();             
void black_or_white(int a[]); // input IR signal to black = 0/ white = 1
void ledDisplay(); 
void reset(int a[]);     // reset bulb value
void StopandRun(int a, int b, int m); // stop for a sec and run for b sec in speed m

int tcounter = 0; // timer counter 
int ledconter = 0; // ledcounter 

// function ti countes time. 
void ti(){ 
    tcounter++; 
} 

void setup(){ 
    Serial.begin(9600); 

    // motor pin 
    pinMode(dir_a, OUTPUT); 
    pinMode(pwn_a, OUTPUT); 
    pinMode(dir_b, OUTPUT); 
    pinMode(pwn_b, OUTPUT); 

    //set displayer pin and LED pin 
    for(i = 0; i < 4; i++){ 
        pinMode(displayer[i], OUTPUT); 
    } 

    pinMode(s, INPUT_PULLUP); 

    Timer1.initialize(1000); 
    Timer1.attachInterrupt(ti); 

    show_status(0); 
    stop(); 
    while(sw == 0){ 
        sw = !digitalRead(s); 
    } 
} 

void loop(){ 

    if(tcounter > 10){ // read value every 10 ms 
 
        tcounter = 0; 

/* 
LEDtimer++; 
if(LEDtimer > 20){ // Change Display every 10*20 = 200 ms 
LEDtimer = 0; 
ledDisplay(); 
}*/ 

// read IR sensor value and put into pv[4] 
        for(i = 0; i < 4; i++){ 
            pv[i] = analogRead(IRpin[i]); 
        }        
        pc(pv); // function pc controls the movement of 2 motors based on array pv[4] 
    } 

for(i = 0; i < 4; i++ ){ 
Serial.print(pv[i]); 
Serial.print( " " ); 
} 

for(i = 0; i < 4; i++ ){ 
Serial.print(white[i]); 
Serial.print( " " ); 
} 

Serial.print( " "); 
Serial.print( display ); 

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
int c;
      /*if(p[0] == 1 && p[1] == 1 && p[2] == 1 && p[3] == 1) c = 1; // all white 
      if(p[0] == 1 && p[1] == 0 && p[2] == 0 && p[3] == 1) c = 2; // left turn
      if(p[0] == 1 && p[1] == 1 && p[2] == 0 && p[4] == 1) c = 3; // right turn */
      if(p[0] == 1 && p[3] == 1){
        if(p[1] == 1 && p[2] == 1){
            c = 1;
        }else if( p[1] == 0 && p[2] == 1){
            c = 2;
        }else if( p[1] == 1 && p[2] == 0){
            c = 3;
        }else{
          stop();
        }

      }

      if(p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 0) c = 4; // all black
      if(p[0] == 0 && p[1] == 0 &&              p[3] == 1) c = 5; // left right angle
      if(p[0] == 1 &&              p[2] == 0 && p[3] == 0) c = 6; // right right angle
      if(p[0] == 0 && p[1] == 1 && p[2] == 1 && p[3] == 1) c = 7; // big left turn
      if(p[0] == 1 && p[1] == 1 && p[2] == 1 && p[3] == 0) c = 8; // big right turn
      

      switch(c){
      case 1: //forward
        run(160);
        show_status(1);
      break;

      case 2: // left turn
            show_status(2);

            digitalWrite(dir_a, 1); 
            analogWrite(pwn_a, 160); 
            digitalWrite(dir_b, 1); 
            analogWrite(pwn_b, 200); 

             curve[0] = 0;
             curve[1] ++;     // enconter continuing curve
            if(curve[1] > 3){  // truns faster if it is a continue curve
                digitalWrite(dir_a, 1); 
                analogWrite(pwn_a, 160); 
                digitalWrite(dir_b, 1); 
                analogWrite(pwn_b, 230); 
            }

                delay(0); 
                sp = 0;
      break;

      case 3: // right turn
          show_status(3); 
          digitalWrite(dir_a, 1); 
          analogWrite(pwn_a, 200); 
          digitalWrite(dir_b, 1); 
          analogWrite(pwn_b, 160); 

            curve[1] = 0;   // reset right curvity 
            curve[0] ++;    // add curvity to left
            if(curve[1] > 3){
               digitalWrite(dir_a, 1); 
               analogWrite(pwn_a, 230); 
               digitalWrite(dir_b, 1); 
               analogWrite(pwn_b, 160); 
              }

            delay(0); 
            sp = 0;
            
      break;

      case 4: // all black
        sp = 0;
        StopandRun( 1000, 800, 160); 
      break;

      case 5: // left right angle
        run(200);      // go fronter to abtain more information
        delay(50);

        for(i = 0; i < 4; i++){ // get latest IR value
            pv[i] = analogRead(IRpin[i]); 
            black_or_white(pv); 
        }

        if(pv[3] == 0){  // find it is cross
            StopandRun(1000, 800, 180);
        }else{           // else it is a left right angle
            show_status(3); 
            digitalWrite(dir_a, 0); 
            analogWrite(pwn_a, 0); 
            digitalWrite(dir_b, 1); 
            analogWrite(pwn_b, 0); 
            delay(60); 
        }
    
        sp = 0;

      break;

      case 6:  // right right angle
      run(200);
        delay(50);

        for(i = 0; i < 4; i++){ 
            pv[i] = analogRead(IRpin[i]); 
            black_or_white(pv); 
        }

        if(pv[0] = 0){
            StopandRun(1000, 800, 180);
        }else{
            show_status(2); 
            digitalWrite(dir_a, 1); 
            analogWrite(pwn_a, 0); 
            digitalWrite(dir_b, 0); 
            analogWrite(pwn_b, 0); 
            delay(40); 
            sp = 0;

        }

      break;

      case 7:  //left turn big
          show_status(3);        
          digitalWrite(dir_a, 1); 
          analogWrite(pwn_a, 240); 
          digitalWrite(dir_b, 1); 
          analogWrite(pwn_b, 160); 
          delay(0);
          sp = 0;
      break;

      case 8: // right turn big
        show_status(2);        
        digitalWrite(dir_a, 1); 
        analogWrite(pwn_a, 160); 
        digitalWrite(dir_b, 1); 
        analogWrite(pwn_b, 240); 
        delay(0);

         sp = 0;
      break;
    } 
} 

void run(int a) { 
    a = a + 10;

    digitalWrite(dir_a, 1); 
    analogWrite(pwn_a, a-sp); 
    digitalWrite(dir_b, 1); 
    analogWrite(pwn_b, a-sp); 

    if( sp > 10){
        sp = sp;
    }else{
        sp--;
    }
} 

void stop(){ 
  analogWrite(pwn_a, 255); 
  analogWrite(pwn_b, 255);
} 

// stop for t1 sec and run in speed for t22 sec
void StopandRun(int t1, int t2, int speed){ 
    show_status(0);
    stop(); 
    delay(t1); 
    show_status(1);
    run(speed); 
    delay(t2); 
}               

// STOP = 0; Forward = 1; Turn Right = 2; Turn left = 3; 
void show_status( int c ){ 
    
    display = c; 
 
    for(i = 0; i < 4; i++){ 
        digitalWrite(displayer[i], d[c][i]); 
    }  
    delay(100) ;
} 
