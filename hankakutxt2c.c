#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *input, *output;
    char buf[256];
    int b;

    if ((input = fopen("hankaku.txt", "r")) == NULL) {
        printf("ERROR: Couldn't open hankaku.txt\n");
        exit(1);
    }

    if ((output = fopen("hankaku.c", "w")) == NULL) {
        printf("ERROR: Couldn't open hankaku.c\n");
        exit(1);
    }

    fprintf(output, "char hankaku[4096] = {\n");

    int count = 0;

    while (fgets(buf, sizeof(buf), input) != NULL) {
        if (strncmp(buf, "char", 4) == 0) {
            for (int i = 0; i < 16; i++) {
                b = 0;
                fgets(buf, sizeof(buf), input);
                for (int j = 0; j < 8; j++) {
                    if (buf[j] == '*')
                        b |= (1 << (8-j-1));
                }
                fprintf(output, "0x%02x", b);
                if (i == 15) {
                    if (count != 255)
                        fprintf(output, ",");
                    fprintf(output, "\n");
                } else {
                    fprintf(output, ", ");
                }
            }
            count++;
        }
    }

    fprintf(output, "};\n");

    fclose(input);
    fclose(output);
    return 0;
}
