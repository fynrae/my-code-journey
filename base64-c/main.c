#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define err(...) fprintf(stderr, __VA_ARGS__);
#define read(...) fgets(__VA_ARGS__, sizeof(__VA_ARGS__), stdin);

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
