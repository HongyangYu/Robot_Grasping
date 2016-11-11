# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 16:26:47 2016

@author: Henry
"""

import scipy.misc
import numpy as np

def convertTo1D(im_raw):
    im = np.array(im_raw)
    im_1D = im.ravel()
    return im_1D

def convertTo2D(im_raw, row, col):
    im_2D = np.reshape(im_raw, (row, col))
    return im_2D

def test():
    im_2D = scipy.misc.imread("question.png",True)
    (row,col) = np.shape(im_2D)
    
    print '========   test 2D to 1D  ============'
    print 'should be ', row*col
    im_1D = convertTo1D(im_2D)
    print 'size=', np.shape(im_1D)
    
    print '========   test 1D to 2D  ============'
    print 'should be ', (row, col)
    im_2D = convertTo2D(im_1D, row, col)
    print 'size=', np.shape(im_2D)
    scipy.misc.imsave('question_new.png',im_2D)
    print '========   Finish  ============'
    
#test()