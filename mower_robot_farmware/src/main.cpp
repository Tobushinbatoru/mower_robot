#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>

#define LEFT_FRONT_1 2
#define LEFT_FRONT_2 3
#define RIGHT_FRONT_1 5
#define RIGHT_FRONT_2 4
#define LEFT_BACK_1 7
#define LEFT_BACK_2 6
#define RIGHT_BACK_1 9
#define RIGHT_BACK_2 8
#define BARIKAN     10

float TORED = 0.5;
float left_pwm_speed;
float right_pwm_speed;

ros::NodeHandle nh;

// 1 = 31.37kHz 2 = 3.921kHz 3 = 490.2kHz 4 = 122.5Hz 5 = 30.64Hz 他は禁止
void change_freq1(int number){
	TCCR2B = ((TCCR2B & 0B11111000) | number);
	TCCR3B = ((TCCR3B & 0B11111000) | number);
	TCCR4B = ((TCCR4B & 0B11111000) | number);
}

void LEFT_FRONT(int PWM)
{
	if(PWM > 0){
		digitalWrite(LEFT_FRONT_1, 0);
		analogWrite(LEFT_FRONT_2, PWM);
	}else if(PWM < 0){
		analogWrite(LEFT_FRONT_1, -PWM);
		digitalWrite(LEFT_FRONT_2, 0);
	}
	else{
		digitalWrite(LEFT_FRONT_1, 0);
		digitalWrite(LEFT_FRONT_2, 0);
	}
}

void RIGHT_FRONT(int PWM)
{
	if(PWM > 0){
		digitalWrite(RIGHT_FRONT_1, 0);
		analogWrite(RIGHT_FRONT_2, PWM);
	}else if(PWM < 0){
		analogWrite(RIGHT_FRONT_1, -PWM);
		digitalWrite(RIGHT_FRONT_2, 0);
	}
	else{
		digitalWrite(RIGHT_FRONT_1, 0);
		digitalWrite(RIGHT_FRONT_2, 0);
	}
}

void LEFT_BACK(int PWM)
{
	if(PWM > 0){
		digitalWrite(LEFT_BACK_1, 0);
		analogWrite(LEFT_BACK_2, PWM);
	}else if(PWM < 0){
		analogWrite(LEFT_BACK_1, -PWM);
		digitalWrite(LEFT_BACK_2, 0);
	}
	else{
		digitalWrite(LEFT_BACK_1, 0);
		digitalWrite(LEFT_BACK_2, 0);
	}
}

void RIGHT_BACK(int PWM)
{
	if(PWM > 0){
		digitalWrite(RIGHT_BACK_1, 0);
		analogWrite(RIGHT_BACK_2, PWM);
	}else if(PWM < 0){
		analogWrite(RIGHT_BACK_1, -PWM);
		digitalWrite(RIGHT_BACK_2, 0);
	}
	else{
		digitalWrite(RIGHT_BACK_1, 0);
		digitalWrite(RIGHT_BACK_2, 0);
	}
}

void messageCb(const geometry_msgs::Twist& twist) 
{
	const float linear_x = twist.linear.x;
  	const float angle_z = twist.angular.z;
  	
  	left_pwm_speed = (linear_x - 0.5 * TORED * angle_z) * 255;
  	right_pwm_speed = (linear_x + 0.5 * TORED * angle_z) * 255;
  	
  	LEFT_FRONT(left_pwm_speed);
	RIGHT_FRONT(right_pwm_speed);
	LEFT_BACK(left_pwm_speed);
	RIGHT_BACK(right_pwm_speed);
  	// cmd_vel受信後のコード
  	//int x = int( twist.linear.x * 255);
  	//char buf[100];
  	//sprintf(buf, "x = %d", x);
  	//nh.loginfo(buf);
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb);

void barikan_cb(const std_msgs::Bool& msg){
	if(msg.data) 	analogWrite(BARIKAN, 255);
  	else 			analogWrite(BARIKAN, 0);
}
ros::Subscriber<std_msgs::Bool> sub0("barikan", &barikan_cb);

void setup()
{
	pinMode(LEFT_FRONT_1, OUTPUT);
	pinMode(LEFT_FRONT_2, OUTPUT);
	pinMode(RIGHT_FRONT_1, OUTPUT);
	pinMode(RIGHT_FRONT_2, OUTPUT);
	pinMode(LEFT_BACK_1, OUTPUT);
	pinMode(LEFT_BACK_2, OUTPUT);
	pinMode(RIGHT_BACK_1, OUTPUT);
	pinMode(RIGHT_BACK_2, OUTPUT);
	
	change_freq1(1);
	
	nh.getHardware()->setBaud(115200);
  	nh.initNode();
  	nh.subscribe(sub);
  	nh.subscribe(sub0);
}

void loop()
{
	//LEFT_FRONT(10);
	//RIGHT_FRONT(10);
	//LEFT_BACK(10);
	//RIGHT_BACK(10);
	nh.spinOnce();
	delay(1);

	/*
	for(int i=0; i<255; i++){
		LEFT_FRONT(i);
		delay(500);
	}
	
	for(int i=255; i>0; i--){
		LEFT_FRONT(i);
		delay(10);
	}
	for(int i=0; i<-255; i--){
		LEFT_FRONT(i);
		delay(10);
	}
	for(int i=-255; i>0; i++){
		LEFT_FRONT(i);
		delay(10);
	}
	*/
}
