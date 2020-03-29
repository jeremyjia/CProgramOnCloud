#ifndef __JPEGENCODERH__
#define __JPEGENCODERH__
 
 
struct JPEG_Enc;
 
/*
    Notes:
        1. New a jpegenc with image features: width, height, and channel (1,3 are valid)
        2. Prepare image raw bytes buffer (for example rawbuf), which should have format Gray or RGB
        3. execute jpegenc_compress
        4. Then get result from jpegenc_get_enc_size(...) and jpegenc_get_enc_buf(...)
*/
 
int jpegenc_compress( struct JPEG_Enc *jpeg_enc, unsigned char *bytes, int quality, int ch );
unsigned long jpegenc_get_enc_size(struct JPEG_Enc *jpeg_enc );
unsigned char *jpegenc_get_enc_buf(struct JPEG_Enc *jpeg_enc );
 
//unsigned char *jpegenc_get_raw_buf(struct JPEG_Enc *jpeg_enc );
 
struct JPEG_Enc *new_jpegenc( int w, int h, int ch );
void dispose_jpegenc( struct JPEG_Enc *jpeg_enc );
 
#endif //__JPEGENCODERH__

