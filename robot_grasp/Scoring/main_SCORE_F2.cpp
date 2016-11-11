// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <pcl/visualization/cloud_viewer.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <math.h>
#include <conio.h>
#include <algorithm>    // std::max

using namespace std;

#define PCD_FILE           "pcd_world_filtered_2.pcd"
#define INPUT_FILE_PATH    "final_configure_scene2.csv"
#define TOTAL_NUMBER_LINE  1680  //pcd1: 1670; pcd2: 1680; pcd3: 1630
#define OUTPUT_FILE_PATH_1 "score_file_scene2_original.csv"
#define OUTPUT_FILE_PATH_2 "score_file_scene2_normalize.csv"

int user_data;


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


inline void parseLine(const string& str, pcl::PointXYZ& Effec, pcl::PointXYZ& Fin1, pcl::PointXYZ& Fin2, pcl::PointXYZ& Fin3)
{
	vector<string> tokens;
	tokenize(str, tokens, ",");

	std::istringstream os_0(tokens.at(0));
	std::istringstream os_1(tokens.at(1));
	std::istringstream os_2(tokens.at(2));

	std::istringstream os_3(tokens.at(3));
	std::istringstream os_4(tokens.at(4));
	std::istringstream os_5(tokens.at(5));

	std::istringstream os_6(tokens.at(6));
	std::istringstream os_7(tokens.at(7));
	std::istringstream os_8(tokens.at(8));

	std::istringstream os_9(tokens.at(9));
	std::istringstream os_10(tokens.at(10));
	std::istringstream os_11(tokens.at(11));

	os_0 >> Effec.x;
	os_1 >> Effec.y;
	os_2 >> Effec.z;

	os_3 >> Fin1.x;
	os_4 >> Fin1.y;
	os_5 >> Fin1.z;

	os_6 >> Fin2.x;
	os_7 >> Fin2.y;
	os_8 >> Fin2.z;

	os_9 >> Fin3.x;
	os_10 >> Fin3.y;
	os_11 >> Fin3.z;

	Effec.x *= 1000.;
	Effec.y *= 1000.;
	Effec.z *= 1000.;

	Fin1.x *= 1000.;
	Fin1.y *= 1000.;
	Fin1.z *= 1000.;

	Fin2.x *= 1000.;
	Fin2.y *= 1000.;
	Fin2.z *= 1000.;

	Fin3.x *= 1000.;
	Fin3.y *= 1000.;
	Fin3.z *= 1000.;
}

/**
 * Calculate the determinant for a 4x4 matrix based on this example:
 * http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
 * This function takes four Vec4f as row vectors and calculates the resulting matrix' determinant
 * using the Laplace expansion.
 *
 */
inline const float Determinant4x4( const pcl::PointXYZ& v0,
                                   const pcl::PointXYZ& v1,
                                   const pcl::PointXYZ& v2,
                                   const pcl::PointXYZ& v3 )
{
float det = 1*v1.z*v2.y*v3.x - v0.z*1*v2.y*v3.x -
            1*v1.y*v2.z*v3.x + v0.y*1*v2.z*v3.x +
 
            v0.z*v1.y*1*v3.x - v0.y*v1.z*1*v3.x -
            1*v1.z*v2.x*v3.y + v0.z*1*v2.x*v3.y +
 
            1*v1.x*v2.z*v3.y - v0.x*1*v2.z*v3.y -
            v0.z*v1.x*1*v3.y + v0.x*v1.z*1*v3.y +
 
            1*v1.y*v2.x*v3.z - v0.y*1*v2.x*v3.z -
            1*v1.x*v2.y*v3.z + v0.x*1*v2.y*v3.z +
 
            v0.y*v1.x*1*v3.z - v0.x*v1.y*1*v3.z -
            v0.z*v1.y*v2.x*1 + v0.y*v1.z*v2.x*1 +
 
            v0.z*v1.x*v2.y*1 - v0.x*v1.z*v2.y*1 -
            v0.y*v1.x*v2.z*1 + v0.x*v1.y*v2.z*1;
    
	return det;
}
 

