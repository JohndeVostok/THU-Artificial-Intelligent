#include <iostream>
#include <vector>
#include <string>

#include "Strategy.h"

int main() {
    int m, n, nox, noy, turn;
    std::cin >> m >> n >> nox >> noy >> turn;
    std::vector<std::vector<int>> board(m, std::vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::string str;
            std::cin >> str;
            if (str == "A") {
                board[i][j] = 1;
            }
            if (str == "B") {
                board[i][j] = 2;
            }
        }
    }
	debug(m, n, nox, noy, 8, 3, turn, board);    
}