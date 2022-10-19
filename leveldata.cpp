﻿#include "leveldata.h"
#include <vector>

const int N = 15;
const int Floor = 0;
const int Wall = 1;
const int Begin = 4;
const int Box = 3;
const int End = 2;

class mapnode
{
public:
    int level;
    int m[N + 1][N + 1];
    mapnode(int level) : level(level * 3)
    {
        while (true)
        {
            memset(m, Floor, sizeof(m));
            for (int i = 1; i <= 14; i++)
            {
                m[1][i] = Wall;
                m[i][15] = Wall;
                m[i + 1][1] = Wall;
                m[15][i + 1] = Wall;
            }
            srand((unsigned)time(0));
            while (true)
            {
                int randi = (rand() % N) + 1;
                int randj = (rand() % N) + 1;
                if (m[randi][randj] == Floor)
                {
                    m[randi][randj] = Begin;
                    break;
                }
            }
            srand((unsigned)time(0));
            int sum = ((rand() + level * 7) % (N * N)) % (N * N / 3) + level * 2;
            int cnt = 0;
            for (int i = 1; i <= sum; i++)
            {
                int p = ((rand() % 7) + (rand() % 7)) % 9 + 1;
                if (p <= 2)
                {
                    if (cnt >= level / 2 + 1)
                        continue;
                    GenBoxEnd();
                    cnt++;
                }
                else
                {
                    GenWall();
                }
            }
            cnt = 0;
            for (int i = 1; i <= N; i++)
            {
                for (int j = 1; j <= N; j++)
                {
                    if (m[i][j] == End)
                        cnt++;
                }
            }
            for (int i = cnt; i <= level / 2 + 1; i++)
            {
                GenBoxEnd();
            }

            int Wacnt = 0;
            for (int i = 2; i <= N - 1; i++)
            {
                for (int j = 2; j <= N - 1; j++)
                {
                    if (m[i][j] == Wall)
                        Wacnt++;
                }
            }
            for (int i = Wacnt / 4; i <= N / 3; i++)
            {
                GenWall();
            }

            int flag = true;
            for (int i = 1; i <= N; i++)
            {
                for (int j = 1; j <= N; j++)
                {
                    int xx = i, yy = j;
                    if (!Check2(xx, yy))
                        flag = false;
                }
            }
            if (flag)
                break;
        }
    }
    void GenWall()
    {
        int bi = (rand() % N) + 1;
        int bj = (rand() % N) + 1;
        srand((unsigned)time(0));
        int cnt = 0;
        int failed = 0;
        while (true)
        {
            int ai = (rand() % 2) - 1;
            int aj = (rand() % 2) - 1;
            ai += bi, aj += bj;
            failed++;
            if (ai == N + 1 || ai == 0)
            {
                continue;
            }
            if (aj == N + 1 || aj == 0)
            {
                continue;
            }
            if (m[ai][aj] == Floor && Check(ai, aj, Wall))
            {
                m[ai][aj] = Wall;
                bi = ai, bj = aj;
                cnt++;
                failed = 0;
            }
            if (failed > 100)
                break;
            if (cnt == 4)
                break;
        }
    }
    void GenBoxEnd()
    {
        srand((unsigned)time(0));
        while (true)
        {
            int randi = (rand() % N) + 1;
            int randj = (rand() % N) + 1;
            if (randi == N - 1 || randi == 2)
                continue;
            if (randj == N - 1 || randj == 2)
                continue;
            if (m[randi][randj] == Floor && Check(randi, randj, Box))
            {
                m[randi][randj] = Box;
                break;
            }
        }
        srand((unsigned)time(0));
        while (true)
        {
            int randi = (rand() % N) + 1;
            int randj = (rand() % N) + 1;
            if (m[randi][randj] == Floor && Check(randi, randj, End))
            {
                m[randi][randj] = End;
                break;
            }
        }
    }
    bool Check(int x, int y, int t)
    {
        m[x][y] = t;
        bool flag = true;
        std::vector<std::pair<int, int>> loc;
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int nx = x + i, ny = y + j;
                if (nx <= 1 || nx >= N)
                    continue;
                if (ny <= 1 || ny >= N)
                    continue;
                loc.emplace_back(nx, ny);
            }
        }
        for (int i = 0; i < loc.size(); i++)
        {
            int xx = loc[i].first, yy = loc[i].second;
            if (!Check2(xx, yy))
            {
                flag = false;
                break;
            }
        }
        m[x][y] = Floor;
        return flag;
    }
    bool Check2(int xx, int yy)
    {
        int flag = true;
        if (m[xx][yy] == End || m[xx][yy] == Begin || m[xx][yy] == Box)
        {
            // 四周包围
            int cnt = 0;
            if (m[xx][yy + 1] == Box || m[xx][yy + 1] == Wall)
                cnt++;
            if (m[xx][yy - 1] == Box || m[xx][yy - 1] == Wall)
                cnt++;
            if (m[xx - 1][yy] == Box || m[xx - 1][yy] == Wall)
                cnt++;
            if (m[xx - 1][yy] == Box || m[xx - 1][yy] == Wall)
                cnt++;
            if (cnt == 4)
                flag = false;
        }
        if (m[xx][yy] == Box)
        {
            // 两个成直角
            if (m[xx][yy + 1] == Wall && m[xx + 1][yy] == Wall)
                flag = false;
            if (m[xx][yy + 1] == Wall && m[xx - 1][yy] == Wall)
                flag = false;
            if (m[xx][yy - 1] == Wall && m[xx + 1][yy] == Wall)
                flag = false;
            if (m[xx][yy - 1] == Wall && m[xx - 1][yy] == Wall)
                flag = false;
        }
        return flag;
    }
};

