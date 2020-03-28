#include<bits/stdc++.h>

struct Tile {    
    int id, x, y, current_speed;

    void setSpeed() {
        // current_speed = ;
    }

    void Move(int addX, int addY, int speed) {
        x += speed * addX; 
        y += speed * addY;
    }
};