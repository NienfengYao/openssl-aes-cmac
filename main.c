#include <stdio.h>
#include <unistd.h>
#include "utils.h"

static char * const help_strings[] = {
    "[-h|--help]",
    "[-t|--test] Do Unit Test",
    "[-i|--in] <file>",
    "[-o|--out] <file>",
    "[-c|--cal] Do AES-CMAC Calculation",
    "[-s|--sign] Do Sign",
};

int main(int argc, char* argv[])
{
    int ch;

    if(argc <= 1){
        printf("%s -h\n", argv[0]);
        return 0;
    }
    while((ch = getopt(argc, argv, "hti:o:cs")) != -1) {
        switch(ch) {
            case 'h':
                show_help(help_strings, ARRAY_SIZE(help_strings));
                printf("example:\n");
                printf("\t%s -t (Do unit test)\n", argv[0]);
                printf("\t%s -i file -c (AES-CMAC value of file)\n", argv[0]);
                printf("\t%s -i file -o file -s (Generate the signature)\n", argv[0]);
                break;
            case 't':
                unit_test();
                break;
            case 'b':
                printf("option b \n");
                break;
            case '?': /* invalid option */
                printf("unknown option \n");
                break;
            default:
                printf("default \n");
        }
    }
    return 0;
}
