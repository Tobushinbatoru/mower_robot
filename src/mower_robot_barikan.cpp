#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/Joy.h"

// Bool型のメッセージを作成
std_msgs::Bool bool_msg;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy_msg) {
    // Joyメッセージ内のボタン情報を確認する
    if (joy_msg->buttons[0] == 1) { // ここではボタン0が押されたかどうかを確認
        ROS_INFO("Button 0 Pressed!");
        // ボタンが押された時のアクションをここに追加
        // 例えば、別のトピックにメッセージをパブリッシュしたり、特定の処理を行ったりする
        bool_msg.data = true; // データを設定（true）
    }
    else{
        bool_msg.data = false; // データを設定（false）
    }
}

int main(int argc, char **argv) {
    // ROSノードの初期化
    ros::init(argc, argv, "barikan_publisher");

    // ノードハンドルの作成
    ros::NodeHandle n;

    // Joyメッセージを購読するためのSubscriberの作成
    ros::Subscriber joy_sub = n.subscribe<sensor_msgs::Joy>("joy", 10, joyCallback);
    
    // パブリッシャの作成（std_msgs::Bool型のメッセージを送信する）
    ros::Publisher bool_pub = n.advertise<std_msgs::Bool>("barikan", 10);

    // ループレート
    ros::Rate loop_rate(1); // ループの周波数を1Hzに設定

    while (ros::ok()) {
        // メッセージをパブリッシュ
        bool_pub.publish(bool_msg);

        // ループを指定した周波数に保つためのウェイト
        loop_rate.sleep();
        ros::spinOnce(); // コールバック関数を呼び出すために必要
    }
    return 0;
}

