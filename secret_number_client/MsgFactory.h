#ifndef MSGFACTORY_H
#define MSGFACTORY_H

#include <QString>
#include "MsgTypes.h"


class MsgFactory
{
public:
    static QString createNewGameRequest(uint32_t playerid);
    static QString createAnswerMsg(uint32_t playerid, uint32_t gameid, int32_t answer);
    static QString createTopScoresRequest(uint32_t playerid);

};

#endif // MSGFACTORY_H
