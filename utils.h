#ifndef __UTILS_H__
#define __UTILS_H__

typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;
typedef unsigned int         uint32_t;
typedef enum { false = 0, true = !false } bool;


# define AES_KEY_LEN    16
# define FILE_NAME_LEN  64
# define ARRAY_SIZE(a)  (sizeof(a) / sizeof((a)[0]))

typedef enum action{
    ACTION_TEST = 0,
    ACTION_CMAC,
    ACTION_SIGN,
    ACTION_MAX
} ACTION;

void show_help(char * const help[], int num);
void print_bytes(unsigned char* buf, const size_t len);
void test_case(void);
unsigned char* read_file(const char *f_name, size_t *buff_len);
void file_cmac(const char *f_name);

#endif /* __UTILS_H__ */
