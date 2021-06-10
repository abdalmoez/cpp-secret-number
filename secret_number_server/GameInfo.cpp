#include "GameInfo.h"


int32_t GameInfo::s_minValue = 1;
int32_t GameInfo::s_maxValue = 100;

std::ostream& operator<<(std::ostream& out, const GameInfo& game)
{
    out << "Game"
            << "|GameId="         << game.getGameId()
            << "|PlayerId="       << game.getPlayerId()
            << "|SecretNumber="   << game.getSecretNumber()
            << "|NbTries="        << game.getNbTries()
            << "|StartTime="      << game.getStartTime()
            << "|EndTime="        << game.getEndTime()
            << "|TotalMs="        << game.getTotalMs()
            << "|IsGameEnded="    << game.isGameEnded()
            << "|isGameResolved=" << game.isGameResolved();
    return out;
}
