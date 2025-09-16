#include "Game.h"

NAMESPACE_UPP

namespace Game {

void Player::Step(Input::Device& in, int ticks) {
    if (in.GetMapping(Input::Device::Key::Left) == 0)
        in.MapKey(Input::Device::Key::Left, 0x4B);
    if (in.GetMapping(Input::Device::Key::Right) == 0)
        in.MapKey(Input::Device::Key::Right, 0x4D);
    if (in.GetMapping(Input::Device::Key::Up) == 0)
        in.MapKey(Input::Device::Key::Up, 0x48);
    if (in.GetMapping(Input::Device::Key::Jump) == 0)
        in.MapKey(Input::Device::Key::Jump, 0x39);

    const int accel = 8;
    const int maxvx = 64;
    const int gravity = 6;
    const int jumpv = -120;

    for (int t=0;t<ticks;++t) {
        if (in.IsDown(Input::Device::Key::Left))  vx -= accel;
        if (in.IsDown(Input::Device::Key::Right)) vx += accel;
        if (vx >  maxvx) vx =  maxvx;
        if (vx < -maxvx) vx = -maxvx;
        if (onGround && in.WasPressed(Input::Device::Key::Jump)) { vy = jumpv; onGround = false; }
        vy += gravity;
        x += vx; y += vy;
        if (y > 0) { y = 0; vy = 0; onGround = true; }
    }
    in.ClearEdgeStates();
}

}

END_UPP_NAMESPACE

