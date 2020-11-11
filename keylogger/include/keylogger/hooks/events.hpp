#pragma once

#include <string>
#include <atomic>
#include <fstream>
#include <memory>
#include <functional>

#include <linux/input.h>

struct Event {
    __u16 type;
};

class Events {
protected:
    void EventLoop(std::function<void(input_event)> handler);
    void CloseEventLoop();

public:
    Events(std::string eventPath);
    ~Events();

    virtual void ConnectHandler(std::function<void(Event &)> callback) = 0;

private:
    std::ifstream eventSymbol;
    std::atomic<bool> eventLoop;
};