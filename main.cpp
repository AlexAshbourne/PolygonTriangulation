#include <iostream>
#include <vector>
#include "DblyConnectedEdgeList.h"
#include "EdgeSearchTree.h"
#include "MonotonePolygon.h"
#include "MonotoneTriangulation.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

std::vector<double> ReadCSV(std::ifstream &file)
{
	std::vector<double> Data;
	while(!file.eof())
		{
			double a, b, c;
			char ch;
			file >> a >> ch >> b >> ch >> c;

			Data.push_back(b);
			Data.push_back(a);
		};
	return Data;
};

int main()
{
	std::ifstream infile("LakeOntario.txt");

	std::vector<double> TestPoly1_vec = ReadCSV(infile);
	std::reverse(TestPoly1_vec.begin(), TestPoly1_vec.end());

	infile.close();
	// Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	MonotonePolygon TestPoly1_Monotone(TestPoly1_vec);

	DblyConnectedEdgeList* test123 = TestPoly1_Monotone.getDCEL();

	std::ofstream outputFileLake;
	outputFileLake.open("LakeBoundary.lin");
	test123->WriteToFile(outputFileLake);
	outputFileLake.close();

	//Create a monotone subdivision of this polygon
	TestPoly1_Monotone.MakeMonotone();

	DblyConnectedEdgeList* LakeMonotone = TestPoly1_Monotone.getDCEL();

	std::ofstream outputFileMono;
	outputFileMono.open("LakeMonotone.lin");
	LakeMonotone->WriteToFile(outputFileMono);
	outputFileMono.close();

	// Initialize a triangulation from this monotone polygon
	MonotoneTriangulation TestPoly1_Triangulation(TestPoly1_Monotone);

	// Triangulate this polygon
	TestPoly1_Triangulation.TriangulatePolygon();

	DblyConnectedEdgeList* LakeTriangle = TestPoly1_Triangulation.getDCEL();

	std::ofstream outputFileTri;
	outputFileTri.open("LakeTrianglulation.lin");
	LakeTriangle->WriteToFile(outputFileTri);
	outputFileTri.close();
	std::cout<< "end of program \n" << std::endl;

		//_CrtDumpMemoryLeaks();
	return 0;

};