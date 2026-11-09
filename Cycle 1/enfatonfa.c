#include <stdio.h>
#define MAX 10

int n, m;                   /* number of states, number of symbols (without e) */
char sym[MAX];
int d[MAX][MAX][MAX];       /* d[q][a][r] = 1 if q --a--> r (original) */
int cl[MAX][MAX];           /* cl[i][j] = 1 if j is in e-closure(i) */
int nd[MAX][MAX][MAX];      /* new NFA transitions */
int fin[MAX], nfin[MAX];

int symIndex(char c) {
    for (int i = 0; i < m; i++)
        if (sym[i] == c) return i;
    return -1;
}

int main() {
    int t, f, from, to;
    char c;

    printf("Number of states: ");  scanf("%d", &n);
    printf("Number of symbols (excluding e): ");  scanf("%d", &m);
    printf("Enter the symbols: ");
    for (int i = 0; i < m; i++) scanf(" %c", &sym[i]);

    printf("Number of transitions: ");  scanf("%d", &t);
    printf("Enter: from symbol to  (e for epsilon)\n");

    for (int i = 0; i < n; i++) cl[i][i] = 1;       /* a state is in its own closure */

    for (int i = 0; i < t; i++) {
        scanf("%d %c %d", &from, &c, &to);
        if (c == 'e') cl[from][to] = 1;             /* epsilon edge */
        else          d[from][symIndex(c)][to] = 1;
    }

    printf("Number of final states: ");  scanf("%d", &f);
    printf("Enter final states: ");
    for (int i = 0; i < f; i++) { scanf("%d", &to); fin[to] = 1; }

    /* Warshall: turn direct e-edges into full closure */
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (cl[i][k] && cl[k][j]) cl[i][j] = 1;

    /* new transitions: cl(q) --a--> p --a--> r, then cl(r) */
    for (int q = 0; q < n; q++)
        for (int a = 0; a < m; a++)
            for (int p = 0; p < n; p++)
                if (cl[q][p])
                    for (int r = 0; r < n; r++)
                        if (d[p][a][r])
                            for (int s = 0; s < n; s++)
                                if (cl[r][s]) nd[q][a][s] = 1;

    /* new final states */
    for (int q = 0; q < n; q++)
        for (int p = 0; p < n; p++)
            if (cl[q][p] && fin[p]) nfin[q] = 1;

    /* output */
    printf("\nNFA without epsilon:\n");
    for (int q = 0; q < n; q++) {
        for (int a = 0; a < m; a++) {
            printf("d'(q%d,%c) = { ", q, sym[a]);
            for (int s = 0; s < n; s++)
                if (nd[q][a][s]) printf("q%d ", s);
            printf("}\n");
        }
    }
    printf("Final states: ");
    for (int q = 0; q < n; q++) if (nfin[q]) printf("q%d ", q);
    printf("\n");
    return 0;
}
