import  os
import numpy as np
import matplotlib.pyplot as plt

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

import tensorflow as tf
from  tensorflow import keras
from  tensorflow.keras import datasets, layers, optimizers, Sequential, metrics


def get_data(data_file_address_train,label_file_address_train,data_file_address_test,label_file_address_test):
    def preprocess(x, y):

        x = tf.cast(x, dtype=tf.float32)
        y = tf.cast(y, dtype=tf.int32)
        return x,y

    x_train=np.load(data_file_address_train)
    y_train=np.load(label_file_address_train)
    x_test=np.load(data_file_address_test)
    y_test=np.load(label_file_address_test)
    x_train = x_train.reshape(x_train.shape[0],15,15,1)
    x_test = x_test.reshape(x_test.shape[0],15,15,1)
    return (x_train,y_train,x_test,y_test)

def compile_model(network):
    # Get our network parameters.
    kernals = network['nb_kernals']
    nb_Conv2D_32_layers=network['nb_Conv2D_32_layers']
    nb_Conv2D_64_layers=network['nb_Conv2D_64_layers']
    nb_Conv2D_128_layers=network['nb_Conv2D_128_layers']
    nb_Dense_layers = network['nb_Dense_layers']
    nb_neurons = network['nb_neurons']
    activation = network['activation']
    optimizer = network['optimizer']

    model = Sequential()
    
    for i in range(nb_Conv2D_32_layers):
         if i == 0: # Need input shape for first layer.
             model.add(layers.Conv2D(filters=32, kernel_size=(kernals,kernals), padding='same',input_shape=(15,15,1),  activation='relu'))
         else:
             model.add(layers.Conv2D(filters=32, kernel_size=(kernals,kernals), padding='same', activation='relu'))

    for i in range(nb_Conv2D_64_layers):
        model.add(layers.Conv2D(filters=64, kernel_size=(kernals,kernals), padding='same',activation='relu'))

    for i in range(nb_Conv2D_128_layers):
        model.add(layers.Conv2D(filters=128, kernel_size=(kernals,kernals), padding='same',activation='relu'))

    model.add(tf.keras.layers.Flatten())

    for i in range(nb_Dense_layers):
        model.add(layers.Dense(nb_neurons, activation=activation))

    model.add(layers.Dense(225,activation='softmax'))
    model.summary()

    model.compile(loss='sparse_categorical_crossentropy',optimizer=optimizer, metrics=['accuracy'])
    
    return model

def train_and_score(network):

    # x_train,y_train,x_test,y_test=get_data('./npy/train_x_11000-11600.npy','./npy/train_y_11000-11600.npy','./npy/test_x_10000-10060.npy','./npy/test_y_10000-10060.npy')
    # x_train,y_train,x_test,y_test=get_data('./npy/train_x_21000-22000.npy','./npy/train_y_21000-22000.npy','./npy/test_x_22000-22100.npy','./npy/test_y_22000-22100.npy')
    x_train,y_train,x_test,y_test=get_data('./npy/train_x_60100-60200.npy','./npy/train_y_60100-60200.npy','./npy/test_x_60000-60010.npy','./npy/test_y_60000-60010.npy')
    model = compile_model(network)

    cb = keras.callbacks.EarlyStopping(monitor='val_accuracy', patience=0, verbose=0, mode='auto')
    history = model.fit(x_train,y_train, epochs=10000, verbose = 1,validation_data=(x_test,y_test), callbacks=[cb])
   # history = model.fit(x_train,y_train, epochs=5, verbose = 1,validation_data=(x_test,y_test))
    score = model.evaluate(x_test, y_test, verbose=1)
    
    return score[1] #return accuracy