#include "BotSimone.h"

void BotSimone::setup(int32_t min, int32_t max)
{
    m_max = max;
    m_min = min;

}

void BotSimone::learn()
{
    m_output = (m_max + m_min) / 2;
}

int32_t BotSimone::getOutput()
{
    return m_output;
}

void BotSimone::onFail(int32_t value, int32_t state)
{
    if(state == -1)
    {
        m_max = value;
    }
    else if(state == 1)
    {
        if(m_min + 1 == m_max)
        {
            m_min = m_max;
        }
        else
        {
            m_min = value;
        }
    }
}
