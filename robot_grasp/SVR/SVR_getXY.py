# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 16:45:08 2016

@author: Henry
"""

import scipy.misc
import numpy as np
import ConvertArray

def getFeature(img_name, img_format, img_range):
    X = []
    for file_index in img_range:
        im_2D = scipy.misc.imread( img_name+str(file_index)+img_format ,True)
        im_1D = ConvertArray.convertTo1D(im_2D)
        X.append( im_1D )
    return X

def saveFeature_try(scene_num, img_bit):
    img_name = './img_data/scene'+str(scene_num)+'/img_'+str(img_bit)+'/img_'+str(img_bit)+'_'
    img_format = '.png'
    range_start = 1001  # 1001
    range_end = 1631 # 1631
    img_range = range(range_start,range_end)  # 1631
    
    feature_list_np = getFeature(img_name, img_format, img_range)
    print np.shape(feature_list_np)
    save_arr_filename = './score/scene'+str(scene_num)+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)+'_range_'+str(range_start)+'_'+str(range_end)
    np.save(save_arr_filename,feature_list_np)


def saveFeature(scene_num, img_bit, file_num):
    img_name = './img_data/scene'+str(scene_num)+'/img_'+str(img_bit)+'/img_'+str(img_bit)+'_'
    img_format = '.png'
    img_range = range(1,file_num+1)  # 1631
    
    feature_list_np = np.array( getFeature(img_name, img_format, img_range) )
    print 'size of feature', np.shape(feature_list_np)
    save_arr_filename = './score/scene'+str(scene_num)+'/feature_scene'+str(scene_num)+'_img_'+str(img_bit)
    np.save(save_arr_filename,feature_list_np)



def run_save_feature():
    scene_num_list = range(1,4)
    file_num_list = [1670, 1680, 1630]
    img_bit = 32 # [32,64,128]
    for scene_num in scene_num_list:
        saveFeature(scene_num, img_bit, file_num_list[scene_num-1])
        print '======   saved feature of scene_'+str(scene_num)+'_img_'+ str(img_bit)+ '  ========'



def getScore(scene_num):
    filename = './score/scene'+str(scene_num)+'/score_file_scene'+str(scene_num)+'_normalize_ave_target.csv'
    score_list = []
    with open(filename, 'rb') as f:
        for line in f:
            score_list.append(float(line))
    score_list_np = np.array(score_list)
    return score_list_np

def run_save_score():
    scene_num_list = range(1,4)
    for scene_num in scene_num_list:
        score_list_np = getScore(scene_num)
        save_arr_filename = './score/scene'+str(scene_num)+'/score_file_scene'+str(scene_num)
        np.save(save_arr_filename, score_list_np)
        print '======   saved score of scene_'+str(scene_num)+' len '+str(len(score_list_np))+'  ========'


run_save_feature()
print ''
run_save_score()

