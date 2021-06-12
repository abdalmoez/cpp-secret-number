#ifndef HISTORYSTORAGE_H
#define HISTORYSTORAGE_H

#include <vector>
#include <QString>
#include "HistoryRecord.h"

class HistoryStorage
{
public:
    static HistoryRecord* add(
            QString              player_name,
            uint64_t             start,
            uint64_t             time_ms,
            uint64_t             end,
            QString              ip,
            uint16_t             port,
            uint32_t             nb_tries,
            uint32_t             playerid,
            int32_t              min,
            int32_t              max,
            HistoryRecordState   status);

    static void save();
    static void load();
    static std::vector<HistoryRecord>& getTop5Players()
    {
        return m_top5Records;
    }
    static uint64_t getRank(HistoryRecord* record)
    {
        if(record == nullptr)
        {
            return 1;
        }
        uint64_t rank = 1;
        for(auto it=m_records.begin();it!=m_records.end();it++)
        {
            if(it->getTimeMs() < record->getTimeMs() &&
                    it->getMax() == record->getMax() &&
                    it->getMin() == record->getMin() &&
                    it->getStatus() == HistoryRecordState_Win)
            {
                rank++;
            }
        }
        return rank;
    }

private:
    static std::vector<HistoryRecord> m_records;
    static std::vector<HistoryRecord> m_top5Records;
};

#endif // HISTORYSTORAGE_H
