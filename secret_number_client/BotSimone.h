#ifndef BOTSIMONE_H
#define BOTSIMONE_H

#include "Bot.h"

class BotSimone : public Bot
{
    public:
        BotSimone()
            : m_output(0)
            , m_max(0)
            , m_min(0)
        {}
        void setup(int32_t min, int32_t max);
        void learn();
        int32_t getOutput();

        void onFail(int32_t value, int32_t state);

    private:
        int32_t m_output;
        int32_t m_max;
        int32_t m_min;
};

#endif // BOTSIMONE_H
