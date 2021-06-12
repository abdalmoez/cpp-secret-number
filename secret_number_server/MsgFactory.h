#ifndef MSGFACTORY_H
#define MSGFACTORY_H

#include <QString>
#include <vector>
#include "MsgTypes.h"
#include "HistoryRecord.h"

class MsgFactory
{
public:
    static QString createAcceptedConnectionMsg(uint32_t playerid, uint64_t logintime);

    static QString createGameCreatedMsg(uint32_t gameid, uint32_t playerid, uint64_t start_time, int32_t max, int32_t min);

    static QString createInvalidAnswerMsg(uint32_t gameid, uint32_t playerid, int32_t answer, int32_t total_tries, int32_t state);

    static QString createGameoverLoserMsg(uint32_t gameid,
                                          uint32_t playerid,
                                          uint64_t start_time,
                                          uint64_t end_time,
                                          uint64_t total_time,
                                          int32_t  total_tries,
                                          int32_t  secret_number,
                                          uint64_t rank,
                                          std::vector<HistoryRecord> top_players);

    static QString createGameoverWinnerMsg(uint32_t gameid,
                                           uint32_t playerid,
                                           uint64_t start_time,
                                           uint64_t end_time,
                                           uint64_t total_time,
                                           int32_t  total_tries,
                                           int32_t  secret_number,
                                           uint64_t rank,
                                           std::vector<HistoryRecord>& top_players);

    static QString createErrorMsg(uint16_t rc);
};

#endif // MSGFACTORY_H
