#################################################

This README file is the Introduction of "EE569_hw1_4946850713_Shen.zip" and 
                    the Guidance of compiling and running the projects.
                    
The source code outside the .zip file is just a sample. Please unzip the .zip file to see and run the complete project.

#################################################

All the projects were built on------------Microsoft Visual C++ 2008 Expression Edition,
the operation system of the computer is---WIN7 (x64)

#################################################

The Steps of running a projects:
1) open Microsoft Visual C++ or Microsoft Visual Studio
2) open a project in the "EE569_hw1_4946850713_Shen.zip"
   (may wait a few minutes for ungrading if you use the latest Microsoft Visual)
3) click "Build->Build Solution" 
4) clike "Debug->Start Debugging"

#################################################
  
Problem 1 (a):   Image Resizing
Project:         Risizing_Image->Risizing_Image->Risizing_Image(.vcproj)
Source File:     resizing(.cpp)
Input Image:     the_starry_night.raw
Output Image:    resizing_image.raw 

#################################################
  
Problem 1 (b):   Image Demosaicing--Binear Demosaicing
Project:         Demosaicing->Demosaicing->Demosaicing(.vcproj)
Source File:     demosaicing(.cpp)
Input Image:     parrot_night.raw
Output Image:    demosaicing_image.raw 

#################################################
  
Problem 1 (b):   Image Demosaicing--MHC Demosaicing
Project:         MHC_demosaicing->MHC_demosaicing->MHC_demosaicing(.vcproj)
Source File:     MHC(.cpp)
Input Image:     parrot_night.raw
Output Image:    MHC_image.raw 

#################################################

  
Problem 2 (a):   Histogram Equalization--Tansfer Function Based
Project:         TransFunction->TransFunction->TransFunction(.vcproj)
Source File:     transfering(.cpp)
Input Image:     jet.raw
Output Image:    trans_image.raw 
Other Outputs:   red_histgrm.txt--------the data of the histogram in R channel of the oringinal image
                 green_histgrm.txt------the data of the histogram in G channel of the oringinal image
                 blue_histgrm.txt-------the data of the histogram in B channel of the oringinal image
                 red_transfun.txt-------the data of transfer function in R channel of the output image
                 green_transfun.txt-----the data of transfer function in G channel of the output image
                 blue_transfun.txt------the data of transfer function in B channel of the output image
                 New_red_histgrm.txt----the data of new histogram in R channel of the output image

#################################################

Problem 2 (a):   Histogram Equalization--Cumulative Probability Based
Project:         Histgm_Equ->Histgm_Equ->Histgm_Equ(.vcproj)
Source File:     equalization(.cpp)
Input Image:     jet.raw
Output Image:    equal_image.raw 
Other Outputs:   red_CDF.txt------------the data of the cumulative histogram in R channel of the original image
                 green_CDF.txt----------the data of the cumulative histogram in G channel of the original image
                 blue_CDF.txt-----------the data of the cumulative histogram in B channel of the original image
                 red_newCDF.txt---------the data of new cumulative histogram in R channel of the original image 
                 green_newCDF.txt-------the data of new cumulative histogram in G channel of the original image
                 blue_newCDF.txt--------the data of new cumulative histogram in B channel of the original image
                 red_histgrm.txt--------the data of the histogram in R channel of the oringinal image
                 New_red_histgrm.txt----the data of new histogram in R channel of the output image

#################################################

Problem 2 (b):   Oil Painting--"barn.raw"
Project:         Oil_painting_barn->Oil_painting_barn->Oil_painting_barn
Source File:     painting(.cpp)
Input Image:     barn.raw
Output Image:    quan64_barn_image.raw(64-color image)
                 oilpainting_image.raw

#################################################

Problem 2 (b):   Oil Painting--"coliseum.raw"
Project:         Oil_painting_coli->Oil_painting_coli->Oil_painting_coli
Source File:     painting2(.cpp)
Input Image:     coliseum.raw
Output Image:    quan64_coliseum_image.raw(64-color image)
                 oilpainting_Cimage.raw

#################################################

Problem 3 (a):   Noise Removal--Median Filter + Gaussian Filter
Project:         NoiseRm_M_G->NoiseRm_M_G->NoiseRm_M_G
Source File:     Median_Gaus(.cpp)
Input Image:     peppers_noisy.raw
Output Image:    smoothG_image.raw

#################################################

Problem 3 (a):   Noise Removal--Median Filter + Bilateral Filter
Project:         NoiseRm_M_B->NoiseRm_M_B->NoiseRm_M_B
Source File:     Median_Bila(.cpp)
Input Image:     peppers_noisy.raw
Output Image:    smoothB_image.raw

#################################################

Problem 3 (b):   Noise Removal--Median Filter + Guided Filter
Project:         Gu_Filter->Gu_Filter->Gu_Filter
Source File:     guided_filter(.cpp)
Input Image:     peppers_noisy.raw
Output Image:    smoothGIF_image.raw
                 smoothGIF_Rimage.raw(output image in R channel)
                 smoothGIF_Gimage.raw(output image in G channel)
                 smoothGIF_Bimage.raw(output image in B channel)

#################################################

