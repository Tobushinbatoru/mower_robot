#!/usr/bin/env python3
import rospy
from std_msgs.msg import Float64
import numpy as np
import matplotlib.pyplot as plt
from std_msgs.msg import Float32MultiArray
import time


start_time = 0.0
t_ = 0.0
y_ = 0.0

def callback(msg):
    print('callback')

    global start_time
    global t_
    global y_

    if start_time>0:
        t_ = time.time() - start_time
        y_ = msg.data[4]

def graph():
    print('realtime_graph')
    rospy.init_node('realtime_graph', anonymous=True)
    rospy.Subscriber("/data", Float32MultiArray, callback)
    

  #グラフの初期化
    t = [0 for i in range(1000)]	#x軸のサイズ
    y = [0 for k in range(1000)]	#y軸のサイズ
    
    plt.ion()
    plt.figure()

    plt.title("y")
    plt.xlabel("time[s]")
    plt.ylabel("y[-]")
    plt.ylim(-1, 30)	#固定：y軸の最小値，最大値
    plt.grid(True)
    li, = plt.plot(t, y)
    
    global start_time
    global t_
    global y_

    start_time = time.time()


  #グラフの更新
    while not rospy.is_shutdown():
        print('loop')
        
        t.append(t_)	#x軸で新しい値を追加
        t.pop(0)	#x軸で古い値を消去
        y.append(y_)	#y軸で新しい値を追加
        y.pop(0)	#y軸で古い値を消去

        li.set_xdata(t)
        li.set_ydata(y)
        plt.xlim(min(t), max(t))	#動的：x軸の最小値，最大値

        plt.draw()
        plt.pause(0.1)	#更新周期
    rospy.spin()

if __name__ == '__main__':
    graph()
