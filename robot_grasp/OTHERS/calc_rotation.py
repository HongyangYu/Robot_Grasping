import numpy as np
import tf
num_hand = 10

def calc_rotation( hand_pos, target_pos ):
    hand_pos = np.array( hand_pos )
    target_pos = np.array( target_pos )
    hand_z_axis_wld = target_pos - hand_pos
    #The rotation from hand_init to world_frame
    rot_h2w = calc_init_rot( hand_z_axis_wld, hand_pos )
    #The rotation from hand_init to hand_final
    rot_h2h = np.eye(4)
    agl = 2 * np.pi / num_hand
    rot_addition = tf.transformations.euler_matrix(0., 0., agl)
    quats = []
    for i in range(num_hand):
        #update rotation angle every time
        rot_h2h = np.dot( rot_addition, rot_h2h )
        #Rotation from hand_final to world_frame
        rot_tmp = np.dot( rot_h2w, rot_h2h )
        quat_tmp = tf.transformations.quaternion_from_matrix(rot_tmp)    
        quats.append(quat_tmp)
    return quats
    

def calc_init_rot( z_axis, hand_pos ):
    y_axis = [z_axis[1], -z_axis[0], 0.]
    x_axis = np.cross( y_axis, z_axis )
    x_axis = x_axis / np.linalg.norm( x_axis )
    y_axis = y_axis / np.linalg.norm( y_axis )
    z_axis = z_axis / np.linalg.norm( z_axis )
    x_axis = np.append(x_axis, 0.)
    y_axis = np.append(y_axis, 0.)
    z_axis = np.append(z_axis, 0.)
    rot = np.array( [x_axis, y_axis, z_axis,[0., 0., 0., 1.]] ).T
    return rot
