#include<TimerOne.h>
#include<Arduino.h>
const int dir_a = 12; // pin motor A direction
const int pwn_a = 3;  // pin motor A output
const int dir_b = 13; // pin motor b direction
const int pwn_b = 11; // pin motor B output

const int s = 14; // pin for switch

const int IRpin[4] = {6, 2, 1, 7}; //pin for IR sensor

const int displayer[4] = {10, 9 , 8, 3}; // pin for 7-segment display 

int i;
int pv[4]; // value read from IR sensor
boolean sw = 0;

// PID coefficients
int kp = 200, ki = 0, kd = 1;
int d[2] = {0, 0};

int err[2] = {0, 0};     // err between current state and goal
int derr[2] = {0, 0};    // err difference betweeen current err and previous err 
int integral[2] = {0, 0};   // accumlation 
int origin[2] = {150, 150};     // target value for IR sensor
int errz[2]  = {0, 0};      // previous err

int u[2] = {0, 0};
int m[2] = {0, 0};

int d0, d1, u0, u1;
int m0, m1;

void show_status(int c); // show 0 1 2 3, depends on current movement
void pc( int a[]);
void pid_controll(int a[]);

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
    origin[0] = analogRead(IRpin[1]);
    origin[1] = analogRead(IRpin[2]);
   
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

  for(i = 0; i < 3; i++ ){
    Serial.print(pv[i]);
    Serial.print( " " );
  }
  Serial.println( pv[3] );
  
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

// pc will detertmine current status detected: right; left; right angle
void pc(int a[]){
int c1;

    if( a[0] > origin[0] && a[3] > origin[3] ){// if a0 = white, a4 = white; use PID control curve and strait line
      c1 = 0;
      pid_controll(a);
    }else if( a[0] < origin[0] && a[1] < origin[1] && a[2] < origin[2] && a[3] > origin[3]){// if a0 = a1 = a2 = black; a3 = white: it is a right angle to the left
      c1 = 1;
    }else if( a[0] > origin[0] && a[1] < origin[1] && a[2] < origin[2] && a[3] < origin[3]){// if a0 = white; a1 = black = a2 = a3 = black: it is a right angle to the right
      c1 = 2;
    }else{// if a0 = a1 = a2 = a3 = black: it is a cross
      c1 = 3;
    }
  show_status(c1);
}

// e = err; de = derr; 
void pid_controll(int a[]){
  int left = a[1];
  int right = a[2];

  int PID[2] = {left, right};

  for(i = 0; i < 2; i++){    
        err[i] = PID[i] - origin[i];
      }

  for(i = 0; i < 2; i++){    
        integral[i] = integral[i] + err[i];
      }

  for(i = 0; i < 2; i++){    
       derr[i] = err[i] - origin[i];
      }
  
  for(i = 0; i < 2; i++){    
       derr[i] = err[i] - errz[i];
      }

  for(i = 0; i < 2; i++){    
       u[i] = kp*err[i] + ki*integral[i] + kd*derr[i];
      }

  for(i = 0; i < 2; i++){    
       d[i] = abs(u[i]/50);
      }
 
  // d[0] for the left IR
  d0 = d[0];
  d1 = d[1];

  u0 = u[0];
  u1 = u[1];

// d1
  if(d0 > 100){
    d0 = 100;
  }

  m0 = 255-d0*255/100;

  if(u0 < 0)
  {
    digitalWrite(dir_a, 1);
    analogWrite(pwn_a,m0);
  }
  else if(u0 == 0 || u0 > 0)
  {
    m0 = 0;
    digitalWrite(dir_a, 1);
    analogWrite(pwn_a,m0);
  }
// d1

  if(d1 > 100){
    d1 = 100;
  }

  m1 = 255-d1*255/100;

  if(u1 < 0)
  {
    digitalWrite(dir_b, 1);
    analogWrite(pwn_b, m1);
  }
  else if(u0 == 0 || u0 > 0)
  {
    m1 = 0;
    digitalWrite(dir_b, 1);
    analogWrite(pwn_b, m1);
  }

  for(i = 0; i < 2; i++){
    errz[i] = err[i];
  }

}


