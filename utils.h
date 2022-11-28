#ifndef __UTILS_H__
#define __UTILS_H__

typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;
typedef unsigned int         uint32_t;
typedef enum { false = 0, true = !false } bool;


# define AES_KEY_LEN 16
# define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

void show_help(char * const help[], int num);
void print_bytes(unsigned char* buf, const size_t len);
void unit_test(void);

#endif /* __UTILS_H__ */
