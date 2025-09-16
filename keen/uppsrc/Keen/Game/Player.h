// Simple player logic prototype (resource-agnostic)
#pragma once

namespace Game {

class Player {
public:
    void Reset() { x=0; y=0; vx=0; vy=0; onGround=true; }
    void Step(Input::Device& in, int ticks);
    int X() const { return x; }
    int Y() const { return y; }
    int VX() const { return vx; }
    int VY() const { return vy; }
    bool OnGround() const { return onGround; }
private:
    int x=0,y=0; // position in units (1 tile = 256 units)
    int vx=0, vy=0; // velocity in units per tick
    bool onGround=true;
};

}

