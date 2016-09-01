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
#include <time.h>
//0.00000001
using namespace std;

////////////////////////////////////////////////////////////////////////////////////
void Parse();
vector <Point> vY;			
vector <Segment> vX;		
vector <Segment> vXPre;
vector <long double> vDarkXNext;
long double yyNext = -1;
string s = "EPC16U88.TOP.ctu3";
bool bOnce = 0;
long int iTP = 0;
//TOM edit
vector <Point> buffer;
vector <int> to_insert;
vector <long int> vYT;

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
	{		iNowLine++;
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
	clock_t t;
	t = clock();

	if(argc > 1)
		s = argv[1];

	fstream  ofile;
	if(argc > 1)
	{
		string s2 = s;
		s2.erase(s.rfind("."));
		s2 += ".nod2";
		//printf("%s\n",s2);
		//system("pause");
		//string sFile = argv[2];
		ofile.open(s2.c_str(),ios::out);
	
	}


	
	
	cout.setf( std::ios::fixed, std:: ios::floatfield );
	cout.precision(10);
	ofile.setf( std::ios::fixed, std:: ios::floatfield );
	ofile.precision(10);
	ofile<<"                                           "<<endl;
	//cout<<"for Viewer"<<endl;
	Parse();  

	InitialY(vY);

	
	while(vY.size() > 0)
	{
		//InitialY(vY);     //Sorting
		long double dTop = vY[vY.size() - 1].y;
		//printf("%f\n",dTop);
		//cout<<dTop<<"	"<<vX.size()<<endl;
		int iTemp = vY.size() - 1;
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
				vY.erase(vY.begin() + iTemp + 1,vY.end());
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
			iTemp--;
		}
//////////把相同y軸的所有點  加到vX //////////
		 
		
		

		
//////////輸出//////////
		vector <Segment> Test2;  //目前 (event y座標) 以上的所有segment集合 (要拿來輸出的)
		for(int i = vX.size() - 1 ; i >= 0 ; i-- )
		{
			if(dTop != vX[i].y)
			{			
				if(vX[i].iCase == 1)
				{
					Segment STemp;
					STemp = vX[i];
					STemp.Ty = dTop;
					STemp.Tx = vX[i].a * dTop + vX[i].b;	
					

					Test2.push_back(STemp);
					

					if(dTop <= vX[i].Ty)
						vX.erase(vX.begin() + i);	//若到達終點 則把此segment從vX中移除
					else
					{
						vX[i].y = dTop;
						vX[i].x = STemp.Tx;	
					}
				}
				else	//如果是弧線
				{
					Segment STemp;
					STemp = vX[i];
					STemp.Ty = dTop;
					

					long double c,r = vX[i].r;
					/*r = (vX[i].x - vX[i].Rx) * (vX[i].x - vX[i].Rx) + (vX[i].y - vX[i].Ry) * (vX[i].y - vX[i].Ry);
					r = sqrt(r);*/
					c = (r * r) - ( (dTop - vX[i].Ry) * (dTop - vX[i].Ry) );		// X - Rx = +- sqrt(c)
					c = fabs(c);
					c = sqrt(c);

					if(vX[i].iCase == 2)
						STemp.Tx = vX[i].Rx + c;	
					else
						STemp.Tx = vX[i].Rx - c;


					
					Test2.push_back(STemp);


					if(dTop <= vX[i].Ty)
						vX.erase(vX.begin() + i);	//若到達終點 則把此segment從vX中移除
					else
					{
						vX[i].y = dTop;
						vX[i].x = STemp.Tx;	
					}
				}
			}
		}

		int iPrev = _CrtSetReportMode(_CRT_ASSERT,0);
		sort(Test2.begin(),Test2.end(),MySort2);
		_CrtSetReportMode(_CRT_ASSERT,iPrev);

		vector <Segment> Test;	//目前 (event y座標) 以上的所有segment集合 (要由左往右一個一個檢查)

		int iColor = 0;
		vector <long double> vDarkXPre;
		vector <long double> vDarkXNow;
		vDarkXPre.assign(vDarkXNext.begin(), vDarkXNext.end()); 
		vDarkXNext.clear();

		long double yyPre = yyNext;
		long double yyNow = 0;
		bool bTemp = 0;
		Segment STemp;
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
				
			
			yyNext = Test2[i].Ty;
			yyNow = Test2[i].y;

			if(iColor != iColor2)			//若顏色不相同 則要輸出
			{
				
				iColor = iColor2;
				
				
				/*if(Test2[i].iCase == 1)
				{
					cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),line,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				}
				else if (Test2[i].iCase == 2)
				{
					if(fabs(Test2[i].x - Test2[i].Tx) > ROUND)
						cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),arc,("<<Test2[i].Rx<<","<<Test2[i].Ry<<"),CW,0.123,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				}
				else if (Test2[i].iCase == 3)
				{
					if(fabs(Test2[i].x - Test2[i].Tx) > ROUND)
						cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),arc,("<<Test2[i].Rx<<","<<Test2[i].Ry<<"),CCW,0.123,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				}*/


				//cout<<Test2[i].x<<"	"<<Test2[i].y<<"	"<<Test2[i].Tx<<"	"<<Test2[i].Ty<<endl;

				if(!bTemp)
				{
					STemp = Test2[i];
				}
				else
				{
					iTP++;
					ofile<<"4 "<<endl;

					if(Test2[i].iCase == 1)
						ofile<<(iTP - 1) * 4<<" 0 "<<Test2[i].x<<" "<<Test2[i].y<<endl;
					else if(Test2[i].iCase == 2)
						ofile<<(iTP - 1) * 4<<" 8 "<<Test2[i].x<<" "<<Test2[i].y<<" "<<Test2[i].Rx<<" "<<Test2[i].Ry<<" "<<Test2[i].r<<endl;
					else if(Test2[i].iCase == 3)
						ofile<<(iTP - 1) * 4<<" 9 "<<Test2[i].x<<" "<<Test2[i].y<<" "<<Test2[i].Rx<<" "<<Test2[i].Ry<<" "<<Test2[i].r<<endl;

					ofile<<(iTP - 1) * 4 + 1<<" 0 "<<Test2[i].Tx<<" "<<Test2[i].Ty<<endl;

					if(STemp.iCase == 1)
						ofile<<(iTP - 1) * 4 + 2<<" 0 "<<STemp.Tx<<" "<<STemp.Ty<<endl;
					else if(STemp.iCase == 2)
						ofile<<(iTP - 1) * 4 + 2<<" 8 "<<STemp.Tx<<" "<<STemp.Ty<<" "<<STemp.Rx<<" "<<STemp.Ry<<" "<<STemp.r<<endl;
					else if(STemp.iCase == 3)
						ofile<<(iTP - 1) * 4 + 2<<" 9 "<<STemp.Tx<<" "<<STemp.Ty<<" "<<STemp.Rx<<" "<<STemp.Ry<<" "<<STemp.r<<endl;

					ofile<<(iTP - 1) * 4 + 3<<" 0 "<<STemp.x<<" "<<STemp.y<<endl;


					ofile<<(iTP - 1) * 4 + 3<<" "<<(iTP - 1) * 4<<endl<<(iTP - 1) * 4 + 2<<" "<<(iTP - 1) * 4 + 1<<endl;
					ofile<<endl;
				}
			

				
				vDarkXNext.push_back(Test2[i].Tx);				
				vDarkXNow.push_back(Test2[i].x);				

				bTemp = !bTemp;
			}			
		}
		vYT.push_back(iTP - 1);
		////處理水平線////
		/*if(yyPre == yyNow)
		{
			for(int i = 0 ; i < vDarkXPre.size() ; i++)
			{
				vDarkXNow.push_back(vDarkXPre[i]);
			}
			sort(vDarkXNow.begin(),vDarkXNow.end());

			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				if(vDarkXNow[k] != vDarkXNow[k + 1])
					cout<<"Draw 9 dark {("<<vDarkXNow[k]<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1]<<","<<yyNow<<")} [1]"<<endl;
			}
		}
		else if (yyPre != yyNow)
		{
			for(int k = 0 ; k < vDarkXPre.size() ; k = k + 2)
			{
				cout<<"Draw 9 dark {("<<vDarkXPre[k]<<","<<yyPre<<"),line,("<<vDarkXPre[k + 1]<<","<<yyPre<<")} [1]"<<endl;
			}
			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				cout<<"Draw 9 dark {("<<vDarkXNow[k]<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1]<<","<<yyNow<<")} [1]"<<endl;
			}
		}
		if(vY.size() == 0 && !bOnce)
		{
			bOnce = 1;
			for(int k = 0 ; k < vDarkXNext.size() ; k = k + 2)
			{
				cout<<"Draw 9 dark {("<<vDarkXNext[k]<<","<<yyNext<<"),line,("<<vDarkXNext[k + 1]<<","<<yyNext<<")} [1]"<<endl;
			}
		}*/
		//	cout<<endl;
		////處理水平線////

