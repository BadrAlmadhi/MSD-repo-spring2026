#pragma once // inclued only once
#include <string>
#include "val.h"
#include "pointer.h"

CLASS(Env) {
public:
    virtual ~Env() {}

    virtual PTR(Val) lookup(std::string name) = 0;
    static PTR(Env) empty;


};

class EmptyEnv : public Env {
public:

    EmptyEnv();
    PTR(Val) lookup(std::string name) override;
};

class ExtendedEnv : public Env {
    public:

    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string name) override;
};