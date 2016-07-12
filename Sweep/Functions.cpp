#include "Functions.h"
#include <algorithm>

bool MySort1 (Point lhs, Point rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	if(lhs.y == rhs.y)
		return lhs.x < rhs.x;
	return lhs.y > rhs.y ; 
}
bool MySort2 (Segment lhs, Segment rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	if(lhs.x == rhs.x)
		return lhs.a > rhs.a;
	return lhs.x < rhs.x ; 
}


bool bCross (Point L,Point R)
{
	bool r = 0;
	
	
	
	
	
	return r;
}

void FindLinearEquation(Point &p)
{
	for(int i = 0 ; i < p.vS.size() ; i++)
	{
		switch(p.vS[i].iCase)
		{
			case 1:
				{
					p.vS[i].a = (p.vS[i].x - p.vS[i].Tx) / (p.vS[i].y - p.vS[i].Ty);
					p.vS[i].b = p.vS[i].x - p.vS[i].a * p.vS[i].y;
				}
				break;
		}
	}	
}

void InitialY(vector <Point> &vY)
{
	sort(vY.begin(),vY.end(),MySort1);
	for(int i = 0 ; i < vY.size() ; i++) 
	{
		FindLinearEquation(vY[i]);	
	}
}
