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
int bbbb = 5;


////////////////////////////////////////////////////////////////////////////////////



void Parse()
{
	int a = 6;
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
			
		
			//cout<<x<<"	"<<y<<endl;						
		}
		
		
		
		
		//cout<<p<<endl;	
	}
	sort(vY.begin(),vY.end(),MySort1);
	for(int i = 0 ; i < vY.size() ; i++) 
	{
		FindLinearEquation(vY[i]);
		cout<<vY[i].x<<"	"<<vY[i].y<<endl;
		for(int j = 0 ; j < vY[i].vS.size() ; j++)
			{
				cout<<vY[i].vS[j].Tx<<"	"<<vY[i].vS[j].Ty<<"	"<<endl<<vY[i].vS[j].a<<"	"<<vY[i].vS[j].b<<"	";
			}
		cout<<endl;
		cout<<endl;
	}
}


int main(int argc, char** argv) 
{
	//A ggg;
	//ggg.a.resize(100);
	cout.setf( std::ios::fixed, std:: ios::floatfield );
	cout.precision(10);
	Parse();  
	
	//b.vS[0].iCase = 3;
	//cout<<b.vS[0].iCase;
	return 0;
}
