import tf
import numpy as np
def camera_to_world( quat_obj, trans_obj ):
    quat_cam = tf.transformations.quaternion_from_euler(1.57, 3.14, 1.57)
    trans_cam = np.array([0, 0, 0.8])
    trans_obj = np.array(trans_obj)
    quat_res = tf.transformations.quaternion_multiply(quat_cam, quat_obj)
    rot_obj = tf.transformations.quaternion_matrix(quat_cam)
    rot_obj = rot_obj[:3,:3]
    rot_obj_t = rot_obj.T
    trans_res = trans_obj.T.dot(rot_obj_t)
    trans_res = trans_res + trans_cam
    result = np.append(trans_res, quat_res)
    return result
