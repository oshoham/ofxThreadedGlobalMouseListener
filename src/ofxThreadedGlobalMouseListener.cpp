#include "ofxThreadedGlobalMouseListener.h"

ofxThreadedGlobalMouseListener* ofxThreadedGlobalMouseListener::pThis = NULL;

ofxThreadedGlobalMouseListener::~ofxThreadedGlobalMouseListener() {
	waitForThread();
}

#ifdef TARGET_WIN32
LRESULT CALLBACK ofxThreadedGlobalMouseListener::mouseHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		MSLLHOOKSTRUCT * pHookStruct = (MSLLHOOKSTRUCT *)lParam;
		POINT pt = pHookStruct->pt;
		lock();
		mouseX = pt.x;
		mouseY = pt.y;
		unlock();
		ofMouseEventArgs mouseEventArgs;
		switch (wParam) {
			case WM_LBUTTONDOWN:
				buttonPressed = true;
				button = OF_MOUSE_BUTTON_LEFT;
				mouseEventArgs = ofMouseEventArgs(ofMouseEventArgs::Pressed, pt.x, pt.y, OF_MOUSE_BUTTON_LEFT);
				mouseEvents.send(move(mouseEventArgs));
				break;
			case WM_LBUTTONUP:
				buttonPressed = false;
				button = OF_MOUSE_BUTTON_LEFT;
				mouseEventArgs = ofMouseEventArgs(ofMouseEventArgs::Released, pt.x, pt.y, OF_MOUSE_BUTTON_LEFT);
				mouseEvents.send(move(mouseEventArgs));
				break;
			case WM_RBUTTONDOWN:
				buttonPressed = true;
				button = OF_MOUSE_BUTTON_RIGHT;
				mouseEventArgs = ofMouseEventArgs(ofMouseEventArgs::Pressed, pt.x, pt.y, OF_MOUSE_BUTTON_RIGHT);
				mouseEvents.send(move(mouseEventArgs));
				break;
			case WM_RBUTTONUP:
				buttonPressed = false;
				button = OF_MOUSE_BUTTON_RIGHT;
				mouseEventArgs = ofMouseEventArgs(ofMouseEventArgs::Released, pt.x, pt.y, OF_MOUSE_BUTTON_RIGHT);
				mouseEvents.send(move(mouseEventArgs));
				break;
			case WM_MOUSEMOVE:
				ofMouseEventArgs::Type type = buttonPressed ? ofMouseEventArgs::Dragged : ofMouseEventArgs::Moved;
				mouseEventArgs = ofMouseEventArgs(type, pt.x, pt.y, button);
				mouseEvents.send(move(mouseEventArgs));
				break;
		}
	}
	return CallNextHookEx(prevMouseHook, nCode, wParam, lParam);
}

void ofxThreadedGlobalMouseListener::postQuitMessage() {
	std::thread & nativeThread = getNativeThread();
	HANDLE handle = nativeThread.native_handle();
	DWORD threadId = GetThreadId(handle);
	PostThreadMessageA(threadId, WM_QUIT, 0, 0);
}
#endif

void ofxThreadedGlobalMouseListener::threadedFunction() {
	ofxThreadedGlobalMouseListener::pThis = this;

#ifdef TARGET_WIN32
	HINSTANCE hInstance = GetModuleHandle(NULL);
	prevMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, NULL);

	MSG message;
	while (isThreadRunning()) {
		BOOL bRet = GetMessage(&message, NULL, 0, 0);
		if (bRet > 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			break;
		}
	}

	UnhookWindowsHookEx(prevMouseHook);
#endif
    
#ifdef TARGET_OSX
    while (isThreadRunning()) {
        CGEventRef event = CGEventCreate(NULL);
        CGPoint cursor = CGEventGetLocation(event);
        lock();
        mouseX = cursor.x;
        mouseY = cursor.y;
        unlock();
        CFRelease(event);
    }
#endif
}

void ofxThreadedGlobalMouseListener::update() {
	ofMouseEventArgs mouseEventArgs;
	while (mouseEvents.tryReceive(mouseEventArgs)) {
		switch (mouseEventArgs.type) {
			case ofMouseEventArgs::Pressed:
				ofNotifyEvent(mousePressed, mouseEventArgs);
				break;
			case ofMouseEventArgs::Released:
				ofNotifyEvent(mouseReleased, mouseEventArgs);
				break;
			case ofMouseEventArgs::Moved:
				ofNotifyEvent(mouseMoved, mouseEventArgs);
				break;
			case ofMouseEventArgs::Dragged:
				ofNotifyEvent(mouseDragged, mouseEventArgs);
				break;
		}
	}
}

void ofxThreadedGlobalMouseListener::waitForThread(bool callStopThread, long milliseconds) {
#ifdef TARGET_WIN32
	postQuitMessage();
#endif
	ofThread::waitForThread(callStopThread, milliseconds);
}
