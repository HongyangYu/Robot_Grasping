// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

using namespace std;

#define FILE_PATH "scene_1_target_fine.csv"
#define NUMBER_POINT 22116  //pcd1: 24503, 22116(fine); pcd2: 5903; pcd3: 48478
#define FILE_PCD  "pcd_world_scene1_target_fine.pcd"

void tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}


void parseLine(const string& str, float* pPoint)
{
	float fXc;
	float fYc;
	float fZc;

	vector<string> tokens;

	tokenize(str, tokens, ",");
	//cout << tokens.at(0) << endl;
	//cout << tokens.at(1) << endl;
	//cout << tokens.at(2) << endl;

	istringstream os_Xc(tokens.at(0));
	istringstream os_Yc(tokens.at(1));
	istringstream os_Zc(tokens.at(2));

	os_Xc >> fXc;
	os_Yc >> fYc;
	os_Zc >> fZc;

	//cout << fXc << endl;
	//cout << fYc << endl;
	//cout << fZc << endl;
	//cout << endl;

	*(pPoint+0) = fXc;
	*(pPoint+1) = fYc;
	*(pPoint+2) = fZc;
}


int main (int argc, char** argv)
{
	char pDataPath[100];
	sprintf_s(pDataPath,"%s", FILE_PATH);

	ifstream  file(pDataPath);
	string line;

	pcl::PointCloud<pcl::PointXYZ> cloud;
	cloud.width    = NUMBER_POINT;
	cloud.height   = 1;
	cloud.is_dense = true;
	cloud.points.resize (cloud.width * cloud.height);

	float* pPoint = new float[3];

	for(size_t i = 0; i < cloud.points.size (); ++i)
	{
		getline(file, line);

		parseLine( line, pPoint );

		cloud.points[i].x = pPoint[0];
		cloud.points[i].y = pPoint[1];
		cloud.points[i].z = pPoint[2];
	}

	pcl::io::savePCDFileASCII (FILE_PCD, cloud);
	std::cerr << "Saved " << cloud.points.size () << " data points to test_pcd.pcd." << std::endl;

	/*
	for (size_t i = 0; i < cloud.points.size (); ++i)
		std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;
	*/

	delete[] pPoint;
	return (0);
}