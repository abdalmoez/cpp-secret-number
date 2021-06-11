#include "BotAlex.h"

void BotAlex::setup(int32_t min, int32_t max)
{
    m_max = max;
    m_min = min;
    m_output = min - 1;
}

void BotAlex::learn()
{
    if(m_output<m_max)
    {
        m_output++;
    }
}

int32_t BotAlex::getOutput()
{
    return m_output;
}

void BotAlex::onFail(int32_t value, int32_t state)
{
    //No backward
}
