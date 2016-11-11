import tf
import csv
import numpy as np
import scipy.misc as misc
P_Mat = np.array([[1.0518949921399060e+03, 0., 9.6220390244513135e+02, 0. ],
                  [0., 1.0601924519557133e+03, 5.3516598291171215e+02, 0. ],
                  [0., 0., 1, 0.], [0., 0., 0., 1.]])
img_rows, img_cols = (1080, 1920)
img_width = 32
def main():
    init_config_file = open( 'init_configure_scene3.csv','r' )
    ptcloud_file = open( 'world_frame_pts_filtered_3.csv','r' )
    color_file = open( 'camera_frame_color_3.csv','r' )
    csv_reader = csv.reader( init_config_file, delimiter=',' )
    ptcloud = csv.reader( ptcloud_file, delimiter=',' )
    color = csv.reader( color_file, delimiter=',' )
    
    pts = np.array( list( ptcloud ) )
    pts = pts.astype( 'float' )
    num_pts = len( pts )
    pts_4d = np.append( pts, np.ones( [num_pts, 1] ), axis = 1 )
    
    colors = np.array( list(color) )
    colors = colors.astype('float')
    colors = colors.astype( np.uint8 )
    
    counter = 1
    dataset = []
    for row in csv_reader:
        trans_w = np.array([ float(v) for v in row[:3] ]) * 1e+3
        quat_h2w = np.array([ float(v) for v in row[3:] ])
        T_h2w = tf.transformations.quaternion_matrix( quat_h2w )
        T_h2w = np.array( T_h2w )
        #add the trans to fourth column
        T_h2w[0:3,3] = trans_w.T
        T_w2h = np.linalg.inv( T_h2w )
        pts_h = T_w2h.dot( pts_4d.T )
        depth_img = np.zeros( (img_rows, img_cols), dtype = np.uint16 )
        color_img = np.zeros( (img_rows, img_cols, 3), dtype = np.uint8 )
        img_pts = P_Mat.dot( pts_h )
        img_pts = img_pts.T
        print 'Saving img{}'.format(counter)
        for pts, cl in zip(img_pts, colors):
            x,y,z,w = pts
            x = int(x / z)
            y = int(y / z)
            if (x not in range(0, img_cols)) or (y not in range(0, img_rows)):
                continue
            if depth_img[y, x] == 0 or z < depth_img[y, x]:
                depth_img[y, x] = int(z)
                color_img[y, x] = cl
        
        filename_depth = 'imgs/depth_{}.png'.format(counter)
        filename_color = 'imgs/color_{}.png'.format(counter)
        tmp_img_color = misc.toimage( color_img )
        tmp_img_depth = misc.toimage( depth_img)
        
        color_resize = misc.imresize(tmp_img_color,(img_width, img_width))
        depth_resize = misc.imresize(tmp_img_depth,(img_width, img_width))
        
        #Can use following lines to save images.
        misc.imsave( filename_color, color_resize )
        misc.imsave( filename_depth, depth_resize )
        
        counter += 1
if __name__ == "__main__":
    main()
