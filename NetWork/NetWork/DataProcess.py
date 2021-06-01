import os
import numpy as np

def get_str_btw(s, f, b):
    par = s.partition(f)
    return (par[2].partition(b))[0][:]




input_state=[]
label=[]
row=0
for j in range(60100,60200):
    old_state=np.zeros((15,15))
    new_state=np.zeros((15,15))
    filename='./Renju_RIF_Dataset/70000Games/Board'+str(j)+'.xml'
    print(filename)
    f=open(filename)
    b=f.read()
    c1='<move>'
    c2='\n'
    str_=get_str_btw(b,c1,c2)
    str_list=str_.split(" ")
    side=-1
    #print(str_list)
    while '' in str_list:
        str_list.remove('')
    for i in range(len(str_list)):
        letter=str_list[i][0]
        number=str_list[i][1:]
        side=side*-1
        row=row+1
        new_state[(int)(ord(letter)-97)][(int)(number)-1]=side
    
        #label_singel=np.zeros((15,15))
        #label_singel[(int)(ord(letter)-97)][(int)(number)-1]=1
        #label=np.append(label,label_singel)
        #label=label.reshape(row,15,15)
        label.append((int)((ord(letter)-97)*15+(int)(number)-1))
        input_state=np.append(input_state,old_state)
        #print(old_state)
        #input_state=input_state.reshape(row,15,15)
        old_state=np.array(new_state,copy=True)
        #print(label)

input_state=(input_state+1)/2
input_state=input_state.reshape(row,15,15)
print(input_state[2])
#np.save("./npy/train_x.npy",input_state)
#np.save("./npy/train_y.npy",label)
np.save("./npy/train_x_60100-60200.npy",input_state)
np.save("./npy/train_y_60100-60200.npy",label)
f.close