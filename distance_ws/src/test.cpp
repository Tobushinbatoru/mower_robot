#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

#define SAMPLE_SIZE 3 //20, 50, 100

ros::NodeHandle  nh;
std_msgs::Float32MultiArray distance_msg;
ros::Publisher pub_vec("distance", &distance_msg);

int count = 0;
int discardcount = 0;

uint16_t lf_temp[SAMPLE_SIZE];
uint16_t lf_sum = 0;
uint16_t lf_average = 0;

uint16_t rf_temp[SAMPLE_SIZE];
uint16_t rf_sum = 0;
uint16_t rf_average = 0;

uint16_t lr_temp[SAMPLE_SIZE];
uint16_t lr_sum = 0;
uint16_t lr_average = 0;

uint16_t rr_temp[SAMPLE_SIZE];
uint16_t rr_sum = 0;
uint16_t rr_average = 0;

VL53L0X sensor[4];


void setup()
{
	//Serial.begin(9600);
  	Wire.begin();
  	
	//VL53:初期化,I2Cアドレス変更　開始
	pinMode(9,OUTPUT);
	digitalWrite(9,LOW);
	
	pinMode(10,OUTPUT);
	digitalWrite(10,LOW);
	
	pinMode(11,OUTPUT);
	digitalWrite(11,LOW);
	
	pinMode(12,OUTPUT);
	digitalWrite(12,LOW);

	pinMode(9,INPUT);
	sensor[0].setTimeout(250);
	if (!sensor[0].init())
	{
    		//Serial.println("sensor[0] Failed to detect and initialize sensor!");
    		while (1) {}
  	}
	sensor[0].setAddress(0X20);
	sensor[0].startContinuous(0);  // 連続計測モード
	Serial.print(" I2C");
	Serial.print(sensor[0].getAddress(),HEX);

	pinMode(10,INPUT);
	sensor[1].setTimeout(250);
	if (!sensor[1].init())
  	{
    		//Serial.println("sensor[1] Failed to detect and initialize sensor!");
    		while (1) {}
  	}
	sensor[1].setAddress(0X21);
	sensor[1].startContinuous(0);  // 連続計測モード
	//Serial.print(" I2C ");
	//Serial.print(sensor[1].getAddress(),HEX);

	pinMode(11,INPUT);
	sensor[2].setTimeout(250);
	if (!sensor[2].init())
  	{
    		//Serial.println("sensor[2] Failed to detect and initialize sensor!");
    		while (1) {}
  	}
	sensor[2].setAddress(0X22);
	sensor[2].startContinuous(0);  // 連続計測モード
	//Serial.print(" I2C");
	//Serial.print(sensor[2].getAddress(),HEX);
	
	pinMode(12,INPUT);
	sensor[3].setTimeout(250);
	if (!sensor[3].init())
  	{
    		//Serial.println("sensor[3] Failed to detect and initialize sensor!");
    		while (1) {}
  	}
	sensor[3].setAddress(0X23);
	sensor[3].startContinuous(0);  // 連続計測モード
	//Serial.print(" I2C");
	//Serial.print(sensor[3].getAddress(),HEX);
	//VL53:初期化,I2Cアドレス変更 終了
	
	nh.initNode();
    nh.advertise(pub_vec);
    distance_msg.data = (float*)malloc(sizeof(float) * 4);
    distance_msg.data_length = 4;
    nh.getHardware()->setBaud(115200);
}

void loop() {

  lf_temp[count] = sensor[0].readRangeContinuousMillimeters();
  rf_temp[count] = sensor[1].readRangeContinuousMillimeters();
  lr_temp[count] = sensor[2].readRangeContinuousMillimeters();
  rr_temp[count] = sensor[3].readRangeContinuousMillimeters();
  
  if (sensor[0].timeoutOccurred()) {Serial.print(" TIMEOUT");}
  if (sensor[1].timeoutOccurred()) {Serial.print(" TIMEOUT");}
  if (sensor[2].timeoutOccurred()) {Serial.print(" TIMEOUT");}
  if (sensor[3].timeoutOccurred()) {Serial.print(" TIMEOUT");}

  count++;
  
  if (count == SAMPLE_SIZE) {
    count = 0;
  }

  if (discardcount < SAMPLE_SIZE) {
    discardcount++;
    return;
  }

  for (int i = 0; i < SAMPLE_SIZE; i++) {
    lf_sum += lf_temp[i];
    rf_sum += rf_temp[i];
    lr_sum += lr_temp[i];
    rr_sum += rr_temp[i];    
  }

  lf_average = lf_sum / SAMPLE_SIZE;
  lf_sum = 0;
  
  rf_average = rf_sum / SAMPLE_SIZE;
  rf_sum = 0;
  
  lr_average = lr_sum / SAMPLE_SIZE;
  lr_sum = 0;
  
  rr_average = rr_sum / SAMPLE_SIZE;
  rr_sum = 0;
  
  distance_msg.data[0] = lf_average - 10.0;
  distance_msg.data[1] = rf_average - 15.0;
  distance_msg.data[2] = lr_average - 35.0;
  distance_msg.data[3] = rr_average + 20.0;

  //Serial.print(lf_average);
  //Serial.println();

  //Serial.print(sensor[0].readRangeContinuousMillimeters());
  //Serial.print(",");
  //Serial.print(sensor[1].readRangeContinuousMillimeters());
  //Serial.print(",");
  //Serial.print(sensor[2].readRangeContinuousMillimeters());
  //Serial.print(",");
  //Serial.println(sensor[3].readRangeContinuousMillimeters());
  
  //distance_msg.data[0] = sensor[0].readRangeContinuousMillimeters();
  
  pub_vec.publish(&distance_msg);
  nh.spinOnce();
}
