#include <vector>

using namespace std;

struct Segment
{
	int iCase;		// 1:Line  2:CW  3:CCW 4: Horizontal
	int iColor;		// 1:Dark  0:Clear
	long int iNumber;
	long int iNumber2;        
	long double x;		//From x
	long double y;		//From y
	long double Tx;		//Target x
	long double Ty;		//Target y
	long double Rx;		//Center x
	long double Ry;		//Center y
	
	long double a,b;	// x = ay + b
	long double D;

	//long double e,f,g;  // ey^2 + fy + g = 0;
	long double r; // radius


	//vector <long double> vCrossX;
	//vector <long double> vCrossY;

	bool bdirection;	// Up : 1
};
struct Point
{	
	long double x;
	long double y;
	vector <Segment> vS;	
};
struct Y
{	
	long double x;
	long double y;
	vector <Segment> vS;	
};
struct PointforYTYB
{	
	long double XT;
	long double XB;
	long double YT;
	long double YB;
	long int iIndex;
}; 	 


struct ForHorizontal
{
	long double x;
	long int iNumber;
};