#include "Functions.h"
#include <algorithm>
#include <math.h>

bool MySort1 (Point lhs, Point rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	if(lhs.y == rhs.y)
		return lhs.x > rhs.x;
	return lhs.y < rhs.y ; 
}
bool MySort2 (Segment lhs, Segment rhs)
{ 	
	//return lhs.dScore > rhs.dScore ; 
	//左右 都是弧線
	if (fabs(lhs.x - rhs.x) < ROUND && lhs.iCase != 1 && rhs.iCase != 1)
	{ 
		long double r_a, r_b, r_c, r_m;
		long double l_a, l_b, l_c, l_m;
		r_a = rhs.x - rhs.Rx;
		r_b = rhs.y - rhs.Ry;
		r_c = (rhs.r * rhs.r) + (r_a * rhs.Rx) + (r_b * rhs.Ry);

		l_a = lhs.x - lhs.Rx;
		l_b = lhs.y - lhs.Ry;
		l_c = (lhs.r * lhs.r) + (l_a * lhs.Rx) + (l_b * lhs.Ry);

		if ( (l_a == 0) || (r_a == 0) ) 
		{
			if(rhs.Tx < lhs.Tx)
				return 0;
			else if (rhs.Tx > lhs.x)
				return 1;
		}
		else 
		{
			l_m =  (-1 * l_b) / l_a;
			r_m =  (-1 * r_b) / r_a;

			return l_m > r_m;
		}	


	}


	//左邊是直線  右邊是弧線 (Perfect)
	if(fabs(lhs.x - rhs.x) < ROUND && lhs.iCase == 1 && rhs.iCase != 1)  
	{
		// ax + by = c
		long double a, b, c, m;
		a = rhs.x - rhs.Rx;
		b = rhs.y - rhs.Ry;
		c = (rhs.r * rhs.r) + (a * rhs.Rx) + (b * rhs.Ry);
		if ( a != 0)
		{
			m = (-1 * b) / a;
			rhs.a = m;
			if(lhs.a == m)
			{
				return lhs.Tx < rhs.Tx ; 
			}
			return lhs.a > rhs.a;//rhs.Tx ;			
		}
		else
		{
			if(rhs.Tx < rhs.x)
				return 0;
			else if (rhs.Tx > rhs.x)
				return 1;
		}


	}

	//左邊是弧線  右邊是直線 (Not sure)					
	if (fabs(lhs.x - rhs.x) < ROUND && lhs.iCase != 1 && rhs.iCase==1)
	{
		long double a, b, c, m;
		a = lhs.x - lhs.Rx;
		b = lhs.y - lhs.Ry;
		c = (lhs.r * lhs.r) + (a * lhs.Rx) + (b * lhs.Ry);
		if ( a != 0)
		{
			m = (-1 * b) / a;
			lhs.a = m;
			if(rhs.a == m)
			{
				return rhs.Tx < lhs.Tx ; 
			}
			//return rhs.a < lhs.a;//rhs.Tx ;					//(?)
			return lhs.a > rhs.a;
		}
		else
		{
			if(lhs.Tx < lhs.x)
				return 0;
			else if (lhs.Tx > lhs.x)
				return 1;
		}
	}

	if(fabs(lhs.x - rhs.x) < ROUND && lhs.iCase == 1 && rhs.iCase == 1)
		return lhs.a > rhs.a;
	return lhs.x < rhs.x ; 
}

bool MySort3 (Segment lhs, Segment rhs)
{
	return lhs.iNumber > rhs.iNumber ; 
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
				p.vS[i].r = 0;
			}
			break;
		case 2:
			{
				p.vS[i].r = sqrt ((p.vS[i].x-p.vS[i].Rx)*(p.vS[i].x-p.vS[i].Rx)+(p.vS[i].y-p.vS[i].Ry)*(p.vS[i].y-p.vS[i].Ry));
				p.vS[i].a = 0;
				p.vS[i].b = 0;
			}
			break;
		case 3:
			{

				p.vS[i].r = sqrt ((p.vS[i].x-p.vS[i].Rx)*(p.vS[i].x-p.vS[i].Rx)+(p.vS[i].y-p.vS[i].Ry)*(p.vS[i].y-p.vS[i].Ry));
				p.vS[i].a = 0;
				p.vS[i].b = 0;

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
