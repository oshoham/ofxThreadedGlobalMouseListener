#include "ofxThreadedGlobalMouseListener.h"

ofxThreadedGlobalMouseListener* ofxThreadedGlobalMouseListener::pThis = NULL;

#ifdef TARGET_WIN32
LRESULT CALLBACK ofxThreadedGlobalMouseListener::mouseHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
		POINT pt = pMouseStruct->pt;
		lock();
		mouseX = pt.x;
		mouseY = pt.y;
		unlock();
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
}

void ofxThreadedGlobalMouseListener::waitForThread(bool callStopThread, long milliseconds) {
#ifdef TARGET_WIN32
	postQuitMessage();
#endif
	ofThread::waitForThread(callStopThread, milliseconds);
}