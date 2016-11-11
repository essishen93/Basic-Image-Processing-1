//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 3 : (a)
//This code is using Median Filter and Gaussian Filter to remove noise in the image
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

const int BytesPerPixel=3;   //24-bit
const int Size=512;               //the size of input image
const int Level=256;             //the number of grey levels
const int R=5;                        //radius of filter window

int  main()
{
	int pO=0,qO=0;                              //coordinate of output
    int pI=0,qI=0;                                  //coordinate of input
    int c=0,chnn=0;                              //channels. 0, RED channel;1,GREEN channel;2,BLUE channel;
	int temp=0;                                     //temporary data saving
	int level=0;                                      //temporary grey level value saving

    unsigned char In_Image[Size][Size][BytesPerPixel];        //the image data of input image
	int IT_Image[Size+2][Size+2][BytesPerPixel];                    //the temporal image data of input image(int)
	int OT_Image[Size][Size][BytesPerPixel];                           //the temporal image data of output image(int)
    int OTG_Image[Size][Size][BytesPerPixel];                           //the temporal image data of output image(int)
	unsigned char COut_Image[Size+2*R][Size+2*R][BytesPerPixel];   //the image data of output image(unsigned char)


 ///////////////Read image///////////////
	ifstream ifile("peppers_noisy.raw",ios_base::in | ios_base::binary);
	if (!ifile)
	{
		cout<<"File CANNOT open!"<<endl;
		exit(1);	
	}
    for (pI=0;pI<Size;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
				ifile.read((char*)&In_Image[pI][qI][c],sizeof(In_Image[pI][qI][c]));

	/////////////////Enlarge input image//////////////
   for (pI=0;pI<Size+2;pI++)
	   for (qI=0;qI<Size+2;qI++)
		   for (c=0;c<BytesPerPixel;c++)
		   {
			//the four corners of the image
			if ( ((pI==0) || (pI==Size+1)) && (qI==0 || (qI==Size+1)))
			{
				IT_Image[0][0][c]=In_Image[1][1][c];
				IT_Image[0][Size+1][c]=In_Image[1][Size-2][c];
				IT_Image[Size+1][0][c]=In_Image[Size-2][1][c];
				IT_Image[Size+1][Size+1][c]=In_Image[Size-2][Size-2][c];
			}

			//the first and last rows
			else if (((pI==0) || (pI==Size+1)) && ((qI!=0) && (qI!=Size+1)))
			{
				if(pI==0)
					IT_Image[pI][qI][c]=In_Image[pI+1][qI-1][c];
				else
					IT_Image[pI][qI][c]=In_Image[pI-3][qI-1][c];
			}

			//the first and last culumns
			else if (((pI!=0) && (pI!=Size+1)) && ((qI==0) || (qI==Size+1)))
			{
				if (qI==0)  
					IT_Image[pI][qI][c]=In_Image[pI-1][qI+1][c];
				else            
					IT_Image[pI][qI][c]=In_Image[pI-1][qI-3][c];
			}

			else
				IT_Image[pI][qI][c]=In_Image[pI-1][qI-1][c];
		}


  ////////////Median Filter for Pepper & Salt//////////
    int N=3;
	int w=0,i=0,j=0;
	//int window[9]={0};
	int *window=new int[N*N];          //store the gray value of all pixel in filter window
	for (w=0;w<N*N;w++)                  //initial window
		window[w]=0;
	int median_order=(int)N*N/2;    //window[median_order]=median
	temp=0;
	for (pI=1;pI<Size+1;pI++)
		for (qI=1;qI<Size+1;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				w=0;
				//put the gray value in window[]
			    for (i=-(int)(N/2);i<1+(int)(N/2);i++)
				    for (j=-(int)(N/2);j<1+(int)(N/2);j++)
					{
						window[w]=IT_Image[pI+i][qI+j][c];
						w++;
					}
				//use Bubble Sort to find the median
				for (i=0;i<N*N;i++)
				{
					for (j=0;j<(N*N-i);j++)
					{
						if (window[j]>window[j+1])
						{
							temp=window[j];
							window[j]=window[j+1];
							window[j+1]=temp;
						}
					}
				}
				//replace the central gray value in the filter window with the median
				OT_Image[pI-(int)(N/2)][qI-(int)(N/2)][c]=window[median_order];
			}

 /////////////////Enlarge the image after Median Filter//////////////
   int IT2_Image[Size+2*R][Size+2*R][BytesPerPixel];                   //the temporal image data of input image(int)
    for (pI=0;pI<Size+2*R;pI++)
		for (qI=0;qI<Size+2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT2_Image[pI][qI][c]=0;
			}
    //the center 512X512
	for (pI=0;pI<Size;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
				IT2_Image[pI+R][qI+R][c]=OT_Image[pI][qI][c];

    //upper boundary
	for (pI=0;pI<R;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
				IT2_Image[R-1-pI][qI+R][c]=OT_Image[pI][qI][c];

    //right boundary
	for (pI=0;pI<Size+R;pI++)
		for (qI=Size;qI<Size+R;qI++)
			for (c=0;c<BytesPerPixel;c++)
				IT2_Image[pI][2*Size+2*R-1-qI][c]=IT2_Image[pI][qI][c];

	//lower boundary
	for (pI=Size;pI<Size+R;pI++)
		for (qI=R;qI<Size+2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
				IT2_Image[2*Size+2*R-1-pI][qI][c]=IT2_Image[pI][qI][c];

	//left boundary
	for (pI=0;pI<Size+2*R;pI++)
		for (qI=R;qI<2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
				IT2_Image[pI][2*R-1-qI][c]=IT2_Image[pI][qI][c];


  /////////////////Gaussian Filter///////////////////////////////
	int pC=0,qC=0;                                                             //the coordinate of the central pixel of filter window
    double sigmaD=R/2;     
	//double sigmaD=3.0;    
	int *windowG=new int[(2*R+1)*(2*R+1)];                   //store the gray value of all pixel in filter windowG
	for (w=0;w<(2*R+1)*(2*R+1);w++)                             //initial windowG
		windowG[w]=0;
	double *weightD=new double [(2*R+1)*(2*R+1)];   //store the weight in filter window(spacial distance)w
	for (w=0;w<(2*R+1)*(2*R+1);w++)                            //initial weightD 
		weightD[w]=0;
	int wD=0;
	double delta=0, x=0, y=0;
	double sumD=0;                                                           //the sum of weight
	double sumC=0;                                                           //the convolution sum 
	w=0;
	
    //compute the weight based on Gaussian Function
	  for (i=-R;i<R+1;i++)
		  for (j=-R;j<R+1;j++)
		  {
			  x=(double)(pC-i);
			  y=(double)(qC-j);
			  delta=sqrt(x*x+y*y)/sigmaD;
			  weightD[wD]=exp((-0.5)*delta*delta)/(2.0*3.1415926*sigmaD);
			  sumD+=weightD[wD];
			  wD++;
		  }
	//normalization of the weight	  
	for (wD=0;wD<(2*R+1)*(2*R+1);wD++)         //initial weight
		weightD[wD]=weightD[wD]/sumD;

	//do the covolution
    for (pI=R;pI<Size+R-1;pI++)
		for (qI=R;qI<Size+R-1;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				w=0;sumC=0.0;
				//put the gray value in window[] and covolute with weightD
			    for (i=-R;i<R+1;i++)
				    for (j=-R;j<R+1;j++)
					{
						windowG[w]=IT2_Image[pI+i][qI+j][c];
						sumC+=(double)windowG[w]*weightD[w];
						w++;
					}
				//replace the central gray value in the filter window with the median
				OTG_Image[pI-R][qI-R][c]=sumC;
			}


  ///////////////////Write image/////////////////////
	ofstream ofile("smoothG_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size;pO++)
		for (qO=0;qO<Size;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				COut_Image[pO][qO][c]=OTG_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}

	ifile.close();
	ofile.close();
	//getchar();
    return 0 ;
}