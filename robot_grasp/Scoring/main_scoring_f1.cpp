// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <math.h>

#define DISTANCE_TOLERANCE 1.0

int user_data;

inline float getDistance(const pcl::PointXYZ& p1, const pcl::PointXYZ& p2)  //tested
{
	float fdffx = p1.x-p2.x;
	float fdffy = p1.y-p2.y;
	float fdffz = p1.z-p2.z;
	float fdistancesquare = fdffx*fdffx + fdffy*fdffy + fdffz*fdffz; 
	return std::sqrt(fdistancesquare);
}

inline float getNorm(const pcl::PointXYZ& p) //tested
{
	float fnormsquare = p.x*p.x + p.y*p.y + p.z*p.z; 
	return std::sqrt(fnormsquare);
}

//p1 is projected onto p2
inline float getProjection(const pcl::PointXYZ& p1, const pcl::PointXYZ& p2) //tested
{
	float fdotproduct = p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	float fnorm_p2 = getNorm(p2);
	return fdotproduct/fnorm_p2;
}

void viewerOneOff (pcl::visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor (1.0, 0.5, 1.0);
	pcl::PointXYZ o;
	o.x = 1.0;
	o.y = 0;
	o.z = 0;
	viewer.addSphere (o, 0.25, "sphere", 0);
	std::cout << "i only run once" << std::endl;

}

void viewerPsycho (pcl::visualization::PCLVisualizer& viewer)
{
	static unsigned count = 0;
	std::stringstream ss;
	ss << "Once per viewer loop: " << count++;
	viewer.removeShape ("text", 0);
	viewer.addText (ss.str(), 200, 300, "text", 0);

	//FIXME: possible race condition here:
	user_data++;
}

int main ()
{
	//printf("sqrt(100.) = %f\n", std::sqrt(100.));

	//while(1);

	//pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::io::loadPCDFile ("pcd_world_filtered.pcd", *cloud_xyz);
	
/*
	pcl::visualization::CloudViewer viewer("Cloud Viewer");

	//blocks until the cloud is actually rendered
	viewer.showCloud(cloud);

	//use the following functions to get access to the underlying more advanced/powerful
	//PCLVisualizer

	//This will only get called once
	viewer.runOnVisualizationThreadOnce (viewerOneOff);

	//This will get called once per visualization iteration
	viewer.runOnVisualizationThread (viewerPsycho);
	
	while (!viewer.wasStopped ())
	{
		//you can also do cool processing here
		//FIXME: Note that this is running in a separate thread from viewerPsycho
		//and you should guardagainst race conditions yourself...
		user_data++;
	}
*/
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
	//pcl::PointCloud<pcl::PointXYZRGB> cloud_xyzrgb;
	cloud_xyzrgb->width    = cloud_xyz->points.size();
	cloud_xyzrgb->height   = 1;
	cloud_xyzrgb->is_dense = true;
	cloud_xyzrgb->points.resize (cloud_xyzrgb->width * cloud_xyzrgb->height);
	
	/////////////////////////////////////////////////////////
	float fAccScore = 0;  //accumulated score

	//Final configuration
	pcl::PointXYZ EndEffector;
	pcl::PointXYZ Finger1;
	pcl::PointXYZ Finger2;
	pcl::PointXYZ Finger3;

	EndEffector.x = 0.564744*1000.;
	EndEffector.y = 0.137319*1000.;
	EndEffector.z = 0.805052*1000.;

	Finger1.x = 0.679353*1000.;
	Finger1.y = 0.041904*1000.;
	Finger1.z = 0.793459*1000.;

	Finger2.x = 0.667991*1000.;
	Finger2.y = 0.0532665*1000.;
	Finger2.z = 0.744004*1000.;

	Finger3.x = 0.564935*1000.;
	Finger3.y = 0.156322*1000.;
	Finger3.z = 0.818697*1000.;

	pcl::PointXYZ FingerCenter;
	FingerCenter.x = (Finger1.x + Finger2.x + Finger3.x)/3.;
	FingerCenter.y = (Finger1.y + Finger2.y + Finger3.y)/3.;
	FingerCenter.z = (Finger1.z + Finger2.z + Finger3.z)/3.;

	///Base vector
	pcl::PointXYZ BaseVector;
	BaseVector.x = FingerCenter.x - EndEffector.x;
	BaseVector.y = FingerCenter.y - EndEffector.y;
	BaseVector.z = FingerCenter.z - EndEffector.z;

	float fNorm_BaseVector = getNorm(BaseVector);
	float fRadius = DISTANCE_TOLERANCE*getDistance(FingerCenter, EndEffector);
	
	printf("Base vector: %f,  %f,  %f\n", BaseVector.x, BaseVector.y, BaseVector.z);
	printf("Norm of base vector: %f mm\n", fNorm_BaseVector);
	printf("Radius: %f mm\n", fRadius);

	/*
	pcl::PointXYZ TestProjectionVector;
	TestProjectionVector.x = 0.241904*1000.;
	TestProjectionVector.y = -0.518697*1000.;
	TestProjectionVector.z = -0.379353*1000.;
	float fPorj_test = getProjection(TestProjectionVector, BaseVector); //TestVector projects onto BaseVector
	printf("Projection result: %f \n", fPorj_test);
	*/

	//while(1);

	///Test vector
	pcl::PointXYZ TestVector;
	long inum_valid_point = 0;

	for (size_t i = 0; i < cloud_xyz->points.size(); ++i)
	{
		cloud_xyzrgb->points[i].x = cloud_xyz->points[i].x;
		cloud_xyzrgb->points[i].y = cloud_xyz->points[i].y;
		cloud_xyzrgb->points[i].z = cloud_xyz->points[i].z;
		cloud_xyzrgb->points[i].r = 255;
		cloud_xyzrgb->points[i].g = 255;
		cloud_xyzrgb->points[i].b = 255;


		float fD = getDistance(cloud_xyz->points[i], EndEffector);
		//printf("fD: %f mm\n", fD);
		
		if( fD > fRadius )
		{
			continue;
		}
		else
		{
			TestVector.x = cloud_xyz->points[i].x - FingerCenter.x;
			TestVector.y = cloud_xyz->points[i].y - FingerCenter.y;
			TestVector.z = cloud_xyz->points[i].z - FingerCenter.z;

			float fPorjection = getProjection(TestVector, BaseVector); //TestVector projects onto BaseVector
			if( fPorjection < 0 )
			{
				inum_valid_point++;
				
				fAccScore += (-fPorjection);

				cloud_xyzrgb->points[i].r = 0;
				cloud_xyzrgb->points[i].g = 0;
			}
			else
			{ }
		}
	}

	printf("Score: %f\n", fAccScore);
	printf("Number of valid points: %d\n", inum_valid_point);

	pcl::visualization::CloudViewer viewer("Cloud Viewer");

	//blocks until the cloud is actually rendered
	viewer.showCloud(cloud_xyzrgb);

	//use the following functions to get access to the underlying more advanced/powerful
	//PCLVisualizer

	//This will only get called once
	viewer.runOnVisualizationThreadOnce (viewerOneOff);

	//This will get called once per visualization iteration
	viewer.runOnVisualizationThread (viewerPsycho);

	while (!viewer.wasStopped ())
	{
		//you can also do cool processing here
		//FIXME: Note that this is running in a separate thread from viewerPsycho
		//and you should guardagainst race conditions yourself...
		user_data++;
	}
	return 0;
}