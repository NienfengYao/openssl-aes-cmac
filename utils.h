#ifndef __UTILS_H__
#define __UTILS_H__

typedef enum { false = 0, true = !false } bool;

# define AES_KEY_LEN    16
# define FILE_NAME_LEN  64
# define ARRAY_SIZE(a)  (sizeof(a) / sizeof((a)[0]))

typedef struct Image_Header{
    unsigned int ih_magic;          // magic number
    unsigned int ih_hdr_size;       // size of image header
    unsigned int ih_img_size;       // size of application image
    unsigned int ih_tlv_size;       // size of tlv area
    unsigned int ih_img_version;    // image version
    unsigned int ih_img_attr;       // image attribution
}Image_Header_T;

typedef struct TLV{                 // Type-Length-Value
    unsigned int tlv_type;
    unsigned int tlv_length;
    unsigned char tlv_value[AES_KEY_LEN];
}TLV_T;

typedef enum action{
    ACTION_TEST = 0,
    ACTION_AES_KEY,
    ACTION_CMAC,
    ACTION_SIGN,
    ACTION_MAX
} ACTION;

extern unsigned char AES_KEY[AES_KEY_LEN];

void show_help(char * const help[], int num);
unsigned char* read_file(const char *f_name, size_t *buff_len);
int write_file(const char *f_name, unsigned char *buff, const size_t buff_len, \
    Image_Header_T *img_p, unsigned int img_size, TLV_T *tlv_p, unsigned int tlv_size);
void do_test(void);
void do_key(const char *f_key);
void do_cmac(const char *f_in, const char *f_key);
void do_sign(const char *f_in, const char *f_out, const char *f_key);

#endif /* __UTILS_H__ */
