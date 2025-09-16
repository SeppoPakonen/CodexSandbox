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
