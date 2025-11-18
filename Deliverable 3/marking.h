#ifndef MARKING_H
#define MARKING_H

class markingSystem {
private:
    int liveScore;
    int threshold;

public:
    markingSystem(int th = 10)
        : liveScore(0), threshold(th) {}

    void addMark(int value) {
        liveScore += value;
    }

    int getMArk() const { return liveScore; }

    bool shouldShutdown() const {
        return liveScore >= threshold;
    }
};

#endif
