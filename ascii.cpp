/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2021

Assignment 01 Skeleton

ascii.cpp

*/

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	//
	//	1. Open BMP file
	//
	Bitmap image_data(argv[2]);

	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}
	//
	//	Your code goes here ....
	//
	//	2. Obtain Luminance
	//
    
    int width = image_data.getWidth();
    int height = image_data.getHeight();
    
    unsigned char red, green, blue;
    double grayscale;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_data.getColor(j, i, red, green, blue);
            grayscale = 0.299 * (double)red + 0.587 * (double)green + 0.114 * (double)blue;
            image_data.setColor(j, i, (unsigned char)grayscale, (unsigned char)grayscale, (unsigned char)grayscale);
            //printf("%f ", grayscale);
        }
    }
    
	//
	//  3. Resize image
	//
    int new_col = width;
    int new_row = height;
    int col_filt = 1, row_filt = 1;
	if (argc > 2)
	{
        int i = 0, max_col = 0, max_row = 0;
        for (; argv[3][i]!=','; i++)
            max_col = max_col * 10 + argv[3][i] - '0';
        for (i+=1; argv[3][i]!='\0'; i++)
            max_row = max_row * 10 + argv[3][i] - '0';
        //printf("%d\n", max_row);
        if (max_row == 0)
        {
            col_filt = width>height?((int)ceil((double)width/(double)max_col)):((int)ceil((double)height/(double)max_col));
            new_col = width/col_filt;
            new_row = height/col_filt;
            row_filt = col_filt;
        }
        else
        {
            col_filt = (int)ceil((double)width/(double)max_col);
            row_filt = (int)ceil((double)height/(double)max_row);
            new_col = width/col_filt;
            new_row = height/row_filt;
        }
        //printf("%d ", new_col);
        //printf("%d\n", new_row);
	}

    //printf("%d\n", new_row);


    Bitmap resized_image(new_col, new_row);
    
    for(int i = 0; i<new_row; i++)
    {
        for(int j = 0; j<new_col; j++)
        {
            int resized_grayscale = 0;
            for(int m = i*row_filt;m<(i+1)*row_filt;m++)
            {
                for(int n = j*col_filt;n<(j+1)*col_filt;n++)
                {
                    image_data.getColor(n, m, red, green, blue);
                    resized_grayscale += red;
                }
            }
            resized_grayscale = resized_grayscale / (row_filt*col_filt);
            //printf("%f ", resized_grayscale);
            resized_image.setColor(j, i, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale);
        }
    }
    
	//
	//	4. Quantization
	//
    
    char ** arr = NULL;
    arr = (char **)malloc(sizeof(char *)*new_row);
    for (int i = 0; i<new_row; i++)
        arr[i] = (char *)malloc(sizeof(char)*new_col);
    
    for(int i=0; i<new_row; i++)
    {
        for(int j = 0; j<new_col; j++)
        {
            resized_image.getColor(j, i, red, green, blue);
            arr[i][j] = (char)((int)red/ 32);
        }
    }
    
	//
	//      5. ASCII Mapping and printout
	//
	
    if(argv[1][0]=='s')
    {
        for(int i = 0; i< new_row; i++)
        {
            for(int j = 0; j<new_col; j++)
                printf("%c", shades[7 - arr[i][j]]);
            printf("\n");
        }
    }
    else if (argv[1][0] == 'p')
    {
        for(int i = 0; i< new_row; i++)
        {
            for(int j = 0; j<new_col; j++)
                printf("%c", shades[arr[i][j]]);
            printf("\n");
        }
    }
    
	//
	//      6. ASCII art txt file
	//
	if (argc > 3)
	{
        FILE *file = fopen(argv[4], "w");
        if(argv[1][0]=='s')
        {
            for(int i = 0; i< new_row; i++)
            {
                for(int j = 0; j<new_col; j++)
                    fputc(shades[7 - arr[i][j]], file);
                fputc('\n', file);
            }
        }
        else if (argv[1][0] == 'p')
        {
            for(int i = 0; i< new_row; i++)
            {
                for(int j = 0; j<new_col; j++)
                    fputc(shades[arr[i][j]], file);
                fputc('\n', file);
            }
        }
        fclose(file);
        
	}
	
	//  free memory
    image_data.~Bitmap();
    resized_image.~Bitmap();
    
    for (int i = 0; i<new_row; i++)
        free(arr[i]);
    
    free(arr);
    arr = NULL;
    
	return 0;
} 
