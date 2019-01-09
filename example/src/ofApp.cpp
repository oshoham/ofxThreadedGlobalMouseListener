#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	mouseListenerThread.startThread(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	mouseListenerThread.lock();
	globalMouseX = mouseListenerThread.mouseX;
	globalMouseY = mouseListenerThread.mouseY;
	mouseListenerThread.unlock();

	cout << "mouse x: " << globalMouseX << ", mouse y: " << globalMouseY << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::exit() {
	mouseListenerThread.waitForThread();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