/* Checks whether the specified point is inside the tetrahedron (by index)
*  based on this approach: http://steve.hollasch.net/cgindex/geometry/ptintet.html
*  @return true if the point_ is inside the tetrahedron (or on one of the four triangles), otherwise false
*  @param point_ the Vec3f point to be checked
*  @param tetra_ the tetrahedron
*/
inline const bool CheckPointInTetra(const pcl::PointXYZ& v0,
									const pcl::PointXYZ& v1,
									const pcl::PointXYZ& v2,
									const pcl::PointXYZ& v3,
									const pcl::PointXYZ& p0 )
{
    const float det0 = Determinant4x4(v0, v1, v2, v3);
    const float det1 = Determinant4x4(p0, v1, v2, v3);
    const float det2 = Determinant4x4(v0, p0, v2, v3);
    const float det3 = Determinant4x4(v0, v1, p0, v3);
    const float det4 = Determinant4x4(v0, v1, v2, p0);

	//printf("det0: %f, det1: %f, det2: %f, det3: %f, det4: %f\n", det0, det1, det2, det3, det4);

    /**
    If by chance the Determinant det0 is 0, then your tetrahedron is degenerate (the points are coplanar).
    If any other Di=0, then P lies on boundary i (boundary i being that boundary formed by the three points other than Vi).
    If the sign of any Di differs from that of D0 then P is outside boundary i.
    If the sign of any Di equals that of D0 then P is inside boundary i.
    If P is inside all 4 boundaries, then it is inside the tetrahedron.
    As a check, it must be that D0 = D1+D2+D3+D4.
    The pattern here should be clear; the computations can be extended to simplicies of any dimension. (The 2D and 3D case are the triangle and the tetrahedron).
    If it is meaningful to you, the quantities bi = Di/D0 are the usual barycentric coordinates.
    Comparing signs of Di and D0 is only a check that P and Vi are on the same side of boundary i.
    */
    if (det0 != 0)
    {
        if (det0 < 0)
        {
            if ((det1 < 0) && (det2 < 0) && (det3 < 0) && (det4 < 0))
            {
                return true;
            }
        }
        if (det0 > 0)
        {
            if ((det1 > 0) && (det2 > 0) && (det3 > 0) && (det4 > 0))
            {
                return true;
            }
        }
    }

    return false;
}

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
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::io::loadPCDFile(PCD_FILE, *cloud_xyz);
	
	/*
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
	cloud_xyzrgb->width    = cloud_xyz->points.size();
	cloud_xyzrgb->height   = 1;
	cloud_xyzrgb->is_dense = true;
	cloud_xyzrgb->points.resize (cloud_xyzrgb->width * cloud_xyzrgb->height);
	*/

	/////////////////////////////////////////////////////////
	char pDataPath[100];
	sprintf_s(pDataPath,"%s", INPUT_FILE_PATH);

	ifstream  file(pDataPath);
	ofstream  scorefile;
	scorefile.open(OUTPUT_FILE_PATH_1);
	
	string line;

	//Final configuration
	pcl::PointXYZ EndEffector;
	pcl::PointXYZ Finger1;
	pcl::PointXYZ Finger2;
	pcl::PointXYZ Finger3;

	long iMaxIndex = 0;
	float fMaxAccScore = 0.;
	for(size_t iLineIndex = 0; iLineIndex < TOTAL_NUMBER_LINE; ++iLineIndex)
	{
		getline(file, line);

		parseLine( line, EndEffector, Finger1, Finger2, Finger3 );
		/*
		cout << EndEffector.x << ", " << EndEffector.y << ", " << EndEffector.z << endl;
		cout << Finger1.x << ", " << Finger1.y << ", " << Finger1.z << endl;
		cout << Finger2.x << ", " << Finger2.y << ", " << Finger2.z << endl;
		cout << Finger3.x << ", " << Finger3.y << ", " << Finger3.z << endl << endl;
		getch();
		*/

		///FingerCenter
		pcl::PointXYZ FingerCenter;
		FingerCenter.x = (Finger1.x + Finger2.x + Finger3.x)/3.;
		FingerCenter.y = (Finger1.y + Finger2.y + Finger3.y)/3.;
		FingerCenter.z = (Finger1.z + Finger2.z + Finger3.z)/3.;
		//cout << "Finger_Center:" << FingerCenter.x << ", " << FingerCenter.y << ", " << FingerCenter.z << endl;
		//getch();

		///Base vector
		pcl::PointXYZ BaseVector;
		BaseVector.x = FingerCenter.x - EndEffector.x;
		BaseVector.y = FingerCenter.y - EndEffector.y;
		BaseVector.z = FingerCenter.z - EndEffector.z;
		//cout << "Base_Vector:" << BaseVector.x << ", " << BaseVector.y << ", " << BaseVector.z << endl;
		//getch();

		///Test vector
		pcl::PointXYZ TestVector;
		
		float fAccScore = 0.;  //accumulated score
		long inum_valid_point = 0;
		for (size_t i = 0; i < cloud_xyz->points.size(); ++i)
		{
			/*
			cloud_xyzrgb->points[i].x = cloud_xyz->points[i].x;
			cloud_xyzrgb->points[i].y = cloud_xyz->points[i].y;
			cloud_xyzrgb->points[i].z = cloud_xyz->points[i].z;
			cloud_xyzrgb->points[i].r = 255;
			cloud_xyzrgb->points[i].g = 255;
			cloud_xyzrgb->points[i].b = 255;
			*/

			if( CheckPointInTetra(Finger1, Finger2, Finger3, EndEffector, cloud_xyz->points[i]) == true )
			{
				TestVector.x = cloud_xyz->points[i].x - FingerCenter.x;
				TestVector.y = cloud_xyz->points[i].y - FingerCenter.y;
				TestVector.z = cloud_xyz->points[i].z - FingerCenter.z;

				float fPorjection = getProjection(TestVector, BaseVector); //TestVector projects onto BaseVector
				fAccScore += (-fPorjection);

				/*
				cloud_xyzrgb->points[i].r = 0;
				cloud_xyzrgb->points[i].g = 0;
				*/

				inum_valid_point++;
			}
			else
			{ }
		}

		//printf("Score: %f\n", fAccScore);
		//printf("Number of valid points: %d\n", inum_valid_point);
		scorefile << fAccScore << endl;

		if ( fAccScore > fMaxAccScore )
		{
			fMaxAccScore = fAccScore;
			iMaxIndex = iLineIndex+1;
		} 
		else
		{ }
		
		/*
		//visualization
		pcl::visualization::CloudViewer viewer("Cloud Viewer");
		viewer.showCloud(cloud_xyzrgb);
		viewer.runOnVisualizationThreadOnce (viewerOneOff);
		viewer.runOnVisualizationThread (viewerPsycho);
		
		while (!viewer.wasStopped ())
		{
			user_data++;
		}
		*/
	}

	scorefile.close();

	printf("The max score is: %f\n", fMaxAccScore);
	printf("The line index is: %ld\n", iMaxIndex);

	///////////////////////////////////////////////////
	//Score Normalization
	///////////////////////////////////////////////////
	ifstream  scorefile_original(OUTPUT_FILE_PATH_1);
	ofstream  scorefile_normalize(OUTPUT_FILE_PATH_2);
	
	float fAccScore = 0.;
	float fAccScore_normalize = 0.;
	for(size_t iLineIndex = 0; iLineIndex < TOTAL_NUMBER_LINE; ++iLineIndex)
	{
		getline(scorefile_original, line);
		std::istringstream is(line);
		std::ostringstream ss;
		
		is >> fAccScore;
		fAccScore = fAccScore >= 0.? fAccScore : 0.;
		fAccScore_normalize = fAccScore*1.0/fMaxAccScore;
		//printf("fAccScore_normalize: %f\n", fAccScore_normalize);

		ss << fAccScore_normalize;
		scorefile_normalize << ss.str() << endl;
	}

	scorefile.close();
	scorefile_normalize.close();

	printf("Finished normalization.\n");

	while(1);
	return 0;
}