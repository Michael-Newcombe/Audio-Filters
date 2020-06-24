#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxMaxim.h"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //audio settings
    void audioOut(ofSoundBuffer & buffer);
    ofSoundStream soundStream;
    float sampleRate;
    float bufferSize;
    
    //audio variables
    //vol
    float amplitude;
    //maxi filters
    maxiFilter lowpassFilter;
    maxiFilter hipassFilter;
    
    //filter variables
    float lowpassValue;
    float hipassValue;
    float filteredSound;
    
    //white noise variables
    float whiteNoise();
    bool whiteNoiseOn;
    
    //saw wave variables
    maxiOsc sawWave;
    float sawFreq;
    bool sawOn;
    
    //square wave variables
    maxiOsc squareWave;
    float squareFreq;
    bool squareOn;
    
    //function for storing filters
    float filters();
    
    //high pass amp
    float hipassAmp;
    
    //lowpass button
    ofxDatGuiToggle* lowpassToggle;
    bool lowpassOn;
    void onToggleEvent(ofxDatGuiToggleEvent e);
    //lowpass slider
    ofxDatGuiSlider* lowpassSlider;
    ofParameter<float> ofParamFloat;
    void onSliderEvent(ofxDatGuiSliderEvent e);
    
    //highpass button
    ofxDatGuiToggle* hipassToggle;
    bool hipassOn;
    //hipass slider
    ofxDatGuiSlider* hipassSlider;
    
    //white noise button
    ofxDatGuiToggle* whiteNoiseToggle;
    
    //saw wave botton
    ofxDatGuiToggle* sawToggle;
    //saw wave slider
    ofxDatGuiSlider* sawSlider;
    
    //square botton
    ofxDatGuiToggle* squareToggle;
    //square wave slider
    ofxDatGuiSlider* squareSlider;
    
    //maxi fft object
    maxiFFT fft;
    //fft frequencies calculation function
    vector<float> frequencies;
    
    ofTrueTypeFont title;
    ofTrueTypeFont filtersTitle;
    
};

