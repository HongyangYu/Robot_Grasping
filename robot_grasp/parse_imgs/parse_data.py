import scipy.misc as misc
import numpy as np
import re
import os
from max_pooling import maxdownsample2d
path = '../scene_5/imgs'


#folder_l = './img_128'
#folder_m = './img_64'
folder_s = './scene5'

suffix = '.png'

#width_l = 128
#width_m = 64
width_s = 32

def main():

    files = os.listdir( path )
    #dataset_l = []
    #dataset_m = []
    dataset_s = []
    print 'Saving images...'
    for fh in files:
        if fh.endswith( suffix ):
            counter = re.findall('[0-9]+', fh)
            counter = counter[0]
            
            filename_orig = os.path.join( path, fh )
            img_orig = misc.imread(filename_orig)
            #img_l = misc.imresize(img_orig,( width_l, width_l) )
            #img_m = misc.imresize(img_orig,( width_m, width_m) )
            #img_s = misc.imresize(img_orig,( width_s, width_s) )
            
            #filename_l = 'img_128_' + counter + suffix
            #filename_l = os.path.join(folder_l, filename_l)
            #misc.imsave( filename_l, img_l )
            
            #filename_m = 'img_64_' + counter + suffix
            #filename_mmaxdownsample2d = os.path.join(folder_m, filename_m)
            #misc.imsave( filename_m, img_m )
            img_tmp = img_orig[110:959,490:1437]
            #misc.imshow(img_tmp)
            img_s = maxdownsample2d( img_tmp,26,29 )
            #print img_s.shape
            filename_s = 'img_32_' + counter + suffix
            filename_s = os.path.join(folder_s, filename_s)
            misc.imsave( filename_s, img_s )
            
            #data_l = []
            #data_l.append( img_l )
            #dataset_l.append( data_l )
            
            #data_m = []
            #data_m.append( img_m )
            #dataset_m.append( data_m )
            
            data_s = []
            data_s.append( img_s )
            dataset_s.append( data_s )
        else:
            continue
    print 'Saving dataset...'
    #dataset_l = np.array(dataset_l)
    #dataset_m = np.array(dataset_m)
    dataset_s = np.array(dataset_s)
    
    #np.save( 'img_128_scene_1', dataset_l )
    #np.save( 'img_64_scene_1', dataset_m )
    np.save( 'img_32_scene_5', dataset_s )
        
if __name__ == '__main__':
    main()    
        
        
        