LevelData::LevelData(int l, QObject *parent) : QObject(parent)
{
    switch (l)
    {
    case -1:
    {
        mapnode *k = new mapnode(rand() % 10 + 1);
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = k->m[i + 1][j + 1];
            }
        }
        break;
    }
    case 1:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
            {0, 0, 1, 1, 3, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 1, 0, 4, 0, 3, 0, 0, 3, 0, 1, 0, 0, 0},
            {0, 0, 1, 0, 2, 2, 1, 0, 3, 0, 1, 1, 0, 0, 0},
            {0, 0, 1, 1, 2, 2, 1, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 2:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 4, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 3, 0, 3, 0, 3, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 3, 1, 1, 0, 0, 1, 0, 0, 0},
            {0, 0, 1, 1, 1, 0, 3, 0, 1, 0, 1, 1, 0, 0, 0},
            {0, 0, 1, 2, 2, 2, 2, 2, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 3:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0},
            {0, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0},
            {0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0},
            {0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0},
            {0, 0, 0, -1, 0, 0, -1, 4, -1, 0, -1, 0, -1, 0, 0},
            {0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1, -1, 0, 0, 0},
            {0, 0, 0, -1, 0, -1, 0, 0, -1, -1, 0, 0, -1, 0, 0},
            {0, 0, 0, -1, 2, 0, 0, 0, 0, 0, 3, 0, -1, 0, 0},
            {0, 0, -1, 0, 0, -1, -1, -1, -1, -1, 0, 0, -1, 0, 0},
            {0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0},
            {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0},
            {0, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0},
            {0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 4:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 2, 0, 3, 1, 1, 0, 1, 1, 0, 0, 0},
            {0, 0, 1, 2, 2, 3, 0, 3, 0, 0, 4, 1, 0, 0, 0},
            {0, 0, 1, 2, 2, 0, 3, 0, 3, 0, 1, 1, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 5:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 3, 0, 1, 1, 1, 0, 3, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 1, 2, 2, 2, 1, 0, 1, 0, 0, 0},
            {0, 0, 1, 1, 0, 1, 2, 2, 2, 1, 0, 1, 1, 0, 0},
            {0, 0, 1, 0, 3, 0, 0, 3, 0, 0, 3, 0, 1, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 4, 0, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 6:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 3, 0, 1, 3, 0, 1, 0, 3, 1, 1, 3, 0, 1},
            {1, 0, 1, 0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 1, 3, 1, 3, 1, 1, 3, 3, 0, 1},
            {1, 0, 1, 0, 1, 0, 2, 2, 2, 0, 1, 0, 0, 0, 1},
            {1, 0, 3, 0, 0, 2, 0, 1, 0, 2, 3, 0, 1, 0, 1},
            {1, 0, 3, 1, 4, 3, 2, 2, 2, 1, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 2, 0, 1, 0, 2, 0, 0, 3, 0, 1},
            {1, 0, 1, 1, 2, 3, 1, 1, 1, 3, 2, 0, 1, 0, 1},
            {1, 0, 1, 0, 3, 2, 2, 2, 2, 2, 0, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 7:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
            {0, 1, 0, 3, 0, 0, 0, 0, 0, 3, 0, 4, 0, 1, 0},
            {0, 1, 1, 0, 1, 3, 1, 1, 2, 1, 1, 0, 0, 1, 0},
            {0, 0, 1, 0, 0, 2, 2, 2, 5, 2, 0, 3, 0, 1, 0},
            {0, 0, 1, 0, 3, 1, 0, 1, 2, 1, 0, 1, 0, 1, 0},
            {0, 0, 1, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0},
            {0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 8:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 4, 1, 1, 0, 0},
            {0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 3, 0, 1, 1, 0},
            {0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 3, 0, 1, 0},
            {0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 3, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 0, 2, 2, 2, 1, 0, 0, 0, 1, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 3, 0, 3, 0, 0, 0, 3, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0, 3, 0, 1},
            {1, 1, 1, 0, 3, 0, 3, 0, 3, 0, 0, 1, 1, 1, 1},
            {0, 0, 1, 1, 0, 0, 0, 3, 0, 3, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 9:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
            {0, 0, 0, 0, 0, 0, 1, 0, 3, 3, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 3, 1, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 1, 2, 0, 2, 2, 0, 1, 1, 1, 3, 1, 1},
            {1, 0, 0, 1, 2, 1, 5, 2, 3, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 2, 1, 2, 5, 1, 0, 1, 0, 0, 0, 1},
            {1, 0, 3, 3, 2, 2, 2, 2, 1, 0, 1, 1, 1, 1, 1},
            {1, 0, 4, 3, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
            {1, 0, 3, 3, 3, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 10:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 4, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 3, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 3, 2, 3, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 2, 3, 2, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 2, 3, 2, 3, 2, 1, 1, 1, 0, 0},
            {0, 0, 1, 0, 0, 3, 2, 3, 2, 3, 0, 0, 1, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 11:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 4, 0, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 3, 0, 3, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 5, 2, 5, 2, 5, 0, 1, 0, 0, 0},
            {0, 0, 1, 1, 0, 2, 3, 0, 3, 2, 0, 1, 1, 0, 0},
            {0, 1, 1, 1, 0, 5, 2, 5, 2, 5, 0, 1, 1, 1, 0},
            {1, 1, 1, 1, 0, 0, 3, 0, 3, 0, 0, 1, 1, 1, 1},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 12:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 3, 0, 3, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 2, 2, 5, 2, 0, 0, 3, 0, 1},
            {1, 0, 0, 3, 4, 3, 2, 2, 2, 2, 1, 3, 3, 0, 1},
            {1, 0, 0, 0, 3, 0, 1, 3, 1, 1, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
            {0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 13:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 3, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 3, 0, 3, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
            {1, 0, 3, 0, 3, 0, 3, 0, 0, 1, 1, 1, 0, 0, 0},
            {1, 0, 3, 0, 3, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0},
            {1, 0, 3, 0, 3, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
            {1, 0, 3, 0, 0, 1, 1, 0, 3, 3, 3, 0, 1, 0, 0},
            {1, 4, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0},
            {1, 1, 0, 1, 0, 1, 2, 3, 3, 3, 3, 3, 2, 1, 0},
            {0, 1, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1},
            {0, 1, 0, 0, 0, 2, 5, 5, 5, 5, 5, 5, 5, 2, 1},
            {0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
            {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 14:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0},
            {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0},
            {-1, 0, 0, 0, 0, -1, 0, 0, 5, 0, 0, -1, -1, 0, -1},
            {0, -1, 0, -1, 4, 0, 0, -1, 0, -1, 0, -1, 0, 0, -1},
            {0, -1, 0, -1, -1, -1, -1, -1, 0, -1, 0, -1, 0, -1, 0},
            {0, -1, 0, 3, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0},
            {0, -1, 0, 0, 0, 0, 0, -1, 0, 0, -1, -1, 0, -1, 0},
            {0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, 0, -1, 0},
            {0, -1, 2, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, -1, 0},
            {0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 2, -1, 0, 0, -1},
            {0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 3, 0, 0, -1, 0},
            {0, -1, 0, -1, 0, -1, -1, 0, -1, 0, 0, -1, 0, -1, 0},
            {0, -1, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, -1, 0},
            {0, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0},
            {0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    case 15:
    {
        int level[MAXN][MAXN] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 3, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 3, 0, 1, 2, 1, 1, 1, 1, 1, 0, 0, 0},
            {1, 1, 3, 0, 1, 1, 2, 1, 1, 0, 0, 1, 1, 1, 1},
            {0, 1, 0, 0, 2, 2, 2, 2, 2, 0, 3, 1, 0, 0, 1},
            {0, 1, 0, 3, 1, 1, 2, 1, 1, 0, 0, 1, 3, 0, 1},
            {0, 1, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 1, 1, 0, 3, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {0, 0, 0, 1, 0, 1, 3, 0, 0, 0, 0, 0, 3, 0, 1},
            {0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        for (int i = 0; i < MAXN; i++)
        {
            for (int j = 0; j < MAXN; j++)
            {
                m[i][j] = level[i][j];
            }
        }
        break;
    }
    }
}
