'''Train a simple deep CNN on the CIFAR10 small images dataset.

GPU run command:
    THEANO_FLAGS=mode=FAST_RUN,device=gpu,floatX=float32 python cifar10_cnn.py

It gets down to 0.65 test logloss in 25 epochs, and down to 0.55 after 50 epochs.
(it's still underfitting at that point, though).

Note: the data was pickled with Python 2, and some encoding issues might prevent you
from loading it in Python 3. You might have to load it in Python 2,
save it in a different format, load it in Python 3 and repickle it.
'''

from __future__ import print_function
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.optimizers import SGD
# from keras.utils import np_utils
import numpy as np

batch_size = 32
nb_classes = 1  # univariable regression
nb_epoch = 200

# input image dimensions
img_rows, img_cols = 32, 32
img_channels = 1  # depth image

# X_train = np.load('training_data_regression.npy')
# y_train = np.load('training_response_regression.npy')
X_train = np.load('MAX_img_s1s2s3.npy')
y_train = np.load('training_response_scene123_project2.npy')
print('X_train shape:', X_train.shape)
print('y_train shape:', y_train.shape)
print(X_train.shape[0], 'train samples')
print(y_train.shape[0], 'train responses')

X_train = X_train.astype('float32')
X_train /= 255
# Y_train = np_utils.to_categorical(y_train, nb_classes)  # convert class vectors to binary class matrices
Y_train = y_train

model = Sequential()

# add the first layer. This applies 32 convolution filters of size 3x3 each.
model.add(Convolution2D(32, 3, 3, border_mode='same', input_shape=(img_channels, img_rows, img_cols)))
model.add(Activation('relu'))
model.add(Convolution2D(32, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Convolution2D(64, 3, 3, border_mode='same'))
model.add(Activation('relu'))
model.add(Convolution2D(64, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(512))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(nb_classes))
model.add(Activation('linear'))  # relu, sigmoid 

# let's train the model using SGD + momentum (how original).
sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)  # configure the optimizer
model.compile(loss='mean_squared_error', optimizer=sgd)  # configure the learning process. By default, shuffle=True

# iterate on training data in batches
model.fit(X_train, Y_train, batch_size=batch_size, nb_epoch=nb_epoch, verbose=0)  # verbose = 0

# save the function
json_string = model.to_json()
# open('Project2_architecture.json', 'w').write(json_string)
# model.save_weights('Project2_weights.h5', overwrite=True)
open('Project2_architecture_scene123_maxpool.json', 'w').write(json_string)
model.save_weights('Project2_weights_scene123_maxpool.h5', overwrite=True)

print("Training finished!")

