#include <stdio.h>
#include <string.h>
#define MAX 20

int n;
int eps[MAX][MAX];      /* eps[i][j] = 1 if there is an epsilon edge i -> j */
int visited[MAX];

void closure(int s) {
    visited[s] = 1;
    for (int j = 0; j < n; j++)
        if (eps[s][j] && !visited[j])
            closure(j);
}

int main() {
    int t, from, to;
    char sym;

    printf("Number of states: ");
    scanf("%d", &n);
    printf("Number of transitions: ");
    scanf("%d", &t);

    printf("Enter transitions as: from symbol to  (use 'e' for epsilon)\n");
    for (int i = 0; i < t; i++) {
        scanf("%d %c %d", &from, &sym, &to);
        if (sym == 'e')
            eps[from][to] = 1;      /* only store epsilon edges */
    }

    for (int s = 0; s < n; s++) {
        memset(visited, 0, sizeof(visited));
        closure(s);
        printf("e-closure(q%d) = { ", s);
        for (int j = 0; j < n; j++)
            if (visited[j]) printf("q%d ", j);
        printf("}\n");
    }
    return 0;
}
