#include "Functions.h"
bool MySort1 (Point lhs, Point rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	if(lhs.y == rhs.y)
		return lhs.x < rhs.x;
	return lhs.y > rhs.y ; 
}
