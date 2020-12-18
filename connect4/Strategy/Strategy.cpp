#include "Strategy.h"
#include "Point.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

static Mcts mcts;
static Hasher hasher;

int print(std::vector<std::vector<int>> &board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
    return 0;
}

Hasher::Hasher() { created = 0; }

Hasher::~Hasher() {}

int Hasher::checkCreated() { return created; }

int Hasher::init(int m, int n) {
    std::default_random_engine rand;
    stat.clear();
    for (int i = 0; i < m; i++) {
        stat.emplace_back(0);
        for (int j = 0; j < n; j++) {
            stat[i].emplace_back(2);
            for (int k = 0; k < 2; k++) {
                stat[i][j][k] = rand();
            }
        }
    }
    return 0;
}

uint32_t Hasher::hash(std::vector<std::vector<int>> &board) {
    uint32_t res = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] > 0) {
                res += stat[i][j][board[i][j] - 1];
            }
        }
    }
    return res;
}

Mcts::Mcts() {
    nt = 0;
    c = 1.96;
    map.clear();
}

Mcts::~Mcts() {}

int Mcts::clear() {
    nt = 0;
    map.clear();
}

int Mcts::getTop(std::vector<int> &top, int nox, int noy,
                 std::vector<std::vector<int>> &board) {
    int m = board.size();
    int n = board[0].size();
    top.clear();
    for (int i = 0; i < n; i++) {
        top.emplace_back(m);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] > 0 && i < top[j]) {
                top[j] = i;
            }
        }
    }
    if (top[noy] - 1 == nox) {
        top[noy] = nox;
    }

    return 0;
}

bool Mcts::checkWin(const int x, const int y,
                    std::vector<std::vector<int>> &board) {
    int M = board.size();
    int N = board[0].size();
    //横向检测
    int i, j;
    int count = 0;
    for (i = y; i >= 0; i--)
        if (!(board[x][i] == 1))
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (!(board[x][i] == 1))
            break;
    count += (i - y - 1);
    if (count >= 4)
        return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (!(board[i][y] == 1))
            break;
    count += (i - x);
    if (count >= 4)
        return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == 1))
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == 1))
            break;
    count += (j - y - 1);
    if (count >= 4)
        return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == 1))
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (!(board[i][j] == 1))
            break;
    count += (j - y - 1);
    if (count >= 4)
        return true;

    return false;
}

bool Mcts::checkLose(const int x, const int y,
                     std::vector<std::vector<int>> &board) {
    int M = board.size();
    int N = board[0].size();
    //横向检测
    int i, j;
    int count = 0;
    for (i = y; i >= 0; i--)
        if (!(board[x][i] == 2))
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (!(board[x][i] == 2))
            break;
    count += (i - y - 1);
    if (count >= 4)
        return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (!(board[i][y] == 2))
            break;
    count += (i - x);
    if (count >= 4)
        return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == 2))
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == 2))
            break;
    count += (j - y - 1);
    if (count >= 4)
        return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == 2))
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (!(board[i][j] == 2))
            break;
    count += (j - y - 1);
    if (count >= 4)
        return true;

    return false;
}

bool Mcts::checkTie(std::vector<int> &top) {
    int n = top.size();
    bool isTie = true;
    for (int i = 0; i < n; i++) {
        if (top[i] > 0) {
            isTie = false;
        }
    }
    if (isTie) {
        return true;
    }
    return false;
}

int Mcts::checkEnd(int nox, int noy, int lx, int ly,
                   std::vector<std::vector<int>> &board) {
    if (lx == -1 || ly == -1) {
        return 0;
    }
    int m = board.size();
    int n = board[0].size();
    // Check user
    if (checkWin(lx, ly, board)) {
        return 3;
    }

    if (checkLose(lx, ly, board)) {
        return 1;
    }

    std::vector<int> top;
    getTop(top, nox, noy, board);
    if (checkTie(top)) {
        return 2;
    }

    return 0;
}

int Mcts::roll(int nox, int noy, int lx, int ly, int turn,
               std::vector<std::vector<int>> &board) {
    int m = board.size();
    int n = board[0].size();
    uint32_t hash = hasher.hash(board);
    if (map.find(hash) == map.end()) {
        Node node;
        node.n = 0;
        node.v = 0;
        map.emplace(hash, node);
    }

    // std::cout << hasher.hash(board) << " " << map[hasher.hash(board)].n << "
    // " << map[hasher.hash(board)].v << std::endl; print(board); std::cout <<
    // std::endl;

    int res = checkEnd(nox, noy, lx, ly, board);
    if (res > 0) {
        nt++;
        map[hash].n++;
        map[hash].v += res - 1;
        return res - 1;
    }

    std::vector<int> top;
    getTop(top, nox, noy, board);

    std::vector<double> p(n);
    double sum = 0;

    std::vector<int> tmp(n);
    for (int i = 0; i < n; i++) {
        tmp[i] = i;
    }
    std::random_shuffle(tmp.begin(), tmp.end());
    for (int k = 0; k < n; k++) {
        int i = tmp[k];
        if (top[i] > 0) {
            board[top[i] - 1][i] = turn;
            uint32_t nextHash = hasher.hash(board);
            if (map.find(nextHash) == map.end()) {
                res = roll(nox, noy, top[i] - 1, i, 3 - turn, board);
                map[hash].n++;
                map[hash].v += res;
                board[top[i] - 1][i] = 0;
                return res;
            } else {
                auto vi = double(map[nextHash].v) / 2;
                auto ni = double(map[nextHash].n);
                if (turn == 2) {
                    vi = ni - vi;
                }
                p[i] = vi / ni + c * sqrt(log(nt) / ni);
                sum += p[i];
            }
            board[top[i] - 1][i] = 0;
        } else {
            p[i] = 0;
        }
    }

    // std::cout << "P: ";
    // for (int i = 0; i < n; i++) {
    // 	std::cout << p[i] << " ";
    // }

    double max = 0;
    int t = -1;
    for (int i = 0; i < n; i++) {
        if (p[i] > max) {
            max = p[i];
            t = i;
        }
    }

    board[top[t] - 1][t] = turn;
    res = roll(nox, noy, top[t] - 1, t, 3 - turn, board);
    board[top[t] - 1][t] = 0;
    map[hash].n++;
    map[hash].v += res;
    return res;

    // uniform_real_distribution<double> u(0, sum);
    // for (int i = 0; i < n; i++) {
    // 	if (x <= p[i]) {
    // 		board[top[i]-1][i] = turn;
    // 		res = roll(nox, noy, top[i]-1, i, 3-turn, board);
    // 		board[top[i]-1][i] = 0;
    // 		map[hash].n++;
    // 		map[hash].v += res;
    // 		return res;
    // 	}
    // 	x -= p[i];
    // }

    return 0;
}

