#include <vector>

using namespace std;
struct Segment
{
	int iCase;		// 1:Line  2:CW  3:CCW
	int iColor;		// 1:Dark  0:Clear
	int iNumber;
	long double Tx;		//Target x
	long double Ty;		//Target y
	long double Rx;
	long double Ry;
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
struct X
{	
	long double x;
	long double y;
};
