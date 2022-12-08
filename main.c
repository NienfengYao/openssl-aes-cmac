#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

static char * const help_strings[] = {
    "[-h|--help]",
    "[-t|--test] Do Test Cases",
    "[-i|--in] <file>",
    "[-o|--out] <file>",
    "[-c|--cal] Do AES-CMAC Calculation",
    "[-s|--sign] Do Sign",
};

int main(int argc, char* argv[])
{
    int ch;
    char f_in[FILE_NAME_LEN], f_out[FILE_NAME_LEN];
    ACTION act = ACTION_MAX;

    memset(f_in, '\0', FILE_NAME_LEN);
    memset(f_out, '\0', FILE_NAME_LEN);
    while((ch = getopt(argc, argv, "hti:o:cs")) != -1) {
        switch(ch) {
            case 'h':
                break;
            case 't':
                act = ACTION_TEST;
                break;
            case 'i':
                strcpy(f_in, optarg);
                break;
            case 'o':
                strcpy(f_out, optarg);
                break;
            case 'c':
                act = ACTION_CMAC;
                break;
            case 's':
                act = ACTION_SIGN;
                break;
            case '?': /* invalid option */
                printf("unknown option \n");
                break;
            default:
                printf("default \n");
        }
    }
    //printf("f_in:%s(%ld)\n", f_in, strlen(f_in));
    //printf("f_out:%s(%ld)\n", f_out, strlen(f_out));
    switch(act) {
        case ACTION_TEST:
            //printf("ACTION_TEST\n");
            test_case();
            break;
        case ACTION_CMAC:
            //printf("ACTION_CMAC\n");
            if(strlen(f_in) == 0)
                break;
            file_cmac(f_in);
            break;
        case ACTION_SIGN:
            //printf("ACTION_SIGN\n");
            if(strlen(f_in)==0 || strlen(f_out)==0)
                break;
            sign(f_in, f_out);
            break;
        default:
            show_help(help_strings, ARRAY_SIZE(help_strings));
            printf("example:\n");
            printf("\t%s -t (Do test case)\n", argv[0]);
            printf("\t%s -i file -c (Read data from file and do AES-CMAC operation)\n", argv[0]);
            printf("\t%s -i file -o file -s (Read data from file and generate the signature)\n", argv[0]);
            break;
    }
    return 0;
}
