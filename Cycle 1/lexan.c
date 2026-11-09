#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NKW 14

int isKeyword(char *s) {
    char *kw[NKW] = {"int","float","char","double","void","if","else",
                     "while","for","do","return","break","continue","long"};
    for (int i = 0; i < NKW; i++)
        if (strcmp(s, kw[i]) == 0) return 1;
    return 0;
}

int main() {
    FILE *fp = fopen("input.c", "r");
    if (!fp) { printf("Cannot open input.c\n"); return 1; }

    int c, next, i;
    char buf[100];

    while ((c = fgetc(fp)) != EOF) {

        /* 1. skip whitespace */
        if (c == ' ' || c == '\t' || c == '\n') continue;

        /* 2. comments, or the division operator */
        if (c == '/') {
            next = fgetc(fp);
            if (next == '/') {                       /* single line */
                while ((c = fgetc(fp)) != EOF && c != '\n');
            } else if (next == '*') {                /* multi line */
                int prev = 0;
                while ((c = fgetc(fp)) != EOF) {
                    if (prev == '*' && c == '/') break;
                    prev = c;
                }
            } else {                                 /* just '/' */
                ungetc(next, fp);
                printf("/\tOPERATOR\n");
            }
            continue;
        }

        /* 3. identifier or keyword */
        if (isalpha(c) || c == '_') {
            i = 0;
            buf[i++] = c;
            while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_') && i < 99)
                buf[i++] = c;
            buf[i] = '\0';
            if (c != EOF) ungetc(c, fp);             /* push back extra char */
            printf("%s\t%s\n", buf, isKeyword(buf) ? "KEYWORD" : "IDENTIFIER");
        }

        /* 4. number */
        else if (isdigit(c)) {
            i = 0;
            buf[i++] = c;
            while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.') && i < 99)
                buf[i++] = c;
            buf[i] = '\0';
            if (c != EOF) ungetc(c, fp);
            printf("%s\tNUMBER\n", buf);
        }

        /* 5. string literal */
        else if (c == '"') {
            i = 0;
            buf[i++] = '"';
            while ((c = fgetc(fp)) != EOF && c != '"' && i < 98)
                buf[i++] = c;
            buf[i++] = '"';
            buf[i] = '\0';
            printf("%s\tSTRING\n", buf);
        }

        /* 6. operators (with 2-char lookahead) */
        else if (c == '=' || c == '<' || c == '>' || c == '!') {
            next = fgetc(fp);
            if (next == '=') printf("%c=\tOPERATOR\n", c);
            else { ungetc(next, fp); printf("%c\tOPERATOR\n", c); }
        }
        else if (strchr("+-*%", c))
            printf("%c\tOPERATOR\n", c);

        /* 7. separators */
        else if (strchr(";,(){}[]", c))
            printf("%c\tSEPARATOR\n", c);

        /* 8. anything else */
        else
            printf("%c\tUNKNOWN\n", c);
    }
    fclose(fp);
    return 0;
}
