import numpy as np
from scipy import misc
import c2w

P_mat = np.array([[1.0518949921399060e+03, 0., 9.6220390244513135e+02 ],
                  [0., 1.0601924519557133e+03, 5.3516598291171215e+02 ],
                  [0., 0., 1]])

img_rgb = misc.imread( 'scene5_color.png' )
img_depth = misc.imread( 'scene5_depth.png' )
rows, cols = img_depth.shape
img_pts = []
img_color = []
for i in range( rows ):
    for j in range( cols ):
        if abs(img_depth[i, j]-0) > 0.5 and img_depth[i, j] < 1.5e+03:
            d = img_depth[i, j]
            img_pts.append([j * d, i * d, d])
            img_color.append(img_rgb[i, j])
img_pts = np.array(img_pts)
img_pts_t = np.transpose(img_pts)
m_3dpts = np.dot(np.linalg.inv( P_mat ), img_pts_t)
#m_3dpts = np.dot(P_mat, img_pts)
m_3dpts_t = np.transpose( m_3dpts )
np.savetxt("camera_frame_pts_filtered_5.csv", m_3dpts_t, delimiter=",")
np.savetxt("camera_frame_color_5.csv", img_color, delimiter = ',')
m_wld_pts = []
for i in range(len(m_3dpts_t)):
    tmp_pt = c2w.camera_to_world( [0.,0.,0.,1.], m_3dpts_t[i]/1000. )
    tmp_pt = tmp_pt * 1000
    m_wld_pts.append( tmp_pt[ 0:3 ] )
m_wld_pts = np.array( m_wld_pts )
np.savetxt("world_frame_pts_filtered_5.csv", m_wld_pts, delimiter=",")


