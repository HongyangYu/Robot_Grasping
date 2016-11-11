// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>

#define PCD_FILE  "pcd_world_filtered_4.pcd"
#define TARGET_CENTER_X  710.0  //4: 710.0; 5: 626.2589605
#define TARGET_CENTER_Y  133.0  //4: 133.0; 5: 319.19109535
#define TARGET_CENTER_Z  780.0  //4: 780.0; 5: 806.01325436


int user_data;

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
	pcl::io::loadPCDFile (PCD_FILE, *cloud);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
	cloud_xyzrgb->width    = cloud->points.size() + 1;  // the additional 1: center of the target
	cloud_xyzrgb->height   = 1;
	cloud_xyzrgb->is_dense = true;
	cloud_xyzrgb->points.resize (cloud_xyzrgb->width * cloud_xyzrgb->height);

	for (size_t i = 0; i < cloud->points.size(); ++i)
	{
		cloud_xyzrgb->points[i].x = cloud->points[i].x;
		cloud_xyzrgb->points[i].y = cloud->points[i].y;
		cloud_xyzrgb->points[i].z = cloud->points[i].z;
		cloud_xyzrgb->points[i].r = 255;
		cloud_xyzrgb->points[i].g = 255;
		cloud_xyzrgb->points[i].b = 255;

		//cloud_xyzrgb->points[i].r = 0;
		//cloud_xyzrgb->points[i].g = 0;
	}

	cloud_xyzrgb->points[cloud->points.size()].x = TARGET_CENTER_X;
	cloud_xyzrgb->points[cloud->points.size()].y = TARGET_CENTER_Y;
	cloud_xyzrgb->points[cloud->points.size()].z = TARGET_CENTER_Z;
	cloud_xyzrgb->points[cloud->points.size()].r = 0;
	cloud_xyzrgb->points[cloud->points.size()].g = 0;
	cloud_xyzrgb->points[cloud->points.size()].b = 255;


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
		//and you should guard against race conditions yourself...
		user_data++;
	}
	return 0;
}