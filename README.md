# Robot_Grasping

Predicting score of robotic grasping

In this project, we designed an algorithm to score results of grasping objects for a robotic end effector and to predict the score using machine learning and pointcloud. And we are given pointcloud data of some cluttered scenes containing several individual objects on a tabletop. The main steps are as follows:

STEP 1: Generate initial hand configurations. Generate a large number of random initial hand configurations from each image to run them through the simulator. 
STEP 2: Run simulator to get final hand configurations. Run the simulator whose input is initial configurations and RGBD images, and output is final hand configurations.
STEP 3: Get scores by scoring function. Use scoring function to get scores according to final hand configurations and RGBD images that input to simulator. 
STEP 4: Extract features. Extract features on RGBD images and initial hand configurations.
STEP 5: Train Neural Network. Input extracted features and corresponding scores as labels to train the Neural Network. 
STEP 6: Predict score. After finishing training the neural network, predict score from new test initial configurations and RGBD images.

