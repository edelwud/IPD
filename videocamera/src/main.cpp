
#pragma comment(lib, "user32.lib")

#include <windows.h>
#include <map>
#include <functional>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void Cheese() {
    VideoCapture camera(0);
    if (camera.isOpened()) {
        Mat frame;
        camera >> frame;
        imwrite("pic.jpg", frame);
    }
}

void ToggleCapture() {
    static bool tracking = false;
    static VideoCapture camera;
    static VideoWriter video;


    if (!tracking) {
        tracking = true;

        thread scanner([](){
            camera.open(0);
            if (!camera.isOpened()) {
                return;
            }

            double frame_width = camera.get(CAP_PROP_FRAME_WIDTH);
            double frame_height = camera.get(CAP_PROP_FRAME_HEIGHT);

            video.open("video2.avi",
                       VideoWriter::fourcc('M','P','4','2'),
                       20,
                       Size(frame_width,frame_height), true);
            if (!video.isOpened()) {
                return;
            }

            while (true) {
                Mat frame;
                if (!camera.read(frame)) {
                    break;
                }
                if (frame.empty())
                    break;
                video << frame;
            }
        });
        scanner.detach();
    } else {
        camera.release();
        video.release();
        tracking = false;
    }
}

static inline map<char, function<void()>> library = {
    { 'Y', Cheese },
    { 'U', ToggleCapture }
};

HHOOK hKeyboardHook;

__declspec(dllexport)
LRESULT
CALLBACK
KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam) {
    if ((nCode == HC_ACTION) &&
        ((wParam == WM_SYSKEYDOWN) ||
         (wParam == WM_KEYDOWN))) {
        KBDLLHOOKSTRUCT key = *reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (library.find(key.vkCode) != library.end()) {
            library[key.vkCode]();
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void MessageLoop() {
    MSG message;
    while (GetMessage(&message, nullptr, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int main() {
    ShowWindow(::GetConsoleWindow(), SW_HIDE);
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    hKeyboardHook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            (HOOKPROC)KeyboardEvent,
            hInstance,
            NULL);
    MessageLoop();
    UnhookWindowsHookEx(hKeyboardHook);
    return 1;
}
