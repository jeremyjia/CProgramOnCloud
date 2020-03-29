#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#include "jpeglib.h"
#include "jerror.h"
#include "stddef.h"
#include "setjmp.h"
 
struct JPEG_Enc
{
    struct jpeg_destination_mgr		dst;        // A data destination manager provides three methods: https://raw.githubusercontent.com/libjpeg-turbo/libjpeg-turbo/master/libjpeg.txt
	struct jpeg_compress_struct		cinfo;
	struct jpeg_error_mgr			jerr;       // Use standard error mgr
 
    int                             quality;    // 1-100
    int                             jpeg_w;
    int                             jpeg_h;
    int                             jpeg_ch;
    // Raw buffer
    unsigned char                   *buffer;    // G, RGB, or RGBA format
    // Output
    FILE                            *fp;
    unsigned char                   *jpegOutput;
    unsigned long                   jpegOutputSize;
};
 
#define kBufferSize (4096)
#define kMaxLines   (8)
 
void jpegenc_init_destination(j_compress_ptr cinfo);
 
unsigned char *jpegenc_get_enc_buf(struct JPEG_Enc *jpeg_enc )
{
    return (unsigned char *)jpeg_enc->jpegOutput;
}
 
unsigned long jpegenc_get_enc_size(struct JPEG_Enc *jpeg_enc )
{
    return (unsigned long)jpeg_enc->jpegOutputSize;
}
 
int jpegenc_compress( struct JPEG_Enc *jpeg_enc, unsigned char *bytes, int quality, int ch )
{
	struct jpeg_compress_struct *cinfo = &jpeg_enc->cinfo;
	JSAMPROW row_pointer[kMaxLines];
	int rowBytes;
    unsigned char *srcScan = NULL;
 
	if (bytes == NULL)
        goto bail;
    if((ch > jpeg_enc->jpeg_ch) || ( ch != 1 && ch != 3 ) )
        goto bail;
 
    cinfo->input_components = ch;
    rowBytes = cinfo->image_width * cinfo->input_components;
    jpeg_enc->cinfo.in_color_space = (ch == 1)? JCS_GRAYSCALE : JCS_RGB;
 
    // Reset output buffer
    jpeg_enc->jpegOutputSize = 0;
	cinfo->dct_method = JDCT_IFAST;
    jpeg_set_defaults(cinfo);
 
    if(quality < 0)
        quality = 0;
    else if (quality > 100 )
        quality = 100;
    jpeg_enc->quality = quality;
	jpeg_set_quality(cinfo, jpeg_enc->quality, (boolean)1);
 
    // Set scan lines, which used by jpeglib
	jpeg_start_compress(cinfo, 1);
 
	while (cinfo->next_scanline < cinfo->image_height) {
        JDIMENSION lines = kMaxLines, i;
        if (lines > (cinfo->image_height - cinfo->next_scanline))
            lines = cinfo->image_height - cinfo->next_scanline;
        
        srcScan = bytes + rowBytes * cinfo->next_scanline;
        for (i = 0; i < lines; i++) {
            row_pointer[i] = srcScan;
            srcScan += rowBytes;
        }
	    jpeg_write_scanlines(cinfo, row_pointer, lines);
    }
 
    jpeg_finish_compress(cinfo);
    
    return 0;
bail:
	return 1;
}
 
void jpegenc_init_destination(j_compress_ptr cinfo)
{
	struct JPEG_Enc *jpeg_enc = (struct JPEG_Enc *)cinfo->dest;
	jpeg_enc->dst.next_output_byte = jpeg_enc->buffer;
    jpeg_enc->dst.free_in_buffer   = kBufferSize;
}
 
boolean jpegenc_empty_output_buffer(j_compress_ptr cinfo)
{
	struct JPEG_Enc *jpeg_enc = (struct JPEG_Enc *)cinfo->dest;
 
	if (NULL != jpeg_enc->fp)
		fwrite(jpeg_enc->buffer, 1, kBufferSize, jpeg_enc->fp);
	else {
		memmove(jpeg_enc->jpegOutput + jpeg_enc->jpegOutputSize, jpeg_enc->buffer, kBufferSize);
		jpeg_enc->jpegOutputSize  += kBufferSize;
	}
 
	jpeg_enc->dst.next_output_byte = jpeg_enc->buffer;
	jpeg_enc->dst.free_in_buffer = kBufferSize;
 
	return 1;
}
 
