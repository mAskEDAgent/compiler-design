#include <stdio.h>
#include <string.h>

#define MAX 20

int n;
char lhs[MAX];
char rhs[MAX][20];

char stack[100];
int top = 0;                 /* number of symbols on the stack */
char input[100];
int ip = 0;                  /* position in the input */

/* find a production whose right side matches the top of the stack.
   If several match, the longest one wins. Returns -1 if none. */
int findReduce(void) {
    int best = -1, bestLen = 0;
    for (int p = 0; p < n; p++) {
        int len = strlen(rhs[p]);
        if (len == 0 || len > top) continue;
        if (strncmp(stack + top - len, rhs[p], len) == 0 && len > bestLen) {
            best = p;
            bestLen = len;
        }
    }
    return best;
}

int main(void) {
    char buf[50];

    printf("Number of productions: ");
    scanf("%d", &n);
    printf("Enter productions as A=xyz:\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        lhs[i] = buf[0];
        strcpy(rhs[i], buf + 2);
    }
    printf("Enter input string (i for id): ");
    scanf("%s", input);

    printf("\n%-16s %-14s %s\n", "STACK", "INPUT", "ACTION");

    while (1) {
        char tmp[100];
        stack[top] = '\0';
        sprintf(tmp, "%s$", input + ip);
        printf("$%-15s %-14s ", stack, tmp);

        /* 1. accept? */
        if (input[ip] == '\0' && top == 1 && stack[0] == lhs[0]) {
            printf("ACCEPT\n");
            return 0;
        }

        /* 2. can we reduce? */
        int p = findReduce();

        /* 3. precedence: if * or / is coming, do not reduce E+E or E-E yet */
        if (p >= 0) {
            char next = input[ip];
            if ((strcmp(rhs[p], "E+E") == 0 || strcmp(rhs[p], "E-E") == 0) &&
                (next == '*' || next == '/'))
                p = -1;
        }

        if (p >= 0) {                                   /* reduce */
            int len = strlen(rhs[p]);
            printf("Reduce %c -> %s\n", lhs[p], rhs[p]);
            top -= len;
            stack[top++] = lhs[p];
        }
        else if (input[ip] != '\0') {                   /* shift */
            printf("Shift %c\n", input[ip]);
            stack[top++] = input[ip++];
        }
        else {                                          /* stuck */
            printf("REJECT\n");
            return 1;
        }
    }
}
