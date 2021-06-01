import  os
import pydot
import numpy as np
import matplotlib.pyplot as plt

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import tensorflow as tf
from    tensorflow import keras
from    tensorflow.keras import datasets, layers, optimizers, Sequential, metrics

assert tf.__version__.startswith('2.')

def preprocess(x, y):

    x = tf.cast(x, dtype=tf.float32)
    y = tf.cast(y, dtype=tf.int32)
    return x,y


#(x_train, y_train), (x_test, y_test) = datasets.mnist.load_data()
#print(x_train)
#x_train = x_train.reshape(x_train.shape[0],28,28,1)/255
#x_test = x_test.reshape(x_test.shape[0],28,28,1)/255

x_train=np.load('./npy/train_x_0-2000.npy')
y_train=np.load('./npy/train_y_0-2000.npy')
x_test=np.load('./npy/test_x_5000-5200.npy')
y_test=np.load('./npy/test_y_5000-5200.npy')
#print(x_train)
x_train = x_train.reshape(x_train.shape[0],15,15,1)
x_test = x_test.reshape(x_test.shape[0],15,15,1)

batchsz = 10

#db_test = tf.data.Dataset.from_tensor_slices((x_test,y_test))
#db_test = db_test.map(preprocess).batch(batchsz)

model = Sequential([
    layers.Conv2D(filters=32, kernel_size=(3,3), padding='same',input_shape=(15,15,1),  activation='relu'),
    #layers.MaxPooling2D(pool_size=(2, 2)),
    
    layers.Conv2D(filters=32, kernel_size=(3,3), padding='same',  activation='relu'),
    #layers.MaxPooling2D(pool_size=(2, 2)),

    layers.Conv2D(filters=64, kernel_size=(3,3), padding='same',  activation='relu'),  

    layers.Conv2D(filters=128, kernel_size=(3,3), padding='same',  activation='relu'),
    layers.Conv2D(filters=128, kernel_size=(3,3), padding='same',  activation='relu'),
    tf.keras.layers.Flatten(),
    
    layers.Dense(1024, activation='elu'),
    layers.Dense(225,activation='softmax')
])

model.summary()
#tf.keras.utils.plot_model(model,to_file=os.path.join('./png/model.png'),show_shapes=True,show_layer_names=True)
#check_path = './ckpt/cp-0004.ckpt'
check_path = './ckpt/cp-1.ckpt'
cb = tf.keras.callbacks.ModelCheckpoint(check_path, save_weights_only=True)
#cb = keras.callbacks.EarlyStopping(monitor='val_loss', patience=0, verbose=0, mode='auto')
#训练配置
model.compile(loss='sparse_categorical_crossentropy',optimizer='nadam', metrics=['accuracy']) 

#开始训练
history = model.fit(x_train,y_train, epochs=8, validation_data=(x_test,y_test), callbacks=[cb])

score=model.evaluate(x_test,y_test,verbose=0)

#储存模型
model.save("./model/model.h5")

#可视化
history_dict=history.history
accuracy_value=history_dict["accuracy"]
val_accuracy_value=history_dict["val_accuracy"]

epochs=range(1,len(accuracy_value)+1)
plt.plot(epochs,accuracy_value,"bo",label="Training accuracy")
plt.plot(epochs,val_accuracy_value,"b",label="Validation accuracy")
plt.xlabel("epochs")
plt.ylabel("accuracy")
plt.legend()
plt.show()
plt.savefig("./png/loss.png")
