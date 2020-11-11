#include <keylogger/hooks/mouse.hpp>

void MouseEvents::ConnectHandler(std::function<void(Event &)> callback) {
    EventLoop([callback](input_event ev){
        if (ev.type == 1 && ev.value == 1) {
            MouseEvent event{};
            if (ev.code == 272) {
                event.leftButtonClicked = true;
            }
            if (ev.code == 273) {
                event.rightButtonClicked = true;
            }
            callback(event);
        }
    });
}
