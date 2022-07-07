#pragma once

class Program
{
public:
    virtual const char* get_name() { return "<unnamed>"; };
    virtual void init() { };
    virtual void tick(unsigned long step, unsigned long time, unsigned long delta) = 0;
};
