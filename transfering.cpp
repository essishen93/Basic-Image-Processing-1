//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 2 : (a)
//This code is using transfer-function-based method to do histogram equalization
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

const int BytesPerPixel=3;   //24-bit
const int Size=512;               //the size of input image
const int Level=256;             //the number of grey levels

int  main()
{
	int pO=0,qO=0;                              //coordinate of output
    int pI=0,qI=0, xR=0,xG=0,xB=0;   //coordinate of input and its components RGB
    int c=0,chnn=0;                              //channels. 0, RED channel;1,GREEN channel;2,BLUE channel;
	int lb=0;                                           //counting for array Level_Bar
	int Fmax,Fmin;                               //the maximum and minimum grey level in each channel
	int temp=0;                                     //temporary data saving
	int level=0;                                      //temporary grey level value saving
	//Histogram
	int R_htgm[Level]={0};
	int G_htgm[Level]={0};
	int B_htgm[Level]={0};
	//CDF
	double R_freq[Level]={0.0};
	double G_freq[Level]={0.0};
	double B_freq[Level]={0.0};
	//TransFunction
	int R_trans[Level]={0};
	int G_trans[Level]={0};
	int B_trans[Level]={0};

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

  /////////////////Histogram Equalization by Transfer Function/////////////////
    int count=0;   //record the number of pixel at each grey level
    
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
	//cout<<Fmax<<"   "<<Fmin<<endl;

    //sorting grey level in increased order and put the order in Level_Bar
	//calculate the probability in every grey level
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
	//write the number of pixel at each grey level in a TXT
	//for drawing the histogram
	ofstream rfile("red_histgrm.txt",ios_base::out);
	if (!rfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		rfile<<level<<" "<<R_htgm[level]<<endl;
	}

	//calculate the CDF and creat grey level's mapping table
	//x to CDF(x)*255
	double sumRF=0.0;
	lb=0;
	for (level=0;level<Level;level++)
	{
		sumRF=sumRF+R_freq[level];
		R_trans[level]=sumRF*255;
		while (RT_Image[Level_Bar[lb]]==level)
		{
		RT_Image[Level_Bar[lb]]=sumRF*255;
		lb++;
		}
		//cout<<level<<"   "<<lb<<"   "<<sumRF*255<<endl;
	}
	//write the data of transfer function in a TXT
	//for drawing the transfer function
	ofstream rTfile("red_transfun.txt",ios_base::out);
	if (!rTfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		rTfile<<level<<" "<<R_trans[level]<<endl;
	}

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
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
	}

	//write the number of pixel at each grey level in a TXT
	//for drawing the histogram
	ofstream Nrfile("New_red_histgrm.txt",ios_base::out);
	if (!Nrfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		Nrfile<<level<<" "<<R_htgm[level]<<endl;
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
	//cout<<Fmax<<"   "<<Fmin<<endl;

    //sorting grey level in increased order and put the order in Level_Bar
	//calculate the probability in every grey level
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
			G_htgm[level]=count;
			G_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
		}
	}

	//write the number of pixel at each gray level in a TXT
	//for drawing the histogram
	ofstream gfile("green_histgrm.txt",ios_base::out);
	if (!gfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		gfile<<level<<" "<<G_htgm[level]<<endl;
	}

	//calculate the CDF and creat grey level's mapping table
	//x to CDF(x)*255
	double sumGF=0.0;
	lb=0;
	for (level=0;level<Level;level++)
	{
		sumGF=sumGF+G_freq[level];
		G_trans[level]=sumGF*255;
		while (GT_Image[Level_Bar[lb]]==level)
		{
		GT_Image[Level_Bar[lb]]=sumGF*255;
		lb++;
		}
		//cout<<level<<"   "<<lb<<"   "<<sumRF*255<<endl;
	}

	ofstream gTfile("green_transfun.txt",ios_base::out);
	if (!gTfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		gTfile<<level<<" "<<G_trans[level]<<endl;
	}

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
	//cout<<Fmax<<"   "<<Fmin<<endl;

    //sorting grey level in increased order and put the order in Level_Bar
	//calculate the probability in every grey level
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
			B_htgm[level]=count;
			B_freq[level]=(double)count/(double)(Size*Size);
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
		}
	}

	//write the number of pixel at each grey level in a TXT
	//for drawing the histogram
	ofstream bfile("blue_histgrm.txt",ios_base::out);
	if (!bfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		bfile<<level<<" "<<B_htgm[level]<<endl;
	}

	//calculate the CDF and creat grey level's mapping table
	//x to CDF(x)*255
	double sumBF=0.0;
	lb=0;
	for (level=0;level<Level;level++)
	{
		sumBF=sumBF+B_freq[level];
		B_trans[level]=sumBF*255;
		while (BT_Image[Level_Bar[lb]]==level)
		{
		BT_Image[Level_Bar[lb]]=sumBF*255;
		lb++;
		}
		//cout<<level<<"   "<<lb<<"   "<<sumRF*255<<endl;
	}

	ofstream bTfile("blue_transfun.txt",ios_base::out);
	if (!bTfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (level=0;level<Level;level++)
	{
		bTfile<<level<<" "<<B_trans[level]<<endl;
	}

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
	ofstream ofile("trans_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size;pO++)
		for (qO=0;qO<Size;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				COut_Image[pO][qO][c]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}
	ifile.close();
	rfile.close();
	rTfile.close();
	Nrfile.close();
	gfile.close();
	gTfile.close();
	bfile.close();
	bTfile.close();
	ofile.close();
    return 0 ;
}