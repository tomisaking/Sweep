#include <iostream>
#include <fstream>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <queue> 
#include <stack> 
#include <time.h>
#include <algorithm>
#include <cmath>
#include "Functions.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////
void Parse();
vector <Point> vY;			
vector <Segment> vX;		
vector <long double> vDarkXNext;
long double yyNext = -1;
string s = "arc_test.ctu3";
////////////////////////////////////////////////////////////////////////////////////



void Parse()
{
	fstream fin;
	//char temp[256]="test.gbr.ctu3";
	fin.open(s.c_str(),ios::in);
	char line[25600];
	int iNowLine = 1;
	fin.getline(line,sizeof(line),'\n');
	while(fin.getline(line,sizeof(line),'\n'))
	{
		iNowLine++;
		if(iNowLine > 1)
		{
			string str = line;
			int iTemp = 0;
			long double x = 0 , y = 0;
			char p[256];
			Point PTemp;
			vector <Point> vPTemp;
			Segment STemp;
			
			iTemp = str.find("dark");			
			if(iTemp > 0)			
				STemp.iColor = 1;
			else
				STemp.iColor = 0;

			STemp.iNumber = iNowLine;
			
			str.erase(0,str.find("(")+1);
			sscanf(str.c_str(),"%Lf%c%Lf",&x,p,&y);
			PTemp.x = x;
			PTemp.y = y;
					
			
			str.erase(0,str.find(")")+2);
			
			iTemp = str.find(" ");	
			while(iTemp != 0)	
			{
 				if(str.find("arc") == 0)
				{
					str.erase(0,5);
					sscanf(str.c_str(),"%Lf%c%Lf",&x,p,&y);
					STemp.Rx = x;
					STemp.Ry = y;
					
					str.erase(0,str.find(")")+2);
					if(str.find("CW") == 0)
					{
						STemp.iCase = 2;
						
						str.erase(0,str.find("(")+1);
						sscanf(str.c_str(),"%Lf%c%Lf",&x,p,&y);
						STemp.Tx = x;
						STemp.Ty = y;
					}
					else if(str.find("CCW") == 0)
					{
						STemp.iCase = 3;
						
						str.erase(0,str.find("(")+1);
						sscanf(str.c_str(),"%Lf%c%Lf",&x,p,&y);
						STemp.Tx = x;
						STemp.Ty = y;					
					}
				}
				if(str.find("line") == 0)
				{
					STemp.iCase = 1;
					
					str.erase(0,str.find("(")+1);
					sscanf(str.c_str(),"%Lf%c%Lf",&x,p,&y);
					STemp.Tx = x;
					STemp.Ty = y;					
				}
				
				
				
				if(STemp.Ty < PTemp.y)
				{				
					STemp.x = PTemp.x;
					STemp.y = PTemp.y;
					PTemp.vS.push_back(STemp);
				}
				vPTemp.push_back(PTemp);			
								
				PTemp = vPTemp[vPTemp.size() - 1];
				PTemp.vS.clear();
				swap(PTemp.x,STemp.Tx);
				swap(PTemp.y,STemp.Ty);
				
				if(STemp.iCase == 2)
					STemp.iCase = 3;
				else if(STemp.iCase == 3)
					STemp.iCase = 2;
				if(STemp.Ty < PTemp.y)
				{				
					STemp.x = PTemp.x;
					STemp.y = PTemp.y;
					PTemp.vS.push_back(STemp);
				}
				
				str.erase(0,str.find(")")+2);
				iTemp = str.find(" ");	
				if(iTemp == 0)
				{					
					for(int i = 0 ; i < PTemp.vS.size() ; i++)
					{
						vPTemp[0].vS.push_back(PTemp.vS[i]);
					}
					for(int i = 0 ; i < vPTemp.size() ; i++)
					{
						vY.push_back(vPTemp[i]);
					}
				}
			}				
		}
	}	
}


