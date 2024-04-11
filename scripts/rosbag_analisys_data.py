#!/usr/bin/env python3
#python3 rosbag_analisys_data.py  ../resources/2100.bag

import rosbag
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

args = sys.argv
print(len(args))
assert len(args)>=2, "you must specify the argument."

# get path
filename=os.path.normpath(os.path.join(os.getcwd(),args[1]))
print(filename)

# read from bag file
bag = rosbag.Bag(filename)
np_poses=None
for topic, msg, t in bag.read_messages():
    if topic=="/data":
        np_pose=np.array([[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]])
        np_pose[0,0]=t.secs
        np_pose[0,1]=t.nsecs
            
        np_pose[0,2]=30 - msg.data[4]
        np_pose[0,3]=30 - msg.data[5]
        np_pose[0,4]=30 - msg.data[6]
        np_pose[0,5]=30 - msg.data[7]
        
        if np_pose[0,2] < 1:
            np_pose[0,2]=0
        elif np_pose[0,2] <= 30:
            np_pose[0,2]=(-3*np_pose[0,2]/3) + 30
        else:
            np_pose[0,2]=10
       
        if np_pose[0,3] < 1:
            np_pose[0,3]=0
        elif np_pose[0,3] <= 30:
            np_pose[0,3]=(-3*np_pose[0,3]/3) + 30
        else:
            np_pose[0,3]=10
        
        if np_pose[0,4] < 1:
            np_pose[0,4]=0
        elif np_pose[0,4] <= 30:
            np_pose[0,4]=(-3*np_pose[0,4]/3) + 30
        else:
            np_pose[0,4]=10
        
        if np_pose[0,5] < 1:
            np_pose[0,5]=0
        elif np_pose[0,5] <= 30:
            np_pose[0,5]=(-3*np_pose[0,5]/3) + 30
        else:
            np_pose[0,5]=10
            
        if np_pose[0,2]<0:
            np_pose[0,2]=0
        
        if np_pose[0,3]<0:
            np_pose[0,3]=0
            
        if np_pose[0,4]<0:
            np_pose[0,4]=0
            
        if np_pose[0,5]<0:
            np_pose[0,5]=0
        
        if np_poses is None:
            np_poses=np_pose
        else:
            np_poses=np.append(np_poses,np_pose,axis=0)

# Define the range you want to plot
start_index = 240
end_index = 350
# reform time
start_sec=np_poses[0,0]
start_nsec=np_poses[0,1]
t=np.zeros(np_poses.shape[0],dtype='float32')
for i in range(np_poses.shape[0]):
    t[i]=(np_poses[i,0]-start_sec)+(np_poses[i,1]-start_nsec)/1000000000.0
    print(i)
    print(t[i])
    
# 1. Figureのインスタンスを生成
fig = plt.figure(figsize=(20, 12))  # 例として8×6インチの図を生成する
print("type(fig): {}".format(type(fig)))

# 2. Axesのインスタンスを生成
# 2行×2列の構造を持つ4つのサブプロット
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222, sharey=ax1)
ax3 = fig.add_subplot(223, sharey=ax1)
ax4 = fig.add_subplot(224, sharey=ax1)
print("type(ax1): {}".format(type(ax1)))

# 3. データを渡してプロット
ax1.plot(t[start_index:end_index], np_poses[start_index:end_index,2], 'b', label="left_front_wheel", linewidth=2)  # 太さを2に設定
ax2.plot(t[start_index:end_index], np_poses[start_index:end_index,3], 'b', label="right_front_wheel", linewidth=2)  # 太さを2に設定
ax3.plot(t[start_index:end_index], np_poses[start_index:end_index,4], 'b', label="left_rear_wheel", linewidth=2)  # 太さを2に設定
ax4.plot(t[start_index:end_index], np_poses[start_index:end_index,5], 'b', label="right_rear_wheel", linewidth=2)  # 太さを2に設定

for ax in [ax1, ax2, ax3, ax4]:
    for label in (ax.get_xticklabels() + ax.get_yticklabels()):
        label.set_fontsize(16)  # ラベルのサイズを設定
        label.set_fontweight('bold')  # ラベルを太字に設定

# 凡例を指定した位置に表示する(一つの引数)(upper right, upper left, lower right lower left)
# 凡例を特定の位置に移動する(bbox_to_anchor=(x, y)では、(0, 0)が図の左下隅、(1, 1)が右上隅)
# 凡例の位置を調整し、テキストを太字で大きく表示する
ax1.legend(loc='upper right')
ax2.legend(loc='upper right')
ax3.legend(loc='upper right')
ax4.legend(loc='upper right')

#ax1.legend(loc='upper left', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax2.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax3.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax4.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})

# ax1にラベルを追加
#ax1.text(0.9, 0.9, 'left_front', ha='center', va='center', transform=ax1.transAxes, fontsize=12, color='red')
# ax2にラベルを追加
#ax2.text(0.9, 0.9, 'right_front', ha='center', va='center', transform=ax2.transAxes, fontsize=12, color='blue')

# 4. y軸の範囲を調節とグラフタイトル・ラベル付け
ax1.set_ylim(-1.0, 30.0)
ax2.set_ylim(-1.0, 30.0)
ax3.set_ylim(-1.0, 30.0)
ax4.set_ylim(-1.0, 30.0)

# タイトルに数式を挿入するために、LaTeXの数式モードを使って、$で囲む
#ax1.set_title("$\ time vs left$")
#ax2.set_title("$\ time vs right$")

# 横線を引く - --
########################################################################################
ax1.axvline(x=6.27, color='red', linestyle='--')
ax1.axhline(y=0, color='gray', linestyle='--')
ax1.axvline(x=8.53, color='red', linestyle='--')
########################################################################################
ax2.axvline(x=6.27, color='red', linestyle='--')
ax2.axhline(y=0, color='gray', linestyle='--')
ax2.axvline(x=8.53, color='red', linestyle='--')
########################################################################################
ax3.axvline(x=6.27, color='red', linestyle='--')
ax3.axhline(y=0, color='gray', linestyle='--')
ax3.axvline(x=8.53, color='red', linestyle='--')
########################################################################################
ax4.axvline(x=6.27, color='red', linestyle='--')
ax4.axhline(y=0, color='gray', linestyle='--')
ax4.axvline(x=8.53, color='red', linestyle='--')
########################################################################################

for ax in [ax1, ax2, ax3, ax4]:
    ax.set_xlabel("times[s]", fontweight='bold', fontsize=14)  # 太字でサイズ14
    ax.set_ylabel("force[N]", fontweight='bold', fontsize=14)  # 太字でサイズ14

# 上下のサブプロット間のスペースを調整
plt.subplots_adjust(hspace=0.5)

# (supported formats: eps, pdf, pgf, png, ps, raw, rgba, svg, svgz)
plt.savefig('output_graph_0838.png', format='png', dpi=300)  # 300dpiのPNG形式で保存
#plt.savefig('output_graph2.pdf', format='pdf')
#plt.savefig('output_graph2.eps', format='eps')

# fig.show()だと一瞬で画面表示して消えてしまう
plt.show()

fig.tight_layout()  # タイトルとラベルが被るのを解消


bag.close()

