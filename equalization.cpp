//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 2 : (a)
//This code is using cumulative-probability-based method to do histogram equalization
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

 const int BytesPerPixel=3;   //24-bit
 const int Size=512;               //the size of input image
 const int Level=256;             //the max grey level of each channel

int  main()
{
	int pO=0,qO=0;                              //coordinate of output
    int pI=0,qI=0, xR=0,xG=0,xB=0;   //coordinate of input and its components RGB
    int c=0,chnn=0;                              //channels. 0, RED channel;1,GREEN channel;2,BLUE channel;
	int lb=0;                                           //counting for array Level_Bar
	int Fmax,Fmin;                               //the maximum and minimum grey level in each channel
	int temp=0;                                     //temporary data saving
	int level=0;                                      //temporary grey level value saving
	int Size_Bar=(Size*Size)/Level;  //the number of pixels in every grey level after histogram equalization
	int b=0;                                            //counting for Size_Bar
	//Histogram
	int R_htgm[Level]={0};
	int G_htgm[Level]={0};
	int B_htgm[Level]={0};
	//CDF
	double R_freq[Level]={0.0};
	double G_freq[Level]={0.0};
	double B_freq[Level]={0.0};

    unsigned char In_Image[Size][Size][BytesPerPixel];        //the image data of input image
	int RT_Image[Size*Size];                                                     //the temporal image data of input image(R channel)
	int GT_Image[Size*Size];                                                     //the temporal image data of input image(G channel)
	int BT_Image[Size*Size];                                                      //the temporal image data of input image(B channel)
	int Level_Bar[Size*Size];                                                      //the sequence of grey level(0--255)
    int OT_Image[Size][Size][BytesPerPixel];                           //the temporal image data of output image(int)
	unsigned char COut_Image[Size][Size][BytesPerPixel];   //the image data of output image(unsigned char)

 ///////////////Read image///////////////
	ifstream ifile("jet.raw",ios_base::in | ios_base::binary);
	if (!ifile)
	{
		cout<<"File CANNOT open!"<<endl;
		exit(1);	
	}
    for (pI=0;pI<Size;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
				ifile.read((char*)&In_Image[pI][qI][c],sizeof(In_Image[pI][qI][c]));

/////////////////Divide input image into R, G and B//////////////
	for (pI=0;pI<Size;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				chnn=c;
				if (chnn==0)
				{
					RT_Image[xR]=In_Image[pI][qI][c];
					xR++;
				}
				else if (chnn==1)
				{
					GT_Image[xG]=In_Image[pI][qI][c];
					xG++;
				}
				else
				{
					BT_Image[xB]=In_Image[pI][qI][c];
					xB++;
				}
			}
		
  /////////////////Histogram Equalization in every channel/////////////////
	int count=0;
	//R channel
	//find MAX and MIN grey level
	xR=0;
	Fmax=RT_Image[xR],Fmin=RT_Image[xR];
	for (xR=0;xR<Size*Size;xR++)
	{
		temp=RT_Image[xR];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
    //sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size*Size;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
			count=0;
			for (xR=0;xR<Size*Size;xR++)
			{
				if (RT_Image[xR]==level)
				{
					Level_Bar[lb]=xR; 
					lb++;
					count++;
				}
			}
			R_htgm[level]=count;
			R_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
		}
	}

    //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			R_freq[level]=R_freq[level];
		else
			R_freq[level]=R_freq[level]+R_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
	}
	ofstream rfile("red_CDF.txt",ios_base::out);
	if (!rfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		rfile<<level<<" "<<R_freq[level]<<endl;

	//write the number of pixel at each grey level in a TXT
	//for drawing the histogram
	ofstream rHfile("red_histgrm.txt",ios_base::out);
	if (!rHfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		rHfile<<level<<" "<<R_htgm[level]<<endl;
	}

	//histogram equalizationl
	for (lb=0;lb<Size*Size;)
	{
		if ((Size*Size-lb)<Size_Bar)
			lb=Size*Size;
		else
		{
			for (level=0;level<Level;level++)
			{
				for (b=0;b<Size_Bar;b++)
				{
					RT_Image[Level_Bar[lb+b]]=level;
				}
				lb=lb+b;
			}
		}
	}

	for (level=0;level<Level;level++)
	{
			count=0;xR=0;
			for (xR=0;xR<Size*Size;xR++)
			{
				if (RT_Image[xR]==level)
					count++;
			}
			R_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
	}
    //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			R_freq[level]=R_freq[level];
		else
			R_freq[level]=R_freq[level]+R_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
	}
	ofstream rNfile("red_newCDF.txt",ios_base::out);
	if (!rNfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		rNfile<<level<<" "<<R_freq[level]<<endl;

	//calculate the probability in every grey level
    for (level=0;level<256;level++)
	{
		count=0;
		for (xR=0;xR<Size*Size;xR++)
		{
			if (RT_Image[xR]==level)
				count++;
		}
			R_htgm[level]=count;
			//cout<<level<<"   "<<count<<"   "<<R_htgm[level]<<endl;
	}

	//write the number of pixel at each grey level in a TXT
	//for drawing the histogram
	ofstream NrHfile("New_red_histgrm.txt",ios_base::out);
	if (!NrHfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		NrHfile<<level<<" "<<R_htgm[level]<<endl;
	}



	//G channel
	//find MAX and MIN grey level
	xG=0;
	Fmax=GT_Image[xG],Fmin=GT_Image[xG];
	for (xG=0;xG<Size*Size;xG++)
	{
		temp=GT_Image[xG];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
    //sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size*Size;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
			count=0;
			for (xG=0;xG<Size*Size;xG++)
			{
				if (GT_Image[xG]==level)
				{
					Level_Bar[lb]=xG; 
					lb++;
					count++;
				}
			}
			G_freq[level]=(double)count/(double)(Size*Size);
		}
	}

    //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			G_freq[level]=G_freq[level];
		else
			G_freq[level]=G_freq[level]+G_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
	}
	ofstream gfile("green_CDF.txt",ios_base::out);
	if (!gfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		gfile<<level<<" "<<G_freq[level]<<endl;


	//histogram equalizationl
	for (lb=0;lb<Size*Size;)
	{
		if ((Size*Size-lb)<Size_Bar)
			lb=Size*Size;
		else
		{
			for (level=0;level<Level;level++)
			{
				for (b=0;b<Size_Bar;b++)
				{
					GT_Image[Level_Bar[lb+b]]=level;
				}
				lb=lb+b;
			}
		}
	}

	for (level=0;level<Level;level++)
	{
			count=0;xG=0;
			for (xG=0;xG<Size*Size;xG++)
			{
				if (GT_Image[xG]==level)
					count++;
			}
			G_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<G_freq[level]<<endl;
	}
    //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			G_freq[level]=G_freq[level];
		else
			G_freq[level]=G_freq[level]+G_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<G_freq[level]<<endl;
	}
	ofstream gNfile("green_newCDF.txt",ios_base::out);
	if (!gNfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		gNfile<<level<<" "<<G_freq[level]<<endl;

	//B channel
	//find MAX and MIN grey level
	xB=0;
	Fmax=BT_Image[xB],Fmin=BT_Image[xB];
	for (xB=0;xB<Size*Size;xB++)
	{
		temp=BT_Image[xB];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
    //sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size*Size;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
			count=0;
			for (xB=0;xB<Size*Size;xB++)
			{
				if (BT_Image[xB]==level)
				{
					Level_Bar[lb]=xB; 
					lb++;
					count++;
				}
			}
			B_freq[level]=(double)count/(double)(Size*Size);
		}
	}

	 //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			B_freq[level]=B_freq[level];
		else
			B_freq[level]=B_freq[level]+B_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<B_freq[level]<<endl;
	}
	ofstream bfile("blue_CDF.txt",ios_base::out);
	if (!bfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		bfile<<level<<" "<<B_freq[level]<<endl;

	//histogram equalization
	for (lb=0;lb<Size*Size;)
	{
		if ((Size*Size-lb)<Size_Bar)
			lb=Size*Size;
		else
		{
			for (level=0;level<Level;level++)
			{
				for (b=0;b<Size_Bar;b++)
				{
					BT_Image[Level_Bar[lb+b]]=level;
				}
				lb=lb+b;
			}
		}
	}

	for (level=0;level<Level;level++)
	{
			count=0;xB=0;
			for (xB=0;xB<Size*Size;xB++)
			{
				if (BT_Image[xB]==level)
					count++;
			}
			B_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<G_freq[level]<<endl;
	}
    //write the probability of pixel at each grey level in a TXT
	//for drawing the CDF
	for (level=0;level<Level;level++)
	{
		if (level==0)
			B_freq[level]=B_freq[level];
		else
			B_freq[level]=B_freq[level]+B_freq[level-1];
		//cout<<level<<"   "<<count<<"   "<<G_freq[level]<<endl;
	}
	ofstream bNfile("blue_newCDF.txt",ios_base::out);
	if (!bNfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
		bNfile<<level<<" "<<B_freq[level]<<endl;


  /////////////////Combine new R, G and B to be output image//////////////
	xR=0;xG=0;xB=0;
    for (pO=0;pO<Size;pO++)
		for (qO=0;qO<Size;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				chnn=c;
				if (chnn==0)
				{
					OT_Image[pO][qO][c]=RT_Image[xR];
					xR++;
				}
				else if (chnn==1)
				{
					OT_Image[pO][qO][c]=GT_Image[xG];
					xG++;
				}
				else
				{
					OT_Image[pO][qO][c]=BT_Image[xB];
					xB++;
				}
			}
      
  ///////////////////Write image/////////////////////
	ofstream ofile("equal_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size;pO++)
		for (qO=0;qO<Size;qO++)
			for (c=3;c<BytesPerPixel+3;c++)
			{
				COut_Image[pO][qO][c]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}
	ifile.close();
	rfile.close();
	rHfile.close();
	rNfile.close();
	NrHfile.close();
	gfile.close();
	gNfile.close();
	bfile.close();
	bNfile.close();
	ofile.close();
	//getchar();
    return 0 ;
}