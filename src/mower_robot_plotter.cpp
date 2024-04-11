#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <math.h>

/*
struct ROBOT{
	std_msgs::Float32 left_front_wheel;
	std_msgs::Float32 left_rear_wheel;
	std_msgs::Float32 right_front_wheel;
	std_msgs::Float32 right_rear_wheel;
};

ROBOT mower_robot_leg = {0.0, 0.0, 0.0, 0.0};
ROBOT mower_robot_current = {0.0, 0.0, 0.0, 0.0};
*/
    
std_msgs::Float32 left_front_wheel;
std_msgs::Float32 left_rear_wheel;
std_msgs::Float32 right_front_wheel;
std_msgs::Float32 right_rear_wheel;

//left_front_wheel.data = 0.0;//sin(0.02 * count * 2 * M_PI);
//left_rear_wheel.data = 0.0;
//right_front_wheel.data = 0.0;
//right_rear_wheel.data = 0.0;

void dataCallback(const std_msgs::Float32MultiArray::ConstPtr& msg) {
    ROS_INFO("Received Data:");
    for (size_t i = 0; i < msg->data.size(); ++i) {
        ROS_INFO("Data[%zu]: %f", i, msg->data[i]);
	left_front_wheel.data = msg->data[0];
	left_rear_wheel.data = msg->data[1];
	right_front_wheel.data = msg->data[2];
	right_rear_wheel.data = msg->data[3];
    }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "info_float32_publisher");
  ros::NodeHandle nh;
  ros::Publisher left_front_wheel_pub = nh.advertise<std_msgs::Float32>("left_front_wheel", 10);
  ros::Publisher left_rear_wheel_pub  = nh.advertise<std_msgs::Float32>("left_rear_wheel", 10);
  ros::Publisher right_front_wheel_pub = nh.advertise<std_msgs::Float32>("right_front_wheel", 10);
  ros::Publisher right_rear_wheel_pub  = nh.advertise<std_msgs::Float32>("right_rear_wheel", 10);
  
  ros::Subscriber sub = nh.subscribe("distance", 10, dataCallback);

  ros::Rate loop_rate(100);
  int count = 0;
  
  while (ros::ok())
  {
    
    left_front_wheel_pub.publish(left_front_wheel);
    left_rear_wheel_pub.publish(left_rear_wheel);
    right_front_wheel_pub.publish(right_front_wheel);
    right_rear_wheel_pub.publish(right_rear_wheel);

    count++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
