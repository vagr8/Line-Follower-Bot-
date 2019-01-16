#include<TimerOne.h>
#include<Arduino.h>

const int dir_a = 13; // pin motor A direction; assume 1= forward; 0 = backward
const int pwn_a = 11;  // pin motor A output ;   0 = MaxSpeed, 255 = STOP
const int dir_b = 12; // pin motor B direction
const int pwn_b = 3; // pin motor B output

const int s = 14; // pin for switch

const int IRpin[4] = {4, 16, 15, 5}; //pin for IR sensor

const int displayer[4] = {10, 9 , 8, 1}; // pin for 7-segment display 



int i;
int pv[4] = {0, 0, 0, 0}; // value read from IR sensor
boolean sw = 0;

int black[4] = {250, 300, 400, 350};    // thershold for black. If input value < black[], it's black
int white[4] = {0, 0, 0, 0};            // determine color detected. white = 1, black = 0

void status( int c[] );
void show_status(int c); // show 0 1 2 3, depends on current movement
void pc(int a[]);
void black_or_white(int a[]);

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
  analogWrite(pwn_a, 255);
  digitalWrite(dir_b, 1);
  analogWrite(pwn_b, 255);

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
        pc(pv);
      }                  
    }

  
    for(i = 0; i < 4; i++ ){
      Serial.print(pv[i]);
      Serial.print( " " );
    }
    for(i = 0; i < 4; i++ ){
     Serial.print(white[i]);
      Serial.print( " " );
    }

    Serial.print("\n");

  }

  void pc(int a[]){
    black_or_white(a);
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
