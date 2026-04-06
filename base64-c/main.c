#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define err(...) fprintf(stderr, __VA_ARGS__);
#define read(...) fgets(__VA_ARGS__, sizeof(__VA_ARGS__), stdin);

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* encode_base2_char(char c) {
    char *result = malloc(9 * sizeof(char)); // a string that has 9 bytes, 8 for data, 1 for '\0'
    // the binary representation (as of a string) of a char always has 8 bytes
    for (int i = 7; i >= 0; --i) {
	if ((c >> i) & 1)
            result[7 - i] = '1';
        else
            result[7 - i] = '0';
    }
    result[8] = '\0';
    return result;
}

char** encode_base2(char *s, int *size) {
    int len = strlen(s);
    *size = len;
    char **result = malloc(len * sizeof(char*));
    for (int i = 0; i < len; ++i)
	result[i] = encode_base2_char(s[i]);
    return result;
}

char *encode_base64(char *src) {
    int len = strlen(src) - 1;
    if (len >= 0 && src[len] == '\n') {
	src[len] = '\0';
    }
    int size = ((len + 2) / 3) * 4 + 1;
    char *result = malloc(size * sizeof(char));
    if (result == NULL) {
	return NULL;
    }
    unsigned char* s = (unsigned char*) src;
    int j = 0;
    for (int i = 0; i < len; i += 3) {
	result[j] = base64_table[s[i] >> 2];
	if (i+1 < len) {
	    result[j+1] = base64_table[(s[i+1] >> 4) | ((s[i] & 3) << 4)];
	} else {
	    result[j+1] = base64_table[(s[i] & 3) << 4];
	    result[j+2] = result[j+3] = '=';
	    j += 4;
	    continue;
	}
	if (i+2 < len) {
	    result[j+2] = base64_table[(s[i+2] >> 6) | ((s[i+1] & 15) << 2)];
	} else {
	    result[j+2] = base64_table[((s[i+1] & 15) << 2)];
	    result[j+3] = '=';
	    j += 4;
	    continue;
	}
	result[j+3] = base64_table[s[i+2] & 63];
	j += 4;
    }
    result[j] = '\0';
    return result;
}

int main() {
    char s[1024];
    read(s);
    char *ans = encode_base64(s);
    if (ans == NULL) {
	err("malloc is NULL");
    }
    printf("%s", ans);	
    free(ans);
    return 0;
}
