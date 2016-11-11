#!/usr/bin/python
from __future__ import print_function
from keras.models import model_from_json
import sys
import numpy as np
import cv2
print(sys.version_info)
print(cv2.__version__)

testing_set_size = 1610 # 1630 for testing scene4, 1610 for testing scene5
feature_dimension_root = 32

def predict_testing_set(testing_set_size, feature_dim_root, classifier):
    cv2.namedWindow('image_test')
    # f_testing_response = open('score_predicted_project2.txt', 'w')
    f_testing_response = open('score_predicted_scene5_project2_maxpool.txt', 'w')

    for i in range(0, testing_set_size):        
        ROI = []
        
        # img = cv2.imread('./test/image_{0}.png'.format(i), cv2.IMREAD_GRAYSCALE)
        img = cv2.imread('./scene5/img_32_{0}.png'.format(i+1), cv2.IMREAD_GRAYSCALE)
        cv2.imshow('image_test', img)
        cv2.waitKey(5) & 0xFF
        
        # ROI.append(img)
        ROI.append([img])
        ROI_array = np.array(ROI, dtype=np.int32)
        # print(ROI_array.shape)
        # ROI_array = np.swapaxes(ROI_array, 1, 3)
        # ROI_array = np.swapaxes(ROI_array, 2, 3)
        ROI_array = ROI_array.astype('float32')
        ROI_array /= 255
        roi_test = ROI_array[0,...]
        roi_test = roi_test[np.newaxis,...]
        # result = classifier.predict_classes(roi_test, verbose=0)
        proba = classifier.predict_proba(roi_test, verbose=0)
        # f_testing_response.write("{0}\n".format(proba)) 
        f_testing_response.write("{0}\n".format(proba[0][0]))

    f_testing_response.close()
    cv2.destroyAllWindows() 


# load function
print('Now loading function')
# model = model_from_json(open('Project2_architecture.json').read())
# model.load_weights('Project2_weights.h5')
model = model_from_json(open('Project2_architecture_scene123_maxpool.json').read())
model.load_weights('Project2_weights_scene123_maxpool.h5')

# prediction
print('Now prediction')
predict_testing_set(testing_set_size, feature_dimension_root, model)

print('It works.')