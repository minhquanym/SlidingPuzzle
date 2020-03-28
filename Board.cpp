#include "Tile.cpp"

struct Board {
    std::vector<Tile> TilePos;
    std::vector<std::vector<int> > a;
    std::pair<int, int> Space_location;

    bool inBoard(int x, int y) {
        return 0 <= x && x < (int) a.size() && 0 <= y && y < (int) a[x].size();
    }

    bool winGame() {
        int cnt = 1;
        for (std::vector<int> row : a) for (int x : row) {
            if (x != cnt) return false;
            cnt = (cnt + 1) % 9;
        }
        return true;
    }
};
