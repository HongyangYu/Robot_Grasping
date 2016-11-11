# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 18:49:10 2016

@author: Henry
"""

from sklearn.externals import joblib
from sklearn.metrics import mean_squared_error
import numpy as np
import csv


def predict_SVR(filename_feature, filename_SVR):
    test_set = np.load(filename_feature) 
    clf = joblib.load(filename_SVR)
    predict_list = clf.predict(test_set)
    return predict_list

def save_predict(path, scene_num, img_bit):
#    filename_SVR = './score/scene3/SVR_test/SVR_scene3_img_32.pkl'
    filename_SVR = './score/SVR_rbf/SVR_img_'+str(img_bit)+'.pkl'
    filename_test_feature = path+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)+'.npy'
    filename_result = path+'/prediction/predict_scene'+str(scene_num)+'_img_'+str(img_bit)
    predict_list =  predict_SVR(filename_test_feature, filename_SVR)
    np.save(filename_result,predict_list)
    return predict_list

def compute_error(A, B):
    mse = mean_squared_error(A, B)
    return mse

def run_predict():
    scene_num = 3
    img_bit = 32
    path = './score/scene'+str(scene_num)
    predict_list = save_predict(path, scene_num, img_bit)
    filename_truth = path+'/score_file_scene'+str(scene_num)+'.npy'
    groundTruth_list = np.load(filename_truth)
    #print '=====', len(groundTruth_list), len(predict_list)
    mse = compute_error(predict_list, groundTruth_list)
    filename_result = path+'/prediction/predict_scene'+str(scene_num)+'_img_'+str(img_bit)+'_rbf.csv'
    #save_to_csv(filename_result, predict_list)
    np.savetxt(filename_result, predict_list, delimiter=",")
    print mse
    return predict_list


def save_to_csv(filename, mylist):
    # result_csv_filename = './score/scene'+str(scene_num)+'/predict_scene3_img_32_range_100.csv'
    with open(filename, 'wb') as f:
        wr = csv.writer(f, quoting=csv.QUOTE_ALL)
        wr.writerow(mylist) 

def save_predict_try(path, range_start, range_end, scene_num, img_bit):
    filename_SVR = path+'/SVR_test/SVR_scene'+str(scene_num)+'_img_'+str(img_bit)+'.pkl'
    filename_test_feature = path+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)+'_range_'+str(range_start)+'_'+str(range_end)+'.npy'
    filename_result = path+'/prediction/predict_scene'+str(scene_num)+'_img_'+str(img_bit)+'_range_'+str(range_start)+'_'+str(range_end)
    predict_list =  predict_SVR(filename_test_feature, filename_SVR)
    np.save(filename_result,predict_list)
    return predict_list

def run_predict_try():
    scene_num = 3
    img_bit = 32
    range_start = 1001
    range_end = 1631
    path = './score/scene'+str(scene_num)
    predict_list = save_predict_try(path, range_start, range_end, scene_num, img_bit)
    filename_truth = path+'/score_file_scene'+str(scene_num)+'.npy'
    groundTruth_list = np.load(filename_truth)    
    groundTruth_list = groundTruth_list[range_start-1:range_end-1]
    #print '=====', len(groundTruth_list), len(predict_list)
    mse = compute_error(predict_list, groundTruth_list)
    filename_result = path+'/prediction/predict_scene'+str(scene_num)+'_img_'+str(img_bit)+'_range_'+str(range_start)+'_'+str(range_end)+'.csv'
    #save_to_csv(filename_result, predict_list)
    np.savetxt(filename_result, predict_list, delimiter=",")
    print mse
    return predict_list

predict_list = run_predict() 
