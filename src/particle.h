#ifndef _PARTICLE
#define _PARTICLE

#include "ofMain.h"

class Particle {
    float birth;
    float life;
    float alpha;
    bool dead;

public:

    ofVec2f position;
    ofVec2f offset;
    ofVec2f velocity;
    float radius;
    float size;

    Particle() {
        birth = ofGetElapsedTimef();
        life = birth + ofRandom(1, 20);
        alpha = 255;
        dead = false;
    }

    void update(float dt) {

        offset.x += velocity.x * dt;
        offset.y += velocity.y * dt;

        position.x = ofSignedNoise(offset.x) * radius;
        position.y = ofSignedNoise(offset.y) * radius;

        size += ofSignedNoise(offset.x*offset.y) * velocity.x;

        // update life
        float now = ofGetElapsedTimef();
        float diff = now - birth;
        alpha = ofMap(diff, birth, birth + life, 255, 0, true);
        if (alpha <= 0) {
            alpha = 0;
            dead = true;
        }
    }

    bool alive() {
        return !dead;
    }

    float getAlpha() {
        return alpha;
    }
};

#endif
