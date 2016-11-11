//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 1 : (b)
//This code is to obtain the color image of  "parrot_CFA.raw" by MHC Demosaicing
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

 const int BytesPerPixel=3;   //24-bit
 const int Size_W=424;        //the width of input image
 const int Size_H=636;         //the height of input image

int  main()
{
	int pO=0,qO=0;    //coordinate of output
    int pI=0,qI=0;        //coordinate of input
    int c=0;                 //channels
	int sensor=0;       //(sensor==0), RED sensor location;(sensor==1); GREEN sensor location;(sensor==2); BLUE sensor location;
	int row_count=0;     //(row_cout==0), even number of row; (row_cout==1), odd number of row;
	int temp_a=0;          //the partial result of MHC demosaicing formula
	int temp_b=0;          //the partial result of MHC demosaicing formula

    unsigned char In_Image[Size_H][Size_W];                                 //the image data of input image
	int IT_Image[Size_H+4][Size_W+4];                                            //the temporal image data of input image(int)
    int OT_Image[Size_H][Size_W][BytesPerPixel];                         //the temporal image data of output image(int)
	unsigned char COut_Image[Size_H][Size_W][BytesPerPixel];   //the image data of output image(unsigned char)

 ///////////////Read image///////////////
	ifstream ifile("parrot_CFA.raw",ios_base::in | ios_base::binary);
	if (!ifile)
	{
		cout<<"File CANNOT open!"<<endl;
		exit(1);	
	}
    for (pI=0;pI<Size_H;pI++)
		for (qI=0;qI<Size_W;qI++)
				ifile.read((char*)&In_Image[pI][qI],sizeof(In_Image[pI][qI]));
	
  /////////////////Enlarge input image//////////////
   for (pI=0;pI<Size_H+4;pI++)
		for (qI=0;qI<Size_W+4;qI++)
		{
			//the corners of the image
			if ( ((pI==0) || (pI==Size_H+3)) && (qI==0 || (qI==Size_W+3)))
			{
				//the 2 pixels on top left
				IT_Image[0][0]=In_Image[2][2];
				IT_Image[1][1]=In_Image[1][1];
				//the 2 pixels on top right
				IT_Image[0][Size_W+3]=In_Image[2][Size_W-3];
				IT_Image[1][Size_W+2]=In_Image[1][Size_W-2];
                //the 2 pixels on bottom left
				IT_Image[Size_H+3][0]=In_Image[Size_H-3][2];
				IT_Image[Size_H+2][1]=In_Image[Size_H-2][1];
				//the 2 pixels on bottom right
				IT_Image[Size_H+3][Size_W+3]=In_Image[Size_H-3][Size_W-3];
				IT_Image[Size_H+2][Size_W+2]=In_Image[Size_H-2][Size_W-2];
			}

			//the first and last rows
			else if (((pI==0) || (pI==Size_H+3)) && ((qI!=0) && (qI!=Size_W+3)))
			{
				if(pI==0)
				{
					if (qI==1)
						IT_Image[pI][qI]=In_Image[2][1];
					else if (qI==Size_W+2)
						IT_Image[pI][qI]=In_Image[2][Size_W-2];
					else
                       IT_Image[pI][qI]=In_Image[pI+2][qI-2];
				}
				else
				{
					if (qI==1)
						IT_Image[pI][qI]=In_Image[Size_H-3][1];
					else if (qI==Size_W+2)
						IT_Image[pI][qI]=In_Image[Size_H-3][Size_W-2];
					else 
						IT_Image[pI][qI]=In_Image[pI-6][qI-2];
				}
			}

			//the second and last but one rows
			else if (((pI==1) || (pI==Size_H+2)) && ((qI!=1) && (qI!=Size_W+2)))
			{
				if(pI==1)
				{
					if (qI==0)
						IT_Image[pI][qI]=In_Image[1][2];
					else if (qI==Size_W+3)
						IT_Image[pI][qI]=In_Image[1][Size_W-3];
					else
                       IT_Image[pI][qI]=In_Image[pI][qI-2];
				}
				else
				{
					if (qI==0)
						IT_Image[pI][qI]=In_Image[Size_H-2][2];
					else if (qI==Size_W+3)
						IT_Image[pI][qI]=In_Image[Size_H-2][Size_W-3];
					else 
						IT_Image[pI][qI]=In_Image[pI-4][qI-2];
				}
			}

			else
			{
				if (qI==0)
					IT_Image[pI][qI]=In_Image[pI-2][qI+2];
				else if (qI==1)
                    IT_Image[pI][qI]=In_Image[pI-2][qI];
				else if (qI==Size_W+2)
                    IT_Image[pI][qI]=In_Image[pI-2][qI-4];
				else if (qI==Size_W+3)
					IT_Image[pI][qI]=In_Image[pI-2][qI-6];
				else
					IT_Image[pI][qI]=In_Image[pI-2][qI-2];
			}
		}
	
/////////////////Demosacing//////////////////////// 
    sensor=0;         //start from RED sensor location
	row_count=0;   //start from even number of row
	pI=1;qI=1;         //start from (1,1) in the enlarged image
	for (pO=0;pO<Size_H;pO++) 
	{
		//even number of row
		if (row_count==0)   
		{
			sensor=0;   //start from R
			for (qO=0;qO<Size_W;qO++)
			{
				//mapping in the enlarged image
				pI=pO+2;qI=qO+2;
				//at RED sensor location
				if (sensor==0)
				{
					c=0;OT_Image[pO][qO][c]=0;
					//R at R
			        OT_Image[pO][qO][c]=IT_Image[pI][qI];c++;
				    //G at R
					temp_a=4*IT_Image[pI][qI]+2*(IT_Image[pI-1][qI]+IT_Image[pI+1][qI]+IT_Image[pI][qI-1]+IT_Image[pI][qI+1]);
					temp_b=IT_Image[pI-2][qI]+IT_Image[pI+2][qI]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
					c++;
			        //B at R
					temp_a=6*IT_Image[pI][qI]+2*(IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]);
					temp_b=3*(IT_Image[pI-2][qI]+IT_Image[pI+2][qI]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2])/2;
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
				    sensor=1;   //next is G
				}
				//at GREEN sensor location
				else 
				{
					c=0;OT_Image[pO][qO][c]=0;
			        //R at G
					temp_a=5*IT_Image[pI][qI]+4*(IT_Image[pI][qI-1]+IT_Image[pI][qI+1])+(IT_Image[pI-2][qI]+IT_Image[pI+2][qI])/2;
					temp_b=IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
					c++;
				    //G at G
                    OT_Image[pO][qO][c]=IT_Image[pI][qI];c++;
			        //B at G
					temp_a=5*IT_Image[pI][qI]+4*(IT_Image[pI-1][qI]+IT_Image[pI+1][qI])+(IT_Image[pI][qI-2]+IT_Image[pI][qI+2])/2;
					temp_b=IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]+IT_Image[pI-2][qI]+IT_Image[pI+2][qI];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;			  
				    sensor=0;   //next is R
				}
			}
		}

		//odd number of row
		else
		{
			sensor=1;   //start from G
			for (qO=0;qO<Size_W;qO++)
			{
				//mapping in the enlarged image
				pI=pO+2;qI=qO+2;
				//at GREEN sensor location
				if (sensor==1)
				{
					c=0;OT_Image[pO][qO][c]=0;
			        //R at G
					temp_a=5*IT_Image[pI][qI]+4*(IT_Image[pI-1][qI]+IT_Image[pI+1][qI])+(IT_Image[pI][qI-2]+IT_Image[pI][qI+2])/2;
					temp_b=IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]+IT_Image[pI-2][qI]+IT_Image[pI+2][qI];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
					c++;
				    //G at G
                    OT_Image[pO][qO][c]=IT_Image[pI][qI];c++;
			        //B at G
					temp_a=5*IT_Image[pI][qI]+4*(IT_Image[pI][qI-1]+IT_Image[pI][qI+1])+(IT_Image[pI-2][qI]+IT_Image[pI+2][qI])/2;
					temp_b=IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
				    sensor=2;   //next is B
				}
				//at BLUE sensor location
				else
				{
					c=0;OT_Image[pO][qO][c]=0;
			        //R at B
					temp_a=6*IT_Image[pI][qI]+2*(IT_Image[pI-1][qI-1]+IT_Image[pI-1][qI+1]+IT_Image[pI+1][qI-1]+IT_Image[pI+1][qI+1]);
					temp_b=3*(IT_Image[pI-2][qI]+IT_Image[pI+2][qI]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2])/2;
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
					c++;
				    //G at B
					temp_a=4*IT_Image[pI][qI]+2*(IT_Image[pI-1][qI]+IT_Image[pI+1][qI]+IT_Image[pI][qI-1]+IT_Image[pI][qI+1]);
					temp_b=IT_Image[pI-2][qI]+IT_Image[pI+2][qI]+IT_Image[pI][qI-2]+IT_Image[pI][qI+2];
					if (temp_a>temp_b)
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					else if (temp_a<temp_b)
						OT_Image[pO][qO][c]=(temp_b-temp_a)/8;
					else
						OT_Image[pO][qO][c]=(temp_a-temp_b)/8;
					if (OT_Image[pO][qO][c]>255)
						OT_Image[pO][qO][c]=255;
					c++;
			        //B at B
			        OT_Image[pO][qO][c]=IT_Image[pI][qI];
				    sensor=1;   //next is G
				}
			}
		}
        //go to next row
		if (row_count==0)
			row_count=1;
		else
			row_count=0;
	}
 
  ///////////////////Write image/////////////////////
	ofstream ofile("MHC_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size_H;pO++)
		for (qO=0;qO<Size_W;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				COut_Image[pO][qO][c]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}
	ifile.close();
	ofile.close();
    return 0 ;
}