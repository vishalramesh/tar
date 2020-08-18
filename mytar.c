#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    bool args_present[4] = {0, 0, 0, 0};

    if (!(argc >= 2)) {
        fflush(stdout); 
        fprintf(stderr, "mytar: need at least one option\n");
        return 2;
    }

    int file_arg_index = 0;

    for (int i = 1; i < argc; ++i) {

        char ch = argv[i][1]; 

        if (argv[i][0] != '-')
            continue;
        else if (ch == 'f') {
            if (i >= argc - 1) {
                fflush(stdout);
                fprintf(stderr, "mytar: option requires an argument -- -%c\n", ch);
            }
            if (i < argc - 1 && (strcmp(argv[i + 1], "-t") == 0)) {
                fflush(stdout);
                fprintf(stderr, "mytar: You must specify one of the options\n");
            }
            file_arg_index = i + 1;
            args_present[0] = 1;
        } else if (ch == 't') {
            args_present[1] = 1;            
        } else if (ch == 'v') {
            args_present[2] = 1;  
        } else if (ch == 'x') {
            args_present[3] = 1;
        } else {
            fflush(stdout);
            fprintf(stderr, "mytar: Unknown option: %c\n", ch);
            return 2;
        }
    }
    
    if (args_present[0]) {
            fflush(stdout);
            fprintf(stderr, "mytar: Refusing to read archive contents from terminal (missing-f option?)\n");
            fflush(stdout);
            fprintf(stderr, "mytar: Error is not recoverable: exiting now\n");
            return 2;
        }

    if (args_present[1] && args_present[3]) {
        fflush(stdout);
        fprintf(stderr, "mytar: You may not specify more than one option\n");
        return 2;
    }

    FILE *file = fopen(argv[file_arg_index], "r");
    if (*file == NULL) {
        fflush(stdout);
        fprintf(stderr, "mytar: %s: Cannot open: No such file or directory\n", argv[file_arg_index]);
        fflush(stdout);
        fprintf(stderr, "mytar: Error is not recoverable: exiting now\n");
        return 2;
    }

}