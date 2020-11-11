#pragma once

#include <keylogger/hooks/events.hpp>

#include <functional>

#define MOUSE_EVENT_PATH "/dev/input/event20"

struct MouseEvent:
        public Event {
    __u16 x;
    __u16 y;
    bool leftButtonClicked;
    bool rightButtonClicked;
};

class MouseEvents:
        public Events {
public:
    MouseEvents() : Events(MOUSE_EVENT_PATH) {};

    void ConnectHandler(std::function<void(Event &)> callback) override;
};
