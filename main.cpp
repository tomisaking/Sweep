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
vector <ForHorizontal> vDarkXNext;
long double yyNext = -1;
string s = "C:\\Users\\100\\Desktop\\Sweep\\01_top.art.ctu3";
bool bOnce = 0;
long int iTP = 0;
//TOM edit
vector <Point> buffer;
vector <int> to_insert;
//vector <long int> vYT;
vector <vector <Segment> > vContour;		
vector <long int> Link;
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
			STemp.bdirection = 0;
			
			iTemp = str.find("dark");			
			if(iTemp > 0)			
				STemp.iColor = 1;
			else
				STemp.iColor = 0;

			STemp.iNumber = iNowLine;
			//STemp.iNumber2 = iNowLine;
			
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
	vContour.resize(iNowLine + 1);
	Link.resize(iNowLine + 1);
}


int main(int argc, char** argv) 
{
	clock_t t;
	t = clock();

	if(argc > 1)
		s = argv[1];

	fstream  ofile;
	//if(argc > 1)
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
	//ofile<<"                                           "<<endl;
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
		vector <ForHorizontal> vDarkXPre;
		vector <ForHorizontal> vDarkXNow;
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
				
				
				/*{
					if(Test2[i].iCase == 1)
					{
						ofile<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),line,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
					}
					else if (Test2[i].iCase == 2)
					{
						if(fabs(Test2[i].x - Test2[i].Tx) > ROUND)
							ofile<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),arc,("<<Test2[i].Rx<<","<<Test2[i].Ry<<"),CW,0.123,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
					}
					else if (Test2[i].iCase == 3)
					{
						if(fabs(Test2[i].x - Test2[i].Tx) > ROUND)
							ofile<<"Draw 9 dark {("<<Test2[i].x<<","<<Test2[i].y<<"),arc,("<<Test2[i].Rx<<","<<Test2[i].Ry<<"),CCW,0.123,("<<Test2[i].Tx<<","<<Test2[i].Ty<<")} [1]"<<endl;
					}
				}*/


				//cout<<Test2[i].x<<"	"<<Test2[i].y<<"	"<<Test2[i].Tx<<"	"<<Test2[i].Ty<<endl;

				if(!bTemp)
				{
					STemp = Test2[i];
					swap(STemp.x,STemp.Tx);
					swap(STemp.y,STemp.Ty);

					STemp.bdirection = 1;
					if(STemp.iCase == 3)
						STemp.iCase = 2;
					else if(STemp.iCase == 2)
						STemp.iCase = 3;
				}
				else
				{
					bool bTemp = 0;
					if(vContour[STemp.iNumber].size() > 0)
					{
						for(int i = vContour[STemp.iNumber].size() - 1 ; i >= 0/*vContour[STemp.iNumber].size() - 4*/ ; i--)
						{
							if(fabs(vContour[STemp.iNumber][i].x - STemp.Tx) <= ROUND && vContour[STemp.iNumber][i].y == STemp.Ty && vContour[STemp.iNumber][i].bdirection == STemp.bdirection && vContour[STemp.iNumber][i].a == STemp.a && vContour[STemp.iNumber][i].iCase == STemp.iCase)
							{
								vContour[STemp.iNumber][i].x = STemp.x;
								vContour[STemp.iNumber][i].y = STemp.y;
								bTemp = 1;
								break;
							}
							if(i == 0)
								break;
						}
						if(!bTemp)
							vContour[STemp.iNumber].push_back(STemp);
					}
					else
						vContour[STemp.iNumber].push_back(STemp);



					bTemp = 0;
					if(vContour[Test2[i].iNumber].size() > 0)
					{
						for(int j = vContour[Test2[i].iNumber].size() - 1 ; j >= 0/*vContour[Test2[i].iNumber].size() - 4*/ ; j--)
						{
							if(fabs(vContour[Test2[i].iNumber][j].Tx - Test2[i].x) <= ROUND && vContour[Test2[i].iNumber][j].Ty == Test2[i].y && vContour[Test2[i].iNumber][j].bdirection == Test2[i].bdirection && vContour[Test2[i].iNumber][j].a == Test2[i].a && vContour[Test2[i].iNumber][j].iCase == Test2[i].iCase)
							{
								vContour[Test2[i].iNumber][j].Tx = Test2[i].Tx;
								vContour[Test2[i].iNumber][j].Ty = Test2[i].Ty;
								bTemp = 1;
								break;
							}
							if(j == 0)
								break;
						}
						if(!bTemp)
							vContour[Test2[i].iNumber].push_back(Test2[i]);
					}
					else
						vContour[Test2[i].iNumber].push_back(Test2[i]);



				

					//iTP++;
					/*ofile<<"4 "<<endl;
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
					ofile<<endl;*/
				}
			

				ForHorizontal Temp;
				Temp.x = Test2[i].Tx;
				Temp.iNumber = Test2[i].iNumber;
				vDarkXNext.push_back(Temp);


				Temp.x = Test2[i].x;
				vDarkXNow.push_back(Temp);				

				bTemp = !bTemp;
			}			
		}
		//vYT.push_back(iTP - 1);
		////處理水平線////
		if(yyPre == yyNow)
		{
			for(int i = 0 ; i < vDarkXPre.size() ; i++)
			{
				vDarkXNow.push_back(vDarkXPre[i]);
			}
			sort(vDarkXNow.begin(),vDarkXNow.end(),MySort6);

			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				if(fabs(vDarkXNow[k].x - vDarkXNow[k + 1].x) > ROUND)
				{
					//ofile<<"Draw 9 dark {("<<vDarkXNow[k].x<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1].x<<","<<yyNow<<")} [1]"<<endl;
					Segment Temp;
					Temp.x = vDarkXNow[k].x;		Temp.y = yyNow;
					Temp.Tx = vDarkXNow[k + 1].x;	Temp.Ty = yyNow;
					Temp.iCase = 4;
					if(Temp.x == Temp.Tx && Temp.y == Temp.Ty)
						continue;
					vContour[vDarkXNow[k].iNumber].push_back(Temp);
					long int iTemp = 0 ;
					if(Link[vDarkXNow[k].iNumber] > 0 && Link[vDarkXNow[k + 1].iNumber] > 0)
					{
						iTemp = min( min(Link[vDarkXNow[k].iNumber] , Link[vDarkXNow[k + 1].iNumber]) , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
					}
					else if(Link[vDarkXNow[k].iNumber] > 0 && Link[vDarkXNow[k + 1].iNumber] == 0)
					{
						iTemp = min(Link[vDarkXNow[k].iNumber] , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
					}
					else if(Link[vDarkXNow[k].iNumber] == 0 && Link[vDarkXNow[k + 1].iNumber] > 0)
					{
						iTemp = min(Link[vDarkXNow[k + 1].iNumber] , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
					}
					else
					{
						iTemp = min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber);
					}
					long int a = Link[vDarkXNow[k].iNumber] , b = Link[vDarkXNow[k + 1].iNumber];
					Link[vDarkXNow[k].iNumber] = iTemp;
					Link[vDarkXNow[k + 1].iNumber] = iTemp;

					if(a > 0)
					{
						while(a != iTemp)
						{
							if(iTemp >= Link[a])
								break;

							long int aTemp = a;
							a = Link[a];
							Link[aTemp] = iTemp;
						}
					}
						
					if(b > 0)
					{
						while(b != iTemp)
						{
							if(iTemp >= Link[b])
								break;

							long int bTemp = b;
							b = Link[b];
							Link[bTemp] = iTemp;
						}
					}
						
			
				}
			}
		}
		else if (yyPre != yyNow)
		{
			for(int k = 0 ; k < vDarkXPre.size() ; k = k + 2)
			{
				//ofile<<"Draw 9 dark {("<<vDarkXPre[k].x<<","<<yyPre<<"),line,("<<vDarkXPre[k + 1].x<<","<<yyPre<<")} [1]"<<endl;
				Segment Temp;
				Temp.x = vDarkXPre[k].x;		Temp.y = yyPre;
				Temp.Tx = vDarkXPre[k + 1].x;	Temp.Ty = yyPre;
				Temp.iCase = 4;
				if(Temp.x == Temp.Tx && Temp.y == Temp.Ty)
					continue;
				vContour[vDarkXPre[k].iNumber].push_back(Temp);

				long int iTemp = 0 ;
				if(Link[vDarkXPre[k].iNumber] > 0 && Link[vDarkXPre[k + 1].iNumber] > 0)
				{
					iTemp = min( min(Link[vDarkXPre[k].iNumber] , Link[vDarkXPre[k + 1].iNumber]) , min(vDarkXPre[k].iNumber , vDarkXPre[k + 1].iNumber) );
				}
				else if(Link[vDarkXPre[k].iNumber] > 0 && Link[vDarkXPre[k + 1].iNumber] == 0)
				{
					iTemp = min(Link[vDarkXPre[k].iNumber] , min(vDarkXPre[k].iNumber , vDarkXPre[k + 1].iNumber) );
				}
				else if(Link[vDarkXPre[k].iNumber] == 0 && Link[vDarkXPre[k + 1].iNumber] > 0)
				{
					iTemp = min(Link[vDarkXPre[k + 1].iNumber] , min(vDarkXPre[k].iNumber , vDarkXPre[k + 1].iNumber) );
				}
				else
				{
					iTemp = min(vDarkXPre[k].iNumber , vDarkXPre[k + 1].iNumber);
				}
				long int a = Link[vDarkXPre[k].iNumber] , b = Link[vDarkXPre[k + 1].iNumber];
				Link[vDarkXPre[k].iNumber] = iTemp;
				Link[vDarkXPre[k + 1].iNumber] = iTemp;

				if(a > 0)
				{
					while(a != iTemp)
					{
						if(iTemp >= Link[a])
							break;

						long int aTemp = a;
						a = Link[a];
						Link[aTemp] = iTemp;
					}
				}
					
				if(b > 0)
				{
					while(b != iTemp)
					{
						if(iTemp >= Link[b])
							break;

						long int bTemp = b;
						b = Link[b];
						Link[bTemp] = iTemp;
					}
				}
			}
			for(int k = 0 ; k < vDarkXNow.size() ; k = k + 2)
			{
				//ofile<<"Draw 9 dark {("<<vDarkXNow[k].x<<","<<yyNow<<"),line,("<<vDarkXNow[k + 1].x<<","<<yyNow<<")} [1]"<<endl;
				Segment Temp;
				Temp.x = vDarkXNow[k].x;		Temp.y = yyNow;
				Temp.Tx = vDarkXNow[k + 1].x;	Temp.Ty = yyNow;
				Temp.iCase = 4;
				if(Temp.x == Temp.Tx && Temp.y == Temp.Ty)
					continue;
				vContour[vDarkXNow[k].iNumber].push_back(Temp);

				long int iTemp = 0 ;
				if(Link[vDarkXNow[k].iNumber] > 0 && Link[vDarkXNow[k + 1].iNumber] > 0)
				{
					iTemp = min( min(Link[vDarkXNow[k].iNumber] , Link[vDarkXNow[k + 1].iNumber]) , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
				}
				else if(Link[vDarkXNow[k].iNumber] > 0 && Link[vDarkXNow[k + 1].iNumber] == 0)
				{
					iTemp = min(Link[vDarkXNow[k].iNumber] , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
				}
				else if(Link[vDarkXNow[k].iNumber] == 0 && Link[vDarkXNow[k + 1].iNumber] > 0)
				{
					iTemp = min(Link[vDarkXNow[k + 1].iNumber] , min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber) );
				}
				else
				{
					iTemp = min(vDarkXNow[k].iNumber , vDarkXNow[k + 1].iNumber);
				}
				long int a = Link[vDarkXNow[k].iNumber] , b = Link[vDarkXNow[k + 1].iNumber];
				Link[vDarkXNow[k].iNumber] = iTemp;
				Link[vDarkXNow[k + 1].iNumber] = iTemp;

				if(a > 0)
				{
					while(a != iTemp)
					{
						if(iTemp >= Link[a])
							break;

						long int aTemp = a;
						a = Link[a];
						Link[aTemp] = iTemp;
					}
				}
					
				if(b > 0)
				{
					while(b != iTemp)
					{
						if(iTemp >= Link[b])
							break;

						long int bTemp = b;
						b = Link[b];
						Link[bTemp] = iTemp;
					}
				}
			}
		}
		if(vY.size() == 0 && !bOnce)
		{
			bOnce = 1;
			for(int k = 0 ; k < vDarkXNext.size() ; k = k + 2)
			{
				//ofile<<"Draw 9 dark {("<<vDarkXNext[k].x<<","<<yyNext<<"),line,("<<vDarkXNext[k + 1].x<<","<<yyNext<<")} [1]"<<endl;
				Segment Temp;
				Temp.x = vDarkXNext[k].x;		Temp.y = yyNext;
				Temp.Tx = vDarkXNext[k + 1].x;	Temp.Ty = yyNext;
				Temp.iCase = 4;
				if(Temp.x == Temp.Tx && Temp.y == Temp.Ty)
					continue;
				vContour[vDarkXNext[k].iNumber].push_back(Temp);

				long int iTemp = 0 ;
				if(Link[vDarkXNext[k].iNumber] > 0 && Link[vDarkXNext[k + 1].iNumber] > 0)
				{
					iTemp = min( min(Link[vDarkXNext[k].iNumber] , Link[vDarkXNext[k + 1].iNumber]) , min(vDarkXNext[k].iNumber , vDarkXNext[k + 1].iNumber) );
				}
				else if(Link[vDarkXNext[k].iNumber] > 0 && Link[vDarkXNext[k + 1].iNumber] == 0)
				{
					iTemp = min(Link[vDarkXNext[k].iNumber] , min(vDarkXNext[k].iNumber , vDarkXNext[k + 1].iNumber) );
				}
				else if(Link[vDarkXNext[k].iNumber] == 0 && Link[vDarkXNext[k + 1].iNumber] > 0)
				{
					iTemp = min(Link[vDarkXNext[k + 1].iNumber] , min(vDarkXNext[k].iNumber , vDarkXNext[k + 1].iNumber) );
				}
				else
				{
					iTemp = min(vDarkXNext[k].iNumber , vDarkXNext[k + 1].iNumber);
				}
				long int a = Link[vDarkXNext[k].iNumber] , b = Link[vDarkXNext[k + 1].iNumber];
				Link[vDarkXNext[k].iNumber] = iTemp;
				Link[vDarkXNext[k + 1].iNumber] = iTemp;

				if(a > 0)
				{
					while(a != iTemp)
					{
						if(iTemp >= Link[a])
							break;

						long int aTemp = a;
						a = Link[a];
						Link[aTemp] = iTemp;
					}
				}
					
				if(b > 0)
				{
					while(b != iTemp)
					{
						if(iTemp >= Link[b])
							break;

						long int bTemp = b;
						b = Link[b];
						Link[bTemp] = iTemp;
					}
				}
			}
		}
		//	cout<<endl;
		////處理水平線////

//////////輸出//////////		





////////找交點/////////		
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



					long int iTemp = 0 ;
					if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
					{
						iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
					}
					else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
					{
						iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
					}
					else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
					{
						iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
					}
					else
					{
						iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
					}				
					long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
					Link[vX[i].iNumber] = iTemp;
					Link[vX[i + 1].iNumber] = iTemp;

					if(a > 0)
					{
						while(a != iTemp)
						{
							if(iTemp >= Link[a])
								break;

							long int aTemp = a;
							a = Link[a];
							Link[aTemp] = iTemp;
						}
					}
						
					if(b > 0)
					{
						while(b != iTemp)
						{
							if(iTemp >= Link[b])
								break;

							long int bTemp = b;
							b = Link[b];
							Link[bTemp] = iTemp;
						}
					}
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
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}
						
					}
					else if (ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_x < xright && ans2_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}
					
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
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}

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
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}
					}
					else if (ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_x < xright && ans2_x >= xleft)
					{
						Point PTemp;
						PTemp.x = ans2_x;
						PTemp.y = ans2_y;
						buffer.push_back(PTemp);
						//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}
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
						long int iTemp = 0 ;
						if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
						{
							iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
						{
							iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
						}
						else
						{
							iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
						}
						long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
						Link[vX[i].iNumber] = iTemp;
						Link[vX[i + 1].iNumber] = iTemp;

						if(a > 0)
						{
							while(a != iTemp)
							{
								if(iTemp >= Link[a])
									break;

								long int aTemp = a;
								a = Link[a];
								Link[aTemp] = iTemp;
							}
						}
							
						if(b > 0)
						{
							while(b != iTemp)
							{
								if(iTemp >= Link[b])
									break;

								long int bTemp = b;
								b = Link[b];
								Link[bTemp] = iTemp;
							}
						}

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
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;
							
							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
						}
						else if (ans2_y < vX[i].y && ans2_y >= vX[i].Ty && ans2_x < x1_right && ans2_x >= x1_left && ans2_y < vX[i+1].y && ans2_y >= vX[i+1].Ty && ans2_x < x2_right && ans2_x >= x2_left)
						{
							Point PTemp;
							PTemp.x = ans2_x;
							PTemp.y = ans2_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;

							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
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
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;

							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
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
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;

							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
						}
						else if (ans1_y < vX[i].y && ans1_y >= vX[i].Ty && ans2_x < vX[i].x && ans2_x >= vX[i].Tx && ans1_y < vX[i+1].y && ans1_y >= vX[i+1].Ty && ans2_x < vX[i+1].x && ans2_x >= vX[i+1].Tx)
						{
							Point PTemp;
							PTemp.x = ans2_x;
							PTemp.y = ans1_y;
							buffer.push_back(PTemp);
							//printf ("x:%Lf   y:%Lf\n", PTemp.x, PTemp.y);
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;

							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
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
							long int iTemp = 0 ;
							if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min( min(Link[vX[i].iNumber] , Link[vX[i + 1].iNumber]) , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] > 0 && Link[vX[i + 1].iNumber] == 0)
							{
								iTemp = min(Link[vX[i].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else if(Link[vX[i].iNumber] == 0 && Link[vX[i + 1].iNumber] > 0)
							{
								iTemp = min(Link[vX[i + 1].iNumber] , min(vX[i].iNumber , vX[i + 1].iNumber) );
							}
							else
							{
								iTemp = min(vX[i].iNumber , vX[i + 1].iNumber);
							}
							long int a = Link[vX[i].iNumber] , b = Link[vX[i + 1].iNumber];
							Link[vX[i].iNumber] = iTemp;
							Link[vX[i + 1].iNumber] = iTemp;

							if(a > 0)
							{
								while(a != iTemp)
								{
									if(iTemp >= Link[a])
										break;

									long int aTemp = a;
									a = Link[a];
									Link[aTemp] = iTemp;
								}
							}
								
							if(b > 0)
							{
								while(b != iTemp)
								{
									if(iTemp >= Link[b])
										break;

									long int bTemp = b;
									b = Link[b];
									Link[bTemp] = iTemp;
								}
							}
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
		
		buffer.clear();
		to_insert.clear();
////////找交點////////		
					
		
	}
	cout<<endl<<"RunTime 1 : "<<clock()-t<<"ms"<<endl;
	
	//for(int i = Link.size() - 1 ; i >= 0 ; i--)
	//	Link[i] = 2;
	for(int i = Link.size() - 1 ; i >= 0 ; i--)
	{
		if(Link[i] > 0)
		{
			int	a = Link[i];
			while (Link[a] != a)
			{
				if(Link[a] > 0)
					a = Link[a];
			}

			if(i != a)
			{
				for(int j = 0 ; j < vContour[i].size() ; j++)
					vContour[a].push_back(vContour[i][j]);
				vContour[i].clear();
			}
		}
		
	}
	/*for(int i = vContour.size() - 1 ; i >= 0 ; i--)
	{
		if(vContour[i].size() == 0)
			vContour.erase(vContour.begin() + i);
	}*/

	for(int i = 0 ; i < vContour.size() ; i++)
	{
		sort(vContour[i].begin(),vContour[i].end(),MySort4);
	}

	int iTemp = 0;

	//cout<<endl<<"RunTime : "<<clock()-t<<"ms"<<endl;
	//system("pause");
	int iSize = vContour.size();
	//cout<<"iSize :"<<iSize<<endl;
	for(int i = 0 ; i < iSize ; i++)
	{
		if(vContour[i].size() > 0)
		{
			vector <Segment> Temp;
			Temp.push_back(vContour[i][0]);
			vContour[i].erase(vContour[i].begin() + 0); 
			int iTemp = vContour[i].size();
			for(int k = 0 ; k < iTemp ; k++)
			//while(vContour[i].size() > 1)
			{
				bool bTemp = 0;
				for(int j = 0 ; j < vContour[i].size() ; j++)
				{
					if(vContour[i][j].a == Temp[Temp.size() - 1].a && vContour[i][j].iCase == Temp[Temp.size() - 1].iCase && vContour[i][j].bdirection == Temp[Temp.size() - 1].bdirection && fabs(vContour[i][j].x - Temp[Temp.size() - 1].Tx) <= ROUND && fabs(vContour[i][j].y - Temp[Temp.size() - 1].Ty) <= ROUND )
					{
						Temp[Temp.size() - 1].Tx = vContour[i][j].Tx;
						Temp[Temp.size() - 1].Ty = vContour[i][j].Ty;
						vContour[i].erase(vContour[i].begin() + j); 
						bTemp = 1;
						break;
					}
					else if(fabs(vContour[i][j].x - Temp[Temp.size() - 1].Tx) <= ROUND && fabs(vContour[i][j].y - Temp[Temp.size() - 1].Ty) <= ROUND )
					{
						Temp.push_back(vContour[i][j]);
						vContour[i].erase(vContour[i].begin() + j); 
						bTemp = 1;
						break;
					}
					else if(fabs(vContour[i][j].Tx - Temp[Temp.size() - 1].Tx) <= ROUND && fabs(vContour[i][j].Ty - Temp[Temp.size() - 1].Ty) <= ROUND && vContour[i][j].iCase == 4)
					{
						swap(vContour[i][j].Tx,vContour[i][j].x);

						Temp.push_back(vContour[i][j]);
						vContour[i].erase(vContour[i].begin() + j); 
						bTemp = 1;
						break;
					}
				}
				if(!bTemp)
				{
					vContour.push_back(Temp);
					Temp.clear();
					if(vContour[i].size() > 1)
					{
						Temp.push_back(vContour[i][0]);
						vContour[i].erase(vContour[i].begin() + 0); 
					}
				}
			}
			vContour[i].clear();
			vContour[i].assign(Temp.begin() , Temp.end());

		}
	}

	//system("pause");
	//return 0;

	long int iPoints = 0;

	vector <PointforYTYB> sPointforYTYB;
	//vector <vector <long int> > YB;
	////////////////////////Output////////////////////////
	//return 0 ;
	vector <vector <Segment> > vUnContour;		
	//cout<<"Break 1"<<endl;
	for(int i = 0 ; i < vContour.size() ; i++)
	{
		if(vContour[i].size() > 0)
			if( fabs(vContour[i][0].x - vContour[i][vContour[i].size() -1].Tx) > ROUND || fabs(vContour[i][0].y - vContour[i][vContour[i].size() -1].Ty) > ROUND)
			{
				vUnContour.push_back(vContour[i]);
				vContour[i].clear();
			}
	}
	for(int i = vUnContour.size() - 1 ; i >= 0 ; i--)
	{
		for(int j = 0 ; j < vUnContour.size() ; j++)
		{
			if( fabs(vUnContour[i][0].x - vUnContour[j][vUnContour[j].size() - 1].Tx) <= ROUND && fabs(vUnContour[i][0].y - vUnContour[j][vUnContour[j].size() - 1].Ty) <= ROUND && i != j)
			{
				//i 是 j 的後面

				for(int k = 0 ; k < vUnContour[i].size() ; k++)
					vUnContour[j].push_back(vUnContour[i][k]);
				vUnContour.erase(vUnContour.begin() + i); 
				break;

			}
			else if(vUnContour[i][0].iCase == 1 && vUnContour[j][vUnContour[j].size() - 1].iCase == 1 && vUnContour[i][0].a == vUnContour[j][vUnContour[j].size() - 1].a && i != j)
			{
				if((vUnContour[i][0].x <= vUnContour[j][vUnContour[j].size() - 1].Tx && vUnContour[i][0].x >= vUnContour[j][vUnContour[j].size() - 1].x)
				&& (vUnContour[i][0].y <= vUnContour[j][vUnContour[j].size() - 1].Ty && vUnContour[i][0].y >= vUnContour[j][vUnContour[j].size() - 1].y))
				{
					for(int k = 0 ; k < vUnContour[i].size() ; k++)
						vUnContour[j].push_back(vUnContour[i][k]);
					vUnContour.erase(vUnContour.begin() + i); 
					break;
				}
			}
		}

	}
//	cout<<"Break 2"<<endl;
	//cout<<"vContour Size :"<<vContour.size()<<endl;
	//cout<<"vUnContour Size :"<<vUnContour.size()<<endl;
	//vContour.clear();
	for(int i = 0 ; i < vUnContour.size() ; i++)
	{
		if( /*fabs(vUnContour[i][0].x - vUnContour[i][vUnContour[i].size() - 1].Tx) <= ROUND && fabs(vUnContour[i][0].y - vUnContour[i][vUnContour[i].size() - 1].Ty) <= ROUND &&*/ vUnContour[i].size() > 1)
			vContour.push_back(vUnContour[i]);
	}
	//cout<<"Fianl vContour Size :"<<vContour.size()<<endl;
	int iStillUnContour = 0 ;
	int iTTemp = 0 ;
	for(int i = 0 ; i < vContour.size() ; i++)
	{
		if(vContour[i].size() > 0)
		{
			iTTemp ++;
			if( fabs(vContour[i][0].x - vContour[i][vContour[i].size() -1].Tx) > ROUND || fabs(vContour[i][0].y - vContour[i][vContour[i].size() -1].Ty) > ROUND)
			{
				iStillUnContour++;
				//cout<<iTTemp<<endl;
			}
		}
	}
	cout<<"iStillUnContour :"<<iStillUnContour<<endl;
	for(int i = 0 ; i < vContour.size() ; i++)
	{
		if(vContour[i].size() > 0)
			iTP++;
	}
	//ofile<<iTP<<endl;
	iTP = 0;
	ofile<<"ff"<<endl;

	for(int i = 0 ; i < vContour.size() ; i++)
	{
		if(vContour[i].size() > 1)
		{
			vector <Segment> vSTemp;
			Segment	STemp;

			PointforYTYB cc;
			cc.iIndex = iTP++;			

			//ofile<<vContour[i].size()<<endl;

			cc.YB = vContour[i][0].y;
			cc.XB = vContour[i][0].x;
			cc.YT = vContour[i][0].y;
			cc.XT = vContour[i][0].x;
			ofile<<"Draw "<<i<<" dark {(";
			for(int j = 0 ; j < vContour[i].size() ; j++)
			{
				if(vContour[i][j].y < cc.YB)
				{
					cc.YB = vContour[i][j].y;
					cc.XB = vContour[i][j].x;
				}
				if(vContour[i][j].y > cc.YB)
				{
					cc.YT = vContour[i][j].y;
					cc.XT = vContour[i][j].x;
				}
				

				/*if(vContour[i][j].iCase == 1)
					ofile<<iPoints++<<" 0 "<<vContour[i][j].x<<" "<<vContour[i][j].y<<endl;
				else if(vContour[i][j].iCase == 2)
					ofile<<iPoints++<<" 8 "<<vContour[i][j].x<<" "<<vContour[i][j].y<<" "<<vContour[i][j].Rx<<" "<<vContour[i][j].Ry<<" "<<vContour[i][j].r<<endl;
				else if(vContour[i][j].iCase == 3)
					ofile<<iPoints++<<" 9 "<<vContour[i][j].x<<" "<<vContour[i][j].y<<" "<<vContour[i][j].Rx<<" "<<vContour[i][j].Ry<<" "<<vContour[i][j].r<<endl;*/

				if(vContour[i][j].iCase == 1 || vContour[i][j].iCase == 4)
				{
					ofile<<vContour[i][j].x<<","<<vContour[i][j].y<<"),line,(";
				}
				else if (vContour[i][j].iCase == 2)
				{
					//if(fabs(vContour[i][j].x - vContour[i][j].Tx) > ROUND)
						ofile<<vContour[i][j].x<<","<<vContour[i][j].y<<"),arc,("<<vContour[i][j].Rx<<","<<vContour[i][j].Ry<<"),CW,0.123,(";
				}
				else if (vContour[i][j].iCase == 3)
				{
					//if(fabs(vContour[i][j].x - vContour[i][j].Tx) > ROUND)
						ofile<<vContour[i][j].x<<","<<vContour[i][j].y<<"),arc,("<<vContour[i][j].Rx<<","<<vContour[i][j].Ry<<"),CCW,0.123,(";
				}

				vContour[i][j].iNumber2 = iPoints - 1;
				vSTemp.push_back(vContour[i][j]);
				
			}
			//if(vContour[i].size() == 1)
			{
				/*if( fabs(vContour[i][0].x - vContour[i][vContour[i].size() - 1].Tx) > ROUND || fabs(vContour[i][0].y - vContour[i][vContour[i].size() - 1].Ty) > ROUND)
				{
					ofile<<vContour[i][vContour[i].size() - 1].Tx<<","<<vContour[i][vContour[i].size() - 1].Ty<<"),line,(";
					ofile<<vContour[i][0].x<<","<<vContour[i][0].y<<")} [1]"<<endl;
				}
				else*/
					ofile<<vContour[i][vContour[i].size() - 1].Tx<<","<<vContour[i][vContour[i].size() - 1].Ty<<")} [1]"<<endl;

				//return 0;
			}

			sPointforYTYB.push_back(cc);

			sort(vSTemp.begin(),vSTemp.end(),MySort5);
			long double y = vSTemp[0].y;

			for(int i = 0 ; i < vSTemp.size() ; i++)
			{ 
				if(vSTemp[i].y == y)
				{
					//ofile<<vSTemp[i].iNumber2<<" ";
				} 
				else if(vSTemp[i].y != y)
				{
					y = vSTemp[i].y;
					//ofile<<endl<<vSTemp[i].iNumber2<<" ";
				}
			}
			//ofile<<endl<<endl;
		}
	}

	/*sort(sPointforYTYB.begin(),sPointforYTYB.end(),MySortYT);
	long double y = sPointforYTYB[0].YT;
	ofile<<";YT"<<endl;
	for(int i = 0 ; i < sPointforYTYB.size() ; i++)
	{
		if(y == sPointforYTYB[i].YT)
			ofile<<sPointforYTYB[i].iIndex<<" ";
		else
		{
			y = sPointforYTYB[i].YT;
			ofile<<endl<<sPointforYTYB[i].iIndex<<" ";
		}
	}
	sort(sPointforYTYB.begin(),sPointforYTYB.end(),MySortYB);
	y = sPointforYTYB[0].YB;
	ofile<<endl<<endl<<";YB"<<endl;
	for(int i = 0 ; i < sPointforYTYB.size() ; i++)
	{
		if(y == sPointforYTYB[i].YB)
			ofile<<sPointforYTYB[i].iIndex<<" ";
		else
		{
			y = sPointforYTYB[i].YB;
			ofile<<endl<<sPointforYTYB[i].iIndex<<" ";
		}
	}*/

	////////////////////////Output////////////////////////



	cout<<endl<<"RunTime : "<<clock()-t<<"ms"<<endl;
	system("pause");
	return 0;
}
