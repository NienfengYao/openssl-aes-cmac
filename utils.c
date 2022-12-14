#include <stdio.h>
#include <string.h>
#include <openssl/cmac.h>
#include "utils.h"


static void print_bytes(unsigned char* buf, const size_t len);
static size_t aes_cmac(unsigned char *in, const int in_len, unsigned char *out, \
    unsigned char *key);
static bool verify_mac(int idx, unsigned char* in, unsigned int len, \
    unsigned char* out, unsigned char* key);

/* Test items base RFC 4493 (https://www.rfc-editor.org/rfc/rfc4493)*/
// K: 2b7e1516 28aed2a6 abf71588 09cf4f3c
unsigned char AES_KEY[] = {
    0x2b,0x7e,0x15,0x16,
    0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88,
    0x09,0xcf,0x4f,0x3c};

/**
  * @brief       show command's help message.
  * @param       help head of help messages array.
  * @param       number of elements in array.
  * @return      None
  */
void show_help(char * const help[], int num)
{
    if (help[0]) {
        printf("help info:\n");
        for (int i = 0; i < num; i++)
            printf("\t%s\n", help[i]);
        printf("\n");
    }
}

void print_bytes(unsigned char* buf, const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

/* AES-128, key length=128 bits */
size_t aes_cmac(unsigned char *in, const int in_len, unsigned char *out, \
    unsigned char *key)
{
    size_t out_len;

    CMAC_CTX *ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    CMAC_Update(ctx, in, in_len);
    CMAC_Final(ctx, out, &out_len);
    CMAC_CTX_free(ctx);
    return out_len;
}

// Verify the CMAC
bool verify_mac(int idx, unsigned char* in, unsigned int len, unsigned char* out, \
    unsigned char* key)
{
    bool flag = true;
    unsigned char result[AES_KEY_LEN];
    
    printf("Test Case %d: M len = %u\n", idx, len);
    printf("\tM:\t\t");
    print_bytes(in, len);
    // printf("\tOut:\t\t");
    // print_bytes(out, AES_KEY_LEN);
    printf("\tAES-CMAC:\t");
    aes_cmac(in, len, result, key);
    print_bytes(result, AES_KEY_LEN);
    printf("\tVeirfied:\t");
    if(!strncmp((char*)out, (char*)result, AES_KEY_LEN))
        printf("[PASS]\n\n");
    else
        printf("[FAIL]\n\n");
    return flag;
}

unsigned char* read_file(const char *f_name, size_t *buff_len)
{
    FILE *fp;
    size_t f_len;
    unsigned char *buff = NULL;

    // Open the file for reading in binary mode
    fp = fopen(f_name, "rb");
    if (fp == NULL){
        printf("Error: Open file:%s Failed.\n", f_name);
        return NULL;
    }
    // Get file length
    fseek(fp, 0, SEEK_END);
    f_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    // Allocate memory
    buff = (char *)malloc(f_len+1);
    if (!buff){
		printf("Error: Allocate memory Failed.\n");
        fclose(fp);
        return NULL;
	}
    //Read file contents into buffer
	fread(buff, f_len, 1, fp);
	fclose(fp);
    *buff_len = f_len;
    return buff;
}

int write_file(const char *f_name, unsigned char *buff, const size_t buff_len, \
    Image_Header_T *img_p, unsigned int img_size, TLV_T *tlv_p, unsigned int tlv_size)
{
    FILE *fp;
    size_t f_len;

    fp = fopen(f_name, "w"); /* Open the file for writing */
    if (fp == NULL){
        printf("Error: Write file:%s Failed.\n", f_name);
        return -1;
    }

    fwrite(img_p, img_size, 1, fp);
    fwrite(tlv_p, tlv_size, 1, fp);
    fwrite(buff, 1, buff_len, fp);
	fclose(fp);
    return 0;
}

void do_test(void)
{
    /* Test items base RFC 4493 (https://www.rfc-editor.org/rfc/rfc4493)*/
    // Test Case 1 =====
    // M: <empty string> Mlen: 0
    unsigned char msg1[] = { };
    // AES-CMAC: bb1d6929 e9593728 7fa37d12 9b756746
    unsigned char out1[] = {
        0xbb, 0x1d, 0x69, 0x29,
        0xe9, 0x59, 0x37, 0x28,
        0x7f, 0xa3, 0x7d, 0x12,
        0x9b, 0x75, 0x67, 0x46};

    // Test Case 2 =====
    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 16
    unsigned char msg2[] = {
        0x6b,0xc1,0xbe,0xe2, 
        0x2e,0x40,0x9f,0x96, 
        0xe9,0x3d,0x7e,0x11, 
        0x73,0x93,0x17,0x2a};
    // AES-CMAC: 070a16b4 6b4d4144 f79bdd9d d04a287c
    unsigned char out2[] = {
        0x07, 0x0a, 0x16, 0xb4,
        0x6b, 0x4d, 0x41, 0x44,
        0xf7, 0x9b, 0xdd, 0x9d,
        0xd0, 0x4a, 0x28, 0x7c};

    // Test Case 3 =====
    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 40
    //    ae2d8a57 1e03ac9c 9eb76fac 45af8e51
    //    30c81c46 a35ce411
    unsigned char msg3[] = {
        0x6b, 0xc1, 0xbe, 0xe2,
        0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57,
        0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac,
        0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46,
        0xa3, 0x5c, 0xe4, 0x11};
    // AES-CMAC: dfa66747 de9ae630 30ca3261 1497c827
    unsigned char out3[] = {
        0xdf, 0xa6, 0x67, 0x47,
        0xde, 0x9a, 0xe6, 0x30,
        0x30, 0xca, 0x32, 0x61,
        0x14, 0x97, 0xc8, 0x27};

    // Test Case 4 =====
    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 64
    //    ae2d8a57 1e03ac9c 9eb76fac 45af8e51
    //    30c81c46 a35ce411 e5fbc119 1a0a52ef
    //    f69f2445 df4f9b17 ad2b417b e66c3710
    unsigned char msg4[] = {
        0x6b, 0xc1, 0xbe, 0xe2,
        0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57,
        0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac,
        0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46,
        0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19,
        0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45,
        0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b,
        0xe6, 0x6c, 0x37, 0x10};
    // AES-CMAC: 51f0bebf 7e3b9d92 fc497417 79363cfe
    unsigned char out4[] = {
        0x51, 0xf0, 0xbe, 0xbf,
        0x7e, 0x3b, 0x9d, 0x92,
        0xfc, 0x49, 0x74, 0x17,
        0x79, 0x36, 0x3c, 0xfe};

    verify_mac(1, msg1, sizeof(msg1), out1, AES_KEY);
    verify_mac(2, msg2, sizeof(msg2), out2, AES_KEY);
    verify_mac(3, msg3, sizeof(msg3), out3, AES_KEY);
    verify_mac(4, msg4, sizeof(msg4), out4, AES_KEY);
}

void do_key(const char *f_key)
{
    size_t buff_len = 0;
    unsigned char *buff = NULL;

    buff = read_file(f_key, &buff_len);
    if(buff_len <= 0){
        printf("Error: Read file failed.\n"); 
        return;
    }else if(buff_len != AES_KEY_LEN){
        printf("Error: Key length %ld != 16 bytes, failed.\n", buff_len);
        return;
    }
    printf("File:%s, size: %ld bytes\n", f_key, buff_len);
    print_bytes(buff, buff_len);
	free(buff);
}

void do_cmac(const char *f_in, const char *f_key)
{
    size_t buff_len=0, key_len=0;
    unsigned char *buff=NULL, *key=NULL;
    unsigned char result[AES_KEY_LEN];

    buff = read_file(f_in, &buff_len);
    if(buff_len <=0){
        printf("Error: Read file(%s) failed.\n", f_in);
        goto exit_do_cmac;
    }
    printf("File:%s, size: %ld bytes\n", f_in, buff_len);
    if (strlen(f_key) == 0){    // Use default AES-KEY
        aes_cmac(buff, buff_len, result, AES_KEY);
    }else{                      // Use external AES-KEY
        key = read_file(f_key, &key_len);
        if(key_len <= 0){
            printf("Error: Read file(%s) failed.\n", f_key);
            goto exit_do_cmac;
        }else if(key_len != AES_KEY_LEN){
            printf("Error: Key length %ld != 16 bytes, failed.\n", key_len);
            goto exit_do_cmac;
        }else{
            printf("File:%s, size: %ld bytes\n", f_key, key_len);
            aes_cmac(buff, buff_len, result, key);
        }
    }
    printf("AES-CMAC: ");
    print_bytes(result, AES_KEY_LEN);
exit_do_cmac:
    if(buff)
        free(buff);
    if(key)
        free(key);
}

void do_sign(const char *f_in, const char *f_out, const char *f_key)
{
    size_t buff_len=0, key_len=0;
    unsigned char *buff=NULL, *key=NULL;
    unsigned char result[AES_KEY_LEN];
    Image_Header_T img;
    TLV_T tlv;

    buff = read_file(f_in, &buff_len);
    if(buff_len <=0){
        printf("Error: Read file failed.\n");
        goto exit_do_sign;
    }
    printf("File:%s, size: %ld bytes\n", f_in, buff_len);
    if (strlen(f_key) == 0){    // Use default AES-KEY
        aes_cmac(buff, buff_len, result, AES_KEY);
    }else{                      // Use external AES-KEY
        key = read_file(f_key, &key_len);
        if(key_len <= 0){
            printf("Error: Read file(%s) failed.\n", f_key); 
            goto exit_do_sign;
        }else if(key_len != AES_KEY_LEN){
            printf("Error: Key length %ld != 16 bytes, failed.\n", key_len); 
            goto exit_do_sign;
        }else{
            aes_cmac(buff, buff_len, result, key);
        }
    }
    printf("AES-CMAC: ");
    print_bytes(result, AES_KEY_LEN);
    printf("img_size:%ld, tlv_size:%ld\n", sizeof(img), sizeof(tlv));
    img.ih_magic = 0x12345678;
    img.ih_hdr_size = sizeof(img);
    img.ih_img_size = buff_len;
    img.ih_tlv_size = sizeof(tlv);
    img.ih_img_version = 0x87654321;
    img.ih_img_attr = 0x01;
    tlv.tlv_type = 0x01;
    tlv.tlv_length = AES_KEY_LEN;
    strncpy(tlv.tlv_value, result, AES_KEY_LEN);
    write_file(f_out, buff, buff_len, &img, sizeof(img), &tlv, sizeof(tlv));

exit_do_sign:
    if(buff)
        free(buff);
    if(key)
        free(key);
}
