#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define err(...) fprintf(stderr, __VA_ARGS__)
#define read(...) fgets(__VA_ARGS__, sizeof(__VA_ARGS__), stdin)

#ifdef DEBUG
#define debug(...) do { printf("[debug] "); printf(__VA_ARGS__); } while (0)
#else
#define debug(...)
#endif

char* xor_cipher(char *input, int *size, char *passphrase, int *pass_size) {
    int len = strlen(input) - 1;
    if (len >= 0 && input[len] == '\n') {
	input[len] = '\0';
    } else {
	len++;
    }
    *size = len;
    int pass_len = strlen(passphrase) - 1;
    if (pass_len >= 0 && passphrase[pass_len] == '\n') {
	passphrase[pass_len] = '\0';
    } else {
	pass_len++;
    }
    *pass_size = pass_len;
    if (len > pass_len) {
	// repeat until phassphrase until len = pass_len
	debug("passphrase length is less than input length, %d < %d\n", pass_len, len);
    }
    if (len < pass_len) {
	// truncate some last char of passphrase until phassphrase until len = pass_len
	debug("passphrase length is more than input length, %d > %d\n", pass_len, len);
    }
    if (len == pass_len) {
	// if it is equal, we don't need to do anything
	debug("passphrase length is equal to input length, %d = %d\n", pass_len, len);
    }
    // here we do malloc
    char *result = malloc(len * sizeof(char) + 1);
    if (result == NULL) {
	err("malloc is NULL");
	return NULL;
    }
    if (pass_len < 1) {
	free(result);
	err("invalid pass_len received: %d", pass_len);
	return NULL;
    }
    for (int i = 0; i < len; ++i) {
	result[i] = input[i] ^ passphrase[i % pass_len];
    }
    result[len] = '\0';
    return result;
    // but we never free it?
}

int main() {
    char buf1[1024], buf2[1024];
    if (read(buf1) == NULL || read(buf2) == NULL) {
	err("bad data");
    	exit(1);
    }
    int size, pass_size;
    char *ans = xor_cipher(buf1, &size, buf2, &pass_size);
    if (ans == NULL) {
	exit(1);
    }
    for (int i = 0; i < size; ++i) {
    	int n = (int)(unsigned char)ans[i]; // unsigned for positive (0 - 255)
	printf("0x%02x\n", n);
    }
    // the solution here is to free ans
    free(ans);
    return 0;
}
