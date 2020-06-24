#include "ofApp.h"

//--------------------------------------------------------------
vector<float> fftFreqs(int fftSize, int sampleRate)
{
    vector<float> freqs;
    //calculating frequencies per bin for fft
    for (int i = 0; i < fftSize; ++i){
        freqs.push_back((sampleRate/float(fftSize)) * i);
    }
    return freqs;
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundColor(90,120,140);
    title.load("Audiowide-Regular.ttf", 24);
    filtersTitle.load("Audiowide-Regular.ttf", 16);
    
    //audio settings
    sampleRate = 44100;
    bufferSize = 512;
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

    //volume
    amplitude = 0.5;
    
    //lowpass level
    lowpassValue = 1.0;
    
    //hipass level
    hipassValue = 0.99;
    //hipassAmp = 0.50;
    
    //saw inital freq
    sawFreq = 440.0;
    
    //square inital freq
    squareFreq = 300;
    
    //fft settings
    //fft size
    int fftSize = 2048;
    //fft bins hop size
    fft.setup(fftSize, 1024, 512);
    //frequencies per bin
    frequencies = fftFreqs(fftSize, sampleRate);
    
    //lowpass slider
    lowpassSlider = new ofxDatGuiSlider(ofParamFloat.set("LOWPASS", 1.0f, 0.0f, 1.0f));
    lowpassSlider->setTheme(new ofxDatGuiThemeAqua());
    lowpassSlider->setPosition(65, 330);
    lowpassSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    //lowpass button
    lowpassToggle = new ofxDatGuiToggle("ON/OFF",false);
    lowpassToggle->setTheme(new ofxDatGuiThemeAqua());
    lowpassToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    lowpassToggle->setPosition(0, 330);
    lowpassToggle->setWidth(65);
    lowpassToggle->toggle();
    lowpassToggle->setChecked(lowpassOn);
    lowpassToggle->getChecked();
    
    //hipass slider
    hipassSlider = new ofxDatGuiSlider(ofParamFloat.set("HIPASS", 0.99f, 0.0f, 0.99f));
    hipassSlider->setTheme(new ofxDatGuiThemeAqua());
    hipassSlider->setPosition(65, 490);
    hipassSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    //hipass button
    hipassToggle = new ofxDatGuiToggle("ON/OFF",false);
    hipassToggle->setTheme(new ofxDatGuiThemeAqua());
    hipassToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    hipassToggle->setPosition(0, 490);
    hipassToggle->setWidth(65);
    hipassToggle->toggle();
    hipassToggle->setChecked(hipassOn);
    hipassToggle->getChecked();
    
    //white noise toggle
    whiteNoiseToggle = new ofxDatGuiToggle("WHITE NOISE",false);
    whiteNoiseToggle->setTheme(new ofxDatGuiThemeAqua());
    whiteNoiseToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    whiteNoiseToggle->setPosition(0, 100);
    whiteNoiseToggle->setWidth(100);
    whiteNoiseToggle->toggle();
    whiteNoiseToggle->setChecked(whiteNoiseOn);
    whiteNoiseToggle->getChecked();
    
    //saw toggle
    sawToggle = new ofxDatGuiToggle("SAW WAVE",false);
    sawToggle->setTheme(new ofxDatGuiThemeAqua());
    sawToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    sawToggle->setPosition(0, 130);
    sawToggle->setWidth(100);
    sawToggle->toggle();
    sawToggle->setChecked(sawOn);
    sawToggle->getChecked();
    //saw slider
    sawSlider = new ofxDatGuiSlider("FREQUENCY", 0.0, 2000.0, 440.0);
    sawSlider->setTheme(new ofxDatGuiThemeAqua());
    sawSlider->setPosition(100, 130);
    sawSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    
    //square toggle
    squareToggle = new ofxDatGuiToggle("SQUARE WAVE",false);
    squareToggle->setTheme(new ofxDatGuiThemeAqua());
    squareToggle->onToggleEvent(this, &ofApp::onToggleEvent);
    squareToggle->setPosition(0, 160);
    squareToggle->setWidth(100);
    squareToggle->toggle();
    squareToggle->setChecked(squareOn);
    squareToggle->getChecked();
    //square slider
    squareSlider = new ofxDatGuiSlider("FREQUENCY", 0.0, 1000.0, 200.0);
    squareSlider->setTheme(new ofxDatGuiThemeAqua());
    squareSlider->setPosition(100, 160);
    squareSlider->onSliderEvent(this, &ofApp::onSliderEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //lowpass gui update
    lowpassSlider->update();
    lowpassToggle->update();
    
    //hipass gui update
    hipassSlider->update();
    hipassToggle->update();
    
    //white noise gui update
    whiteNoiseToggle->update();
    
    //saw wave gui update
    sawToggle->update();
    sawSlider->update();
   
    //square wave gui update
    squareToggle->update();
    squareSlider->update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    title.drawString("AUDIO FILTERS", ofGetWidth()/3,40);
    
    ofDrawBitmapString("This is an interactive application demonstrating the lowpass and highpass audio filters from the Maximilian library \nusing openFrameworks. First select a sound from below then listen to the sound with a filter being applied to it. \nTo use a filter press the on/off button for that filter and then use the slider to adjust the level of the filter.", 50, 60);
    
    filtersTitle.drawString("LOWPASS FILTER", 10,220);
    ofDrawBitmapString("A lowpass filter works by allowing all frequencies below the cutoff frequency to pass through but reduces the amplitude of all \nfrequencies above the cutoff frequency. Therefore, lowpass filters are used to reduce the high frequency components from a \nsound, thus making the sound feel less bright. The lowpass filter in Maximilian ranges from 0 to 1, therefore when the level \nis at 1 no filtering is being applied to the sound and when the level is at 0 the maximum amount of filtering is being applied \nto the sound. Use the slider below to hear how the high frequency components of a sound are reduced as the filter decreases \nfrom 1 to 0. Furthermore, for a visual representation look at the FFT at the bottom to see how the high frequency energy in \nthe magnitude spectrum also changes as the filter decreases from 1 to 0.",10, 240);
    
    filtersTitle.drawString("HIGHPASS FILTER", 10,390);
    
    ofDrawBitmapString("A highpass filter is the opposite of a lowpass filter. Frequencies above the cutoff frequency are instead passed through and \nfrequencies below the cutoff frequency have their amplitude reduced. This means that a highpass filter is used to reduce the \nlow frequency components from a sound, perceptually this will make the sound seem lighter. The highpass filter in Maximilian \nworks the same as the lowpass filter and ranges from 0 to 1. Use the slider below to hear how the low frequency components of \na sound are reduced as the filter decreases from 1 to 0. Furthermore, by looking at the FFT as the filter gets closer to 0 \nthe high frequency energy in the magnitude spectrum will also start to become bigger.", 10, 410);
    
    ofDrawBitmapString("0 Hz",5,720);
    ofDrawBitmapString("22050 Hz",955,720);

    //lowpass gui draw
    lowpassSlider->draw();
    lowpassToggle->draw();
    
    //hipass gui draw
    hipassSlider->draw();
    hipassToggle->draw();
    
    //white noise gui draw
    whiteNoiseToggle->draw();
    
    //saw wave gui draw
    sawToggle->draw();
    sawSlider->draw();
    
    //square wave gui draw
    squareToggle->draw();
    squareSlider->draw();
    
    //fft magnitude spectrum drawing
    ofTranslate(0, ofGetHeight()/1.10);
    ofFill();

    ofSetColor(192,192,192);
    
    int binWidth = ofGetWidth()/fft.bins;

    ofDrawLine(0, 0, ofGetWidth(), 0);
    
    int drawFreq = 0;
    
    for(int i = 0; i < fft.bins; ++i) {
        ofDrawRectangle(i*binWidth, 0, binWidth, fft.magnitudes[i] * -2.);
        
        if (drawFreq <= 0 && fft.magnitudes[i] > 1.0) {
            drawFreq = 15;
        }
        --drawFreq;
    }
 
}

//--------------------------------------------------------------
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    //white noise button logic
    if(e.target == whiteNoiseToggle && e.checked == 1){
        
        whiteNoiseOn = true;
        sawOn = false;
        squareOn = false;
        sawToggle->setChecked(false);
        squareToggle->setChecked(false);
        //cout << "white noise on " << whiteNoiseOn << endl;
        //cout << "white noise togggle " << e.checked << endl;
    }
    else if(e.target == whiteNoiseToggle && e.checked == 0){
        whiteNoiseOn = false;
        //cout << "white noise togggle " << e.checked << endl;
    }
    
    //saw button logic
    if(e.target == sawToggle && e.checked == 1){
        sawOn = true;
        whiteNoiseOn = false;
        squareOn = false;
        whiteNoiseToggle->setChecked(false);
        squareToggle->setChecked(false);
        //cout << "saw " << sawOn << endl;
        //cout << "saw toggle " << e.checked << endl;
    }
    else if(e.target == sawToggle && e.checked ==0){
        sawOn = false;
    }
    
    //square button logic
    if(e.target == squareToggle && e.checked == 1){
        
        squareOn = true;
        sawOn = false;
        whiteNoiseOn = false;
        
        sawToggle->setChecked(false);
        whiteNoiseToggle->setChecked(false);
        //cout << "saw " << sawOn << endl;
        //cout << "saw toggle " << e.checked << endl;
    }
    else if(e.target == squareToggle && e.checked == 0){
        squareOn = false;
    }
    
    //lowpass button logic
    if(e.target == lowpassToggle && e.checked == 1){
        lowpassOn = true;
        hipassToggle->setChecked(false);
        //cout << "lowpass " << lowpassOn << endl;
        //cout << "lowpass on " << lowpassOn << endl;
    }
    else {
        lowpassOn = false;
        lowpassToggle->setChecked(false);
        //cout << "lowpass on " << lowpassOn << endl;
        //cout << "lowpass " << lowpassOn << endl;
    }
    
    //high pass button logic
    if (e.target == hipassToggle && e.checked == 1){
        hipassOn = true;
        lowpassToggle->setChecked(false);
        // cout << "hipass " << hipassOn << endl;
        // cout << "hipass on " << hipassOn << endl;
    }
    else {
        hipassOn = false;
        hipassToggle->setChecked(false);
        //cout << "hipass on " << hipassOn << endl;
        // cout << "hipass " << hipassOn << endl;
    }
    //cout << e.target->getLabel() << " checked = " << e.checked << endl;
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    //saw wave slider logic
    if (e.target == sawSlider) {
        sawSlider->bind(sawFreq);
    }
    
    //square wave slider logic
    if (e.target == squareSlider){
        squareSlider->bind(squareFreq);
    }
    
    //lowpass slider logic
    if (e.target == lowpassSlider){
        lowpassSlider->bind(lowpassValue);
        // cout << lowpassValue << endl;
        // cout << e.target->getLabel() << " value = "; e.value;
    }
    
    //highpass slider logic
    if (e.target == hipassSlider){
        hipassSlider->bind(hipassValue);
        // cout << hipassValue << endl;
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & output)
{
    size_t nChannels = output.getNumChannels();
    
    for(int i = 0; i < output.getNumFrames(); i++){
        
        //variable for storing the sound
        float sound = 0.0;
        //adding the filtered sound to the sound variable
        sound += filters();
        //left and right channels
        output [i * nChannels] = sound * amplitude;
        output [i * nChannels + 1] = sound * amplitude;
        //for next fft calculation
        fft.process(output [i * nChannels]);
    }
}

//--------------------------------------------------------------
float ofApp::whiteNoise()
{
    //generating white noise
    return ofRandom(-0.50,0.50)*2;
}

float ofApp::filters()
{
    //variable for storing the filtered sounds
    filteredSound = 0.0;
    
    //if white noise has been selected logic
    if(whiteNoiseOn == true){
        //if white noise has been selected and lowpass filter has been turned on
        if(lowpassOn == true)
        {
            //applying a lowpass filter to white noise
            filteredSound += lowpassFilter.lopass(whiteNoise(), lowpassValue);
        }
        //if white noise has been selected and highpass filter has been turned on
        else if(hipassOn == true)
        {
            //increasing intial amplidute due to the high pass filters affect on the amplidute
            if(hipassValue >= 0.99){hipassAmp = 2.0;}
            else{hipassAmp = 1.0;}
            //applying a highpass filter to white noise
            filteredSound += hipassFilter.hipass(whiteNoise(), hipassValue) * hipassAmp;
        }
        //if white noise has been selected and no filters have been selected
        else {
            filteredSound +=whiteNoise();
        }
    }
    
    //if saw wave has been selected logic
    if(sawOn == true){
        //if saw wave has been selected and lowpass filter has been turned on
        if(lowpassOn == true)
        {
            //applying a lowpass filter to saw wave
            filteredSound += lowpassFilter.lopass(sawWave.saw(sawFreq), lowpassValue);
        }
        //if saw wave has been selected and highpass filter has been turned on
        else if(hipassOn == true)
        {
            //applying a highpass filter to saw wave
            if(hipassValue >= 0.99){hipassAmp = 2.0;}
            else{hipassAmp = 1.0;}
            filteredSound += hipassFilter.hipass(sawWave.saw(sawFreq), hipassValue) * hipassAmp;
        }
        else {
            //if saw wave has been selected and no filters have been selected
            filteredSound +=sawWave.saw(sawFreq);
        }
    }
    
    //if square wave has been selected logic
    if(squareOn == true){
        //if square wave has been selected and lowpass filter has been turned on
        if(lowpassOn == true)
        {
            //applying a lowpass filter to square wave
            filteredSound += lowpassFilter.lopass(squareWave.square(squareFreq), lowpassValue) * 0.50;
        }
        //if square wave has been selected and highpass filter has been turned on
        else if(hipassOn == true)
        {
            //applying a highpass filter to square wave
            if(hipassValue >= 0.99){hipassAmp = 2.0;}
            else{hipassAmp = 1.0;}
            filteredSound += hipassFilter.hipass(squareWave.square(squareFreq), hipassValue) * 0.50 * hipassAmp;
        }
        else {
            //if square wave has been selected and no filters have been selected
            filteredSound +=squareWave.square(squareFreq)* 0.50;
        }
    }
    
    return filteredSound;
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






