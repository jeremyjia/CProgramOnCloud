#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "jpeglib.h"
#include "jpegenc.h"
 
void getTimemMS( unsigned long *ms)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    *ms = now.tv_sec * 1000 + now.tv_usec/1000;
}
 
void createGrayImage(unsigned char *pDataBuffer, int w, int h) {
	unsigned char *pBuf = pDataBuffer;
    for(int i = 0; i < h;i++)
    {
         for(int j = 0; j < w; j++)
         {
             pBuf[w*i+j] = (unsigned char)(j & 0xff);
         }
         
    }
}
 
void create_gray_jpeg()
{
    struct JPEG_Enc  *jpeg_enc;
    FILE *fp = NULL;
	int  ch = 3;
	int width = 640;
	int height = 480;
 
	printf("Creating an gray image\n");
    jpeg_enc = new_jpegenc(width, height, ch);
    if(jpeg_enc == NULL) {
        return;
    }
	unsigned long t0, t1;
	unsigned char *pGrayBuf = malloc(width * height * sizeof(char));
	createGrayImage(pGrayBuf, width, height);
	
	getTimemMS(&t0);
	jpegenc_compress(jpeg_enc, pGrayBuf,  60, 1);
	getTimemMS(&t1);
	printf("Compress gray image to JPEG taking %ldms\n", t1-t0);
 
    printf("Saving to JPEG\n");
	fp = fopen("grayImage.jpg", "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file for writing\n");
		return;
	}
    fwrite(jpegenc_get_enc_buf(jpeg_enc), 1, jpegenc_get_enc_size(jpeg_enc), fp);
	dispose_jpegenc(jpeg_enc);
    if (fp != NULL)
		fclose(fp);
	
}

 int main(void)
{
    create_gray_jpeg();
 }
