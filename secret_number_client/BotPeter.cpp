#include "BotPeter.h"


void BotPeter::setup(int32_t min, int32_t max)
{
    m_max = max;
    m_min = min;

}

void BotPeter::learn()
{
    m_output = m_min + (m_max - m_min) / 2;
}

int32_t BotPeter::getOutput()
{
    return m_output;
}

void BotPeter::onFail(int32_t value, int32_t state)
{
    if(state == -1)
    {
        m_max = value - 1;
    }
    else if(state == 1)
    {
        m_min = value + 1;
    }
}
