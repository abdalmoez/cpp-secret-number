#ifndef BOT_H
#define BOT_H

#include <QString>

class Bot
{
    public:
        virtual  void setup(int32_t min, int32_t max) = 0;
        virtual  void learn() = 0;
        virtual  int32_t getOutput() = 0;

        virtual  void onFail(int32_t value, int32_t state) = 0;

};

#endif // BOT_H
