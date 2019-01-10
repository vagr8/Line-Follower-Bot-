#include<TimerOne.h>
#include<Arduino.h>

const int dir_a = 12; // pin motor A direction
const int pwn_a = 3;  // pin motor A output
const int dir_b = 13; // pin motor B direction
const int pwn_b = 11; // pin motor B output

const int s = 14; // pin for switch

const int IRpin[4] = {0, 16, 15, 2}; //pin for IR sensor

const int displayer[4] = {10, 9 , 8, 1}; // pin for 7-segment display 



int i;
int pv[4] = {0, 0, 0, 0}; // value read from IR sensor
boolean sw = 0;
int black[4] = {0, 0, 0, 0};

void status( int c[] );
void show_status(int c); // show 0 1 2 3, depends on current movement

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

   for(i = 0; i < 4; i++ ){
    Serial.print(pv[i]);
    Serial.print( " " );
  }
  Serial.println( "\n" );

    if(tcounter > 10)         // read value every 10 ms 
    {
      tcounter = 0;
      
      // read IR sensor value and put into pv[4]
      for(i = 0; i < 4; i++){    
        pv[i] = analogRead(IRpin[i]);
      }                  
    }

  }
