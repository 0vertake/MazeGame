#pragma once
#include <string>

class PowerUp {
protected:
    int duration = 4; // Set to 4 because of the initial tick when the player picks up the item.
    bool active = false;

public:
    virtual ~PowerUp() {}
    virtual std::string getName() const = 0;
    virtual void activate() { active = true; }
    virtual void deactivate() { active = false; }
    virtual bool isActive() const { return active; }
    virtual int getRemainingTurns() const { return duration; }
    virtual void tick() {
        if (active && duration > 0)
            duration--;
        if (duration == 0)
            deactivate();
    }
};
