#include<bits/stdc++.h>

struct Tile {    
    int id, x, y, current_speed;

    bool operator != (const Tile &a) const {
        return x != a.x || y != a.y;
    }

    void setSpeed() {
        // current_speed = ;
    }

    void Move(int addX, int addY, int speed) {
        x += speed * addX; 
        y += speed * addY;
    }
};