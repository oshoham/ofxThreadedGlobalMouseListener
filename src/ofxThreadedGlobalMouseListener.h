#pragma once

#include "ofMain.h"

class ofxThreadedGlobalMouseListener : public ofThread {
	public:
        static ofxThreadedGlobalMouseListener *pThis;

		ofxThreadedGlobalMouseListener();	
		~ofxThreadedGlobalMouseListener();

#ifdef TARGET_WIN32
		static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
			return ofxThreadedGlobalMouseListener::pThis->mouseHook(nCode, wParam, lParam);
		}

		LRESULT CALLBACK mouseHook(int nCode, WPARAM wParam, LPARAM lParam);
#endif

		void threadedFunction();
		void waitForThread(bool callStopThread = true, long milliseconds = INFINITE_JOIN_TIMEOUT);
		void update();

		int mouseX, mouseY;

		ofEvent<ofMouseEventArgs> mousePressed, mouseReleased, mouseMoved, mouseDragged;

#ifdef TARGET_WIN32
		HHOOK prevMouseHook;
#endif

	private:
		int button;
		bool buttonPressed;
		ofThreadChannel<ofMouseEventArgs> mouseEvents;
#ifdef TARGET_WIN32
		void postQuitMessage();
#endif
};
