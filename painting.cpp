//Name: Maoru Shen
//USC ID Number: 4946-8507-13
//USC Email: maorushe@usc.edu
//Submission Date: 09/20/2015
////////////////////////////////////////////////////////////////////
//Problem 2 : (b)
//This code is the oil painting process of "barn.raw"
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

 const int BytesPerPixel=3;   //24-bit
 const int Size_W=380;        //the width of input image
 const int Size_H=275;         //the height of input image
 const int Level=256;             //the number of grey levels before quantification

int  main()
{
	int pO=0,qO=0;                                    //coordinate of output
    int pI=0,qI=0, xR=0,xG=0,xB=0;         //coordinate of input and its components RGB
    int c=0,chnn=0;                                    //channels. 0, RED channel;1,GREEN channel;2,BLUE channel;
	int lb=0;                                                 //counting for array Level_Bar
	int Fmax,Fmin;                                     //the maximum and minimum grey level in each channel
	int temp=0;                                           //temporary data saving
	int level=0;                                            //temporary grey level value saving
	//the 4 quantified grey levels in each channel
	int R_level[4];
	int G_level[4];
	int B_level[4];
	double R_freq[Level]={0.0};
	double G_freq[Level]={0.0};
	double B_freq[Level]={0.0};
	//define a 1D array consists of R,G and B of a pixel used for comparing
	int tp[3]={0};

    unsigned char In_Image[Size_H][Size_W][BytesPerPixel];                  //the image data of input image
	int RT_Image[Size_H*Size_W];                                                               //the temporal image data of input image(R channel)
	int GT_Image[Size_H*Size_W];                                                               //the temporal image data of input image(G channel)
	int BT_Image[Size_H*Size_W];                                                                //the temporal image data of input image(B channel)
	int Level_Bar[Size_H*Size_W];                                                                //the sequence of grey level(0--255)
	int IT_Image[Size_H+4][Size_W+4][BytesPerPixel];                              //the temporal image data of input image(int)
    int OT_Image[Size_H][Size_W][BytesPerPixel];                                   //the temporal image data of output image(int)
	unsigned char COut_Image[Size_H+2][Size_W+2][BytesPerPixel];   //the image data of output image(unsigned char)

  ////////////////////Choose the size of image filter/window///////////////////
	int N;     //size of image filter/window
	int chk=0;  //check the choise
	char x;
	for (chk=0;chk<1;)
	{
	cout<<"please choose the size of window:"<<endl;
    cout<<"a)3*3"<<"            "<<"b)5*5"<<endl;
	cin>>x;
	if (x=='a')
	{N=3;chk=1;}
	else if (x=='b')
		{N=5;chk=1;}
	else
		chk=0;
	}
	//define a 2D array has N*N sub-vector consisting of R,G and B of a pixel in N*N window
	int **comp=new int*[N*N];
	for (chk=0;chk<N*N;chk++)
		comp[chk]=new int[N];

 ///////////////Read image///////////////
	ifstream ifile("barn.raw",ios_base::in | ios_base::binary);
	//ifstream ifile("coliseum.raw",ios_base::in | ios_base::binary);
	if (!ifile)
	{
		cout<<"File CANNOT open!"<<endl;
		exit(1);	
	}
    for (pI=0;pI<Size_H;pI++)
		for (qI=0;qI<Size_W;qI++)
			for (c=0;c<BytesPerPixel;c++)
				ifile.read((char*)&In_Image[pI][qI][c],sizeof(In_Image[pI][qI][c]));

/////////////////Divide input image into R, G and B//////////////
	for (pI=0;pI<Size_H;pI++)
		for (qI=0;qI<Size_W;qI++)
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


  /////////////////Changing to 64-color image/////////////////
    int count=0;   //record the number of pixel at each grey level
    double sumF=0.0;   //sum of fequency
	int Level_Thrh[3]={0};   //3 thresholds to divide [0,255] into 4 parts
	double sum1=0.0;   //numerator in calculation of finding 4 quantified grey level
	double sum2=0.0;   //denominator in calculation of finding 4 quantified grey level
	
	//R channel
	//find MAX and MIN grey level
	xR=0;
	Fmax=RT_Image[xR],Fmin=RT_Image[xR];
	for (xR=0;xR<Size_H*Size_W;xR++)
	{
		temp=RT_Image[xR];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
	//cout<<Fmax<<"   "<<Fmin<<endl;
	//sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size_H*Size_W;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
            count=0;
			for (xR=0;xR<Size_H*Size_W;xR++)
			{
				if (RT_Image[xR]==level)
				{
					Level_Bar[lb]=xR; 
					lb++;
					count++;
				}
			}
			R_freq[level]=(double)count/(double)(Size_H*Size_W);
			//cout<<level<<"   "<<count<<"   "<<R_freq[level]<<endl;
		}
	}
	//find the 3 grey levels to divide [0,255] into 4 parts
	sumF=0.0;
	for (level=0;level<256;level++)
	{
			sumF=sumF+R_freq[level];
			if (abs(sumF-0.25)<0.003)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[0]=level;
			}
			if (abs(sumF-0.5)<0.003)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[1]=level;
			}
			if (abs(sumF-0.75)<0.009)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[2]=level;
			}
	}
	Level_Thrh[2]=210;       //modify the threshold from experience

	//find the 4 quantified grey levels
	//by calculate the mean of each part
	sum1=0.0;sum2=0.0;
	lb=0;
	while (RT_Image[Level_Bar[lb]]<(Level_Thrh[0]+1))
	{
		sum1=sum1+(float)RT_Image[Level_Bar[lb]]*R_freq[RT_Image[Level_Bar[lb]]];
		sum2=sum2+R_freq[RT_Image[Level_Bar[lb]]];
		lb++;
	}
	R_level[0]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (RT_Image[Level_Bar[lb]]<(Level_Thrh[1]+1))
	{
		sum1=sum1+(float)RT_Image[Level_Bar[lb]]*R_freq[RT_Image[Level_Bar[lb]]];
		sum2=sum2+R_freq[RT_Image[Level_Bar[lb]]];
		lb++;
	}
	R_level[1]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (RT_Image[Level_Bar[lb]]<(Level_Thrh[2]+1))
	{
		sum1=sum1+(float)RT_Image[Level_Bar[lb]]*R_freq[RT_Image[Level_Bar[lb]]];
		sum2=sum2+R_freq[RT_Image[Level_Bar[lb]]];
		lb++;
	}
	//R_level[2]=sum1/sum2;
	R_level[2]=160;
	sum1=0.0;sum2=0.0;
	while (lb<Size_H*Size_W)
	{
		sum1=sum1+(float)RT_Image[Level_Bar[lb]]*R_freq[RT_Image[Level_Bar[lb]]];
		sum2=sum2+R_freq[RT_Image[Level_Bar[lb]]];
		lb++;
	}
	R_level[3]=sum1/sum2;
	cout<<"4 quantified gray value of R:"<<endl;
    cout<<R_level[0]<<endl;
	cout<<R_level[1]<<endl;
	cout<<R_level[2]<<endl;
	cout<<R_level[3]<<endl;
    
	//Quantification	
	for (xR=0;xR<Size_H*Size_W;xR++)
	{
		if (RT_Image[xR]<(Level_Thrh[0]+1))
			RT_Image[xR]=R_level[0];
		else if ( (RT_Image[xR]>Level_Thrh[0])  &&  (RT_Image[xR]<(Level_Thrh[1]+1)) )  
			RT_Image[xR]=R_level[1];
		else if ( (RT_Image[xR]>Level_Thrh[1])  &&  (RT_Image[xR]<(Level_Thrh[2]+1)) )  
			RT_Image[xR]=R_level[2];
		else 
			RT_Image[xR]=R_level[3];
	}

    //G channel
	//find MAX and MIN grey level
	xG=0;
	Fmax=GT_Image[xG],Fmin=GT_Image[xG];
	for (xG=0;xG<Size_H*Size_W;xG++)
	{
		temp=GT_Image[xG];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
	//cout<<Fmax<<"   "<<Fmin<<endl;
	//sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size_H*Size_W;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
            count=0;
			for (xG=0;xG<Size_H*Size_W;xG++)
			{
				if (GT_Image[xG]==level)
				{
					Level_Bar[lb]=xG; 
					lb++;
					count++;
				}
			}
			G_freq[level]=(double)count/(double)(Size_H*Size_W);
			//cout<<level<<"   "<<count<<"   "<<G_freq[level]<<endl;
		}
	}
	//find the 3 grey levels to divide [0,255] into 4 parts
	sumF=0.0;
	for (level=0;level<256;level++)
	{
			sumF=sumF+G_freq[level];
			if (abs(sumF-0.25)<0.001)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[0]=level;
			}
			if (abs(sumF-0.5)<0.004)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[1]=level;
			}
			if (abs(sumF-0.75)<0.009)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[2]=level;
			}
	}
	Level_Thrh[2]=220;     //modify the threshold from experience

	//find the 4 quantified grey levels
	//by calculate the mean of each part
	sum1=0.0;sum2=0.0;
	lb=0;
	while (GT_Image[Level_Bar[lb]]<(Level_Thrh[0]+1))
	{
		sum1=sum1+(float)GT_Image[Level_Bar[lb]]*G_freq[GT_Image[Level_Bar[lb]]];
		sum2=sum2+G_freq[GT_Image[Level_Bar[lb]]];
		lb++;
	}
	G_level[0]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (GT_Image[Level_Bar[lb]]<(Level_Thrh[1]+1))
	{
		sum1=sum1+(float)GT_Image[Level_Bar[lb]]*G_freq[GT_Image[Level_Bar[lb]]];
		sum2=sum2+G_freq[GT_Image[Level_Bar[lb]]];
		lb++;
	}
	G_level[1]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (GT_Image[Level_Bar[lb]]<(Level_Thrh[2]+1))
	{
		sum1=sum1+(float)GT_Image[Level_Bar[lb]]*G_freq[GT_Image[Level_Bar[lb]]];
		sum2=sum2+G_freq[GT_Image[Level_Bar[lb]]];
		lb++;
	}
	//G_level[2]=sum1/sum2;
	G_level[2]=193;
	sum1=0.0;sum2=0.0;
	while (lb<Size_H*Size_W)
	{
		sum1=sum1+(float)GT_Image[Level_Bar[lb]]*G_freq[GT_Image[Level_Bar[lb]]];
		sum2=sum2+G_freq[GT_Image[Level_Bar[lb]]];
		lb++;
	}
	G_level[3]=sum1/sum2;
	cout<<"4 quantified gray value of G:"<<endl;
    cout<<G_level[0]<<endl;
	cout<<G_level[1]<<endl;
	cout<<G_level[2]<<endl;
	cout<<G_level[3]<<endl;

	//Quantification	
	for (xG=0;xG<Size_H*Size_W;xG++)
	{
		if (GT_Image[xG]<(Level_Thrh[0]+1))
			GT_Image[xG]=G_level[0];
		else if ( (GT_Image[xG]>Level_Thrh[0])  &&  (GT_Image[xG]<(Level_Thrh[1]+1)) )  
			GT_Image[xG]=G_level[1];
		else if ( (GT_Image[xG]>Level_Thrh[1])  &&  (GT_Image[xG]<(Level_Thrh[2]+1)) )  
			GT_Image[xG]=G_level[2];
		else 
			GT_Image[xG]=G_level[3];
	}

	//B channel
	//find MAX and MIN grey level
	xB=0;
	Fmax=BT_Image[xB],Fmin=BT_Image[xB];
	for (xB=0;xB<Size_H*Size_W;xB++)
	{
		temp=BT_Image[xB];
		if (temp>Fmax)
			Fmax=temp;
		if (temp<Fmin)
			Fmin=temp;
	}
	//cout<<Fmax<<"   "<<Fmin<<endl;
	//sorting grey level in increased order
	//put the order in Level_Bar
	for (lb=0;lb<Size_H*Size_W;)
	{
		for (level=Fmin;level<(Fmax+1);level++)
		{
            count=0;
			for (xB=0;xB<Size_H*Size_W;xB++)
			{
				if (BT_Image[xB]==level)
				{
					Level_Bar[lb]=xB; 
					lb++;
					count++;
				}
			}
			B_freq[level]=(double)count/(double)(Size_H*Size_W);
			//cout<<level<<"   "<<count<<"   "<<B_freq[level]<<endl;
		}
	}
	//find the 3 grey levels to divide [0,255] into 4 parts
	sumF=0.0;
	for (level=0;level<256;level++)
	{
			sumF=sumF+B_freq[level];
			if (abs(sumF-0.25)<0.002)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[0]=level;
			}
			if (abs(sumF-0.5)<0.0004)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[1]=level;
			}
			if (abs(sumF-0.75)<0.003)
			{
				//cout<<sumF<<endl;
				//cout<<level<<endl;
				Level_Thrh[2]=level;
			}
	}

    //find the 4 quantified grey levels
	//by calculate the mean of each part
	sum1=0.0;sum2=0.0;
	lb=0;
	while (BT_Image[Level_Bar[lb]]<(Level_Thrh[0]+1))
	{
		sum1=sum1+(float)BT_Image[Level_Bar[lb]]*B_freq[BT_Image[Level_Bar[lb]]];
		sum2=sum2+B_freq[BT_Image[Level_Bar[lb]]];
		lb++;
	}
	B_level[0]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (BT_Image[Level_Bar[lb]]<(Level_Thrh[1]+1))
	{
		sum1=sum1+(float)BT_Image[Level_Bar[lb]]*B_freq[BT_Image[Level_Bar[lb]]];
		sum2=sum2+B_freq[BT_Image[Level_Bar[lb]]];
		lb++;
	}
	B_level[1]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (BT_Image[Level_Bar[lb]]<(Level_Thrh[2]+1))
	{
		sum1=sum1+(float)BT_Image[Level_Bar[lb]]*B_freq[BT_Image[Level_Bar[lb]]];
		sum2=sum2+B_freq[BT_Image[Level_Bar[lb]]];
		lb++;
	}
	B_level[2]=sum1/sum2;
	sum1=0.0;sum2=0.0;
	while (lb<Size_H*Size_W)
	{
		sum1=sum1+(float)BT_Image[Level_Bar[lb]]*B_freq[BT_Image[Level_Bar[lb]]];
		sum2=sum2+B_freq[BT_Image[Level_Bar[lb]]];
		lb++;
	}
	B_level[3]=sum1/sum2;
	cout<<"4 quantified gray value of B:"<<endl;
    cout<<B_level[0]<<endl;
	cout<<B_level[1]<<endl;
	cout<<B_level[2]<<endl;
	cout<<B_level[3]<<endl;

	//Quantification	
	for (xB=0;xB<Size_H*Size_W;xB++)
	{
		if (BT_Image[xB]<(Level_Thrh[0]+1))
			BT_Image[xB]=B_level[0];
		else if ( (BT_Image[xB]>Level_Thrh[0])  &&  (BT_Image[xB]<(Level_Thrh[1]+1)) )  
			BT_Image[xB]=B_level[1];
		else if ( (BT_Image[xB]>Level_Thrh[1])  &&  (BT_Image[xB]<(Level_Thrh[2]+1)) )  
			BT_Image[xB]=B_level[2];
		else 
			BT_Image[xB]=B_level[3];
	}

  /////////////////Combine R, G and B to be new 64-color input image//////////////
	xR=0;xG=0;xB=0;
    for (pO=0;pO<Size_H;pO++)
		for (qO=0;qO<Size_W;qO++)
			for (c=0;c<BytesPerPixel;c++)
			{
				chnn=c;
				if (chnn==0)
				{
					In_Image[pO][qO][c]=RT_Image[xR];
					xR++;
				}
				else if (chnn==1)
				{
					In_Image[pO][qO][c]=GT_Image[xG];
					xG++;
				}
				else
				{
					In_Image[pO][qO][c]=BT_Image[xB];
					xB++;
				}
			}
  ////////////////////////Write 64-color image/////////////////////////////
	ofstream tfile("quan64_barn_image.raw",ios_base::out | ios_base::binary);
	if (!tfile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size_H;pO++)
		for (qO=0;qO<Size_W;qO++)
			for (c=3;c<BytesPerPixel+3;c++)
			{
				COut_Image[pO][qO][c]=In_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				tfile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}

	
   /////////////////Enlarge input image//////////////
   for (pI=0;pI<Size_H+4;pI++)
		for (qI=0;qI<Size_W+4;qI++)
			for (c=3;c<BytesPerPixel+3;c++)
		{
			//the corners of the image
			if ( ((pI==0) || (pI==Size_H+3)) && (qI==0 || (qI==Size_W+3)))
			{
				//the 2 pixels on top left
				IT_Image[0][0][c]=In_Image[2][2][c];
				IT_Image[1][1][c]=In_Image[1][1][c];
				//the 2 pixels on top right
				IT_Image[0][Size_W+3][c]=In_Image[2][Size_W-3][c];
				IT_Image[1][Size_W+2][c]=In_Image[1][Size_W-2][c];
                //the 2 pixels on bottom left
				IT_Image[Size_H+3][0][c]=In_Image[Size_H-3][2][c];
				IT_Image[Size_H+2][1][c]=In_Image[Size_H-2][1][c];
				//the 2 pixels on bottom right
				IT_Image[Size_H+3][Size_W+3][c]=In_Image[Size_H-3][Size_W-3][c];
				IT_Image[Size_H+2][Size_W+2][c]=In_Image[Size_H-2][Size_W-2][c];
			}

			//the first and last rows
			else if (((pI==0) || (pI==Size_H+3)) && ((qI!=0) && (qI!=Size_W+3)))
			{
				if(pI==0)
				{
					if (qI==1)
						IT_Image[pI][qI][c]=In_Image[2][1][c];
					else if (qI==Size_W+2)
						IT_Image[pI][qI][c]=In_Image[2][Size_W-2][c];
					else
                       IT_Image[pI][qI][c]=In_Image[pI+2][qI-2][c];
				}
				else
				{
					if (qI==1)
						IT_Image[pI][qI][c]=In_Image[Size_H-3][1][c];
					else if (qI==Size_W+2)
						IT_Image[pI][qI][c]=In_Image[Size_H-3][Size_W-2][c];
					else 
						IT_Image[pI][qI][c]=In_Image[pI-6][qI-2][c];
				}
			}

			//the second and last but one rows
			else if (((pI==1) || (pI==Size_H+2)) && ((qI!=1) && (qI!=Size_W+2)))
			{
				if(pI==1)
				{
					if (qI==0)
						IT_Image[pI][qI][c]=In_Image[1][2][c];
					else if (qI==Size_W+3)
						IT_Image[pI][qI][c]=In_Image[1][Size_W-3][c];
					else
                       IT_Image[pI][qI][c]=In_Image[pI][qI-2][c];
				}
				else
				{
					if (qI==0)
						IT_Image[pI][qI][c]=In_Image[Size_H-2][2][c];
					else if (qI==Size_W+3)
						IT_Image[pI][qI][c]=In_Image[Size_H-2][Size_W-3][c];
					else 
						IT_Image[pI][qI][c]=In_Image[pI-4][qI-2][c];
				}
			}

			else
			{
				if (qI==0)
					IT_Image[pI][qI][c]=In_Image[pI-2][qI+2][c];
				else if (qI==1)
                    IT_Image[pI][qI][c]=In_Image[pI-2][qI][c];
				else if (qI==Size_W+2)
                    IT_Image[pI][qI][c]=In_Image[pI-2][qI-4][c];
				else if (qI==Size_W+3)
					IT_Image[pI][qI][c]=In_Image[pI-2][qI-6][c];
				else
					IT_Image[pI][qI][c]=In_Image[pI-2][qI-2][c];
			}
		}

  ///////////////////Image Filter/////////////////////
	int i=0,j=0;         //coordinates of N*N window
	int n=0,m=0;     //coordinates of vector
	int countF=0;     //counting for frequency
    int max_fq=0;   //the maxium frequency
	int max_od=0;   //the order of a pixel with maxium frequency

    for (pI=(int)(N/2);pI<Size_H+(int)(N/2);pI++)
		for (qI=(int)(N/2);qI<Size_W+(int)(N/2);qI++)
		{
			n=0;
			//put image data in  "comp"
			for (i=-(int)(N/2);i<1+(int)(N/2);i++)
				for (j=-(int)(N/2);j<1+(int)(N/2);j++)
				{
					comp[n][0]=IT_Image[pI+i][qI+j][0];
					comp[n][1]=IT_Image[pI+i][qI+j][1];
					comp[n][2]=IT_Image[pI+i][qI+j][2];
					n++;
				}
			//the first pixel
			tp[0]=comp[0][0];
			tp[1]=comp[0][1];
			tp[2]=comp[0][2];
			max_fq=0;
			max_od=0;
			for (n=0;n<N*N;n++)
			{
				if (tp[0]==comp[n][0])
				{
					if (tp[1]==comp[n][1])
					{
						if (tp[2]==comp[n][2])
							max_fq=max_fq+1;
					}
				}
			}
			//calculate the frequency of every pixel in the window
			//and find which has the maxium frequency
			for (m=1;m<N*N;m++)
			{
				tp[0]=comp[m][0];
			    tp[1]=comp[m][1];
			    tp[2]=comp[m][2];
				countF=0;
				for (n=0;n<N*N;n++)
				{
					if (tp[0]==comp[n][0])
					{
						if (tp[1]==comp[n][1])
						{
							if (tp[2]==comp[n][2])
								countF++;
						}
					}
				}
				if (countF>max_fq)
				{
					max_fq=count;
					max_od=m;
				}
			}
			//ouput RGB of a pixel with the maxium frequency
			OT_Image[pI-(int)(N/2)][qI-(int)(N/2)][0]=comp[max_od][0];
			OT_Image[pI-(int)(N/2)][qI-(int)(N/2)][1]=comp[max_od][1];
			OT_Image[pI-(int)(N/2)][qI-(int)(N/2)][2]=comp[max_od][2];
		}
    
  ///////////////////Write image/////////////////////
	ofstream ofile("oilpainting_image.raw",ios_base::out | ios_base::binary);
	if (!ofile)
	{
		cout<<"open failed"<<endl;
		exit(1);	
	}
	for (pO=0;pO<Size_H;pO++)
		for (qO=0;qO<Size_W;qO++)
			for (c=3;c<BytesPerPixel+3;c++)
			{
				COut_Image[pO][qO][c]=OT_Image[pO][qO][c]+0x00;    //Convert to hex or bin
				ofile.write((char*)&COut_Image[pO][qO][c],sizeof(COut_Image[pO][qO][c]));
			}
	ifile.close();
	tfile.close();
	ofile.close();
	for (chk=0;chk<N*N;chk++)
		delete []comp[chk];
	delete []comp;
	getchar();
    return 0 ;
}