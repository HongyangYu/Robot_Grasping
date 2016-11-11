# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 18:15:03 2016

@author: Henry
"""
from sklearn.externals import joblib
from sklearn import svm
import numpy as np

def trainSVR(X, Y, filename_SVR):
    clf = svm.SVR('linear') #'linear'
    clf.fit(X, Y) 
    joblib.dump(clf, filename_SVR)

def run_trainSVR():
    scene_num_list = [1,2]#range(2,4)
    img_bit = 32
    X=np.array([])
    Y=np.array([])
    i = 0
    for scene_num in scene_num_list:
        print 'scene '+str(scene_num)
        path = './score/scene'+str(scene_num)
        filename_X = path+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)+'.npy'
        filename_Y = path+'/score_file_scene'+str(scene_num)+'.npy'
        
        if i == 0:
            X = np.load(filename_X)
            Y = np.load(filename_Y)
            i=1
        else:
            X = np.concatenate((X, np.load(filename_X) ), axis=0)
            Y = np.concatenate((Y, np.load(filename_Y) ), axis=0)
#        np.hstack( (X, np.load(filename_X)) )
#        np.hstack( (Y, np.load(filename_Y)) )
    print np.shape(X), np.shape(Y)
    filename_SVR = './score/SVR_linear/SVR_img_'+str(img_bit)+'.pkl'      
    trainSVR(X,Y, filename_SVR)


def run_trainSVR_try(range_start, range_end):
    # range_num = range_end - range_start
    scene_num_list = range(3,4)
    img_bit = 32
    for scene_num in scene_num_list:
        filename_X = './score/scene'+str(scene_num)+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)+'_range_'+str(range_start)+'_'+str(range_end)+'.npy'
        filename_Y = './score/scene'+str(scene_num)+'/score_file_scene'+str(scene_num)+'.npy'
        filename_SVR = './score/scene'+str(scene_num)+'/SVR_test/SVR_scene'+str(scene_num)+'_img_'+str(img_bit)+'.pkl'
        X = np.load(filename_X)
        Y = np.load(filename_Y)
        Y = Y[range_start-1:range_end-1]
        trainSVR(X,Y, filename_SVR)

run_trainSVR()

#run_trainSVR_try(1001, 1631)
print '======= TRAIN  END  ========'



