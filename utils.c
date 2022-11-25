#include <stdio.h>
#include <openssl/cmac.h>

/**
  * @brief       show command's help message.
  * @param       help head of help messages array.
  * @param       number of elements in array.
  * @return      None
  */
void show_help(char * const help[], int num)
{
    int i;

    if (help[0]) {
        printf("help info:\n");
        for (i = 0; i < num; i++)
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
//unsigned char* aes_cmac(unsigned char *in, unsigned int len, unsigned char* out, unsigned char* key)
size_t aes_cmac(unsigned char *in, const int in_len, unsigned char *out, unsigned char *key)
{
    size_t out_len;

    CMAC_CTX *ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    //printf("aes_cmac(): message length = %d bytes\n", in_len);
    //print_bytes(in, in_len);
    CMAC_Update(ctx, in, in_len);
    CMAC_Final(ctx, out, &out_len);
    CMAC_CTX_free(ctx);
    //printf("out_len = %lu \n", out_len);
    //print_bytes(out, out_len);
    return out_len;
}

void unit_test2(void)
{
    // K: 2b7e1516 28aed2a6 abf71588 09cf4f3c
    unsigned char key[] = {
        0x2b,0x7e,0x15,0x16, 
        0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c};

    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 128
    unsigned char message[] = {
        0x6b,0xc1,0xbe,0xe2, 
        0x2e,0x40,0x9f,0x96, 
        0xe9,0x3d,0x7e,0x11, 
        0x73,0x93,0x17,0x2a};

    unsigned char mact[16] = {0}; 
    size_t mactlen;

    printf("message length = %lu bytes (%lu bits)\n",sizeof(message), sizeof(message)*8);
    print_bytes(message, sizeof(message));
    
    /*
    CMAC_CTX *ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    printf("message length = %lu bytes (%lu bits)\n",sizeof(message), sizeof(message)*8);

    CMAC_Update(ctx, message, sizeof(message));
    CMAC_Final(ctx, mact, &mactlen);
    CMAC_CTX_free(ctx);
    */

    // expected result T = 070a16b4 6b4d4144 f79bdd9d d04a287c
    mactlen = aes_cmac(message, sizeof(message), mact, key);
    print_bytes(mact, mactlen);
}

void unit_test1(void)
{
    // K: 2b7e1516 28aed2a6 abf71588 09cf4f3c
    unsigned char key[] = {
        0x2b,0x7e,0x15,0x16, 
        0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c};

    // M: 6bc1bee2 2e409f96 e93d7e11 7393172a Mlen: 128
    unsigned char message[] = {
        0x6b,0xc1,0xbe,0xe2, 
        0x2e,0x40,0x9f,0x96, 
        0xe9,0x3d,0x7e,0x11, 
        0x73,0x93,0x17,0x2a};

    unsigned char mact[16] = {0}; 
    size_t mactlen;

    CMAC_CTX *ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    printf("message length = %lu bytes (%lu bits)\n",sizeof(message), sizeof(message)*8);
    print_bytes(message, sizeof(message));

    CMAC_Update(ctx, message, sizeof(message));
    CMAC_Final(ctx, mact, &mactlen);

    /* expected result T = 070a16b4 6b4d4144 f79bdd9d d04a287c */
    print_bytes(mact, mactlen);
    CMAC_CTX_free(ctx);
}
