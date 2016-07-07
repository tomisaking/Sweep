#include "Functions.h"
bool MySort1 (Point lhs, Point rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	if(lhs.y == rhs.y)
		return lhs.x < rhs.x;
	return lhs.y > rhs.y ; 
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


