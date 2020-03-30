#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include "jpeglib.h"
#include "jpegenc.h"

enum
{
    IMAGE_COLOR_RED=1,
    IMAGE_COLOR_GREEN =2,
    IMAGE_COLOR_BLUE =3
};
void getTimemMS( unsigned long *ms)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    *ms = now.tv_sec * 1000 + now.tv_usec/1000;
}
 
void createGrayImage(unsigned char *pDataBuffer, int w, int h) {
	unsigned char *pBuf = pDataBuffer;
    //圆心坐标及半径
    int x0=h/3;
    int y0=w/4;
    int nR=100;
    //矩形区域
    int x1=w*2/3;
    int x2=w*2/3+100;
    int y1=h*2/3;
    int y2=h*2/3+100;

    memset(pDataBuffer, 0x00, w*h);
    for(int i = 0; i < h;i++)
    {
         for(int j = 0; j < w; j++)
         {
             bool bCircle = ((x0-j)*(x0-j)+(y0-i)*(y0-i))<=nR*nR?1:0;
             if (bCircle) {
                 pBuf[w*i+j] = (unsigned char)(IMAGE_COLOR_RED);
             } 

            if (j>x1&&j<x2&&i>y1&&i<y2) {
                pBuf[w*i+j] = (unsigned char)(IMAGE_COLOR_BLUE);
             }       
             
         }       
    }
}

void convertRaw2RGB(unsigned char *pOutBuffer, unsigned char *pInBuffer, int w, int h, int ch) 
{
	int i,j;
	int line_sz = w * ch;
	int pixel_sz = ch;
    unsigned char *ptr=NULL;
    memset(pOutBuffer, 0xcc, w*h*ch);
	for (i=0 ; i<h ; i++) 
    {
		for (j=0 ; j<w ; j++) 
		{
            ptr = &(pOutBuffer[i*line_sz + j*pixel_sz ]);
            int val = (int)(pInBuffer[i*w + j]);
            if (val == IMAGE_COLOR_RED) {
                ptr[0] = 255; 
                ptr[1] = 0; 
                ptr[2] = 0; 
            }
            else if(val == IMAGE_COLOR_GREEN)
            {
                ptr[0] = 0; 
                ptr[1] = 255; 
                ptr[2] = 0;
            } 
            else if(val == IMAGE_COLOR_BLUE)
            {
                ptr[0] = 0; 
                ptr[1] = 0; 
                ptr[2] = 255;
            }        

		}
	}
	
 }
 
void create_color_jpeg()
{
    struct JPEG_Enc  *jpeg_enc;
    FILE *fp = NULL;
	int  ch = 3;
	int width = 640;
	int height = 480;
 
	printf("Creating an color image\n");
    jpeg_enc = new_jpegenc(width, height, ch);
    if(jpeg_enc == NULL) {
        return;
    }
	unsigned long t0, t1;
	unsigned char *pGrayBuf = malloc(width * height * sizeof(char));
	createGrayImage(pGrayBuf, width, height);

    unsigned char *pRGBBuf = (unsigned char *) malloc(3 * width * height * sizeof(unsigned char));
	if (pRGBBuf == NULL) {
		return;
	}
    convertRaw2RGB(pRGBBuf, pGrayBuf, width, height, 3);

	getTimemMS(&t0);
	jpegenc_compress(jpeg_enc, pRGBBuf,  60, 3);
	getTimemMS(&t1);
	printf("Compress color image to JPEG taking %ldms\n", t1-t0);
 
    printf("Saving to JPEG\n");
	fp = fopen("graphicImage.jpg", "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file for writing\n");
		return;
	}
    fwrite(jpegenc_get_enc_buf(jpeg_enc), 1, jpegenc_get_enc_size(jpeg_enc), fp);
	dispose_jpegenc(jpeg_enc);
    if (fp != NULL) fclose(fp);

    free(pRGBBuf);
    free(pGrayBuf);
	
}

 int main(void)
{
    create_color_jpeg();
 }
