#include "PlayerInfo.h"

std::ostream& operator<<(std::ostream& out, const PlayerInfo& player)
{
    out << "Player"
              << "|ID="                << player.getId()
              << "|IP="                << player.getIp().toUtf8().constData()
              << "|PORT="              << player.getPort()
              << "|NAME="              << player.getName().toUtf8().constData()
              << "|NB_TRIES="          << player.getTotalTries()
              << "|TotalSolvedGames="  << player.getTotalSolvedGames()
              << "|LAST_LOGIN="        << player.getLastLogin();
    return out;
}
