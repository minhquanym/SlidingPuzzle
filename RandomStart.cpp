namespace RandomStart {
    const int limitTimeSwap = 1e6;
    int h[4] = { 0, -1, 1, 0 };
    int c[4] = { -1, 0, 0, 1 };

    void randomStart(Board &board) {
        srand(time(NULL));

        int TimeSwap = limitTimeSwap + 1, preDir = -1;
        for (int i = 1; i <= TimeSwap; ++i) {
            int x = board.Space_location.first, y = board.Space_location.second;
            
            int dir = rand() % 4;
            while (true) {
                dir = (dir + 1) % 4;
                if ( dir == 3-preDir ) continue;

                int u = x + h[dir], v = y + c[dir];
                if ( !board.inBoard(u, v) ) continue;

                break;
            }
            preDir = dir;

            int u = x + h[dir], v = y + c[dir];
            assert( board.inBoard(u, v) );

            std::swap( board.a[u][v], board.a[x][y] );
            std::swap( board.TilePos[ board.a[u][v] ].x, board.TilePos[ board.a[x][y] ].x );
            std::swap( board.TilePos[ board.a[u][v] ].y, board.TilePos[ board.a[x][y] ].y );
            board.Space_location = std::make_pair(u, v);
        }

        while ( board.Space_location.first != (int) board.a.size() - 1 ) {
            int dir = 2;

            int x = board.Space_location.first, y = board.Space_location.second;
            int u = x + h[dir], v = y + c[dir];
            assert( board.inBoard(u, v) );

            std::swap( board.a[u][v], board.a[x][y] );
            std::swap( board.TilePos[ board.a[u][v] ].x, board.TilePos[ board.a[x][y] ].x );
            std::swap( board.TilePos[ board.a[u][v] ].y, board.TilePos[ board.a[x][y] ].y );
            board.Space_location = std::make_pair(u, v);
        } 

        while ( board.Space_location.second != (int) board.a.size() - 1 ) {
            int dir = 3;

            int x = board.Space_location.first, y = board.Space_location.second;
            int u = x + h[dir], v = y + c[dir];
            assert( board.inBoard(u, v) );

            std::swap( board.a[u][v], board.a[x][y] );
            std::swap( board.TilePos[ board.a[u][v] ].x, board.TilePos[ board.a[x][y] ].x );
            std::swap( board.TilePos[ board.a[u][v] ].y, board.TilePos[ board.a[x][y] ].y );
            board.Space_location = std::make_pair(u, v);
        } 
    } 
}