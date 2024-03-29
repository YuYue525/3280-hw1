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
    //-----------------------------------------standard requirements-----------------------------------------//
    // when we implement the standard requirement, argv[1] must be 's' or 'p'. We use this property to distinguish enhancement part and the standard one 
    int width = image_data.getWidth();
    int height = image_data.getHeight();
    
    Bitmap gray_image(width, height);
    
    if (argv[1][0] == 's' || argv[1][0] == 'p')
    {


        unsigned char red, green, blue;
        double grayscale;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image_data.getColor(j, i, red, green, blue);
                grayscale = 0.299 * (double)red + 0.587 * (double)green + 0.114 * (double)blue;
                gray_image.setColor(j, i, (unsigned char)grayscale, (unsigned char)grayscale, (unsigned char)grayscale);
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
            for (; argv[3][i] != ','; i++)
                max_col = max_col * 10 + argv[3][i] - '0';
            for (i += 1; argv[3][i] != '\0'; i++)
                max_row = max_row * 10 + argv[3][i] - '0';
            //printf("%d\n", max_row);
            if (max_row == 0)
            {
                col_filt = width > height ? ((int)ceil((double)width / (double)max_col)) : ((int)ceil((double)height / (double)max_col));
                new_col = width / col_filt;
                new_row = height / col_filt;
                row_filt = col_filt;
            }
            else
            {
                col_filt = (int)ceil((double)width / (double)max_col);
                row_filt = (int)ceil((double)height / (double)max_row);
                new_col = width / col_filt;
                new_row = height / row_filt;
            }
            //printf("%d ", new_col);
            //printf("%d\n", new_row);
        }

        //printf("%d\n", new_row);


        Bitmap resized_image(new_col, new_row);

        for (int i = 0; i < new_row; i++)
        {
            for (int j = 0; j < new_col; j++)
            {
                int resized_grayscale = 0;
                for (int m = i * row_filt; m < (i + 1) * row_filt; m++)
                {
                    for (int n = j * col_filt; n < (j + 1) * col_filt; n++)
                    {
                        gray_image.getColor(n, m, red, green, blue);
                        resized_grayscale += red;
                    }
                }
                resized_grayscale = resized_grayscale / (row_filt * col_filt);
                //printf("%f ", resized_grayscale);
                resized_image.setColor(j, i, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale);
            }
        }

        //
        //	4. Quantization
        //

        char** arr = NULL;
        arr = (char**)malloc(sizeof(char*) * new_row);
        for (int i = 0; i < new_row; i++)
            arr[i] = (char*)malloc(sizeof(char) * new_col);

        for (int i = 0; i < new_row; i++)
        {
            for (int j = 0; j < new_col; j++)
            {
                resized_image.getColor(j, i, red, green, blue);
                arr[i][j] = (char)((int)red / 32);
            }
        }

        //
        //      5. ASCII Mapping and printout
        //

        if (argv[1][0] == 's')
        {
            for (int i = 0; i < new_row; i++)
            {
                for (int j = 0; j < new_col; j++)
                    printf("%c", shades[7 - arr[i][j]]);
                printf("\n");
            }
        }
        else if (argv[1][0] == 'p')
        {
            for (int i = 0; i < new_row; i++)
            {
                for (int j = 0; j < new_col; j++)
                    printf("%c", shades[arr[i][j]]);
                printf("\n");
            }
        }

        //
        //      6. ASCII art txt file
        //
        if (argc > 3)
        {
            FILE* file = fopen(argv[4], "w");
            if (argv[1][0] == 's')
            {
                for (int i = 0; i < new_row; i++)
                {
                    for (int j = 0; j < new_col; j++)
                        fputc(shades[7 - arr[i][j]], file);
                    fputc('\n', file);
                }
            }
            else if (argv[1][0] == 'p')
            {
                for (int i = 0; i < new_row; i++)
                {
                    for (int j = 0; j < new_col; j++)
                        fputc(shades[arr[i][j]], file);
                    fputc('\n', file);
                }
            }
            fclose(file);

        }

        //  free memory
        image_data.~Bitmap();
        resized_image.~Bitmap();
        gray_image.~Bitmap();

        for (int i = 0; i < new_row; i++)
            free(arr[i]);

        free(arr);
        arr = NULL;
    }
    //-----------------------------------------enhancement requirements-----------------------------------------//
    else if (argv[1][0] == 'e')// We test the enhancement part when the argv[1] is 'e'
    {

        unsigned char red, green, blue;
        double grayscale;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image_data.getColor(j, i, red, green, blue);
                if(argv[1][1]=='n')
                    grayscale = 0.299 * (double)red + 0.587 * (double)green + 0.114 * (double)blue;
                else if (argv[1][1] == 'b')
                {
                    if (red == green && red == blue && red <128)
                        grayscale = 31;
                    else if(red == green && red == blue)
                        grayscale = 255;
                    else
                    {
                        if(red >= green && green >= blue)
                            grayscale = 223;
                        else if(green>=red && red >= blue)
                            grayscale = 191;
                        else if(green >= blue && blue >=red)
                            grayscale = 159;
                        else if(blue>=green &&green>=red)
                            grayscale = 127;
                        else if(blue >=red && red >= green)
                            grayscale = 95;
                        else if(red>=blue && blue>=green)
                            grayscale = 63;
                    }
                }
                    
                gray_image.setColor(j, i, (unsigned char)grayscale, (unsigned char)grayscale, (unsigned char)grayscale);
                //printf("%f ", grayscale);
            }
        }
        
        //
        //  3. Resize image
        //
        int new_col = width;
        int new_row = height;
        int col_filt = 1, row_filt = 1;
        int row_remain=0, col_remain = 0;
        if (argc > 2)
        {
            if (argv[3][0] == 'n')
            {
                int i = 1, max_col = 0, max_row = 0;
                for (; argv[3][i] != ','; i++)
                    max_col = max_col * 10 + argv[3][i] - '0';
                for (i += 1; argv[3][i] != '\0'; i++)
                    max_row = max_row * 10 + argv[3][i] - '0';
                //printf("%d\n", max_row);
                if (max_row == 0)
                {
                    col_filt = width > height ? ((int)ceil((double)width / (double)max_col)) : ((int)ceil((double)height / (double)max_col));
                    new_col = width / col_filt;
                    new_row = height / col_filt;
                    row_filt = col_filt;
                }
                else
                {
                    col_filt = (int)ceil((double)width / (double)max_col);
                    row_filt = (int)ceil((double)height / (double)max_row);
                    new_col = width / col_filt;
                    new_row = height / row_filt;
                }
            }
            else if (argv[3][0] == 'b')
            {
                int i = 1, col = 0, row = 0;
                for (; argv[3][i] != ','; i++)
                    col = col * 10 + argv[3][i] - '0';
                for (i += 1; argv[3][i] != '\0'; i++)
                    row = row * 10 + argv[3][i] - '0';
                //printf("%d\n", max_row);
                col_filt = width/col;
                col_remain = width % col;
                row_filt = height / row;
                row_remain = height % row;

                new_col = col;
                new_row = row;

            }
            //printf("%d ", col_filt);
            //printf("%d\n", col_remain);
        }

        //printf("%d\n", new_row);


        Bitmap resized_image(new_col, new_row);

        for (int i = 0; i < new_row; i++)
        {
            for (int j = 0; j < new_col; j++)
            {
                if (argv[3][0] == 'n')
                {
                    int resized_grayscale = 0;
                    for (int m = i * row_filt; m < (i + 1) * row_filt; m++)
                    {
                        for (int n = j * col_filt; n < (j + 1) * col_filt; n++)
                        {
                            gray_image.getColor(n, m, red, green, blue);
                            resized_grayscale += red;
                        }
                    }
                    resized_grayscale = resized_grayscale / (row_filt * col_filt);
                    //printf("%f ", resized_grayscale);
                    resized_image.setColor(j, i, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale);
                }
                else if (argv[3][0] == 'b')
                {
                   
                    int resized_grayscale = 0;
                    //printf("%f ", resized_grayscale);
                    
                    int x, y;
                    x = (int)(i / (double)new_row * (double)height);
                    y = (int)(j / (double)new_col * (double)width);
                    x = x>(height-1)?(height-1):x;
                    y = y>(width-1)?(width-1):y;
                    gray_image.getColor(y, x, red, green, blue);
                    resized_grayscale = red;
                    resized_image.setColor(j, i, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale, (unsigned char)resized_grayscale);
                }
            }
        }

        //
        //	4. Quantization
        //

        char** arr = NULL;
        arr = (char**)malloc(sizeof(char*) * new_row);
        for (int i = 0; i < new_row; i++)
            arr[i] = (char*)malloc(sizeof(char) * new_col);

        for (int i = 0; i < new_row; i++)
        {
            for (int j = 0; j < new_col; j++)
            {
                resized_image.getColor(j, i, red, green, blue);
                arr[i][j] = (char)((int)red / 32);
            }
        }

        //
        //      5. ASCII Mapping and printout
        //

        if (argv[1][2] == 's')
        {
            for (int i = 0; i < new_row; i++)
            {
                for (int j = 0; j < new_col; j++)
                    printf("%c", shades[7 - arr[i][j]]);
                printf("\n");
            }
        }
        else if (argv[1][2] == 'p')
        {
            for (int i = 0; i < new_row; i++)
            {
                for (int j = 0; j < new_col; j++)
                    printf("%c", shades[arr[i][j]]);
                printf("\n");
            }
        }

        //
        //      6. ASCII art txt file
        //
        if (argc > 3)
        {
            FILE* file = fopen(argv[4], "w");
            if (argv[1][2] == 's')
            {
                for (int i = 0; i < new_row; i++)
                {
                    for (int j = 0; j < new_col; j++)
                        fputc(shades[7 - arr[i][j]], file);
                    fputc('\n', file);
                }
            }
            else if (argv[1][2] == 'p')
            {
                for (int i = 0; i < new_row; i++)
                {
                    for (int j = 0; j < new_col; j++)
                        fputc(shades[arr[i][j]], file);
                    fputc('\n', file);
                }
            }
            fclose(file);
        };
        //bonus output:
                //bonus output:
        if(argc > 4)
        {
            // printf("%d", argc);
            //Colored ASCII Art html
            if(argv[5][0]=='1')
            {
                char* space = "&#160;";
                FILE * html_file = fopen("ASCII_art.html","w");
                fprintf(html_file, "<!DOCTYPE html>\n<html>\n<head>\n<style>p{font-family:monospace;line-height:0.01em;}\n</style>\n</head>\n" );
                fprintf(html_file, "<body>\n<style>p{font-family:monospace;line-height:0.01em;}\n</style>\n");
                int a = 0, b = 0, c = 0;
                //fprintf(html_file, "<p class=\"small\">\n");
                //fprintf(html_file, "<a style = \"color:rgb(%d,%d,%d);\">&nbsp;</a>", a, b, c);
                //fprintf(html_file, "</p >\n");
                
                for (int i = 0; i < new_row; i++)
                {
                    fprintf(html_file,"<p class=\"small\">\n");

                    for (int j = 0; j < new_col; j++)
                    {

                        if (argv[3][0] == 'n')
                        {
                            //fprintf(html_file, "<a style = \"color:rgb(%d,%d,%d);\">&nbsp;</a>", a, b, c);
                            int new_red=0, new_green=0, new_blue=0, gray = 0;
                            for (int m = i * row_filt; m < (i + 1) * row_filt; m++)
                            {
                                for (int n = j * col_filt; n < (j + 1) * col_filt; n++)
                                {
                                    image_data.getColor(n, m, red, green, blue);
                                    new_red += red;
                                    new_green += green;
                                    new_blue += blue;
                                }
                            }
                            new_red /= (row_filt * col_filt);
                            new_green /= (row_filt * col_filt);
                            new_blue /= (row_filt * col_filt);
                            //printf("%f ", resized_grayscale);
                            if (argv[1][2] == 's')
                            {
                                if(shades[7-arr[i][j]] == ' ')
                                    fprintf(html_file, "<a style = \"color:rgb(%d,%d,%d);\">&nbsp;</a>", red, green, blue);
                                else
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">%c</a>", new_red, new_green, new_blue, shades[7 - arr[i][j]]);
                            }
                            else if (argv[1][2] == 'p')
                            {
                                if (shades[arr[i][j]] == ' ')
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">&nbsp;</a>", new_red, new_green, new_blue);
                                else
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">%c</a>", new_red, new_green, new_blue, shades[arr[i][j]]);
                            }
                        }
                        else if (argv[3][0] == 'b')
                        {
                            //fprintf(html_file, "<a style = \"color:rgb(%d,%d,%d);\">&nbsp;</a>", a, b, c);
                            int x, y;
                            x = (int)(i / (double)new_row * (double)height);
                            y = (int)(j / (double)new_col * (double)width);
                            x = x>(height-1)?(height-1):x;
                            y = y>(width-1)?(width-1):y;
                            image_data.getColor(y, x, red, green, blue);
                            if (argv[1][2] == 's')
                            {
                                //fprintf(html_file, "<a style = \"color:rgb(%d,%d,%d);\">&nbsp;</a>", a, b, c);
                                if (shades[7-arr[i][j]] == ' ')
                                {
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">&nbsp;</a>", red, green, blue);
                                }
                                else
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">%c</a>", red, green, blue, shades[7 - arr[i][j]]);
                            }
                            else if (argv[1][2] == 'p')
                            {
                                if (shades[arr[i][j] ]== ' ')
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">&nbsp;</a>", red, green, blue);
                                else
                                    fprintf(html_file, "<a style=\"color:rgb(%d,%d,%d);\">%c</a>", red, green, blue, shades[arr[i][j]]);
                            }
                        }
                    }
                    fprintf(html_file,"</p>\n");
                }
                
                fprintf(html_file, "</body>\n");
                fprintf(html_file, "</html>\n");
                fclose(html_file);
            }
            
            if(argv[5][1]=='1')
            {
                //printf("yes");
                Bitmap ascii_bmp(8 * new_col, 8 * new_row);
                unsigned char red, green,blue;
                char *path[8];
                path[0] = "shades/0.bmp\0";
                path[1] = "shades/1.bmp\0";
                path[2] = "shades/2.bmp\0";
                path[3] = "shades/3.bmp\0";
                path[4] = "shades/4.bmp\0";
                path[5] = "shades/5.bmp\0";
                path[6] = "shades/6.bmp\0";
                path[7] = "shades/7.bmp\0";
                
                for(int i = 0; i< new_row; i++)
                {
                    for(int j = 0; j<new_col; j++)
                    {
                        if(argv[1][2]=='s')
                        {
                            //printf("yes");
                            Bitmap tmp(path[7-arr[i][j]]);
                            for(int m = 0; m < 8; m++)
                            {
                                for (int n = 0; n<8; n++)
                                {
                                    //printf("yes");
                                    tmp.getColor(n, m, red, green, blue);
                                    //printf("%d", (int)red);
                                    ascii_bmp.setColor(j*8+n, i*8+m, red, green, blue);
                                }
                            }
                            tmp.~Bitmap();
                        }
                        else if(argv[1][2]=='p')
                        {
                            Bitmap tmp(path[arr[i][j]]);
                            for(int m = 0; m < 8; m++)
                            {
                                for (int n = 0; n<8; n++)
                                {
                                        
                                    tmp.getColor(n, m, red, green, blue);
                                    ascii_bmp.setColor(j*8+n, i*8+m, red, green, blue);
                                }
                            }
                            tmp.~Bitmap();
                        }
                    }
                }
                
                ascii_bmp.save("ASCII_bmp.bmp");
            }

        }
        
        //  free memory
        image_data.~Bitmap();
        resized_image.~Bitmap();
        gray_image.~Bitmap();

        for (int i = 0; i < new_row; i++)
            free(arr[i]);

        free(arr);
        arr = NULL;
    }
	return 0;
} 
