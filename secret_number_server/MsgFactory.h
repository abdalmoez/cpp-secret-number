#ifndef MSGFACTORY_H
#define MSGFACTORY_H

#include <QString>
#include "MsgTypes.h"

class MsgFactory
{
public:
    static QString createAcceptedConnectionMsg(uint32_t playerid, uint64_t logintime);

    static QString createGameCreatedMsg(uint32_t gameid, uint32_t playerid, uint64_t start_time);
    static QString createInvalidAnswerMsg(uint32_t gameid, uint32_t playerid, uint32_t total_tries, int32_t state);
    static QString createGameoverLoserMsg(uint32_t gameid, uint32_t playerid, uint64_t start_time, uint64_t end_time, uint64_t total_time, uint32_t total_tries, uint64_t rank);
    static QString createGameoverWinnerMsg(uint32_t gameid, uint32_t playerid, uint64_t start_time, uint64_t end_time, uint64_t total_time, uint32_t total_tries, uint64_t rank);
    static QString createErrorMsg(uint16_t rc);
};

#endif // MSGFACTORY_H
