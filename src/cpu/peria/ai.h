#ifndef CPU_PERIA_H_
#define CPU_PERIA_H_

#include "core/client/ai/ai.h"

#include <memory>  // available with C++11

namespace peria {


class Ai : public ::AI {
 public:
  Ai(int argc, char* argv[]);
  virtual ~Ai();

 protected:
  struct Attack;

  virtual DropDecision think(int frame_id,
                             const PlainField& field,
                             const KumipuyoSeq& seq,
                             const AdditionalThoughtInfo& info) override;
  virtual void gameWillBegin(const FrameData& frame_data) override;
  virtual void enemyGrounded(const FrameData& frame_data) override;

  // Information about opponent's attacks.
  std::unique_ptr<Attack> attack_;
};

}  // namespace peria

#endif // CPU_PERIA_H_
