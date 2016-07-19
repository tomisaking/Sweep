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

////////////////////////////////////////////////////////////////////////////////////



void Parse()
{
	fstream fin;
	char temp[256]="test.gbr.ctu3";
	fin.open(temp,ios::in);
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
				/*	for(int i = 0 ; i < vPTemp.size() ; i++)
					{
						cout<<vPTemp[i].x<<"	"<<vPTemp[i].y<<"	";
						cout<<endl;
						for(int j = 0 ; j < vPTemp[i].vS.size() ; j++)
						{
							cout<<vPTemp[i].vS[j].Tx<<"	"<<vPTemp[i].vS[j].Ty<<"	";
						}
						cout<<endl;
						cout<<endl;

					}
					cout<<"////////////////////////////////////"<<endl;*/
				}
			}				
		}
	}	
}


int main(int argc, char** argv) 
{
	
	cout.setf( std::ios::fixed, std:: ios::floatfield );
	cout.precision(10);
	cout<<"for Viewer"<<endl;
	Parse();  
	//InitialY(vY);
	
	/*for(int i = 0 ; i < vY.size() ; i++) 
	{
		cout<<vY[i].x<<"	"<<vY[i].y<<endl;
		for(int j = 0 ; j < vY[i].vS.size() ; j++)
		{
			cout<<vY[i].vS[j].Tx<<"	"<<vY[i].vS[j].Ty<<"	"<<endl<<vY[i].vS[j].a<<"	"<<vY[i].vS[j].b<<"	";
		}
		cout<<endl;
		cout<<endl;
	}*/
	
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
		//long double TopofEndofY = 0;	//所有segments終點中最高的值
		vector <Segment> Test2;  //目前 (event y座標) 以上的所有segment集合
		
		for(int i = vX.size() - 1 ; i >= 0 ; i-- )
		{
			//TopofEndofY = max(TopofEndofY,vX[i].Ty);

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
		for(int i = 0 ; i < Test2.size() ; i++ )
		{		
			bool b = 0;
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
				cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),line,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				//cout<<Test2[i].x<<"	"<<Test2[i].y<<"	"<<Test2[i].Tx<<"	"<<Test2[i].Ty<<endl;
			}			
		}
		//cout<<endl;
//////////輸出//////////		


////////找交點////////
		sort(vX.begin(),vX.end(),MySort2);
		int a = vX.size();
		if(vX.size() > 0)
		for(int i = 0 ; i < a - 1 ; i++ )
		{		
			if(vX[i].a != vX[i + 1].a)	//若斜率不同 
			{
				long double Da = vX[i].a - vX[i + 1].a;
				long double Db = vX[i].b - vX[i + 1].b;	//Da * y + Db = 0 => y = -Db/Da
				long double y = -Db / Da;
				
				
				if(y < vX[i].y && y < vX[i + 1].y && y >= vX[i].Ty && y >= vX[i + 1].Ty)
				{
					long double x = vX[i].a * y + vX[i].b;	
					Point PTemp;
					PTemp.x = x; PTemp.y = y;
					vY.push_back(PTemp);
					//cout<<x<<"	"<<y<<endl;
				}
				/*if((y < vX[i].y) && (y < vX[i + 1].y) && (y >= vX[i].Ty) && (y >= vX[i + 1].Ty) && (y >= TopofEndofY))
				{
					long double x = vX[i].a * y + vX[i].b;					
													
					
					vX[i].x = x ; vX[i].y = y;
					vX[i + 1].x = x ; vX[i + 1].y = y;
					
					
					cout<<x<<"	"<<vX[i].y<<"	"<<TopofEndofY<<endl;
					if(vX.size() == 0)
						break;
				}*/
			}
		}				
////////找交點////////		
					
		/*sort(vX.begin(),vX.end(),MySort2);
		for(int i = 0 ; i < vX.size() ; i++) 	
		{
			cout<<vX[i].x<<"	"<<vX[i].y<<"	"<<vX[i].Tx<<"	"<<vX[i].Ty<<endl;
		}
		cout<<endl;		*/
		//cout<<vY.size()<<endl;
		//vX.clear();
	}
	
	
	
	
	//b.vS[0].iCase = 3;
	//cout<<b.vS[0].iCase;
	system("pause");
	return 0;
}
