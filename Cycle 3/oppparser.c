#include <stdio.h>
#include <string.h>

char stack[100], input[100];
int top = -1;

int idx(char c) {
    switch (c) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '(': return 4;
        case ')': return 5;
        case 'i': return 6;
        case '$': return 7;
    }
    return -1;
}

/* rows: terminal on top of stack, columns: next input symbol
   '<' shift, '=' shift (brackets), '>' reduce, 'x' error, 'a' accept */
char table[8][8] = {
/*          +    -    *    /    (    )    i    $  */
/* + */ { '>', '>', '<', '<', '<', '>', '<', '>' },
/* - */ { '>', '>', '<', '<', '<', '>', '<', '>' },
/* * */ { '>', '>', '>', '>', '<', '>', '<', '>' },
/* / */ { '>', '>', '>', '>', '<', '>', '<', '>' },
/* ( */ { '<', '<', '<', '<', '<', '=', '<', 'x' },
/* ) */ { '>', '>', '>', '>', 'x', '>', 'x', '>' },
/* i */ { '>', '>', '>', '>', 'x', '>', 'x', '>' },
/* $ */ { '<', '<', '<', '<', '<', 'x', '<', 'a' }
};

/* position of the topmost terminal on the stack (skips E) */
int topTerminal() {
    int i = top;
    while (stack[i] == 'E') i--;
    return i;
}

void show(int ip) {
    stack[top + 1] = '\0';
    printf("%-14s %-12s ", stack, input + ip);
}

int main() {
    int ip = 0;
    printf("Enter expression (use i for id), e.g. i+i*i : ");
    scanf("%s", input);
    strcat(input, "$");

    stack[++top] = '$';
    printf("\n%-14s %-12s %s\n", "STACK", "INPUT", "ACTION");

    while (1) {
        int t = topTerminal();
        char a = stack[t], b = input[ip];

        if (idx(b) < 0) {
            show(ip);
            printf("Error: invalid symbol '%c'\n", b);
            return 1;
        }

        char rel = table[idx(a)][idx(b)];
        show(ip);

        if (rel == 'a') {
            printf("ACCEPT\n");
            break;
        }
        else if (rel == '<' || rel == '=') {            /* shift */
            printf("Shift %c\n", b);
            stack[++top] = b;
            ip++;
        }
        else if (rel == '>') {                          /* reduce */
            int cur = t, start;

            /* walk down the stack to find where the handle begins */
            while (1) {
                int k = cur - 1;
                while (k >= 0 && stack[k] == 'E') k--;
                if (table[idx(stack[k])][idx(stack[cur])] == '<') {
                    start = k + 1;
                    break;
                }
                cur = k;
            }

            char h[100];
            int len = top - start + 1;
            strncpy(h, stack + start, len);
            h[len] = '\0';

            int ok = 0;
            if (strcmp(h, "i") == 0) ok = 1;
            else if (strcmp(h, "(E)") == 0) ok = 1;
            else if (len == 3 && h[0] == 'E' && h[2] == 'E' && strchr("+-*/", h[1]))
                ok = 1;

            if (!ok) {
                printf("Error: invalid handle '%s'\n", h);
                return 1;
            }

            printf("Reduce %s -> E\n", h);
            top = start - 1;
            stack[++top] = 'E';
        }
        else {
            printf("Error: no relation between %c and %c\n", a, b);
            return 1;
        }
    }
    return 0;
}
