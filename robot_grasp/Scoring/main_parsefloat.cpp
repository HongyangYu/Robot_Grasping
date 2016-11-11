// CSV2PCD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define FILE_PATH "pts_camera_frame.csv"


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


void parseLine(const string& str)
{
	double Xc;
	double Yc;
	double Zc;

	vector<string> tokens;

	tokenize(str, tokens, ",");
	cout << tokens.at(0) << endl;
	cout << tokens.at(1) << endl;
	cout << tokens.at(2) << endl;
	
	istringstream os_Xc(tokens.at(0));
	istringstream os_Yc(tokens.at(1));
	istringstream os_Zc(tokens.at(2));
		
	os_Xc >> Xc;
	os_Yc >> Yc;
	os_Zc >> Zc;

	cout << Xc << endl;
	cout << Yc << endl;
	cout << Zc << endl;
	cout << endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
	char pDataPath[100];
	sprintf_s(pDataPath,"%s", FILE_PATH);


	ifstream  file(pDataPath);

	//CPoint* pPoint = new CPoint[iPOINT_NUMBER];
	//CPoint* pPoint_backup = pPoint;

	string line;

	for(unsigned long cnt_line = 0; cnt_line < 5; cnt_line++ )
	{
		
		getline(file, line);

		parseLine( line );
	}

	while(1);
	return 0;
}

