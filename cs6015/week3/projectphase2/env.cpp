#include "env.h"
#include <stdexcept>


// shared empty enviroment
PTR(Env) Env::empty = NEW(EmptyEnv)();

// compty constructor
EmptyEnv::EmptyEnv() {}

// Empty Enviroment lookup
PTR(Val) EmptyEnv::lookup(std::string name) {
    throw std::runtime_error("free variable: " + name);
}

// Extended constructor
ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

// 
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name)
        return val;
    return rest->lookup(find_name);
}