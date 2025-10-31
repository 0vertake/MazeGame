#pragma once
#include "PowerUp.h"

class Fog : public PowerUp {
public:
    std::string getName() const override { return "Fog of War"; }
};

class Sword : public PowerUp {
public:
    std::string getName() const override { return "Sword"; }
};

class Shield : public PowerUp {
public:
    std::string getName() const override { return "Shield"; }
};

class Hammer : public PowerUp {
public:
    std::string getName() const override { return "Hammer"; }
};