int Mcts::getUniform(int nox, int noy, int turn,
                     std::vector<std::vector<int>> &board) {
    int m = board.size();
    int n = board[0].size();

    std::vector<int> top;
    getTop(top, nox, noy, board);

    double sum = 0;

    std::vector<double> p(n, 0);
    for (int i = 0; i < n; i++) {
        if (top[i] > 0) {
            board[top[i] - 1][i] = turn;
            uint32_t hash = hasher.hash(board);
            p[i] = double(map[hash].n);
            sum += p[i];
            board[top[i] - 1][i] = 0;
        }
    }

    uniform_real_distribution<double> u(0, sum);
    double x = u(randEngine);
    for (int i = 0; i < n; i++) {
        if (x <= p[i]) {
            return i;
        }
        x -= p[i];
    }

    return -1;
}

int Mcts::getMax(int nox, int noy, int turn,
                 std::vector<std::vector<int>> &board) {
    int m = board.size();
    int n = board[0].size();

    std::vector<int> top;
    getTop(top, nox, noy, board);

    double max = 0;
    int res = -1;
    for (int i = 0; i < n; i++) {
        if (top[i] > 0) {
            board[top[i] - 1][i] = turn;
            uint32_t hash = hasher.hash(board);
            auto vi = double(map[hash].v) / 2;
            auto ni = double(map[hash].n);
            if (turn == 2) {
                vi = ni - vi;
            }
            double tmp = vi / ni;
            //  + c * sqrt(log(nt) / ni);
            // std::cout << i << " " << vi << " " << ni << " " << nt <<
            // " " << tmp << " " << vi / ni << " " << c * sqrt(log(nt) /
            // ni) << std::endl;
            if (tmp > max) {
                max = tmp;
                res = i;
            }
            board[top[i] - 1][i] = 0;
        }
    }
    // std::cout << res << std::endl;
    return res;
}

int Mcts::getRound(std::vector<std::vector<int>> &board) {
    int cnt = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] > 0) {
                cnt++;
            }
        }
    }
    return cnt;
}

extern "C" Point *getPoint(const int M, const int N, const int *_top,
                           const int *_board, const int lastX, const int lastY,
                           const int noX, const int noY) {
    int x = -1, y = -1; //最终将你的落子点存到x,y中
    std::vector<std::vector<int>> board(M, std::vector<int>(N, 0));
    std::vector<int> top(N, 0);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = _board[i * N + j];
            if (board[i][j] > 0) {
                board[i][j] = 3 - board[i][j];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        top[i] = _top[i];
    }

    if (!hasher.checkCreated()) {
        hasher.init(M, N);
    }

    // mcts.clear();
    for (int i = 0; i < 5000; i++) {
        mcts.roll(noX, noY, lastX, lastY, 1, board);
    }

    int r = 0;
    int round = mcts.getRound(board);
    // if (round < 5) {
    //     r = mcts.getUniform(noX, noY, 1, board);
    // } else {
    r = mcts.getMax(noX, noY, 1, board);
    // }

    // print(board);
    // std::cout << top[r] << " " << r << std::endl;

    return new Point(top[r] - 1, r);
}

int debug(int m, int n, int nox, int noy, int lx, int ly, int turn,
          std::vector<std::vector<int>> &board) {
    if (!hasher.checkCreated()) {
        hasher.init(m, n);
    }

    for (int i = 0; i < 10000; i++) {
        mcts.roll(nox, noy, lx, ly, turn, board);
    }

    int r = mcts.getMax(nox, noy, turn, board);
}

/*
        getPoint函数返回的Point指针是在本so模块中声明的，为避免产生堆错误，应在外部调用本so中的
        函数来释放空间，而不应该在外部直接delete
*/
extern "C" void clearPoint(Point *p) {
    delete p;
    return;
}

/*
        清除top和board数组
*/
void clearArray(int M, int N, int **board) {
    for (int i = 0; i < M; i++) {
        delete[] board[i];
    }
    delete[] board;
}

/*
        添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h
   .cpp文件来辅助实现你的想法
*/
