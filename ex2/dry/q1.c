#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

char* partialCopyString(char* str, bool copy_even, int* out_len) {
    char* out_str;
    if (NULL == str || NULL == out_len) return NULL;
	*out_len = strlen(str) / 2;
    if (!copy_even) {
        *out_len += strlen(str) % 2;
    }
    out_str = malloc(*out_len + 1);
    if (NULL == out_str) return NULL;
    for (int i = 0; i < strlen(str); i += 2) {
        out_str[i / 2] = str[i + (int)copy_even];
    }
	out_str[*out_len] = '\0';
    return out_str;
}

int main1(int argc, char **argv) {
    char *newstr;
    int len;
    bool copy_even;

    if (argv[2][0] == '1')
        copy_even = true;
    else
        copy_even = false;

	printf("Input String: %s\n", argv[1]);
	printf("copy_even %d\n", copy_even);

    newstr = partialCopyString(argv[1], copy_even, &len);
    printf("Length is %d\n", len);
    printf("Str is: %s\n", newstr);

    return 0;
}
