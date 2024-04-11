#!/usr/bin/env python

import rospy
import serial
from std_msgs.msg import String

class SerialCommunicationNode:
    def __init__(self):
        rospy.init_node('serial_communication_node')
        self.pub = rospy.Publisher('serial_data', String, queue_size=10)
        self.ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)  # シリアルポートとボーレートを設定

    def run(self):
        rate = rospy.Rate(10)  # ループレートを設定

        while not rospy.is_shutdown():
            data = self.ser.readline().strip()
            rospy.loginfo("Received data: %s", data)
            self.pub.publish(data)
            rate.sleep()

if __name__ == '__main__':
    try:
        node = SerialCommunicationNode()
        node.run()
    except rospy.ROSInterruptException:
        pass

