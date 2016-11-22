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
