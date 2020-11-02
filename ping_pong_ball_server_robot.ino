#include <Servo.h>
#include <IRremote.h>

const int RECV_PIN = 4; //pin output for infrared receiver
const int BLDC_ARM1 = 5;
const int BLDC_ARM2 = 3; 
const int SERVO_VERTICAL = 6; //servo that control altitude aim
const int SERVO_HORIZONTAL = 10; //servo that control azimuth aim
const int SERVO_SPIN = 11; //servo that rotates the shooting barrel
const int SERVO_FEED = 9; //servo that feeds the ball

int pos = 90; //servo motor position value
int ran_num;
int positionHorizontal; // 60 120
int positionVertical; // 75 180
int positionFeed = 135;
int positionTilt = 95;
int toggle = 0;
int speed1 = 1000;
int speed2 = 1000;
int count = 0;
int ran_count=0 ;
int sweep_count = 0;

unsigned long last_code; //to store last IR Code
unsigned long res;

IRrecv irrecv(RECV_PIN);
decode_results results;

Servo verticalServo;
Servo horizontalServo;
Servo tiltServo;
Servo feedServo;
Servo BLDC1;
Servo BLDC2;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  verticalServo.attach(SERVO_VERTICAL);
  horizontalServo.attach(SERVO_HORIZONTAL);
  tiltServo.attach(SERVO_SPIN);
  feedServo.attach(SERVO_FEED);
  BLDC1.attach(BLDC_ARM1);
  BLDC2.attach(BLDC_ARM2);
  verticalServo.write(180);
  horizontalServo.write(pos);
  tiltServo.write(positionTilt);
  feedServo.write(positionFeed);
  Serial.println("Initializing...");
  delay(3000);
  BLDC1.writeMicroseconds(speed1);
  BLDC2.writeMicroseconds(speed2);
  delay(5000);
  randomSeed(analogRead(0));
  Serial.println("Initialized!");
  }


