#!/usr/bin/env python3
#python3 rosbag_analisys_data.py  ../resources/2100.bag

import rosbag
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

dirname = "20240117/"
os.makedirs(dirname, exist_ok=True)

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
    if topic=="/distance":
        np_pose=np.array([[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]])
        np_pose[0,0]=t.secs
        np_pose[0,1]=t.nsecs

        np_pose[0,2]=msg.data[0]
        np_pose[0,3]=msg.data[1]
        np_pose[0,4]=msg.data[2]
        np_pose[0,5]=msg.data[3]
        
        np_pose[0,2] = 80 - np_pose[0,2]
        np_pose[0,3] = 80 - np_pose[0,3]
        np_pose[0,4] = 80 - np_pose[0,4]
        np_pose[0,5] = 80 - np_pose[0,5]
        
        if np_pose[0,2] > 30:
            np_pose[0,2]=np_pose[0,2]
        else:
            np_pose[0,2]=(-4*np_pose[0,2]/5) + 52
      
        if np_pose[0,3] > 30:
            np_pose[0,3]=np_pose[0,3]
        else:
            np_pose[0,3]=(-4*np_pose[0,3]/5) + 52
        
        if np_pose[0,4] > 30:
            np_pose[0,4]=np_pose[0,4]
        else:
            np_pose[0,4]=(-4*np_pose[0,4]/5) + 52
          
        if np_pose[0,5] > 30:
            np_pose[0,5]=np_pose[0,5]
        else:
            np_pose[0,5]=(-4*np_pose[0,5]/5) + 52
              
        if np_poses is None:
            np_poses=np_pose
        else:
            np_poses=np.append(np_poses,np_pose,axis=0)

# Define the range you want to plot
start_index = 0
end_index = 400
# reform time
start_sec=np_poses[0,0]
start_nsec=np_poses[0,1]
t=np.zeros(np_poses.shape[0],dtype='float32')
for i in range(np_poses.shape[0]):
    t[i]=(np_poses[i,0]-start_sec)+(np_poses[i,1]-start_nsec)/1000000000.0
    #print(i)
    #print(t[i])
    

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
ax1.plot(t[start_index:end_index], np_poses[start_index:end_index, 2], 'b', label="left_front_wheel", linewidth=2)
ax2.plot(t[start_index:end_index], np_poses[start_index:end_index, 3], 'b', label="right_front_wheel", linewidth=2)
ax3.plot(t[start_index:end_index], np_poses[start_index:end_index, 4], 'b', label="left_rear_wheel", linewidth=2)
ax4.plot(t[start_index:end_index], np_poses[start_index:end_index, 5], 'b', label="right_rear_wheel", linewidth=2)
#ax1.plot(t, np_poses[:,2], 'r', label="left_front_wheel", linewidth=2)  # 太さを2に設定
#ax2.plot(t, np_poses[:,3], 'b', label="right_front_wheel", linewidth=2)  # 太さを2に設定
#ax3.plot(t, np_poses[:,4], 'r', label="left_rear_wheel", linewidth=2)  # 太さを2に設定
#ax4.plot(t, np_poses[:,5], 'b', label="right_rear_wheel", linewidth=2)  # 太さを2に設定

for ax in [ax1, ax2, ax3, ax4]:
    for label in (ax.get_xticklabels() + ax.get_yticklabels()):
        label.set_fontsize(16)  # ラベルのサイズを設定
        label.set_fontweight('bold')  # ラベルを太字に設定

# 凡例を指定した位置に表示する(一つの引数)(upper right, upper left, lower right lower left)
# 凡例を特定の位置に移動する(bbox_to_anchor=(x, y)では、(0, 0)が図の左下隅、(1, 1)が右上隅)
# 凡例の位置を調整し、テキストを太字で大きく表示する
ax1.legend(loc='upper left')
ax2.legend(loc='upper left')
ax3.legend(loc='upper left')
ax4.legend(loc='upper left')

#ax1.legend(loc='upper left', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax2.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax3.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})
#ax4.legend(loc='lower right', bbox_to_anchor=(0.3, 0.05), prop={'size': 14, 'weight': 'bold'})

# ax1にラベルを追加
#ax1.text(0.9, 0.9, 'left_front', ha='center', va='center', transform=ax1.transAxes, fontsize=12, color='red')
# ax2にラベルを追加
#ax2.text(0.9, 0.9, 'right_front', ha='center', va='center', transform=ax2.transAxes, fontsize=12, color='blue')

# 4. y軸の範囲を調節とグラフタイトル・ラベル付け
ax1.set_ylim(-1.0, 120.0)
ax2.set_ylim(-1.0, 120.0)
ax3.set_ylim(-1.0, 120.0)
ax4.set_ylim(-1.0, 120.0)

# タイトルに数式を挿入するために、LaTeXの数式モードを使って、$で囲む
#ax1.set_title("$\ time vs left$")
#ax2.set_title("$\ time vs right$")

# 横線を引く - --
########################################################################################
# Add text to a specific point in ax1 (left_front_wheel subplot)
#x_coord = 1.25  # x-coordinate
#y_coord = 18.0  # y-coordinate
#text_to_add = "○ Start"
#ax1.text(x_coord, y_coord, text_to_add, fontsize=12, color='green')
#ax1.axvline(x=7.0, color='red', linestyle='--')
#ax1.axhline(y=50, color='gray', linestyle='--')
#ax1.axvline(x=1.73, color='red', linestyle='--')
#ax1.axvline(x=12.0, color='red', linestyle='--')
#ax1.axhline(y=28.0, color='gray', linestyle='--')
#####################################################################################
#ax2.axvline(x=7.0, color='red', linestyle='--')
#ax2.axhline(y=50, color='gray', linestyle='--')
#ax2.axvline(x=1.73, color='red', linestyle='--')
#ax2.axvline(x=12.0, color='red', linestyle='--')
#ax2.axhline(y=28.0, color='gray', linestyle='--')
#######################################################################################
#ax3.axvline(x=7.0, color='red', linestyle='--')
#ax3.axhline(y=50, color='gray', linestyle='--')
#ax3.axvline(x=1.73, color='red', linestyle='--')
#ax3.axvline(x=12.0, color='red', linestyle='--')
#ax3.axhline(y=28.0, color='gray', linestyle='--')
#####################################################################################
#ax4.axvline(x=7.0, color='red', linestyle='--')
#ax4.axhline(y=50, color='gray', linestyle='--')
#ax4.axvline(x=1.73, color='red', linestyle='--')
#ax4.axvline(x=12.0, color='red', linestyle='--')
#ax4.axhline(y=28.0, color='gray', linestyle='--')
####################################################################################

for ax in [ax1, ax2, ax3, ax4]:
    ax.set_xlabel("times[s]", fontweight='bold', fontsize=14)  # 太字でサイズ14
    ax.set_ylabel("force[N]", fontweight='bold', fontsize=14)  # 太字でサイズ14

# 上下のサブプロット間のスペースを調整
plt.subplots_adjust(hspace=0.5)

# (supported formats: eps, pdf, pgf, png, ps, raw, rgba, svg, svgz)
filename = dirname + "output_graph_2120.png"
plt.savefig(filename, format='png', dpi=300)  # 300dpiのPNG形式で保存
#plt.savefig('output_graph2.pdf', format='pdf')
#plt.savefig('output_graph2.eps', format='eps')

# fig.show()だと一瞬で画面表示して消えてしまう
plt.show()

fig.tight_layout()  # タイトルとラベルが被るのを解消


bag.close()

