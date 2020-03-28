#include<Tile.cpp>

struct Board {
    vetor<Tile> TilePos;
    vector<vector<int> > a;
    pair<int, int> Space_location;

    bool inBoard(int x, int y) {
        return 0 <= x && x < (int) a.size() && 0 <= y && y < (int) a[x].size();
    }

    bool winGame() {
        int cnt = 1;
        for (vector<int> row) for (int x : row) {
            if (x != cnt) return false;
            cnt = (cnt + 1) % 9;
        }
        return true;
    }
}
