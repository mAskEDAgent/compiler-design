#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *ip;            /* current position in the input */
int error = 0;
int depth = 0;       /* for the call trace */

void E(void);  void Eprime(void);
void T(void);  void Tprime(void);
void F(void);

void skip(void) { while (*ip == ' ') ip++; }

void enter(const char *name) {
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", name);
    depth++;
}
void leave(void) { depth--; }

void E(void) {                      /* E -> T E' */
    enter("E");
    T();
    Eprime();
    leave();
}

void Eprime(void) {                 /* E' -> + T E' | - T E' | epsilon */
    enter("E'");
    skip();
    if (*ip == '+' || *ip == '-') {
        ip++;                       /* match the operator */
        T();
        Eprime();
    }
    /* else: epsilon, do nothing */
    leave();
}

void T(void) {                      /* T -> F T' */
    enter("T");
    F();
    Tprime();
    leave();
}

void Tprime(void) {                 /* T' -> * F T' | / F T' | epsilon */
    enter("T'");
    skip();
    if (*ip == '*' || *ip == '/') {
        ip++;
        F();
        Tprime();
    }
    leave();
}

void F(void) {                      /* F -> ( E ) | id */
    enter("F");
    skip();
    if (*ip == '(') {
        ip++;
        E();
        skip();
        if (*ip == ')') ip++;       /* match the closing bracket */
        else error = 1;
    }
    else if (isalnum((unsigned char)*ip)) {
        while (isalnum((unsigned char)*ip)) ip++;   /* consume the id/number */
    }
    else
        error = 1;
    leave();
}

int main(void) {
    char s[100];
    printf("Enter expression: ");
    fgets(s, sizeof s, stdin);
    s[strcspn(s, "\n")] = '\0';

    ip = s;
    printf("\nCall trace:\n");
    E();
    skip();

    if (!error && *ip == '\0')
        printf("\nValid expression\n");
    else
        printf("\nInvalid expression\n");
    return 0;
}
