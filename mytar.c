#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    int args_present[4] = {0, 0, 0, 0};

    int file_arg_index = 0;
    int list_arg_index = 0;
    int extract_arg_index = 0;

    int list_arg_present = 0;

    if (!(argc >= 2)) {
        fflush(stdout); 
        fprintf(stderr, "mytar: need at least one option\n");
        return 2;
    }

    for (int i = 1; i < argc; ++i) {

        char ch = argv[i][1]; 

        if (argv[i][0] != '-') {
            continue;
        } else if (ch == 'f') {
            if (i >= argc - 1) {
                fflush(stdout);
                fprintf(stderr, "mytar: option requires an argument -- -%c\n", ch);
                return 64;
            }
            if (i < argc - 1 && (strcmp(argv[i + 1][0], '-') == 0)) {
                fflush(stdout);
                fprintf(stderr, "mytar: You must specify one of the options\n");
                return 2;
            }
            file_arg_index = i + 1;
            args_present[0] = 1;
        } else if (ch == 't') {
            list_arg_index = i + 1;
            args_present[1] = 1;            
        } else if (ch == 'v') {
            args_present[2] = 1;  
        } else if (ch == 'x') {
            extract_arg_index = i + 1;
            args_present[3] = 1;
        } else {
            fflush(stdout);
            fprintf(stderr, "mytar: Unknown option: %c\n", ch);
            return 2;
        }
    }

    if (!args_present[0]) {
        fflush(stdout);
        fprintf(stderr, "mytar: Refusing to read archive contents from terminal (missing-f option?)\n");
        fprintf(stderr, "mytar: Error is not recoverable: exiting now\n");
        return 2;
    }

    if (args_present[1] && args_present[3]) {
        fflush(stdout);
        fprintf(stderr, "mytar: You may not specify more than one option\n");
        return 2;
    }

    FILE *tar_file = fopen(argv[file_arg_index], "r");
    if (tar_file == NULL) {
        fflush(stdout);
        fprintf(stderr, "mytar: %s: Cannot open: No such file or directory\n", argv[file_arg_index]);
        fprintf(stderr, "mytar: Error is not recoverable: exiting now\n");
        return 2;
    }

    char header[512];
    char file_name[100];
    char size[12];
    char magic[6];
    char typeflag;

    while (tar_file != NULL) {

        fread(header, 512, 1, tar_file);

        for (int i = 0; i < 100; ++i) {
            file_name[i] = header[i];
        }
        for (int i = 124; i < 136; ++i) {
            size[i - 124] = header[i];
        }
        for (int i = 257; i < 263; ++i) {
            magic[i - 257] = header[i];
        }
        typeflag = header[156];

        if (magic[0] != 'u' || magic[1] != 's' || magic[2] != 't' || magic[3] != 'a' || magic[4] != 'r' || magic[5] != '\0') {
            fflush(stdout);
            fprintf(stderr, "mytar: This does not look like a tar archive\n");
            fprintf(stderr, "mytar: Exiting with failure status due to previous errors\n");
            return 2;
        }

        if (typeflag != '0' && typeflag != '\0') {
            fflush(stdout);
            fprintf(stderr, "mytar: Unsupported header type: %d\n", header[156]);
            return 2;
        }

         if (!list_arg_present && args_present[1]) {
            print_default_list_output(file_name);
        }

        // if (list_arg_present) {
        //     print_list_arg_output(argv, print_file, file_name, list_arg_index, final_list_arg_index);
        // }

    }
    

    fclose(tar_file);
}

void print_default_list_output(char file_name[]) {
    int i = 0;
    int printable = 0;
    while (file_name[i] != '\0' && i < 100) {
        if (isalnum(file_name[i])) {
            printable = 1;
        }
        i += 1;
    }
    if (printable) {
        printf("%s\n", file_name);
        fflush(stdout);
    }
}