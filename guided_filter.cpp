//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 3 : (b)
//This code is using Guided Image Filtering to remove noise in the image
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

const int BytesPerPixel=3;   //24-bit
const int Size=512;               //the size of input image
const int N=3;
const int R=8;                        //radius
const double e=2500;             //regularization


int  main()
{
	
	int pO=0,qO=0;                              //coordinate of output
    int pI=0,qI=0;
    int c=0,chnn=0;                              //channels. 0, RED channel;1,GREEN channel;2,BLUE channel;

    unsigned char In_Image[Size][Size][BytesPerPixel];        //the image data of input image
	int IM_Image[Size+2][Size+2][BytesPerPixel];                    //the temporal image data of input image(int)
	int IT_Image[Size+2*R][Size+2*R][BytesPerPixel];            //the temporal image data of input image(int)
	int IG_Image[Size+2*R][Size+2*R][BytesPerPixel];            //the temporal image data of guided image(int)
	int OT_Image[Size][Size][BytesPerPixel];                           //the temporal image data of output image(int)
	unsigned char COut_Image[Size][Size][BytesPerPixel];   //the image data of output image(unsigned char)
	unsigned char COut_RImage[Size][Size];                           //the image data of output image(R channel)
	unsigned char COut_GImage[Size][Size];                           //the image data of output image(G channel)
	unsigned char COut_BImage[Size][Size];                           //the image data of output image(B channel)


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
				IM_Image[0][0][c]=In_Image[1][1][c];
				IM_Image[0][Size+1][c]=In_Image[1][Size-2][c];
				IM_Image[Size+1][0][c]=In_Image[Size-2][1][c];
				IM_Image[Size+1][Size+1][c]=In_Image[Size-2][Size-2][c];
			}

			//the first and last rows
			else if (((pI==0) || (pI==Size+1)) && ((qI!=0) && (qI!=Size+1)))
			{
				if(pI==0)
					IM_Image[pI][qI][c]=In_Image[pI+1][qI-1][c];
				else
					IM_Image[pI][qI][c]=In_Image[pI-3][qI-1][c];
			}

			//the first and last culumns
			else if (((pI!=0) && (pI!=Size+1)) && ((qI==0) || (qI==Size+1)))
			{
				if (qI==0)  
					IM_Image[pI][qI][c]=In_Image[pI-1][qI+1][c];
				else            
					IM_Image[pI][qI][c]=In_Image[pI-1][qI-3][c];
			}

			else
				IM_Image[pI][qI][c]=In_Image[pI-1][qI-1][c];
		}


  ////////////Median Filter for Pepper & Salt//////////
    
	int w=0,i=0,j=0;
	int window[N*N]={0};   //store the gray value of all pixel in filter window
	int median_order=(int)N*N/2;            //window[median_order]=median
	int temp_M=0;
	for (pI=1;pI<Size+1;pI++)
		for (qI=1;qI<Size+1;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				w=0;
				//put the gray value in window[]
			    for (i=-(int)(N/2);i<1+(int)(N/2);i++)
				    for (j=-(int)(N/2);j<1+(int)(N/2);j++)
					{
						window[w]=IM_Image[pI+i][qI+j][c];
						w++;
					}
				//use Bubble Sort to find the median
				for (i=0;i<N*N;i++)
				{
					for (j=0;j<(N*N-i);j++)
					{
						if (window[j]>window[j+1])
						{
							temp_M=window[j];
							window[j]=window[j+1];
							window[j+1]=temp_M;
						}
					}
				}
				//replace the central gray value in the filter window with the median
				In_Image[pI-(int)(N/2)][qI-(int)(N/2)][c]=window[median_order];
			}

	/////////////////Enlarge input image//////////////
   
    for (pI=0;pI<Size+2*R;pI++)
		for (qI=0;qI<Size+2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[pI][qI][c]=0;
				IG_Image[pI][qI][c]=0;
			}
    //the center 512X512
	for (pI=0;pI<Size;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[pI+R][qI+R][c]=In_Image[pI][qI][c];
				IG_Image[pI+R][qI+R][c]=In_Image[pI][qI][c];
			}
    //upper boundary
	for (pI=0;pI<R;pI++)
		for (qI=0;qI<Size;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[R-1-pI][qI+R][c]=In_Image[pI][qI][c];
				IG_Image[R-1-pI][qI+R][c]=In_Image[pI][qI][c];
			}
    //right boundary
	for (pI=0;pI<Size+R;pI++)
		for (qI=Size;qI<Size+R;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[pI][2*Size+2*R-1-qI][c]=IT_Image[pI][qI][c];
				IG_Image[pI][2*Size+2*R-1-qI][c]=IG_Image[pI][qI][c];
			}
	//lower boundary
	for (pI=Size;pI<Size+R;pI++)
		for (qI=R;qI<Size+2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[2*Size+2*R-1-pI][qI][c]=IT_Image[pI][qI][c];
				IG_Image[2*Size+2*R-1-pI][qI][c]=IG_Image[pI][qI][c];
			}
	//left boundary
	for (pI=0;pI<Size+2*R;pI++)
		for (qI=R;qI<2*R;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				IT_Image[pI][2*R-1-qI][c]=IT_Image[pI][qI][c];
				IG_Image[pI][2*R-1-qI][c]=IG_Image[pI][qI][c];
			}


  //////////////////////Guided Filter//////////////////////////
	//int w=0,i=0,j=0;
	//int p=0,q=0,cc=0;
	double temp=0;
    double mean_In=0,mean_Gu=0;   //the mean of input and guiede image in a window
	double sigma_Gu=0;   //the variance of the guided image in a window
	double corr_IG=0;         //the correlation of input and guiede image in a window
	double In_sum=0;         //the sum of the production of input image in a window
	double Gu_sum=0;         //the sum of the production of guided image in a window
	double corr_IG_sum=0;         //the sum of the production of input and guided image in a window
	double sigma_Gu_sum=0; 
    //the coefficients a and b
	double a=0,b=0;
	//double a_sum[Size+2*R][Size+2*R][BytesPerPixel]={0};
	//double b_sum[Size+2*R][Size+2*R][BytesPerPixel]={0};
	int a_count[Size+2*R][Size+2*R][BytesPerPixel]={0};
	int b_count[Size+2*R][Size+2*R][BytesPerPixel]={0};
	double mean_a[Size+2*R][Size+2*R][BytesPerPixel]={0};
	double mean_b[Size+2*R][Size+2*R][BytesPerPixel]={0};
   
	for (pI=R;pI<Size+R-1;pI++)
		for (qI=R;qI<Size+R-1;qI++)
			 for (c=0;c<BytesPerPixel;c++)
			 {
				 //initial the variances
				 w=0;
				 mean_In=0;mean_Gu=0; 
	             sigma_Gu=0;  
				 corr_IG=0; 
				 In_sum=0;Gu_sum=0;
                 corr_IG_sum=0;
				 sigma_Gu_sum=0;
				 //computation
			     for (i=-R;i<1+R;i++)
				     for (j=-R;j<1+R;j++)
					 {
						 In_sum+=IT_Image[pI+i][qI+j][c];
						 Gu_sum+=IG_Image[pI+i][qI+j][c];
						 corr_IG_sum+=IT_Image[pI+i][qI+j][c]*IG_Image[pI+i][qI+j][c];        
	                    //corr_GG[w]=IG_Image[pI+i][qI+j][c]*IG_Image[pI+i][qI+j][c]; 
					 }
				 mean_In=In_sum/((2*R+1)*(2*R+1));
				 mean_Gu=Gu_sum/((2*R+1)*(2*R+1));
				 corr_IG=corr_IG_sum/((2*R+1)*(2*R+1));
				 for (i=-R;i<1+R;i++)
				     for (j=-R;j<1+R;j++)
						 sigma_Gu_sum+=(IG_Image[pI+i][qI+j][c]-mean_Gu)*(IG_Image[pI+i][qI+j][c]-mean_Gu);
				 sigma_Gu=sigma_Gu_sum/((2*R+1)*(2*R+1));

				 a=(corr_IG-mean_In*mean_Gu)/(sigma_Gu+e);
				 b=mean_In-a*mean_Gu;

				 for (i=-R;i<1+R;i++)
				     for (j=-R;j<1+R;j++)
					 {
						 mean_a[pI+i][qI+j][c]+=a;
						 mean_b[pI+i][qI+j][c]+=b;
						 a_count[pI+i][qI+j][c]+=1;
						 b_count[pI+i][qI+j][c]+=1;
					 }
			 }

    for (pI=R;pI<Size+R-1;pI++)
		for (qI=R;qI<Size+R-1;qI++)
			for (c=0;c<BytesPerPixel;c++)
			{
				mean_a[pI][qI][c]=mean_a[pI][qI][c]/a_count[pI][qI][c];
			    mean_b[pI][qI][c]=mean_b[pI][qI][c]/b_count[pI][qI][c];
			    OT_Image[pI-R][qI-R][c]=mean_a[pI][qI][c]*IG_Image[pI][qI][c]+mean_b[pI][qI][c];
			}
			

  ///////////////////Write image/////////////////////
	ofstream tfile("smoothGIF_image.raw",ios_base::out | ios_base::binary);
	if (!tfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	ofstream rfile("smoothGIF_Rimage.raw",ios_base::out | ios_base::binary);
	if (!rfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	ofstream gfile("smoothGIF_Gimage.raw",ios_base::out | ios_base::binary);
	if (!gfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	ofstream bfile("smoothGIF_Bimage.raw",ios_base::out | ios_base::binary);
	if (!bfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size;pO++)
		for (qO=0;qO<Size;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				COut_Image[pO][qO][c]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				tfile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));

				if (c==0)
				{
					COut_RImage[pO][qO]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				    rfile.write((char*)&COut_RImage[pO][qO],sizeof(COut_RImage[pO][qO]));
				}
				else if (c==1)
				{
					COut_GImage[pO][qO]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				    gfile.write((char*)&COut_GImage[pO][qO],sizeof(COut_GImage[pO][qO]));
				}
				else
				{
					COut_BImage[pO][qO]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				    bfile.write((char*)&COut_BImage[pO][qO],sizeof(COut_BImage[pO][qO]));
				}

			}
	ifile.close();
	rfile.close();
	gfile.close();
	bfile.close();
    tfile.close();
	//getchar();
    return 0 ;
}