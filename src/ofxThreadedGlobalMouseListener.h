#pragma once

#include "ofMain.h"

class ofxThreadedGlobalMouseListener : public ofThread {
	public:
#ifdef TARGET_WIN32
		static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
			return GlobalMouseListenerThread::pThis->mouseHook(nCode, wParam, lParam);
		}

		static GlobalMouseListenerThread *pThis;

		LRESULT CALLBACK mouseHook(int nCode, WPARAM wParam, LPARAM lParam);
#endif

		void threadedFunction();
		void waitForThread(bool callStopThread = true, long milliseconds = INFINITE_JOIN_TIMEOUT);

		int mouseX, mouseY;

#ifdef TARGET_WIN32
		HHOOK prevMouseHook;
#endif

	private:
#ifdef TARGET_WIN32
		void postQuitMessage();
#endif
};