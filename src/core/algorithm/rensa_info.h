#ifndef CORE_ALGORITHM_RENSA_INFO_H_
#define CORE_ALGORITHM_RENSA_INFO_H_

#include <string>
#include "core/field/rensa_result.h"
#include "core/algorithm/column_puyo_list.h"

class FeasibleRensaInfo {
public:
    FeasibleRensaInfo() {}
    FeasibleRensaInfo(const RensaResult& rensaResult, int initiatingFrames) :
        rensaResult_(rensaResult), initiatingFrames_(initiatingFrames)
    {
    }

    const RensaResult& rensaResult() const { return rensaResult_; }

    int score() const { return rensaResult_.score; }
    int chains() const { return rensaResult_.chains; }
    int totalFrames() const { return rensaResult_.frames + initiatingFrames_; }
    int initiatingFrames() const { return initiatingFrames_; }

private:
    RensaResult rensaResult_;
    int initiatingFrames_;
};

struct PossibleRensaInfo {
    RensaResult rensaResult;
    ColumnPuyoList necessaryPuyoSet;

    std::string toString() const {
        return rensaResult.toString() + necessaryPuyoSet.toString();
    }
};

struct TrackedPossibleRensaInfo {
    RensaResult rensaResult;
    ColumnPuyoList necessaryPuyoSet;
    RensaTrackResult trackResult;
};

struct OngoingRensaInfo {
    OngoingRensaInfo() {}
    OngoingRensaInfo(const RensaResult& rensaResult, int finishingRensaFrame) :
        rensaResult(rensaResult), finishingRensaFrame(finishingRensaFrame) {}

    RensaResult rensaResult;
    int finishingRensaFrame;
};

#endif