void jpegenc_term_destination(j_compress_ptr cinfo)
{
	struct JPEG_Enc * jpeg_enc = (struct JPEG_Enc *)cinfo->dest;
	int size = (int)(kBufferSize - jpeg_enc->dst.free_in_buffer);
 
    if (NULL != jpeg_enc->fp)
        fwrite(jpeg_enc->buffer, 1, size, jpeg_enc->fp);
	else {
		memmove(jpeg_enc->jpegOutput + jpeg_enc->jpegOutputSize, jpeg_enc->buffer, size);
		jpeg_enc->jpegOutputSize  += size;
	}
}
 
struct JPEG_Enc *new_jpegenc( int w, int h, int ch )
{
    struct JPEG_Enc * jpeg_enc = (struct JPEG_Enc *) malloc(sizeof(struct JPEG_Enc));
    if( jpeg_enc == NULL )
        goto fail;
    memset(jpeg_enc, 0, sizeof(struct JPEG_Enc));
 
    if(ch != 1 && ch != 3)
        goto fail;
 
    jpeg_enc->jpeg_w  = w;
    jpeg_enc->jpeg_h  = h;
    jpeg_enc->jpeg_ch = ch;
 
    // TODO: Just to elimnate reallocate buffer
    // Buffer for encoded bytes for one image/frame
    jpeg_enc->jpegOutput = (unsigned char*)malloc(jpeg_enc->jpeg_w * jpeg_enc->jpeg_h * ch);  
	if (jpeg_enc->jpegOutput == NULL)
	{
		goto fail;
    }
 
    // Used by scan lines - temp buffer for encoded bytes
    jpeg_enc->buffer = (unsigned char*)malloc(kBufferSize);
	if (jpeg_enc->buffer == NULL)
	{
		goto fail;
    }
 
    // Allocate and initialize JPEG compression object
    // Error manager
    jpeg_enc->cinfo.err = jpeg_std_error(&jpeg_enc->jerr);
	//jpeg_enc->cinfo.err->error_exit = ???;
 
    jpeg_create_compress(&jpeg_enc->cinfo);
 
    // destination manager
    jpeg_enc->cinfo.dest = &jpeg_enc->dst;
	jpeg_enc->dst.init_destination = jpegenc_init_destination;
	jpeg_enc->dst.empty_output_buffer = jpegenc_empty_output_buffer;
	jpeg_enc->dst.term_destination = jpegenc_term_destination;
 
    jpeg_enc->quality = 60;
 
    jpeg_enc->cinfo.image_width = w;
	jpeg_enc->cinfo.image_height = h;
	jpeg_enc->cinfo.input_components = ch;
	jpeg_enc->cinfo.in_color_space = (ch == 1)? JCS_GRAYSCALE : JCS_RGB;
 
	jpeg_enc->cinfo.dct_method = JDCT_IFAST;
 
    return jpeg_enc;
 
fail:
    if(jpeg_enc)
    {
        if(jpeg_enc->buffer)
            free(jpeg_enc->buffer);
        if(jpeg_enc->jpegOutput)
            free(jpeg_enc->jpegOutput);
 
        free(jpeg_enc);
    }
    return NULL;
}
 
void dispose_jpegenc( struct JPEG_Enc *jpeg_enc )
{
    if(jpeg_enc)
    {
        if(jpeg_enc->buffer)
            free(jpeg_enc->buffer);
        if(jpeg_enc->jpegOutput)
            free(jpeg_enc->jpegOutput);
 
        if(jpeg_enc->fp)
            fclose(jpeg_enc->fp);
 
        jpeg_destroy_compress(&jpeg_enc->cinfo);
        free(jpeg_enc);
    }
}