int main(int argc, char** argv) 
{
	if(argc > 1)
		s = argv[1];
	cout.setf( std::ios::fixed, std:: ios::floatfield );
	cout.precision(10);
	cout<<"for Viewer"<<endl;
	Parse();  
	
	
	
	while(vY.size() > 0)
	{
		InitialY(vY);
		long double dTop = vY[0].y;
		int iTemp = 0;
//////////把相同y軸的所有點  加到vX //////////
		for(;;)
		{		
			if(iTemp >= vY.size())
			{
				vY.clear();
				break;
			}
			if(vY[iTemp].y != dTop)
			{		
				vY.erase(vY.begin(),vY.begin()+iTemp);
				break;
			}
			else if(vY[iTemp].y == dTop)
			{
				/*for(int i = vX.size() - 1 ; i >= 0 ; i-- )
				{
					if(vY[iTemp].x == vX[i].Tx && vY[iTemp].y == vX[i].Ty)
					{						
						vX.erase(vX.begin() + i);
						//break;
					}
				}*/
				for(int i = 0 ; i < vY[iTemp].vS.size() ; i++)
					vX.push_back(vY[iTemp].vS[i]);
			}
			iTemp++;
		}
//////////把相同y軸的所有點  加到vX //////////
		 
		
		

		
//////////輸出//////////
		vector <Segment> Test2;  //目前 (event y座標) 以上的所有segment集合
		for(int i = vX.size() - 1 ; i >= 0 ; i-- )
		{
			if(dTop != vX[i].y)
			{							
				Segment STemp;
				STemp = vX[i];
				STemp.Ty = dTop;
				STemp.Tx = vX[i].a * dTop + vX[i].b;
				

				Test2.push_back(STemp);
				
				vX[i].y = dTop;
				vX[i].x = vX[i].a * vX[i].y + vX[i].b;	
				
				if(dTop <= vX[i].Ty)
					vX.erase(vX.begin() + i);	//若到達終點 則把此segment從vX中移除
			}
		}

		sort(Test2.begin(),Test2.end(),MySort2);
		vector <Segment> Test;	//目前 (event y座標) 以上的所有segment集合 (要由左往右一個一個檢查)

		int iColor = 0;
		vector <long double> vDarkXPre;
		vector <long double> vDarkXNow;
		vDarkXPre.assign(vDarkXNext.begin(), vDarkXNext.end()); 
		vDarkXNext.clear();

		long double yyPre = yyNext;
		long double yyNow = 0;
		for(int i = 0 ; i < Test2.size() ; i++ )
		{		
			bool b = 0;									//此segment出現過了沒？
			for(int j = 0 ; j < Test.size() ; j++ )		//如果在集合中發現相同編號的segment 則移除
			{
				if(Test[j].iNumber == Test2[i].iNumber)
				{
					Test.erase(Test.begin() + j);
					b = 1;
					break;
				}
			}			

			if(!b)
			{
				Test.push_back(Test2[i]);				
				sort(Test.begin(),Test.end(),MySort3);		//將所有segment 依iNumber做sort iNumber越大的代表越上層
			}
			
			int iColor2 = 0;
			if(Test.size() > 0)
				iColor2 = Test[0].iColor;		//選取最上層的顏色
				
			
			if(iColor != iColor2)			//若顏色不相同 則要輸出
			{
				iColor = iColor2;
				//cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),line,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				//cout<<Test2[i].x<<"	"<<Test2[i].y<<"	"<<Test2[i].Tx<<"	"<<Test2[i].Ty<<endl;
				yyNext = Test2[i].Ty;
				yyNow = Test2[i].y;
				vDarkXNext.push_back(Test2[i].Tx);				
				vDarkXNow.push_back(Test2[i].x);				
			}			
		}

		////處理水平線////
		if(yyPre == yyNow && vY.size() > 0)
		{
			for(int i = 0 ; i < vDarkXPre.size() ; i++)
			{
				vDarkXNow.push_back(vDarkXPre[i]);
			}
			sort(vDarkXNow.begin(),vDarkXNow.end());

			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				if(vDarkXNow[k] != vDarkXNow[k + 1])
				{
					//cout<<"Draw 9 dark {("<<vDarkXNow[k]<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1]<<","<<yyNow<<")} [1]"<<endl;
				}
			}
		}
		else if (yyPre != yyNow)
		{
			for(int k = 0 ; k < vDarkXPre.size() ; k = k + 2)
			{
				//cout<<"Draw 9 dark {("<<vDarkXPre[k]<<","<<yyPre<<"),line,("<<vDarkXPre[k + 1]<<","<<yyPre<<")} [1]"<<endl;
			}
			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				//cout<<"Draw 9 dark {("<<vDarkXNow[k]<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1]<<","<<yyNow<<")} [1]"<<endl;
			}
		}
		else if (vY.size() == 0)
		{
			for(int k = 0 ; k < vDarkXPre.size() ; k = k + 2)
			{
				//cout<<"Draw 9 dark {("<<vDarkXPre[k]<<","<<yyNext<<"),line,("<<vDarkXPre[k + 1]<<","<<yyNext<<")} [1]"<<endl;
			}
		}
		//	cout<<endl;
		////處理水平線////

