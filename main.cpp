#include <iostream>
#include <vector>
#include "FTIDblyConnectedEdgeList.h"
#include "FTIEdgeSearchTree.h"
#include "FTIMonotonePolygon.h"
#include "FTIMonotoneTriangulation.h"

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
	// Test Polygon
	//{
	// //double TestPoly1[30] = {10.0, 10.0, 8.0, 8.0, 6.0, 12.0, 4.0, 11.0, 3.0, 12.0, 1.0, 10.0, 2.5, 8.5, 2.0, 7.0, 0.5, 8.0, 0.0, 6.0, 2.0, 3.0, 3.0, 4.0, 7.0, 0.0, 5.0, 7.0, 9.0, 5.0};
	// 	double TestPoly1[80] = {7,31, 4,28, 9,24, 8,20, 4,23, 0,21, 1,16, 7,18, 7,14, 2,11,
	// 						2,5, 6,5, 4,7, 7,12, 9,6, 8,2, 5,0, 17,0, 12,6, 16,4,
	// 						12,9, 19,12, 16,7, 21,7, 21,22, 17,25, 13,20, 16,17, 18,19, 16,21, 
	// 						20,21, 19,15, 17,13, 12,15, 11,20, 16,27, 12,26, 14,30, 11,30, 10,28};
	// //// Test Polygon
	// //double TestPoly1[6] = {10.0, 10.0, 8.0, 8.0, 9.0, 5.0};

	// // Convert it into vector form
	// std::vector<double> TestPoly1_vec(TestPoly1, TestPoly1 + sizeof(TestPoly1)/sizeof(TestPoly1[0]));

	// // Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	// FTIMonotonePolygon TestPoly1_Monotone(TestPoly1_vec);

	// // Create a monotone subdivision of this polygon
	// TestPoly1_Monotone.MakeMonotone();

	// FTIDblyConnectedEdgeList* test123 = TestPoly1_Monotone.getDCEL();

	// std::ofstream outputFile;
	// outputFile.open("MonotoneTest1.lin");
	// test123->WriteToFile(outputFile);
	// outputFile.close();
	// // Initialize a triangulation from this monotone polygon
	// FTIMonotoneTriangulation TestPoly1_Triangulation(TestPoly1_Monotone);

	// // Triangulate this polygon
	// TestPoly1_Triangulation.TriangulatePolygon();

	// FTIDblyConnectedEdgeList* test1234 = TestPoly1_Triangulation.getDCEL();

	// std::ofstream outputFile2;
	// outputFile2.open("TriangulationTest1.lin");
	// test1234->WriteToFile(outputFile2);
	// outputFile2.close();

	//_CrtDumpMemoryLeaks();
	//};
	//// Second Test polygon
	//double TestPoly2[30] = {5.0, 16.0, 0.0, 12.0, 4.0, 13.0, 2.0, 8.0, 1.0, 6.0, 2.0, 3.0, 6.0, 0.0, 11.0, 2.0, 6.0, 4.0, 9.0, 10.0, 10.0, 6.0, 13.0, 9.0, 11.0, 11.0, 12.0, 14.0, 10.0, 17.0};

	//// Convert it into vector form
	//std::vector<double> TestPoly2_vec(TestPoly2, TestPoly2 + sizeof(TestPoly2)/sizeof(TestPoly2[0]));

	//// Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	//FTIMonotonePolygon TestPoly2_Monotone(TestPoly2_vec);

	//// Create a monotone subdivision of this polygon
	//TestPoly2_Monotone.MakeMonotone();

	//// Initialize a triangulation from this monotone polygon
	//FTIMonotoneTriangulation TestPoly2_Triangulation(TestPoly2_Monotone);

	//// Triangulate this polygon
	//TestPoly2_Triangulation.TriangulatePolygon();

	//// Third Test polygon
	//double TestPoly3[34] = {15,1, 13,8, 12,5, 7,10, 11,17, 15,14, 14,22, 8,20, 5,23, 6,18, 1,21, 4,15, 4,12, 1,14, 1,1, 9,2, 8,6};

	//// Convert it into vector form
	//std::vector<double> TestPoly3_vec(TestPoly3, TestPoly3 + sizeof(TestPoly3)/sizeof(TestPoly3[0]));

	//// Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	//FTIMonotonePolygon TestPoly3_Monotone(TestPoly3_vec);

	//// Create a monotone subdivision of this polygon
	//TestPoly3_Monotone.MakeMonotone();

	//// Initialize a triangulation from this monotone polygon
	//FTIMonotoneTriangulation TestPoly3_Triangulation(TestPoly3_Monotone);

	//// Triangulate this polygon
	//TestPoly3_Triangulation.TriangulatePolygon();

	//// Fourth Test polygon
	//double TestPoly4[80] = {7,31, 4,28, 9,24, 8,20, 4,23, 0,21, 1,16, 7,18, 7,14, 2,11,
	//						2,5, 6,5, 4,7, 7,12, 9,6, 8,2, 5,0, 17,0, 12,6, 16,4,
	//						12,9, 19,12, 16,7, 21,7, 21,22, 17,25, 13,20, 16,17, 18,19, 16,21, 
	//						20,21, 19,15, 17,13, 12,15, 11,20, 16,27, 12,26, 14,30, 11,30, 10,28};

	//// Convert it into vector form
	//std::vector<double> TestPoly4_vec(TestPoly4, TestPoly4 + sizeof(TestPoly4)/sizeof(TestPoly4[0]));

	//// Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	//FTIMonotonePolygon TestPoly4_Monotone(TestPoly4_vec);

	//// Create a monotone subdivision of this polygon
	//TestPoly4_Monotone.MakeMonotone();

	//// Initialize a triangulation from this monotone polygon
	//FTIMonotoneTriangulation TestPoly4_Triangulation(TestPoly4_Monotone);

	//// Triangulate this polygon
	//TestPoly4_Triangulation.TriangulatePolygon();

	std::ifstream infile("LakeOntario.txt");

	std::vector<double> TestPoly1_vec = ReadCSV(infile);
	std::reverse(TestPoly1_vec.begin(), TestPoly1_vec.end());

	infile.close();
	// Initialize a Monotone Polygon from the vector of nodes (not yet monotone)
	FTIMonotonePolygon TestPoly1_Monotone(TestPoly1_vec);

	FTIDblyConnectedEdgeList* test123 = TestPoly1_Monotone.getDCEL();

	std::ofstream outputFileLake;
	outputFileLake.open("LakeBoundary.lin");
	test123->WriteToFile(outputFileLake);
	outputFileLake.close();

	//Create a monotone subdivision of this polygon
	TestPoly1_Monotone.MakeMonotone();

	FTIDblyConnectedEdgeList* LakeMonotone = TestPoly1_Monotone.getDCEL();

	std::ofstream outputFileMono;
	outputFileMono.open("LakeMonotone.lin");
	LakeMonotone->WriteToFile(outputFileMono);
	outputFileMono.close();

	// Initialize a triangulation from this monotone polygon
	FTIMonotoneTriangulation TestPoly1_Triangulation(TestPoly1_Monotone);

	// Triangulate this polygon
	TestPoly1_Triangulation.TriangulatePolygon();

	FTIDblyConnectedEdgeList* LakeTriangle = TestPoly1_Triangulation.getDCEL();

	std::ofstream outputFileTri;
	outputFileTri.open("LakeTrianglulation.lin");
	LakeTriangle->WriteToFile(outputFileTri);
	outputFileTri.close();
	std::cout<< "end of program \n" << std::endl;

		//_CrtDumpMemoryLeaks();
	return 0;


};