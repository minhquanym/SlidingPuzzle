// #include "GUI.cpp"
// #include "Board.cpp"

namespace Solution {
    struct Score {
        int g, h, f;
        Score() {}; Score(int g, int h, int f) : g(g), h(h), f(f) {};

        bool operator < (const Score &a) const {
            if (f != a.f) return f < a.f;
            if (g != a.g) return g < a.g;
            return h < a.h;
        }
    };
    struct dataNode {
        int id, add_x, add_y;
        Score score;

        dataNode() {};
        dataNode(int id, int add_x, int add_y, Score score) : id(id), add_x(add_x), add_y(add_y), score(score) {};

        bool operator < (const dataNode &a) const {
            return score < a.score;
        }
        bool operator > (const dataNode &a) const {
            return a.score < score;
        }
    };

    int numID = 0;
    std::map<Board, int> Map;
    std::vector<Board> Match;
    std::vector<Score> dist;
    std::vector<dataNode> Trace;
    std::priority_queue<dataNode, std::vector<dataNode>, std::greater<dataNode> > pq;

    void update(Board &board, int addX, int addY, Score score, dataNode preNode) {
        if (!Map.count(board)) {
            Map[board] = ++numID;
            Match.push_back(board);
            Trace.push_back(preNode);
            dist.push_back( Score(1e9, 1e9, 1e9) );
        }

        int id = Map[board];
        if (score < dist[id]) {
            dist[id] = score;
            Trace[id] = preNode;
            pq.push( dataNode(id, addX, addY, score) );
        }
    }

    int h[4] = { 0, 0, -1, 1 }, c[4] = { -1, 1, 0, 0 };
    std::vector<std::pair<int, int> > lsTrace;

    void Trace_back(dataNode &foo) {
        while (true) {
            if (foo.id == -1) break; 

            std::cerr << "TRACING:\n";
            Match[foo.id].debug_board();
            std::cerr << "ADD: " << foo.add_x << " " << foo.add_y << '\n';
            std::cerr << '\n';

            int add_x = foo.add_x, add_y = foo.add_y;
            /// GUI adaption
            // std::swap(add_x, add_y);
            // add_x = -add_x; add_y = -add_y;

            lsTrace.push_back( std::make_pair(add_x, add_y) );
            foo = Trace[foo.id];
        }
        reverse(lsTrace.begin(), lsTrace.end());

            // for (std::pair<int, int> foo : lsTrace) std::cerr << foo.first << " " << foo.second << '\n';
    }

    void Solution_A_star(Board board) {
        std::cerr << " ----------------FINDING SOLUTION---------------\n\n";

        std::cerr << "START: \n";
        board.debug_board();
        std::cerr << '\n';

        /// init dist array
        dist.resize(1);
        Match.resize(1);
        Trace.resize(1);

        /// start state
        dataNode endDataNode;
        endDataNode.id = -1;
        update( board, -10, -10, Score(0, board.desDistance(), board.desDistance()), endDataNode );

        /// main process
        bool Found_sol = false;
        while (pq.size()) {
            dataNode foo = pq.top(); pq.pop();
            Board ore = Match[foo.id];
            if (foo.score.h == 0) {
                ore.debug_board();

                Trace_back(foo);
                Found_sol = true;
                break;
            }

                // ore.debug_board();
                // std::cerr << "dataNODE: " 
                //     << foo.id << " --> " << foo.score.g << " " << foo.score.h << " " << foo.score.f << '\n';
                // std::cerr << "\n";

            for (int dir = 0; dir < 4; ++dir) {
                int s_x = ore.Space_location.first, s_y = ore.Space_location.second;
                int t_x = s_x + h[dir], t_y = s_y + c[dir];
                if ( !ore.inBoard(t_x, t_y) ) continue;
                
                /// new state
                std::swap( ore.a[s_x][s_y], ore.a[t_x][t_y] );
                ore.Space_location = std::make_pair( t_x, t_y );

                Score val = foo.score;
                val.g++;
                val.h = ore.desDistance();
                val.f = val.g + val.h;
                update( ore, h[dir], c[dir], val, foo );

                /// previous state
                std::swap( ore.a[s_x][s_y], ore.a[t_x][t_y] );
                ore.Space_location = std::make_pair( s_x, s_y );
            }

            if (Found_sol) break;
        }
        
        if (Found_sol) std::cerr << "---------WE GOT THE SOLUTION--------\n";
        else std::cerr << "----------NO SOLUTION FOUND-----------\n";
    }
} 