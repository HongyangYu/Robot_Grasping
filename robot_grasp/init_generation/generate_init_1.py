import numpy as np
import yaml
import calc_rotation as calc_R
import csv
width = 0.3
step_size = 0.03
dist = [0.2, 0.21]
obj_pos = [0.71075491,  0.18023076,  0.78478167,  0.69974217, -0.34939717, -0.34933946,  0.5159888]

#Need to do
#1. change the obj_pos to a random point in the pointcloud.

def main():
    #yaml file is used for simulation
    fh = open( 'init_configure_scene1.yaml', 'w' )
    #csv file is used for generate depth images from different view points
    fh_csv = open( 'init_configure_scene1.csv', 'w')
    csv_writer = csv.writer(fh_csv)
    #get object translation and quaternion
    trans_obj = obj_pos[0:3]
    quat_obj = obj_pos[3:7]
    #define the space that we choose initial config from
    x_min = trans_obj[0] - width
    x_max = trans_obj[0] + width
    y_min = trans_obj[1] - width
    y_max = trans_obj[1] + width
    z_min = trans_obj[2] - width
    z_max = trans_obj[2] + width
    init_configure = []
 
    for x in np.arange( x_min, x_max, step_size ):
        for y in np.arange( y_min, y_max, step_size):
            for z in np.arange( z_min, z_max, step_size):
            #eliminate pts that are within range 0.2m to 0.3m
                distance = np.linalg.norm([ x - trans_obj[0], y - trans_obj[1], z - trans_obj[2]])
                if  distance < dist[0] or distance > dist[1]:
                    continue
                #calculate quaternions for each hand location
                quats = calc_R.calc_rotation([x, y, z], trans_obj)
                for quat in quats:
                    tmp_pos = [float(x),float(y),float(z)]
                    tmp_cong = np.append(tmp_pos,quat)
                    tmp_cong = tmp_cong.tolist()
                    #write to csv file
                    csv_writer.writerow(tmp_cong)
                    #write to yaml file
                    fh.write('-\n')
                    fh.write('  relative_config: '+str(tmp_cong)+'\n' )
                    
                    fh.write('  grasp_mode:  2\n')
                    fh.write('  release_mode:  3\n')
    fh_csv.close()
    fh.close()

if __name__ == "__main__":
    main()



          

