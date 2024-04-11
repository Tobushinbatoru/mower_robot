#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <algorithm>

class SerialCommunicationNode {
public:
    SerialCommunicationNode() {
        // シリアルポートを設定
        serial_port_.setPort("/dev/ttyUSB0");
        serial_port_.setBaudrate(115200);

        try {
            // シリアルポートを開く
            serial_port_.open();
            ROS_INFO("Serial port opened.");
        } catch (const serial::IOException& e) {
            ROS_ERROR_STREAM("Failed to open serial port: " << e.what());
            ros::shutdown();
        }

        // ROSパブリッシャを設定
        pub_ = nh_.advertise<std_msgs::Int32>("int_serial_data", 10);

        // デフォルト値を設定
        last_data_.data = 0; // 初期値として整数0を代入
    }

    void run() {
        ros::Rate rate(100); // ループレートを設定

        while (ros::ok()) {
            if (serial_port_.isOpen()) {
                try {
                    std_msgs::String msg;
                    msg.data = serial_port_.readline();
                    
                    // 改行文字を削除
                    msg.data.erase(std::remove_if(msg.data.begin(), msg.data.end(), [](char c) { return c == '\n' || c == '\r'; }), msg.data.end());    

                    if (!msg.data.empty()) {
                        // 文字列を整数に変換
                        try {
                            std_msgs::Int32 int_msg;
                            int_msg.data = std::stoi(msg.data);
                            last_data_ = int_msg;
                            ROS_INFO_STREAM("Received data (int): " << int_msg.data);
                            pub_.publish(last_data_);
                        } catch (const std::invalid_argument& e) {
                            ROS_ERROR_STREAM("Failed to convert to int: " << e.what());
                        }
                    }
                } catch (const serial::IOException& e) {
                    ROS_ERROR_STREAM("Serial communication error: " << e.what());
                }
            }

            ros::spinOnce();
            rate.sleep();
        }
    }

private:
    ros::NodeHandle nh_;
    serial::Serial serial_port_;
    ros::Publisher pub_;
    std_msgs::Int32 last_data_; // 整数データを保持
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "serial_communication_node");
    SerialCommunicationNode node;
    node.run();
    return 0;
}


/*
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>

class SerialCommunicationNode {
public:
    SerialCommunicationNode() {
        // シリアルポートを設定
        serial_port_.setPort("/dev/ttyUSB0");
        serial_port_.setBaudrate(115200);

        try {
            // シリアルポートを開く
            serial_port_.open();
            ROS_INFO("Serial port opened.");
        } catch (const serial::IOException& e) {
            ROS_ERROR_STREAM("Failed to open serial port: " << e.what());
            ros::shutdown();
        }

        // ROSパブリッシャを設定
        pub_ = nh_.advertise<std_msgs::String>("serial_data", 10);

        // デフォルト値を設定
        last_data_.data = "Initial Value";
    }

    void run() {
        ros::Rate rate(100); // ループレートを設定

        while (ros::ok()) {
            if (serial_port_.isOpen()) {
                try {
                    std_msgs::String msg;
                    msg.data = serial_port_.readline();
                    
                    // 改行文字を削除
                    msg.data.erase(std::remove(msg.data.begin(), msg.data.end(), '\n'), msg.data.end());
                    msg.data.erase(std::remove(msg.data.begin(), msg.data.end(), '\r'), msg.data.end());    

                    if (!msg.data.empty()) {
                        // メッセージが更新された場合にのみ前のデータを更新
                        last_data_ = int(msg);
                        ROS_INFO_STREAM("Received data: " << last_data_);
                    }

                    // 前のデータをパブリッシュ
                    pub_.publish(last_data_);
                } catch (const serial::IOException& e) {
                    ROS_ERROR_STREAM("Serial communication error: " << e.what());
                }
            }

            ros::spinOnce();
            rate.sleep();
        }
    }

private:
    ros::NodeHandle nh_;
    serial::Serial serial_port_;
    ros::Publisher pub_;
    std_msgs::String last_data_; // 前回のデータを保持
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "serial_communication_node");
    SerialCommunicationNode node;
    node.run();
    return 0;
}

*/


/*
// serial_communication_node.cpp
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <serial/serial.h>  // serialライブラリを使用

int main(int argc, char** argv)
{
    ros::init(argc, argv, "serial_communication_node");
    ros::NodeHandle nh;

    // シリアルポートの設定
    serial::Serial my_serial("/dev/ttyUSB0", 115200, serial::Timeout::simpleTimeout(1000));

    // ROSトピックの定義
    ros::Publisher serial_pub = nh.advertise<std_msgs::String>("serial_data", 1000);

    ros::Rate loop_rate(100);

    while (ros::ok())
    {
        if (my_serial.available())
        {
            std_msgs::String msg;
            msg.data = my_serial.read(my_serial.available());
            serial_pub.publish(msg);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

*/
