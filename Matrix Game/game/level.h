int dx[4] = {+1, -1, 0, 0};
int dy[4] = {0, 0, +1, -1};
char genBoard[8][8];
char min_distance[8][8];

void Clear() {
    for (int i = 0; i< 8; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            genBoard[i][j] = 0;
        }
    }
}

bool Inside(int x, int y) {
    return 0 <= x and x <= 7 and 0 <= y and y <= 7;
}

void Find(int type, int& x, int& y) {
    for (int i = 0; i< 8; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            if (genBoard[i][j] == type) {
                x = i;
                y = j;
                return;
            }
        }
    }
}

char Distance(int a, int b, int x, int y) {
    for (int i = 0; i< 8; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            min_distance[i][j] = 100;
        }
    }

    min_distance[a][b] = 0;
    bool ok = true;
    while (ok) {
        ok = false;
        for (int i = 0; i < 8; i += 1) {
            for (int j = 0; j < 8; j += 1) {
                if (genBoard[i][j] == 1) {
                    continue;
                }

                for (int d = 0; d < 4; d += 1) {
                    int nx = i + dx[d];
                    int ny = j + dy[d];
                    if (not Inside(nx, ny)) { 
                        continue;
                    }

                    if (min_distance[nx][ny] > char(1 + min_distance[i][j])) {
                        min_distance[nx][ny] = char(1 + min_distance[i][j]);
                        ok = true;
                    }
                }
            }
        }
    }

    return min_distance[x][y];
}

bool IsConex() {
    int x = 0, y = 0;
    Find(0, x, y);
    bool visited[8][8] = {{0}};
    for (int i = 0; i< 8; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            visited[i][j] = false;
        }
    }

    visited[x][y] = true;

    bool ok;
    do {
        ok = false;

        for (int i = 0; i < 8; i += 1) {
            for (int j = 0; j < 8; j += 1) {
                if (genBoard[i][j] != 1 and visited[i][j] == true) {
                    for (int d = 0; d < 4; d += 1) {
                        if (Inside(i + dx[d], j + dy[d]) and !visited[i + dx[d]][j + dy[d]]) {
                            visited[i + dx[d]][j + dy[d]] = true;
                            ok = true;
                        }
                    }
                }
            }
        }
    } while (ok);

    for (int i = 0; i < 8; i += 1) {
        for (int j = 0; j < 8; j += 1) {
            if (genBoard[i][j] != 1 and visited[i][j] == false) {
                return false;
            }
        }
    }

    return true;
}

bool Generate(int h, int w, int num_bombs, int expansion_chx) {
    int offset_x = rand() % (min(3, 8 - h + 1));
    int offset_y = rand() % (min(4, 8 - w + 1));

    while (num_bombs) {
        int x = rand() % h;
        int y = rand() % w;
        
        if (genBoard[offset_x + x][offset_y + y] == 1) {
            int d = rand() % 4;
            x += dx[d];
            y += dy[d];

            if (genBoard[offset_x + x][offset_y + y] != 0) {
                continue;
            }

            if (0 > x || 0 > y || h < x || w < y) {
                continue;
            }
        } else if (rand() % expansion_chx == 0) {
            
        } else {
            continue;
        }

        genBoard[x + offset_x][y + offset_y] = 1;
        num_bombs -= 1;
    }

    if (IsConex() == false) {
        return false;
    }

    for (int _ = 0; _ <= 10; _ += 1) {
        for (int p = 2; p < 3; p += 1) {
            int x = rand() % h + offset_x;
            int y = rand() % w + offset_y;

            if (genBoard[x][y] == 0) {
                genBoard[x][y] = p;
            } else {
                p -= 1;
            }
        }

        int a = 0, b = 0;
        Find(2, a, b);

        Distance(a, b, 0, 0);
        

        int x = -1, y = -1;
        int cnt = 0;
        for (int i = 0; i < 8; i += 1) {
            for (int j = 0; j < 8; j += 1) {
                if (i >= offset_x and i < offset_x + h and 
                    j >= offset_y and j < offset_y + w and 
                    genBoard[i][j] == 0 and min_distance[i][j] > h + w) {
                    if (rand() % (cnt + 1) == 0) {
                        x = i;
                        y = j;
                    }

                    cnt += 1;
                }
            }
        }

        if (x == -1) {
            genBoard[a][b] = 0;
        } else {
            genBoard[x][y] = 3;
            return true;
        }
    }

    return false;
}

void Generate(int level_num) {
    while (1) {

        int h = min(4 + level_num / 5, 8);
        int w = min(4 + level_num / 3, 8);
        int num_bombs = min(20, 4 + level_num * 2 / 3); 

        Clear();

        if (Generate(h, w, num_bombs, max(4, 10 - level_num))) {
            break;
        }
    }
}
