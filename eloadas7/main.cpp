#include <iostream>
#include <iomanip>

#define N 100

using namespace std;

void hargitaDFS(int i, int j, int i2, int j2, int n, int a[][N], int &numOfAsc);

void medveDFS(int i, int j, int i2, int j2, int x1, int y1 ,int x2, int y2, int n, int a[][N], int &numOfAsc);


int main() {
    int n, a[N][N];
    freopen("hargita.txt", "r", stdin);
    cin >> n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a[i][j];
            cout << setw(2) << a[i][j] << " ";
        }
        cout << endl;
    }
    int i1, j1; //panzio
    cin >> i1 >> j1;
    int i2, j2; //csucs
    cin >> i2 >> j2;
    int x1, y1, x2, y2; //medve
    cin >> x1 >> y1 >> x2 >> y2;
    int numOfAsc = 0;

    hargitaDFS(i1, j1, i2, j2, n, a, numOfAsc);
    medveDFS(i1, j1, i2, j2, x1, y1, x2, y2, n, a, numOfAsc);


    cout << numOfAsc;

    return 0;
}

void hargitaDFS(int i, int j, int i2, int j2, int n, int a[][N], int &numOfAsc) {
    if (i == i2 and j == j2) {
        ++numOfAsc;
        return;
    }
    int di[] = {-1, 0, 1, 0};
    int dj[] = {0, 1, 0, -1};
    for (int k = 0; k < 4; ++k) {
        int nextI = i + di[k];
        int nextJ = j + dj[k];
//        if (valid(nextI, nextJ, n, a[i][j])){}
        if(nextI < 0 or nextI > n -1 or nextJ < 0 or nextJ > n - 1){
            continue;
        }
        if(a[nextI][nextJ] <= a[i][j]){
            continue;
        }
        hargitaDFS(nextI, nextJ, i2, j2, n, a, numOfAsc);
    }
}

void medveDFS(int i, int j, int i2, int j2, int x1, int y1 ,int x2, int y2, int n, int a[][N], int &numOfAsc) {
    if (i == i2 and j == j2) {
        ++numOfAsc;
        return;
    }
    int di[] = {-1, 0, 1, 0};
    int dj[] = {0, 1, 0, -1};
    for (int k = 0; k < 4; ++k) {
        int nextI = i + di[k];
        int nextJ = j + dj[k];
//        if (valid(nextI, nextJ, n, a[i][j])){}
        if(nextI < 0 or nextI > n -1 or nextJ < 0 or nextJ > n - 1){
            continue;
        }
        if(a[nextI][nextJ] <= a[i][j]){
            continue;
        }
        if()
        medveDFS(nextI, nextJ, i2, j2, x1, y1, x2, y2, n, a, numOfAsc);
    }
}