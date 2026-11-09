#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXV 50
#define MAXL 50

char names[MAXV][20];
int  isInput[MAXV];
int  val[MAXV];
int  nvars = 0;

struct line {
    char res[20], a1[20], a2[20];
    char op;                 /* + - * /   or '=' for a plain copy */
} code[MAXL];
int nlines = 0;

int isNumber(const char *s) { return isdigit((unsigned char)s[0]); }

int findVar(const char *n) {
    for (int i = 0; i < nvars; i++)
        if (strcmp(names[i], n) == 0) return i;
    return -1;
}

/* a name that is read before it was ever assigned is an input */
void useVar(const char *n) {
    if (isNumber(n) || findVar(n) >= 0) return;
    strcpy(names[nvars], n);
    isInput[nvars++] = 1;
}

/* a name that is assigned by an instruction */
void defVar(const char *n) {
    if (findVar(n) >= 0) return;
    strcpy(names[nvars], n);
    isInput[nvars++] = 0;
}

int main(void) {
    FILE *in = fopen("tac.txt", "r");
    if (!in) {
        printf("Cannot open tac.txt (run the intermediate code program first)\n");
        return 1;
    }

    char buf[100], op[2];
    while (nlines < MAXL && fgets(buf, sizeof buf, in)) {
        struct line *L = &code[nlines];
        int k = sscanf(buf, "%19s = %19s %1s %19s", L->res, L->a1, op, L->a2);
        if (k == 2) { L->op = '='; L->a2[0] = '\0'; }
        else if (k == 4) L->op = op[0];
        else continue;                     /* skip blank or malformed lines */

        useVar(L->a1);
        if (L->op != '=') useVar(L->a2);
        defVar(L->res);
        nlines++;
    }
    fclose(in);

    /* ask for the starting value of every input variable */
    for (int i = 0; i < nvars; i++)
        if (isInput[i]) {
            printf("Enter value of %s: ", names[i]);
            scanf("%d", &val[i]);
        }

    FILE *out = fopen("output.asm", "w+");

    fprintf(out, ".MODEL SMALL\n.STACK 100H\n.DATA\n");
    for (int i = 0; i < nvars; i++)
        fprintf(out, "    %s DW %d\n", names[i], isInput[i] ? val[i] : 0);

    fprintf(out, ".CODE\nMAIN PROC\n    MOV AX, @DATA\n    MOV DS, AX\n\n");

    for (int i = 0; i < nlines; i++) {
        struct line *L = &code[i];

        if (L->op == '=') {
            fprintf(out, "    ; %s = %s\n", L->res, L->a1);
            fprintf(out, "    MOV AX, %s\n", L->a1);
        } else {
            fprintf(out, "    ; %s = %s %c %s\n", L->res, L->a1, L->op, L->a2);
            fprintf(out, "    MOV AX, %s\n", L->a1);
            switch (L->op) {
                case '+': fprintf(out, "    ADD AX, %s\n", L->a2); break;
                case '-': fprintf(out, "    SUB AX, %s\n", L->a2); break;
                case '*': fprintf(out, "    MOV BX, %s\n    MUL BX\n", L->a2); break;
                case '/': fprintf(out, "    MOV DX, 0\n    MOV BX, %s\n    DIV BX\n", L->a2); break;
            }
        }
        fprintf(out, "    MOV %s, AX\n\n", L->res);
    }

    fprintf(out, "    MOV AH, 4CH\n    INT 21H\nMAIN ENDP\nEND MAIN\n");

    /* show the file on screen */
    printf("\nGenerated 8086 code (also saved in output.asm):\n\n");
    rewind(out);
    int c;
    while ((c = fgetc(out)) != EOF) putchar(c);
    fclose(out);
    return 0;
}
