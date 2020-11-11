#include <keylogger/hooks/mouse.hpp>

void MouseEvents::ConnectHandler(std::function<void(Event &)> callback) {
    EventLoop([](input_event ev){});
}
