// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <math.h>

int user_data;

float distance(const pcl::PointXYZ& p1, const pcl::PointXYZ& p2)
{
	float fdistancesquare = (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z);
	return sqrt(fdistancesquare);
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
	//pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGBA>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::io::loadPCDFile ("pcd_world.pcd", *cloud);
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

	/////////////////////////////////////////////////////////
	pcl::PointXYZ EndEffector;
	EndEffector.x = -1.1;
	EndEffector.y = 2.3;
	EndEffector.z = 0.7;

/*
	pcl::PointXYZ TestPoint;
	TestPoint.x = 0.3;
	TestPoint.y = -1.8;
	TestPoint.z = -1.6;

	float fdistance = distance(EndEffector, TestPoint);

	printf("This distance between EndEffector and TestPoint: %f\n", fdistance);
	
	while(1);
*/

	for (size_t i = 0; i < cloud->points.size(); ++i)
	{
		float fdistance = distance(EndEffector, cloud->points[i]);
		printf("This distance between EndEffector and the Point: %f\n", fdistance);
	}
	return 0;
}