//#include <MsTimer2.h>
#include <Arduino.h>
#include <ros.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle  nh;
std_msgs::Float32MultiArray data_msg;
ros::Publisher pub_vec("data", &data_msg);

Adafruit_INA219 LF (0x40);       	//LEFT_FRONT    ー ー
Adafruit_INA219 RF (0x41); 			//RIGHT_FRONT   ○　ー
Adafruit_INA219 LB (0x44); 			//LEFT_BACK     ー ○
Adafruit_INA219 RB (0x45); 			//RIGHT_BACK    ○  ○

int left_front_pin  = A0;    		// select the input pin for the potentiometer
int right_front_pin = A1;
int left_back_pin   = A2;
int right_back_pin  = A3;



//void flash(); 

void setup(void)
{
	//Serial.begin(115200);
	LF.begin();
	RF.begin();
	LB.begin();
	RB.begin();

	nh.initNode();
    nh.advertise(pub_vec);
    data_msg.data = (float*)malloc(sizeof(float) * 8);
    data_msg.data_length = 8;
	//ina219.setCalibration_16V_400mA();
	//MsTimer2::set(50, flash);     // 500ms毎にflash( )割込み関数を呼び出す様に設定
    //MsTimer2::start();             // タイマー割り込み開始
}

void loop(void)
{
	//float LF_shuntvoltage = 0;
	//float LF_busvoltage = 0;
	float LF_current_mA = 0;
	//float LF_loadvoltage = 0;
	//float LF_power_mW = 0;

	//float RF_shuntvoltage = 0;
	//float RF_busvoltage = 0;
	float RF_current_mA = 0;
	//float RF_loadvoltage = 0;
	//float RF_power_mW = 0;

	//float LB_shuntvoltage = 0;
	//float LB_busvoltage = 0;
	float LB_current_mA = 0;
	//float LB_loadvoltage = 0;
	//float LB_power_mW = 0;

	//float RB_shuntvoltage = 0;
	//float RB_busvoltage = 0;
	float RB_current_mA = 0;
	//float RB_loadvoltage = 0;
	//float RB_power_mW = 0;

	// variable to store the value coming from the sensor
	int left_front_val  = 0;
	int right_front_val = 0;
	int left_back_val   = 0;
	int right_back_val  = 0;

	// read the value from the sensor
	left_front_val  = analogRead(left_front_pin);
  	right_front_val = analogRead(right_front_pin);
  	left_back_val   = analogRead(left_back_pin);
  	right_back_val  = analogRead(right_back_pin);

	//LF_shuntvoltage = LF.getShuntVoltage_mV();
	//LF_busvoltage = LF.getBusVoltage_V();
	LF_current_mA = LF.getCurrent_mA();
	//LF_power_mW = LF.getPower_mW();
	//LF_loadvoltage = LF_busvoltage + (LF_shuntvoltage / 1000);

	//RF_shuntvoltage = RF.getShuntVoltage_mV();
	//RF_busvoltage = RF.getBusVoltage_V();
	RF_current_mA = RF.getCurrent_mA();
	//RF_power_mW = RF.getPower_mW();
	//RF_loadvoltage = RF_busvoltage + (RF_shuntvoltage / 1000);

	//LB_shuntvoltage = LB.getShuntVoltage_mV();
	//LB_busvoltage = LB.getBusVoltage_V();
	LB_current_mA = LB.getCurrent_mA();
	//LB_power_mW = LB.getPower_mW();
	//LB_loadvoltage = LB_busvoltage + (LB_shuntvoltage / 1000);

	//RB_shuntvoltage = RB.getShuntVoltage_mV();
	//RB_busvoltage = RB.getBusVoltage_V();
	RB_current_mA = RB.getCurrent_mA();
	//RB_power_mW = RB.getPower_mW();
	//RB_loadvoltage = RB_busvoltage + (RB_shuntvoltage / 1000);

	//Serial.print("A0:"); Serial.print(left_front_val);
  	//Serial.print("A1:"); Serial.print(right_front_val);
  	//Serial.print("A2:"); Serial.print(left_back_val);
  	//Serial.print("A3:"); Serial.println(right_back_val);
  	//map(val, 0, 1023, 0, 255);
  	data_msg.data[0] = int(LF_current_mA);
	data_msg.data[1] = int(RF_current_mA);
	data_msg.data[2] = int(LB_current_mA);
	data_msg.data[3] = int(RB_current_mA);
	
	data_msg.data[4] = -0.0988*left_front_val+70.6;//map(left_front_val, 0, 1000, 0, 60);
	data_msg.data[5] = -0.096*right_front_val+71.6;//map(right_front_val, 0, 1000, 0, 60);
	data_msg.data[6] = -0.0882*left_back_val+67.6;//map(left_back_val, 0, 1000, 0, 60);
	data_msg.data[7] = -0.091*right_back_val+68.9;//map(right_back_val, 0, 1000, 0, 60);

	pub_vec.publish(&data_msg);
    nh.spinOnce();
	delay(20);
}
/*
void flash() {
  	data_msg.data[0] = LF_current_mA;
	data_msg.data[1] = RF_current_mA;
	data_msg.data[2] = LB_current_mA;
	data_msg.data[3] = RB_current_mA;
	data_msg.data[4] = left_front_val;
	data_msg.data[5] = right_front_val;
	data_msg.data[6] = left_back_val;
	data_msg.data[7] = right_back_val;
}
*/
/*
	Serial.print("Bus Voltage(V):    "); Serial.print(LF_busvoltage); Serial.print(" "); Serial.print(RF_busvoltage); Serial.print(" "); Serial.print(LB_busvoltage); Serial.print(" "); Serial.print(RB_busvoltage); Serial.println(" ");
	Serial.print("Shunt Voltage(mV): "); Serial.print(LF_shuntvoltage); Serial.print(" "); Serial.print(RF_shuntvoltage); Serial.print(" "); Serial.print(LB_shuntvoltage); Serial.print(" "); Serial.print(RB_shuntvoltage); Serial.println(" ");
	Serial.print("Load Voltage(V):   "); Serial.print(LF_loadvoltage); Serial.print(" "); Serial.print(RF_loadvoltage); Serial.print(" "); Serial.print(LB_loadvoltage); Serial.print(" "); Serial.print(RB_loadvoltage); Serial.println(" ");
	Serial.print("Current(mA):       "); Serial.print(LF_current_mA); Serial.print(" "); Serial.print(RF_current_mA); Serial.print(" "); Serial.print(LB_current_mA); Serial.print(" "); Serial.print(RB_current_mA); Serial.println(" ");
	Serial.print("Power(mW):         "); Serial.print(LF_power_mW); Serial.print(" "); Serial.print(RF_power_mW); Serial.print(" "); Serial.print(LB_power_mW); Serial.print(" "); Serial.print(RB_power_mW); Serial.println(" ");
	Serial.println("");
*/
