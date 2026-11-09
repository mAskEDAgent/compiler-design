#include <stdio.h>
#include <string.h>

#define MAX 30

int n;
char lhs[MAX], rhs[MAX][20];
int first[26][128], follow[26][128];

int isNT(char c) { return c >= 'A' && c <= 'Z'; }

/* add c to set; return 1 if it was new */
int add(int *set, int c) {
    if (!set[c]) { set[c] = 1; return 1; }
    return 0;
}

void computeFirst() {
    int changed;
    do {
        changed = 0;
        for (int p = 0; p < n; p++) {
            int A = lhs[p] - 'A';
            char *r = rhs[p];

            if (r[0] == '#') {                    /* A -> epsilon */
                changed |= add(first[A], '#');
                continue;
            }
            int allNull = 1;
            for (int i = 0; r[i]; i++) {
                if (!isNT(r[i])) {                /* terminal: add and stop */
                    changed |= add(first[A], r[i]);
                    allNull = 0;
                    break;
                }
                int B = r[i] - 'A';
                for (int c = 0; c < 128; c++)     /* FIRST(B) minus epsilon */
                    if (first[B][c] && c != '#')
                        changed |= add(first[A], c);
                if (!first[B]['#']) { allNull = 0; break; }
            }
            if (allNull) changed |= add(first[A], '#');
        }
    } while (changed);
}

void computeFollow() {
    int changed;
    add(follow[lhs[0] - 'A'], '$');               /* rule 1 */
    do {
        changed = 0;
        for (int p = 0; p < n; p++) {
            int A = lhs[p] - 'A';
            char *r = rhs[p];

            for (int i = 0; r[i]; i++) {
                if (!isNT(r[i])) continue;
                int B = r[i] - 'A';
                int restNull = 1;                 /* is everything after B nullable? */

                for (int j = i + 1; r[j]; j++) {
                    if (!isNT(r[j])) {            /* terminal after B */
                        changed |= add(follow[B], r[j]);
                        restNull = 0;
                        break;
                    }
                    int C = r[j] - 'A';
                    for (int c = 0; c < 128; c++) /* FIRST(C) minus epsilon */
                        if (first[C][c] && c != '#')
                            changed |= add(follow[B], c);
                    if (!first[C]['#']) { restNull = 0; break; }
                }
                if (restNull)                     /* FOLLOW(A) flows into FOLLOW(B) */
                    for (int c = 0; c < 128; c++)
                        if (follow[A][c])
                            changed |= add(follow[B], c);
            }
        }
    } while (changed);
}

void printSet(const char *name, char nt, int *set) {
    printf("%s(%c) = { ", name, nt);
    for (int c = 0; c < 128; c++)
        if (set[c]) printf("%c ", c);
    printf("}\n");
}

int main() {
    char buf[50];
    printf("Number of productions: ");
    scanf("%d", &n);
    printf("Enter productions as A=xyz (# for epsilon):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        lhs[i] = buf[0];
        strcpy(rhs[i], buf + 2);
    }

    computeFirst();
    computeFollow();

    char seen[26] = {0};
    printf("\n");
    for (int p = 0; p < n; p++) {
        if (seen[lhs[p] - 'A']) continue;
        seen[lhs[p] - 'A'] = 1;
        printSet("FIRST", lhs[p], first[lhs[p] - 'A']);
    }
    printf("\n");
    memset(seen, 0, sizeof(seen));
    for (int p = 0; p < n; p++) {
        if (seen[lhs[p] - 'A']) continue;
        seen[lhs[p] - 'A'] = 1;
        printSet("FOLLOW", lhs[p], follow[lhs[p] - 'A']);
    }
    return 0;
}
