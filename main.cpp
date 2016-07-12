#include <iostream>
#include <fstream>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <queue> 
#include <time.h>
#include <algorithm>
#include <cmath>
#include "Functions.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////
void Parse();
vector <Point> vY;
vector <Segment> vX;
vector <int> viTemp;


////////////////////////////////////////////////////////////////////////////////////



void Parse()
{
	fstream fin;
	char temp[256]="for_merge.ctu3";
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
			
					
				int a; 
				a=2;
			}				
		}
	}	
}


int main(int argc, char** argv) 
{

	cout.setf( std::ios::fixed, std:: ios::floatfield );
	cout.precision(10);
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
		double dTop = vY[0].y;
		int iTemp = 0;
		//把相同y軸的所有點  加到vX 
		for(;;)
		{
		
			if(vY[iTemp].y != dTop || iTemp >= vY.size())
			{
				vY.erase(vY.begin(),vY.begin()+iTemp);
				break;
			}
			else if(vY[iTemp].y == dTop)
			{
				for(int i = 0 ; i < vX.size() ; i++ )
				{
					if(vY[iTemp].x == vX[i].Tx && vY[iTemp].y == vX[i].Ty)
					{
						vX.erase(vX.begin() + i);
						break;
					}
				}
				for(int i = 0 ; i < vY[iTemp].vS.size() ; i++)
					vX.push_back(vY[iTemp].vS[i]);
			}
			iTemp++;
		}
		//把相同y軸的所有點  加到vX
		 
		sort(vX.begin(),vX.end(),MySort2);
		viTemp.clear();
		//找交點 
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
					Segment STemp;
					
					PTemp.x = x; PTemp.y = y;
					STemp.x = x; STemp.y = y;
					
					STemp.Tx = vX[i].Tx; STemp.Ty = vX[i].Ty;
					STemp.a = vX[i].a; STemp.b = vX[i].b; 
					if(vX[i].Ty != y)			
						vX.push_back(STemp);
					
					
					STemp.Tx = vX[i + 1].Tx; STemp.Ty = vX[i + 1].Ty;
					STemp.a = vX[i + 1].a; STemp.b = vX[i + 1].b; 
					if(vX[i + 1].Ty != y)
						vX.push_back(STemp);								
						
					viTemp.push_back(i);
					//vX.erase(vX.begin() + i , vX.begin() + i + 2);			
						
					cout<<x<<"	"<<y<<endl;
					if(vX.size() == 0)
						break;
				}
			}
		}		
		//找交點
		for(int i = viTemp.size() -1 ; i >= 0 ; i-- )
		{			
			vX.erase(vX.begin() + viTemp[i], vX.begin() + viTemp[i] + 2);
		}
		 
	/*	for(int i = 0 ; i < vX.size() ; i++)
		{
			cout<<vX[i].x<<"	"<<vX[i].y<<"	"<<vX[i].Tx<<"	"<<vX[i].Ty<<endl;
		}		
		cout<<vY.size()<<endl;*/
		//vX.clear();
	}
	
	
	
	
	//b.vS[0].iCase = 3;
	//cout<<b.vS[0].iCase;
	return 0;
}
