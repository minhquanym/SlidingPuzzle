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
        for (int j = 0; j < (int) a[0].size(); ++j) 
            for (int i = 0; i < (int) a.size(); ++i) {
                if (a[i][j] != cnt) return false;
                cnt = (cnt+1) % 9;
            }
        return true;
    }
    
    void debug_board() {
        std::cerr << "BOARD: \n";
        for (int i = 0; i < (int) a.size(); ++i) {
            for (int j = 0; j < (int) a[0].size(); ++j) std::cerr << a[i][j] << " ";
            std::cerr << '\n';
        }
    }
};

