#include<TimerOne.h>

const int dir_a = 12; // pin motor A direction
const int pwn_a = 3;  // pin motor A output
const int dir_b = 13; // pin motor b direction
const int pwn_b = 11; // pin motor B output

const int s = 14; // pin for switch

const int IRpin[4] = {1, 2, 3, 4}; //pin for IR sensor

const int d[4] = {10, 9 , 8, 3}; // pin for 7-segment display 

int i;
int pv[4]; // value read from IR sensor
boolean sw = 0;

void show_status(int c); // show 0 1 2 3, depends on current movement
void pc( int a[]);

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

  pinMode(d[0], OUTPUT);
  pinMode(d[1], OUTPUT);
  pinMode(d[2], OUTPUT);
  pinMode(d[3], OUTPUT);

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
    for(i = 0; i < 4; i++){    
        pv[i] = analogRead(IRpin[i]);
      }
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
    Serial.println(pv[i]);
  }

  Serial.println(" ");
  
  }

void show_status( int c ){
 int b[4] = {0, 0, 0, 0};

 switch(c){

    case 0:
     b[0] = 0;
     b[1] = 0;
     b[2] = 0;
     b[3] = 0;
     break;

    case 1:
     b[0] = 1;
     b[1] = 0;
     b[2] = 0;
     b[3] = 0; 
     break;

    case 2:
     b[0] = 0;
     b[1] = 1;
     b[2] = 0;
     b[3] = 0;    
     break;

    case 3:
     b[0] = 1;
     b[1] = 1;
     b[2] = 0;
     b[3] = 0;
     break;
   }

  for(i = 0; i < 4; i++){
    digitalWrite(d[i], b[i]);
  }
 }

// pc controls motor speed based on array input.
void pc(int a[]){


}
