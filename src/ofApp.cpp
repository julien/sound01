#include "ofApp.h"

const int NBANDS = 512;
float spectrum[NBANDS];
int bandRadius = 10;
int bandVelocity = 80;

float radius = 1200;
float velocity = 0.5;
const int numPoints = 500;
float tx[numPoints], ty[numPoints]; // translation
float size[numPoints];
int color[numPoints];
ofPoint points[numPoints];


float time0 = 0;

void ofApp::setup() {
    ofSetBackgroundColor(ofColor(255, 255, 255));

    sound.load("Spikenard.mp3");
    sound.play();

    for (int i = 0; i < numPoints; i++) {
        tx[i] = ofRandom(0, 800);
        ty[i] = ofRandom(0, 800);

        size[i] = ofRandom(2, 12);

        color[i] = ofRandom(0, 128);
    }
}

void ofApp::update() {
    // ofSoundUpdate();

    float *val = ofSoundGetSpectrum(NBANDS);

    for (int i = 0; i < NBANDS; i++) {
        spectrum[i] *= 0.98;
        spectrum[i] = max(spectrum[i], val[i]);
    }

    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp(dt, 0.0, 0.1);
    time0 = time;

    radius =   ofMap(spectrum[bandRadius], 1, 2, 400, 800, true);
    velocity = ofMap(spectrum[bandVelocity], 1, 0.4, 0.05, 0.25);

    for (int i = 0; i < numPoints; i++) {
        tx[i] += velocity * dt;
        ty[i] += velocity * dt;

        points[i].x = ofSignedNoise(tx[i]) * radius;
        points[i].y = ofSignedNoise(ty[i]) * radius;

        size[i] += ofSignedNoise(tx[i]*ty[i]) * velocity;
    }
}

float angleX, angleY, angleZ = 0.0;

void ofApp::draw() {
    // ofEnableAlphaBlending();
    ofPushMatrix();

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    for (int i = 0; i < numPoints; i++) {

        ofSetColor(color[i], color[i], color[i]);
        // ofFill();
        ofDrawCircle(points[i], size[i]);
    }

    ofRotateX(angleX);
    angleX += 0.5;

    ofRotateZ(angleZ);
    angleZ += 0.02;

    ofPopMatrix();
}
