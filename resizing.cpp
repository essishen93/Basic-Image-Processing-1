//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 1 : (a)
//This code is resizing the image  "the_starry_night.raw"
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

 const int BytesPerPixel=3; //24-bit
 const int Size_In=512;        //the size of input image
 const int Size_Out=650;    //the size of input image

int  main()
{
	int pO=0,qO=0;    //coordinate of output
    int pI=0,qI=0;        //coordinate of input
    int c=0;                 //channels

    unsigned char In_Image[Size_In][Size_In][BytesPerPixel];               //the image data of input image
	unsigned char COut_Image[Size_Out][Size_Out][BytesPerPixel];  //the image data of output image(unsigned char)
    int Out_Image[Size_Out][Size_Out][BytesPerPixel];                        //the temporal image data of output image(int)
	
 ///////////////Read image///////////////
	ifstream ifile("the_starry_night.raw",ios_base::in | ios_base::binary);
	if (!ifile)
	{
		cout<<"File CANNOT open!"<<endl;
		exit(1);	
	}
    for (pI=0;pI<Size_In;pI++)
		for (qI=0;qI<Size_In;qI++)
			for (c=0;c<BytesPerPixel;c++)
				ifile.read((char*)&In_Image[pI][qI][c],sizeof(In_Image[pI][qI][c]));

 /////////////////Resizing//////////////////////// 
	unsigned short a,b;         //coefficients in bilinear interpolation
    unsigned short m,n;       //coefficients in bilinear interpolation (on edge)

	for (pO=0;pO<Size_Out;++pO)
		for (qO=0;qO<Size_Out;++qO)
			for (c=0;c<BytesPerPixel;++c)
			{
                Out_Image[pO][qO][c]=0;   //initialization

				//the four corners of the image
				if ( ((pO==0) || (pO==Size_Out-1)) && (qO==0 || (qO==Size_Out-1)))
				{
					Out_Image[0][0][c]=In_Image[0][0][c];
				    Out_Image[0][Size_Out-1][c]=In_Image[0][Size_In-1][c];
				    Out_Image[Size_Out-1][0][c]=In_Image[Size_In-1][0][c];
				    Out_Image[Size_Out-1][Size_Out-1][c]=In_Image[Size_In-1][Size_In-1][c];
				}   
				
				//the first and last rows
				else if (((pO==0) || (pO==Size_Out-1)) && ((qO!=0) && (qO!=Size_Out-1)))
				{
					//mapping and find the start of the area in original image
					if (pO==0)  pI=0;
				    else            pI=Size_In-1;
					qI=qO*Size_In/Size_Out; 
					//if the mapping of one pixel is on a corner of input image
					if (qI==Size_In-1)
						Out_Image[pO][qO][c]=In_Image[pI][Size_In-1][c];
					else
					{
	                    m=(unsigned short)qO*Size_In/Size_Out-qI;                                                 //calculate coefficients of Bilinear Interpolation
					    Out_Image[pO][qO][c]=(1-m)*In_Image[pI][qI][c]+m*In_Image[pI][qI+1][c];   //Bilinear Interpolation
					}
				}
				
				//the first and last culumns
				else if (((pO!=0) && (pO!=Size_Out-1)) && ((qO==0) || (qO==Size_Out-1)))
				{
					//mapping and find the start of the area in original image
					if (qO==0)  qI=0;
				    else            qI=Size_In-1;
				    pI=pO*Size_In/Size_Out;
					//if the mapping of one pixel is on a corner of input image
					if (pI==Size_In-1)
						Out_Image[pO][qO][c]=In_Image[Size_In-1][qI][c];
					else
					{
						n=(unsigned short)pO*Size_In/Size_Out-pI;                                                //calculate coefficients of Bilinear Interpolation
						Out_Image[pO][qO][c]=(1-n)*In_Image[pI][qI][c]+n*In_Image[pI+1][qI][c];   //Bilinear Interpolation
					}
				}
				
				else
				{
					//mapping and find the start of the area in original image
					pI=pO*Size_In/Size_Out;
					qI=qO*Size_In/Size_Out;    
					//calculate coefficients of Bilinear Interpolation
					a=(unsigned short)pO*Size_In/Size_Out-pI;   
	                b=(unsigned short)qO*Size_In/Size_Out-qI;
					//Bilinear Interpolation
					Out_Image[pO][qO][c]=(1-a)*(1-b)*In_Image[pI][qI][c]+(1-a)*b*In_Image[pI][qI+1][c]+a*(1-b)*In_Image[pI+1][qI][c]+a*b*In_Image[pI+1][qI+1][c];    
				}
			}
			//cout<<c<<endl;
            
  ///////////////////Write image/////////////////////
	ofstream ofile("resizing_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size_Out;pO++)
		for (qO=0;qO<Size_Out;qO++)
			for (c=3;c<BytesPerPixel+3;c++)
			{
				COut_Image[pO][qO][c]=Out_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}
	ifile.close();
	ofile.close();
    //getchar();
    return 0 ;
}