void loop(){
if (irrecv.decode(&results)){
  res = results.value;
  if (res == 0XFFFFFFFF){ // makes sure when a button is long pressed it would continue to execute the program of last button pressed
      res = last_code;
  }

  switch(res){
     case 0xFD00FF: //arming
        Serial.println("Arming..."); // on/off
        last_code = res;
        
        break;
     case 0xFD30CF: //disarming
        Serial.println("Disarming..."); // on/off
        last_code = res;
        while(speed1 > 1000){
            delay(50);
            speed1 = speed1 - 10;
            BLDC1.writeMicroseconds(speed1);  
          }

        while(speed2 > 1000){
            delay(50);
            speed2 = speed2 - 10;
            BLDC2.writeMicroseconds(speed2);  
          }
        break;
     case 0xFD08F7:
        Serial.println("Top spin"); //1
        last_code = res;
        speed1 = 1300;
        BLDC1.writeMicroseconds(speed1);
        while(speed2 > 1000){
          speed2 = speed2 - 10;
          BLDC2.writeMicroseconds(speed2);  
        }
        while(positionTilt != 104){
          if(positionTilt > 104){
            delay(50);
            positionTilt = positionTilt - 1;
            tiltServo.write(positionTilt);  
          }
          else if(positionTilt < 104){
            delay(50);
            positionTilt = positionTilt + 1;
            tiltServo.write(positionTilt);  
          }  
        }
        break;
     case 0xFD8877:
        Serial.println("Under spin"); //2
        last_code = res;
        speed2 = 1200;
        BLDC2.writeMicroseconds(speed2);
        while(speed1 > 1000){
          speed1 = speed1 - 10;
          BLDC1.writeMicroseconds(speed1);  
        }

        while(positionTilt != 104){
          if(positionTilt > 104){
            delay(50);
            positionTilt = positionTilt - 1;
            tiltServo.write(positionTilt);  
          }
          else if(positionTilt < 104){
            delay(50);
            positionTilt = positionTilt + 1;
            tiltServo.write(positionTilt);  
          }  
        }
        break;
     case 0xFD48B7:
        Serial.println("Right spin"); //3
        last_code = res;


        while(positionTilt != 145){
          if(positionTilt > 145){
            delay(50);
            positionTilt = positionTilt - 1;
            tiltServo.write(positionTilt);
              
          }
          else if(positionTilt < 145){
            delay(50);
            positionTilt = positionTilt + 1;
            tiltServo.write(positionTilt);  
          }  
        }
        speed1 = 1200;
        BLDC1.writeMicroseconds(speed1);
        while(speed2 > 1000){
          speed2 = speed2 - 10; 
          BLDC2.writeMicroseconds(speed2); 
        }
        break;
     case 0xFD28D7:
        Serial.println("Left spin"); //4
        last_code = res;

        while(positionTilt != 145){
          if(positionTilt > 145){
            delay(50);
            positionTilt = positionTilt - 1;  
            tiltServo.write(positionTilt);
          }
          else if(positionTilt < 145){
            delay(50);
            positionTilt = positionTilt + 1;
            tiltServo.write(positionTilt);  
          }  
        }
        speed2 = 1200;
        BLDC2.writeMicroseconds(speed2);
        while(speed1 > 1000){
          speed1 = speed1 - 10;
          BLDC1.writeMicroseconds(1000);  
        }
        break ;
     case 0xFDA857:
        Serial.println("Random"); // 5
        last_code = results.value;
        speed1 = 1300;
        BLDC1.writeMicroseconds(speed1);
        while(speed2 > 1000){
          speed2 = speed2 - 10;
          BLDC2.writeMicroseconds(speed2);  
        }
        
        while(ran_count < 12 ){
          ran_num = random(70, 110);
          Serial.println(ran_num);
          while(positionHorizontal <= 110 && ran_num != positionHorizontal){
            positionHorizontal = positionHorizontal + 1;
            delay(25);
            Serial.println(positionHorizontal);
            horizontalServo.write(positionHorizontal);  
          }
          while(positionHorizontal >= 70 && ran_num != positionHorizontal){
            positionHorizontal = positionHorizontal - 1;
            delay(25);
            Serial.println(positionHorizontal);
            horizontalServo.write(positionHorizontal);
          }
          while(positionFeed != 35){
            delay(15);
            positionFeed = positionFeed - 1;
            feedServo.write(positionFeed);
          }
          while(positionFeed != 145){
            delay(15);
            positionFeed = positionFeed + 10;
            feedServo.write(positionFeed);     
          }     
          ran_count = ran_count + 1;
        }
        ran_count = 0;
        while(speed1 > 1000){
            delay(50);
            speed1 = speed1 - 10;
            BLDC1.writeMicroseconds(speed1);  
          }

        while(speed2 > 1000){
            delay(50);
            speed2 = speed2 - 10;
            BLDC2.writeMicroseconds(speed2);  
          }
        break;              
     case 0xFD609F:
        Serial.println("Right"); // Next
        last_code = results.value;
        positionHorizontal = positionHorizontal + 5;
        if(positionHorizontal > 110){
          positionHorizontal = 110;
          }
        if(positionHorizontal < 60){
          positionHorizontal = 60;
          }
        horizontalServo.write(positionHorizontal);
        break ;  
     case 0xFD20DF:
        Serial.println("Left"); // Prev
        last_code = results.value;
        positionHorizontal = positionHorizontal - 5;
        if(positionHorizontal > 110){
          positionHorizontal = 110;
          }
        if(positionHorizontal < 60){
          positionHorizontal = 60;
          }
        horizontalServo.write(positionHorizontal);
        break ;
     case 0xFD10EF:
        Serial.println("Down"); // Down
        last_code = results.value;
        positionVertical = positionVertical + 5;
        if(positionVertical > 180){
          positionVertical = 180;
          }
        if(positionVertical < 70){
          positionVertical = 70;
          }
        Serial.println(positionVertical);
        verticalServo.write(positionVertical);
        break ;  
     case 0xFD50AF: 
        Serial.println("Up"); // Up
        last_code = results.value;
        positionVertical = positionVertical - 5;
        if(positionVertical > 180){
          positionVertical = 180;
          }
        if(positionVertical < 70){
          positionVertical = 70;
          }
        Serial.println(positionVertical);
        verticalServo.write(positionVertical);
        break;   
     case 0xFDA05F: 
        Serial.println("Firing sequence"); // Play
        last_code = results.value;
        while(count < 12){
          while(positionFeed != 35){
            positionFeed = positionFeed - 1;
            delay(15);
            feedServo.write(positionFeed);
          }
          while(positionFeed != 145){
            positionFeed = positionFeed + 10;
            delay(15);
            feedServo.write(positionFeed);     
          }
          count = count +1;
          Serial.println(count);
        }  
          count = 0;
          while(speed1 > 1000){
            delay(50);
            speed1 = speed1 - 10;
            BLDC1.writeMicroseconds(speed1);  
          }

          while(speed2 > 1000){
            delay(50);
            speed1 = speed1 - 10;
            BLDC2.writeMicroseconds(speed2);  
          }

          BLDC2.writeMicroseconds(1000);
          break;

     case 0xFD807F: 
        Serial.println("Speed1 +"); // Vol+
        last_code = results.value;
        speed1 = speed1 + 10;
        BLDC1.writeMicroseconds(speed1);
        break;
     case 0xFD906F: 
        Serial.println("Speed1 -"); // Vol- 
        last_code = results.value;
        speed1 = speed1 - 10;
        BLDC1.writeMicroseconds(speed1);
        break;
     case 0xFD6897: 
        Serial.println("Speed2 +"); // 6
        last_code = results.value;
        speed2 = speed2 + 10;
        BLDC2.writeMicroseconds(speed2);
        break;
     case 0xFD58A7: 
        Serial.println("Speed2 -"); // 9
        last_code = results.value;
        speed2 = speed2 - 10;
        BLDC2.writeMicroseconds(speed2);
        break;        
     default:
        break;  
   
  }
  
  delay(30);
  irrecv.resume();
  
  }
}