//////////輸出//////////		





////////找交點////////
		iPrev = _CrtSetReportMode(_CRT_ASSERT,0);
		sort(vX.begin(),vX.end(),MySort2);
		_CrtSetReportMode(_CRT_ASSERT,iPrev);

		int a = vX.size();
		if(vX.size() > 0)
		for(int i = 0 ; i < a - 1 ; i++ )
		{		
			if(vX[i].a != vX[i + 1].a && vX[i].iCase == 1 && vX[i + 1].iCase == 1  && vX[i].Tx > vX[i + 1].Tx)	//若斜率不同		//直線
			{
				long double Da = vX[i].a - vX[i + 1].a;
				long double Db = vX[i].b - vX[i + 1].b;	//Da * y + Db = 0 => y = -Db/Da
				long double y = -Db / Da;
				
				
				if(y < vX[i].y && y < vX[i + 1].y && y >= vX[i].Ty && y >= vX[i + 1].Ty)
				{
					long double x = vX[i].a * y + vX[i].b;	
					Point PTemp;
					PTemp.x = x; PTemp.y = y;

					buffer.push_back(PTemp);	//若有交點則新增回vY
					//cout<<x<<"	"<<y<<endl;
				}				
			}
			else if (vX[i].r == 0 && vX[i+1].r != 0 && vX[i].Tx > vX[i + 1].Tx)  // i = 直線  , i+1 = 弧線    (Perfect)
			{
				long double e, f, g = 0, D;
				long double ans1_x, ans1_y, ans2_x, ans2_y;
				e = vX[i].a * vX[i].a + 1;     				
				f = 2*vX[i].b*vX[i].a - 2*vX[i+1].Rx*vX[i].a - 2*vX[i+1].Ry;
				g = pow(vX[i].b, 2) - 2*vX[i+1].Rx*vX[i].b + pow(vX[i+1].Rx, 2)+ pow(vX[i+1].Ry, 2) - vX[i+1].r*vX[i+1].r;		
				D = f * f - 4*e*g;

				long double xleft = min(vX[i+1].x,vX[i+1].Tx);
				long double xright = max(vX[i+1].x,vX[i+1].Tx);
				if (D > 0)
				{
					ans1_y = (-1*f + sqrt(D)) / (2*e);
					ans2_y = (-1*f - sqrt(D)) / (2*e);
					ans1_x = vX[i].a * ans1_y + vX[i].b;
					ans2_x = vX[i].a * ans2_y + vX[i].b;
					
						

					if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < xright && ans1_x >= xleft )
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
					else if (ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_x < xright && ans2_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
				}
				else if (D == 0)
				{
					ans1_y = (-1*f) / (2*e);
					ans1_x = vX[i].a * ans1_y + vX[i].b;
					if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < xright && ans1_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);

					}
				}

			}
			else if (vX[i].r != 0 && vX[i+1].r == 0 && vX[i].Tx > vX[i + 1].Tx)  // i = 弧線  , i+1 = 直線   (Not sure)
			{
				long double e, f, g, D;
				long double ans1_x, ans1_y, ans2_x, ans2_y;
				e = vX[i+1].a * vX[i+1].a + 1;
				f = 2*vX[i+1].b*vX[i+1].a - 2*vX[i].Rx*vX[i+1].a - 2*vX[i].Ry;
				g = pow(vX[i+1].b, 2) - 2*vX[i].Rx*vX[i+1].b + pow(vX[i].Rx, 2)+ pow(vX[i].Ry, 2) - vX[i].r*vX[i].r;
				D = f * f - 4*e*g;

				long double xleft = min(vX[i].x,vX[i].Tx);		//Why i+1  not i?
				long double xright = max(vX[i].x,vX[i].Tx);
				if (D > 0)
				{
					ans1_y = (-1*f + sqrt(D)) / (2*e);
					ans2_y = (-1*f - sqrt(D)) / (2*e);
					ans1_x = vX[i+1].a * ans1_y + vX[i+1].b;
					ans2_x = vX[i+1].a * ans2_y + vX[i+1].b;
				

					if (ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < xright && ans1_x >= xleft )
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
					else if (ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_x < xright && ans2_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
					}
				}
				else if (D == 0)
				{
					ans1_y = (-1*f) / (2*e);
					ans1_x = vX[i+1].a * ans1_y + vX[i+1].b;
					if (ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < xright && ans1_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans1_x;
						PTemp.y = ans1_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);

					}
				}
			}
			else if (vX[i].r != 0 && vX[i+1].r != 0 && vX[i].Tx > vX[i + 1].Tx)  // i = 弧線  , i+1 = 弧線    (Not sure)
			{
				
				long double m, k;
				long double e, f, g, D;
				long double ans1_x, ans1_y, ans2_x, ans2_y;
				if (vX[i].Rx != vX[i+1].Rx )
				{
					m = ( vX[i].Ry - vX[i+1].Ry ) / ( vX[i+1].Rx - vX[i].Rx );
					k = ( pow(vX[i].r, 2) - pow(vX[i+1].r, 2) - pow(vX[i].Rx, 2) + pow(vX[i+1].Rx, 2) - pow(vX[i].Ry, 2) + pow(vX[i+1].Ry, 2) ) / (2 * (vX[i+1].Rx - vX[i].Rx ) );
					e = 1 + m*m;
					f = 2 * ( k*m - vX[i+1].Rx*m - vX[i+1].Ry);
					g = k*k - 2*vX[i+1].Rx*k + vX[i+1].Rx*vX[i+1].Rx + vX[i+1].Ry*vX[i+1].Ry - vX[i+1].r* vX[i+1].r;
					D = f*f - 4*e*g;
					if (D>0)
					{
						
						ans1_y = (-1*f + sqrt(D)) / (2*e);
						ans2_y = (-1*f - sqrt(D)) / (2*e);
						ans1_x = m * ans1_y + k;
						ans2_x = m * ans2_y + k;
						//printf ("D:%Lf\n", (-1*f + sqrt(D)) / (2*e));
						//printf ("ans1_x:%Lf   ans1_y:%Lf\n", ans1_x, ans1_y);
						//printf ("ans2_x:%Lf   ans2_y:%Lf\n", ans2_x, ans2_y);
						long double x1_left = min(vX[i].x,vX[i].Tx);
						long double x1_right = max(vX[i].x,vX[i].Tx);
						long double x2_left = min(vX[i+1].x,vX[i+1].Tx);
						long double x2_right = max(vX[i+1].x,vX[i+1].Tx);

						if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < x1_right && ans1_x >= x1_left && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < x2_right && ans1_x >= x2_left)
						{
							Point PTemp;
							PTemp.x = ans1_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
						else if (ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_x < x1_right && ans2_x >= x1_left && ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_x < x2_right && ans2_x >= x2_left)
						{
							Point PTemp;
							PTemp.x = ans2_x;
							PTemp.y = ans2_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
					}
					else if (D==0)
					{
						ans1_y = (-1*f) / (2*e);
						ans1_x = m * ans1_y + k;
						if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < vX[i].x && ans1_x >= vX[i].Tx && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < vX[i+1].x && ans1_x >= vX[i+1].Tx)
						{
							Point PTemp;
							PTemp.x = ans1_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
					}
				}
				else if (vX[i].Rx == vX[i+1].Rx )
				{
					long double ans1_y, ans1_x, ans2_x;
					long double e, f, g, D;
					ans1_y = ( pow(vX[i].r, 2) - pow(vX[i+1].r, 2) - pow(vX[i].Ry, 2) + pow(vX[i+1].Ry, 2) ) / 2*(vX[i+1].Ry - vX[i].Ry);
					e = 1;
					f = -2*vX[i].Rx;
					g = pow(vX[i].Rx, 2) + pow(ans1_y, 2) - 2*vX[i].Ry*ans1_y + pow(vX[i].Ry, 2) - pow(vX[i].r, 2);
					D = f*f - 4*e*g;
					if (D>0)
					{
						ans1_x = (-1*f + sqrt(D)) / (2*e);
						ans2_x = (-1*f - sqrt(D)) / (2*e);
						if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < vX[i].x && ans1_x >= vX[i].Tx && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < vX[i+1].x && ans1_x >= vX[i+1].Tx)
						{
							Point PTemp;
							PTemp.x = ans1_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
						else if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans2_x < vX[i].x && ans2_x >= vX[i].Tx && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans2_x < vX[i+1].x && ans2_x >= vX[i+1].Tx)
						{
							Point PTemp;
							PTemp.x = ans2_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
					}
					else if (D==0)
					{
						ans1_x = (-1*f) / (2*e);
						if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans1_x < vX[i].x && ans1_x >= vX[i].Tx && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans1_x < vX[i+1].x && ans1_x >= vX[i+1].Tx)
						{
							Point PTemp;
							PTemp.x = ans1_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						}
					}
				}
			}
		}

		//TOM edit
		sort(buffer.begin(),buffer.end(),MySort1);

		int index = vY.size() - 1;
			
		//printf ("to_insert  size : %d\n", to_insert.size());
		for (int i = buffer.size() - 1 ; i >= 0; i--){
			for (int j = index ; j >= 0 ; j--){
				if (buffer[i].y > vY[j].y )
				{					
					index = j;
					to_insert.push_back(j);
					break;
				}
				else if  (buffer[i].y == vY[j].y )
				{
					if (buffer[i].x < vY[j].x )
					{
						//insert into vY at j
						//vY.insert(vY.begin()+j, buffer[i] );
						index = j;
						to_insert.push_back(j);
						break;
					}
				}
			}
		}
		//printf ("Buffer  size : %d\n", buffer.size());
		//printf ("to_insert  size : %d\n", to_insert.size());
		

		iTemp = 0;
		if(to_insert.size() > 0)
		{
			/*if(to_insert[0] == 775 )
			{
				int a; 
				a=2;
			}*/
			for (int i = to_insert.size() - 1; i >= 0 ; i-- )
			{
				vY.insert(vY.begin()+to_insert[i] + iTemp +  1, buffer[iTemp]);
				iTemp++;
			}
		}
		/*vector <Point> vY2;		
		for(int i = 0 ; i < vY.size() ; i++)
		{
			vY2.push_back(vY[i]);
		}*/
	
		/*for (int i = 0; i < buffer.size(); i++){
			vY.push_back(buffer[i]);
		}*/
		//sort(vY.begin(),vY.end(),MySort1);

		/*for(int i = 0 ; i < vY.size() ; i++)
		{
			if(vY[i].x != vY2[i].x || vY[i].y != vY2[i].y)
			{
				int a; 
				a = 2 ;
			}
		}*/

		buffer.clear();
		to_insert.clear();
////////找交點////////		
					
		
	}
	
	ofile<<";YT";
	int	iTemp = 0;
	for(int i = 0 ; i < vYT.size() ; i++)
	{
		for(iTemp; iTemp <= vYT[i] ; iTemp++)
		{
			ofile<<iTemp<<" ";
		}
		ofile<<endl;
	}

	ofile<<endl;
	ofile<<";YB";

	iTemp = 0;
	for(int i = 0 ; i < vYT.size() ; i++)
	{
		for(iTemp; iTemp <= vYT[i] ; iTemp++)
		{
			ofile<<iTemp<<" ";
		}
		ofile<<endl;
	}

	ofile.seekg(0);
	ofile<<iTP;
	//system("pause");
	cout<<endl<<"RunTime : "<<clock()-t<<"ms"<<endl;
	system("pause");
	return 0;
}
