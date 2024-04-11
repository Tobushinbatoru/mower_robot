#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>

class ArduinoDataReceiver {
public:
  ArduinoDataReceiver() {
    // Initialize the ROS node
    ros::NodeHandle nh;

    // Create a subscriber to receive Arduino data
    arduino_data_subscriber = nh.subscribe("arduino_data_topic", 10, &ArduinoDataReceiver::arduinoDataCallback, this);

    // Create a publisher to publish processed data
    processed_data_publisher = nh.advertise<std_msgs::Float32MultiArray>("processed_data", 10);
  }

  void arduinoDataCallback(const std_msgs::Float32MultiArray::ConstPtr& msg) {
    // Received Arduino data
    std_msgs::Float32MultiArray processed_data;

    // Process the data (you can add your processing logic here)
    for (size_t i = 0; i < msg->data.size(); i++) {
      // Modify or process the data as needed
      // For example, you can perform calculations or transformations here
      processed_data.data.push_back(msg->data[i]);
    }

    // Publish the processed data to a ROS topic
    processed_data_publisher.publish(processed_data);

    // Display debug information
    ROS_INFO("Received and processed Arduino data.");
  }

private:
  ros::Subscriber arduino_data_subscriber;
  ros::Publisher processed_data_publisher;
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "arduino_data_receiver");
  ArduinoDataReceiver receiver;

  ros::spin();

  return 0;
}

