#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

static char * const help_strings[] = {
    "[-h|--help]",
    "[-t|--test] Do Test Cases",
    "[-i|--in] <file>",
    "[-o|--out] <file>",
    "[-k|--key] <file>",
    "[-c|--cal] Do AES-CMAC Calculation",
    "[-s|--sign] Do Sign",
};

int main(int argc, char* argv[])
{
    int ch;
    char f_in[FILE_NAME_LEN], f_out[FILE_NAME_LEN];
    char f_key[FILE_NAME_LEN];
    ACTION act = ACTION_MAX;

    memset(f_in, '\0', FILE_NAME_LEN);
    memset(f_out, '\0', FILE_NAME_LEN);
    memset(f_key, '\0', FILE_NAME_LEN);
    while((ch = getopt(argc, argv, "htcsi:o:k:")) != -1) {
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
            case 'k':
                strcpy(f_key, optarg);
                act = ACTION_AES_KEY;
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
    /*
    printf("f_in:%s(%ld)\n", f_in, strlen(f_in));
    printf("f_out:%s(%ld)\n", f_out, strlen(f_out));
    printf("f_key:%s(%ld)\n", f_key, strlen(f_key));
    */
    switch(act) {
        case ACTION_TEST:
            do_test();
            break;
        case ACTION_AES_KEY:
            if(strlen(f_key) == 0)
                break;
            do_key(f_key);
            break;
        case ACTION_CMAC:
            if(strlen(f_in) == 0)
                break;
            do_cmac(f_in, f_key);
            break;
        case ACTION_SIGN:
            if(strlen(f_in)==0 || strlen(f_out)==0)
                break;
            do_sign(f_in, f_out, f_key);
            break;
        default:
            show_help(help_strings, ARRAY_SIZE(help_strings));
            printf("example:\n");
            printf("\t%s -t (Do test case)\n", argv[0]);
            printf("\t%s -k (Read key[128-bits] from file and show the content)\n", argv[0]);
            printf("\t%s -i file -c (Read data from file and do AES-CMAC operation)\n", argv[0]);
            printf("\t%s -i file -o file -s (Read data from file and generate the signature)\n", argv[0]);
            printf("\t%s -i file -k file -c (Read data/key from file and do AES-CMAC operation)\n", argv[0]);
            printf("\t%s -i file -k file -o file -s (Read data/key from file and generate the signature)\n", argv[0]);
            break;
    }
    return 0;
}
