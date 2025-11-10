// Input facade (null backend initially)
#pragma once


namespace Input {

using ScanCode = uint8_t;

class Device {
public:
    static Device& Default();

    void Startup();
    void Shutdown();
    void Poll();

    bool IsDown(ScanCode sc) const { return keys_.test(sc); }
    void SetKey(ScanCode sc, bool down) { keys_.set(sc, down); }

    // Logical keys for gameplay
    enum class Key { Left, Right, Up, Down, Jump, Fire, COUNT };
    void MapKey(Key k, ScanCode sc);
    ScanCode GetMapping(Key k) const;
    bool IsDown(Key k) const;
    bool WasPressed(Key k) const;
    bool WasReleased(Key k) const;
    void ClearEdgeStates();

    // Low-level event injection (e.g., from tests)
    struct Event { ScanCode sc; bool down; };
    void PushEvent(ScanCode sc, bool down);

private:
    std::bitset<256> keys_{};
    std::bitset<256> prev_{};
    std::vector<Event> queue_;
    std::array<ScanCode, (size_t)Key::COUNT> map_{};
};

}

// For compatibility with the original code structure
enum IN_Direction {
    IN_dir_None = -1,
    IN_dir_Left = 0,
    IN_dir_Right = 1,
    IN_dir_Up = 2,
    IN_dir_Down = 3
};

enum IN_Motion {
    IN_motion_Left = -1,
    IN_motion_None = 0,
    IN_motion_Right = 1
};

typedef struct IN_ControlFrame
{
    bool jump, pogo, button2, button3;
    int x, y;
    IN_Motion xDirection, yDirection;
    IN_Direction dir;
} IN_ControlFrame;
