#pragma once
#include "PowerUp.h"

class Fog : public PowerUp {
public:
    std::string getName() const override { return "Magla rata"; }
};

class Sword : public PowerUp {
public:
    std::string getName() const override { return "Mac"; }
};

class Shield : public PowerUp {
public:
    std::string getName() const override { return "Stit"; }
};

class Hammer : public PowerUp {
public:
    std::string getName() const override { return "Cekic"; }
};