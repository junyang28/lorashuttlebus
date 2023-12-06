import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
from sklearn.preprocessing import MinMaxScaler 
from sklearn.preprocessing import StandardScaler 
from sklearn.model_selection import train_test_split
from sklearn import preprocessing
import pandas as pd
import numpy as np

from keras.models import Sequential
from keras.layers import Dense
from keras.utils import to_categorical

## ------------------------------------------------------Training set -----------------------------------
dataset = pd.read_csv("input1.csv", sep=',') 

X = np.array(dataset.drop(['locationx', 'locationy', 'label'], 1))
X = np.float32(X)
scaler = StandardScaler().fit(X) 
X = scaler.transform(X) 
y = np.array(dataset['label'])
y = np.float32(y)
y = to_categorical(y)
y_train = y
X_train = X

dataset2 = pd.read_csv("input2.csv", sep=',') 
X2 = np.array(dataset2.drop(['locationx', 'locationy', 'label'], 1))
X2 = np.float32(X2)
scaler2 = StandardScaler().fit(X2) 
X2 = scaler.transform(X2) 
y2 = np.array(dataset['label'])
y2 = np.float32(y2)
y2 = to_categorical(y2)
y_train2 = y2
X_train2 = X2

dataset3 = pd.read_csv("input3.csv", sep=',') 
X3 = np.array(dataset3.drop(['locationx', 'locationy', 'label'], 1))
X3 = np.float32(X3)
scaler3 = StandardScaler().fit(X3) 
X3 = scaler.transform(X3) 
y3 = np.array(dataset['label'])
y3 = np.float32(y3)
y3 = to_categorical(y3)
y_train3 = y3
X_train3 = X3

dataset4 = pd.read_csv("input4.csv", sep=',') 
X4 = np.array(dataset4.drop(['locationx', 'locationy', 'label'], 1))
X4 = np.float32(X4)
scaler = StandardScaler().fit(X4) 
X4 = scaler.transform(X4) 
y4 = np.array(dataset['label'])
y4 = np.float32(y4)
y4 = to_categorical(y4)
y_train4 = y4
X_train4 = X4

dataset5 = pd.read_csv("input5.csv", sep=',') 
X5 = np.array(dataset5.drop(['locationx', 'locationy', 'label'], 1))
X5 = np.float32(X5)
scaler = StandardScaler().fit(X5) 
X5 = scaler.transform(X5) 
y5 = np.array(dataset['label'])
y5 = np.float32(y5)
y5 = to_categorical(y5)
y_train5 = y5
X_train5 = X5
## -----------------------------------------------------------------------------------------------------

print(X_train.shape)
print(y_train.shape)

model = Sequential()
model.add(Dense(120, input_dim=2, activation='relu'))
model.add(Dense(96, activation='relu'))
model.add(Dense(2, activation='softmax'))
# Compile model
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.summary()

#----------------------training----------------------
model.fit(X_train, y_train, epochs=100,batch_size = 100, verbose = 1)
_, accuracy = model.evaluate(X_train, y_train)
print('Accuracy: %.2f' % (accuracy*100))


#model2
model2 = Sequential()
for layer in model.layers[:-1]:
	model2.add(layer)
model2.add(Dense(67, activation='softmax'))
for layer in model2.layers[3:]:
	layer.trainable = False
model2.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model2.summary()
model2.fit(X2, y2, epochs=60)
_, accuracy = model2.evaluate(X_train2, y_train2)
print('Accuracy: %.2f' % (accuracy*100))

#model3
model3 = Sequential()
for layer in model.layers[:-1]:
	model3.add(layer)
model2.add(Dense(67, activation='softmax'))
for layer in model2.layers[3:]:
	layer.trainable = False
model3.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model3.summary()
model3.fit(X, y, epochs=60)
_, accuracy = model2.evaluate(X_train, y_train)
print('Accuracy: %.2f' % (accuracy*100))