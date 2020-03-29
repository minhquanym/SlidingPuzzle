#include "Tile.cpp"

struct Board {
    const int SPEED = 10;

    std::vector<Tile> TilePos;
    std::vector<std::vector<int> > a;
    std::pair<int, int> Space_location;

    bool operator < (const Board &board) const {
        for (int i = 0; i < (int) a.size(); ++i) 
            for (int j = 0; j < (int) a[i].size(); ++j) 
                if (a[i][j] != board.a[i][j]) return a[i][j] < board.a[i][j];
        return false; 
    }

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

    void destination(int gridSize, int rawSize) {
        a.resize(gridSize);
        for (int i = 0; i < gridSize; ++i) a[i].resize(3);

            int cc = 0;
            for (int j = 0; j < gridSize; ++j) for (int i = 0; i < gridSize; ++i) a[i][j] = (++cc) % (gridSize*gridSize); 
            Space_location = std::make_pair(gridSize-1, gridSize-1);

            //debug_board();

        int m = (int) a.size();
        int n = (int) a[0].size();
        TilePos.resize(m*n);

        int current_x = 0, current_y = 0, cnt = 0;
        for (int i = 1; i <= m*n; ++i) {
            int id = i % (m*n);
            TilePos[id].id = id;
            TilePos[id].x = current_x;
            TilePos[id].y = current_y;
            TilePos[id].current_speed = SPEED;

            current_x += rawSize;
            if (++cnt == n) {
                cnt = 0;
                current_y += rawSize;
                current_x = 0;
            } 
        }

    }

    std::pair<int, int> PosValInBoard(int val) {
        if (val == 0) return std::make_pair(a.size()-1, a[0].size()-1);
        int x = (val-1) % (int) a.size(), y = (val-1) / (int) a.size();
        return std::make_pair(x, y);
    }

    int desDistance() {
        int res = 0;
        for (int i = 0; i < (int) a.size(); ++i) 
            for (int j = 0; j < (int) a[0].size(); ++j) {
                if (a[i][j] == 0) continue;
                std::pair<int, int> pos = PosValInBoard(a[i][j]);
                res += abs(i - pos.first) + abs(j - pos.second);
            }
        return res;
    }
};

