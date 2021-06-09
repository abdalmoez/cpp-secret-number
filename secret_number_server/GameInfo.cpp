#include "GameInfo.h"


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
