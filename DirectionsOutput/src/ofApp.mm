#include "ofApp.h"
#include <AppKit/AppKit.h>

// change this to a recognizeable piece of your audio output
// e.g. setAudioOutput("AirPlay"); or setAudioOutput("VoilaDevice");
// returns true on success / false on beef

// as always, all thanks to StackOverflow for the amazing
// enumerate devices script:
// http://stackoverflow.com/questions/1983984/how-to-get-audio-device-uid-to-pass-into-nssounds-setplaybackdeviceidentifier
bool setAudioOutput( string targetDevice ){
    AudioObjectPropertyAddress  propertyAddress;
    AudioObjectID               *deviceIDs;
    UInt32                      propertySize;
    NSInteger                   numDevices;
    
    propertyAddress.mSelector = kAudioHardwarePropertyDevices;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMaster;
    
    // enumerate all current/valid devices
    if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize) == noErr) {
        numDevices = propertySize / sizeof(AudioDeviceID);
        deviceIDs = (AudioDeviceID *)calloc(numDevices, sizeof(AudioDeviceID));
        
        if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize, deviceIDs) == noErr) {
            AudioObjectPropertyAddress      deviceAddress;
            char                            deviceName[64];
            char                            manufacturerName[64];
            
            for (NSInteger idx=0; idx<numDevices; idx++) {
                propertySize = sizeof(deviceName);
                deviceAddress.mSelector = kAudioDevicePropertyDeviceName;
                deviceAddress.mScope = kAudioObjectPropertyScopeGlobal;
                deviceAddress.mElement = kAudioObjectPropertyElementMaster;
                
                // get the deets!
                if (AudioObjectGetPropertyData(deviceIDs[idx], &deviceAddress, 0, NULL, &propertySize, deviceName) == noErr) {
                    propertySize = sizeof(manufacturerName);
                    deviceAddress.mSelector = kAudioDevicePropertyDeviceManufacturer;
                    deviceAddress.mScope = kAudioObjectPropertyScopeGlobal;
                    deviceAddress.mElement = kAudioObjectPropertyElementMaster;
                    if (AudioObjectGetPropertyData(deviceIDs[idx], &deviceAddress, 0, NULL, &propertySize, manufacturerName) == noErr) {
                        CFStringRef     uidString;
                        
                        propertySize = sizeof(uidString);
                        deviceAddress.mSelector = kAudioDevicePropertyDeviceUID;
                        deviceAddress.mScope = kAudioObjectPropertyScopeGlobal;
                        deviceAddress.mElement = kAudioObjectPropertyElementMaster;
                        if (AudioObjectGetPropertyData(deviceIDs[idx], &deviceAddress, 0, NULL, &propertySize, &uidString) == noErr) {
                            // comment this out if you don't want to log everything
                            
                            NSLog(@"device %s by %s id %@", deviceName, manufacturerName, uidString);
                            CFRelease(uidString);
                        }
                        
                        string name(deviceName);
                        if ( name.find( targetDevice ) != string::npos ){
                            propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
                            propertyAddress.mScope = kAudioDevicePropertyScopeOutput;
                            propertyAddress.mElement = kAudioObjectPropertyElementMaster;
                            
                            return AudioObjectSetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, sizeof(AudioDeviceID), &deviceIDs[idx]) == noErr;
                        }
                    }
                }
            }
        }
        
        free(deviceIDs);
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofXml settings;
    settings.load("settings.xml");
    string server = settings.getValue("server");
    string device = settings.getValue("device");
    int port = settings.getValue("port", 9000);
    
    spacebrew.setAutoReconnect();
    spacebrew.connect(server, port, "gelVoice");
    
    cout << device << endl;
    
    bool success = setAudioOutput(device);
    
    speech.listVoices();
    speech.initSynthesizer();
    
    rates[DIRECTION_LEFT].lastSent = 0;
    rates[DIRECTION_LEFT].sendRate = 1000;
    
    rates[DIRECTION_RIGHT].lastSent = 0;
    rates[DIRECTION_RIGHT].sendRate = 1000;
    
    rates[DIRECTION_STRAIGHT].lastSent = 0;
    rates[DIRECTION_STRAIGHT].sendRate = 500;
    
//    rates[DIRECTION_LOOK].lastSent = 0;
//    rates[DIRECTION_LOOK].sendRate = 500;
    
    rates[DIRECTION_STOP].lastSent = 0;
    rates[DIRECTION_STOP].sendRate = 1000;
    
    // level marker
    currentLevel = 0;
    
    // spacebrew game messages
    spacebrew.addSubscribe("gameevent", "event");
    
    ofAddListener(spacebrew.onMessageEvent, this, &ofApp::onMessage);
    
    inputProcessor.setup(spacebrew);
}


//--------------------------------------------------------------
void ofApp::speak( int dir, int power  ){
    Direction d = (Direction) dir;
    
    if ( ofGetElapsedTimeMillis() - rates[d].lastSent > rates[d].sendRate ){
        rates[d].lastSent = ofGetElapsedTimeMillis();
        
        switch ( d ){
                case DIRECTION_LEFT:
                bWalking = false;
                speech.speakPhrase(currentLevel == 3 ? "hot" : "left");
                break;
                
                case DIRECTION_RIGHT:
                bWalking = false;
                speech.speakPhrase(currentLevel == 3 ? "cold" : "right");
                break;
                
                case DIRECTION_STRAIGHT:
                bWalking = true;
                {
                    switch (power) {
                        case 0:
                            speech.speakPhrase("walk");
                            break;
                            
                        case 1:
                            rates[DIRECTION_STRAIGHT].lastSent = ofGetElapsedTimeMillis();
                            speech.speakPhrase("walk fast");
                            break;
                            
                        case 2:
                            rates[DIRECTION_STRAIGHT].lastSent = ofGetElapsedTimeMillis();
                            speech.speakPhrase("walk really fast");
                            break;
                    }
                }
                
                break;
                
                case DIRECTION_STOP:
                if ( bWalking && ofGetElapsedTimeMillis() - rates[DIRECTION_STRAIGHT].lastSent > rates[DIRECTION_STRAIGHT].sendRate) {
                    speech.speakPhrase("stop");
                    bWalking = false;
                }
                
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (inputProcessor.shouldSend()){
        speak( inputProcessor.getCurrentValue(), inputProcessor.getCurrentPower() );
    } else {
        speak( (int) DIRECTION_STOP );
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::onMessage( Spacebrew::Message & m ){
    Json::Reader    jsonReader;
    Json::Value     json;
    
    if ( m.name == "gameevent" ){
        ofStringReplace(m.value, "\\", ""); // is this still necessary?
        bool b = jsonReader.parse( m.value, json);
        
        //        {
        //            name:"eventtype",
        //            value:"stuff"
        //        }
        
        if ( b ){
            string name = json["name"].asString();
            string value = json["value"].asString();
            
            if (name == "level") {
                int l = currentLevel;
                if (value == "level one"){
                    currentLevel = 1;
                    
                } else if ( value == "level two" ){
                    currentLevel = 2;
                    
                } else if ( value == "level three"){
                    currentLevel = 3;
                }
                
//                if ( l != currentLevel )
                    speech.speakPhrase(value);
            } else if(name == "trigger"){
                speech.speakPhrase(value);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'l'){
        speech.speakPhrase(currentLevel == 3 ? "cold" : "left");
    } else if ( key == 'r' ){
        speech.speakPhrase(currentLevel == 3 ? "hot" : "right");
    } else if ( key == 's' ){
        speech.speakPhrase("stop");
    } else if ( key == 'f' ){
        speech.speakPhrase("walk");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ }
