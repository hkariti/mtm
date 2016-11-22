#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

char* partialCopyString(char* str, bool copy_even, int* out_len) {
    char* OUT;
    if (copy_even) {
        out_len = malloc(sizeof(int));
        *out_len = strlen(str) / 2;
        OUT = malloc(*out_len);
        for (int i = 0; i < strlen(str); i += 2) {
            OUT[i / 2] = str[i + 1];
        }
    }
    else
    {
        out_len = malloc(sizeof(int));
        *out_len = strlen(str) / 2 + strlen(str) % 2;
        OUT = malloc(*out_len);
        for (int i = 0; i < strlen(str); i += 2)
        {
            OUT[i / 2] = str[i];
        }
    }
    return OUT;
}

int main(int argc, char **argv) {
    char *newstr;
    int len;
    bool copy_even;

    if (argv[2][0] == '1')
        copy_even = true;
    else
        copy_even = false;

    newstr = partialCopyString(argv[1], copy_even, &len);
    printf("Length is %d\n", len);
    printf("Str is: %s\n", newstr);

    return 0;
}
