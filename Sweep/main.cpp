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
string s = "test.gbr.ctu3";
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
//////////��ۦPy�b���Ҧ��I  �[��vX //////////
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
//////////��ۦPy�b���Ҧ��I  �[��vX //////////
		 
		
		

		
//////////��X//////////
		vector <Segment> Test2;  //�ثe (event y�y��) �H�W���Ҧ�segment���X
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
					vX.erase(vX.begin() + i);	//�Y��F���I �h�⦹segment�qvX������
			}
		}

		sort(Test2.begin(),Test2.end(),MySort2);
		vector <Segment> Test;	//�ثe (event y�y��) �H�W���Ҧ�segment���X (�n�ѥ����k�@�Ӥ@���ˬd)

		int iColor = 0;
		vector <long double> vDarkXPre;
		vector <long double> vDarkXNow;
		vDarkXPre.assign(vDarkXNext.begin(), vDarkXNext.end()); 
		vDarkXNext.clear();

		long double yyPre = yyNext;
		long double yyNow = 0;
		for(int i = 0 ; i < Test2.size() ; i++ )
		{		
			bool b = 0;									//��segment�X�{�L�F�S�H
			for(int j = 0 ; j < Test.size() ; j++ )		//�p�G�b���X���o�{�ۦP�s����segment �h����
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
				sort(Test.begin(),Test.end(),MySort3);		//�N�Ҧ�segment ��iNumber��sort iNumber�V�j���N��V�W�h
			}
			
			int iColor2 = 0;
			if(Test.size() > 0)
				iColor2 = Test[0].iColor;		//����̤W�h���C��
				
			
			if(iColor != iColor2)			//�Y�C�⤣�ۦP �h�n��X
			{
				iColor = iColor2;
				cout<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),line,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
				//cout<<Test2[i].x<<"	"<<Test2[i].y<<"	"<<Test2[i].Tx<<"	"<<Test2[i].Ty<<endl;
				yyNext = Test2[i].Ty;
				yyNow = Test2[i].y;
				vDarkXNext.push_back(Test2[i].Tx);				
				vDarkXNow.push_back(Test2[i].x);				
			}			
		}

		////�B�z�����u////
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
		else if (vY.size() == 0)
		{
			for(int k = 0 ; k < vDarkXPre.size() ; k = k + 2)
			{
				cout<<"Draw 9 dark {("<<vDarkXPre[k]<<","<<yyNext<<"),line,("<<vDarkXPre[k + 1]<<","<<yyNext<<")} [1]"<<endl;
			}
		}
		//	cout<<endl;
		////�B�z�����u////

//////////��X//////////		





////////����I////////
		sort(vX.begin(),vX.end(),MySort2);
		int a = vX.size();
		if(vX.size() > 0)
		for(int i = 0 ; i < a - 1 ; i++ )
		{		
			if(vX[i].a != vX[i + 1].a)	//�Y�ײv���P 
			{
				long double Da = vX[i].a - vX[i + 1].a;
				long double Db = vX[i].b - vX[i + 1].b;	//Da * y + Db = 0 => y = -Db/Da
				long double y = -Db / Da;
				
				
				if(y < vX[i].y && y < vX[i + 1].y && y >= vX[i].Ty && y >= vX[i + 1].Ty)
				{
					
					long double x = vX[i].a * y + vX[i].b;	
					Point PTemp;
					PTemp.x = x; PTemp.y = y;

					vY.push_back(PTemp);	//�Y�����I�h�s�W�^vY
					//cout<<x<<"	"<<y<<endl;
				}				
			}
		}				
////////����I////////		
					
		
	}
	system("pause");
	return 0;
}
