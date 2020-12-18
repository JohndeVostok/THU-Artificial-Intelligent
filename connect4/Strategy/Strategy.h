/********************************************************
 *	Strategy.h : 策略接口文件                           *
 *	张永锋                                              *
 *	zhangyf07@gmail.com                                 *
 *	2014.5                                              *
 *********************************************************/

#pragma once

#include "Point.h"
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" Point *getPoint(const int M, const int N, const int *top,
                           const int *_board, const int lastX, const int lastY,
                           const int noX, const int noY);

extern "C" void clearPoint(Point *p);

void clearArray(int M, int N, int **board);

class Hasher {
  private:
    int created;
    std::vector<std::vector<std::vector<int>>> stat;

  public:
    Hasher();
    ~Hasher();
    int checkCreated();
    int init(int m, int n);
    uint32_t hash(std::vector<std::vector<int>> &board);
};

struct Node {
    int n, v;
};

class Mcts {
  private:
    int nt;
    double c;
    std::unordered_map<uint32_t, Node> map;
    std::default_random_engine randEngine;

  public:
    Mcts();
    ~Mcts();
    int clear();
    int getTop(std::vector<int> &top, int nox, int noy,
               std::vector<std::vector<int>> &board);
    int getRound(std::vector<std::vector<int>> &board);
    // 0 : not end, 1 : user win, 2 : machine win, 3 : tie
    int checkEnd(int nox, int noy, int lx, int ly,
                 std::vector<std::vector<int>> &board);
    bool checkWin(int lx, int ly, std::vector<std::vector<int>> &board);
    bool checkLose(int lx, int ly, std::vector<std::vector<int>> &board);
    bool checkTie(std::vector<int> &top);

    int roll(int nox, int noy, int lx, int ly, int turn,
             std::vector<std::vector<int>> &board);
    int getUniform(int nox, int noy, int turn,
                   std::vector<std::vector<int>> &board);
    int getMax(int nox, int noy, int turn,
               std::vector<std::vector<int>> &board);
};

int debug(int m, int n, int nox, int noy, int lx, int ly, int turn,
          std::vector<std::vector<int>> &board);