//////////輸出//////////		





////////找交點////////
		sort(vX.begin(),vX.end(),MySort2);
		int a = vX.size();
		if(vX.size() > 0)
		for(int i = 0 ; i < a - 1 ; i++ )
		{		
			if(vX[i].a != vX[i + 1].a && vX[i].iCase == 1 && vX[i + 1].iCase == 1)	//若斜率不同 
			{
				long double Da = vX[i].a - vX[i + 1].a;
				long double Db = vX[i].b - vX[i + 1].b;	//Da * y + Db = 0 => y = -Db/Da
				long double y = -Db / Da;
				
				
				if(y < vX[i].y && y < vX[i + 1].y && y >= vX[i].Ty && y >= vX[i + 1].Ty)
				{
					
					long double x = vX[i].a * y + vX[i].b;	
					Point PTemp;
					PTemp.x = x; PTemp.y = y;

					vY.push_back(PTemp);	//若有交點則新增回vY
					//cout<<x<<"	"<<y<<endl;
				}				
			}
			else if (vX[i].r == 0 && vX[i+1].r != 0 )  // i = 直線  , i+1 = 弧線    (Perfect)
			{
				long double e, f, g, D;
				long double ans1_x, ans1_y, ans2_x, ans2_y;
				e = vX[i].a * vX[i].a + 1;     				
				f = 2*vX[i].b*vX[i].a - 2*vX[i+1].Rx*vX[i].a - 2*vX[i+1].Ry;
				g = pow(vX[i].b, 2) - 2*vX[i+1].Rx*vX[i].b + pow(vX[i+1].Rx, 2)+ pow(vX[i+1].Ry, 2) - vX[i+1].r*vX[i+1].r;
				D = f * f - 4*e*g;
				if (D > 0)
				{
					ans1_y = (-1*f + sqrt(D)) / (2*e);
					ans2_y = (-1*f - sqrt(D)) / (2*e);
					ans1_x = vX[i].a * ans1_y + vX[i].b;
					ans2_x = vX[i].a * ans2_y + vX[i].b;
					if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < vX[i+1].x && ans1_x >= vX[i+1].Tx)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
					else if (ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_x < vX[i+1].x && ans2_x >= vX[i+1].Tx)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
				}
				else if (D == 0)
				{
					ans1_y = (-1*f) / (2*e);
					ans1_x = vX[i].a * ans1_y + vX[i].b;
					if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < vX[i+1].x && ans1_x >= vX[i+1].Tx)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);

					}
				}

			}
			else if (vX[i].r != 0 && vX[i+1].r == 0 )  // i = 弧線  , i+1 = 直線   (Not sure)
			{
				long double e, f, g, D;
				long double ans1_x, ans1_y, ans2_x, ans2_y;
				e = vX[i+1].a * vX[i+1].a + 1;
				f = 2*vX[i+1].b*vX[i+1].a - 2*vX[i].Rx*vX[i+1].a - 2*vX[i].Ry;
				g = pow(vX[i+1].b, 2) - 2*vX[i].Rx*vX[i+1].b + pow(vX[i].Rx, 2)+ pow(vX[i].Ry, 2) - vX[i].r*vX[i].r;
				D = f * f - 4*e*g;
				if (D > 0)
				{
					ans1_y = (-1*f + sqrt(D)) / (2*e);
					ans2_y = (-1*f - sqrt(D)) / (2*e);
					ans1_x = vX[i+1].a * ans1_y + vX[i+1].b;
					ans2_x = vX[i+1].a * ans2_y + vX[i+1].b;
					if (ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < vX[i].x && ans1_x >= vX[i].Tx)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
					else if (ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_x < vX[i].x && ans2_x >= vX[i].Tx)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
				}
				else if (D == 0)
				{
					ans1_y = (-1*f) / (2*e);
					ans1_x = vX[i+1].a * ans1_y + vX[i+1].b;
					if (ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < vX[i].x && ans1_x >= vX[i].Tx)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						vY.push_back(PTemp);
						printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);

					}
				}


			}
		}				
////////找交點////////		
					
		
	}
	system("pause");
	return 0;
}
