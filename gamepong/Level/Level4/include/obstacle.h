#pragma once 
#include <vector>
#include <iostream>
#include <fstream>

class Obstacle {
private:
    bool check_inside(int** matrix, int n, int m) {
        //(0, 0) <= (a, b, c, d) < (n , m)
        if (!(0 <= a && a < n && 0 <= c && c < n && 0 <= b && b < m && 0 <= d && d < m)) return false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (matrix[i][j] == 1 || matrix[i][j] == 2 || matrix[i][j] == 3) {
                    //(a, b) <= (i, j) <= (c, d)
                    if (a <= i && i <= c && b <= j && j <= d) return false;
                }
            }
        }
        return true;
    }
public:
    int a, b, c, d;
    Obstacle(int a, int b, int c, int d) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }
    void move_up(int** matrix, int NUMROW, int NUMCOL) {
        a--, c--;
        if (!check_inside(matrix, NUMROW, NUMCOL)) move_down(matrix, NUMROW, NUMCOL);
    }
    void move_down(int** matrix, int NUMROW, int NUMCOL) {
        a++, c++;
        if (!check_inside(matrix, NUMROW, NUMCOL)) move_up(matrix, NUMROW, NUMCOL);
    }
    void move_left(int** matrix, int NUMROW, int NUMCOL) {
        b--, d--;
        if (!check_inside(matrix, NUMROW, NUMCOL)) move_right(matrix, NUMROW, NUMCOL);
    }
    void move_right(int** matrix, int NUMROW, int NUMCOL) {
        b++, d++;
        if (!check_inside(matrix, NUMROW, NUMCOL)) move_left(matrix, NUMROW, NUMCOL);
    }
    bool intersection(const Obstacle& other) {
        //[b, d] , [other.b, other.d]
        if (d < other.b || other.d < b) return false;
        //[a, c], [other.a, other.c]
        if (c < other.a || other.c < a) return false;
        return true;
    }
    std::pair <int, int> size() {
        return { abs(c - a), abs(d - b) };
    }
};