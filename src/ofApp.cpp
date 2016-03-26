#include "ofApp.h"
#include "particle.h"

ofSoundPlayer sound;

const int NBANDS = 512;
float spectrum[NBANDS];
int bandRadius =   10;
int bandVelocity = 10;

ofFbo fbo;

const int numParticles = 500;
vector<Particle*> particles;
ofVec3f angle;
float time0 = 0;

void ofApp::setup() {

    ofBackgroundGradient(ofColor(255), ofColor(128));

    fbo.allocate(ofGetWidth(), ofGetHeight());

    sound.load("Spikenard.mp3");
    sound.play();

    angle.set(ofRandom(0.5), 0, ofRandom(2.6));

    for (int i = 0; i < numParticles; i++) {
        Particle* p = new Particle();
        p->radius = ofRandom(ofGetWidth() / 2, ofGetWidth());
        p->velocity.set(ofRandom(0.5), ofRandom(0.5));
        p->offset.set(ofRandom(0, 1000), ofRandom(0, 1000));
        p->size = ofRandom(4, 8);
        particles.push_back(p);
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
    dt = ofClamp(dt, 0.0, 0.5);
    time0 = time;

    // radius =   ofMap(spectrum[bandRadius], 1, 2, 400, 800, true);
    // velocity = ofMap(spectrum[bandVelocity], 1, 0.4, 0.05, 0.25);

    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles.at(i);
        p->radius = ofMap(spectrum[bandRadius], 1, 2, 400, 800, true);
        p->velocity.x = ofMap(spectrum[bandVelocity], 1, 0.2, 0.05, 0.5);
        p->velocity.y = ofMap(spectrum[bandVelocity], 1, 0.4, 0.01, 0.5);
        p->update(dt);

        if (!p->alive()) {
            particles.erase(particles.begin() + i);

            Particle* p = new Particle();
            p->radius = ofRandom(ofGetWidth() / 2, ofGetWidth());
            p->velocity.set(ofRandom(0.5), ofRandom(0.5));
            p->offset.set(ofRandom(0, 1000), ofRandom(0, 1000));
            p->size = ofRandom(4, 8);
            particles.push_back(p);

        }
    }
}

void ofApp::draw() {

    ofEnableAlphaBlending();

    fbo.begin();

    ofBackgroundGradient(ofColor(255), ofColor(128));

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles.at(i);
        ofSetColor(0, 0, 0, p->getAlpha());
        ofDrawCircle(p->position, p->size);
    }

    ofRotateX(angle.x);
    ofRotateZ(angle.z);
    angle.x += 1.5;
    angle.z += 0.5;

    fbo.end();
    ofPopMatrix();
    fbo.draw(0, 0);
}
