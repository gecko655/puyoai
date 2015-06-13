#ifndef CAPTURE_AC_ANALYZER_H_
#define CAPTURE_AC_ANALYZER_H_

#include "base/base.h"
#include "capture/analyzer.h"
#include "gui/bounding_box.h"  // TODO(mayah): Consider removing this

struct Box;
struct HSV;
struct RGB;

class ACAnalyzer : public Analyzer {
public:
    ACAnalyzer();
    virtual ~ACAnalyzer();

    enum class AllowOjama { DONT_ALLOW_OJAMA, ALLOW_OJAMA };
    RealColor analyzeBox(const SDL_Surface* surface, const Box& b, AllowOjama = AllowOjama::ALLOW_OJAMA, bool showsColor = false) const;

    // Draw each pixel of |surface| with RealColor. This is helpful for image analyzing test.
    void drawWithAnalysisResult(SDL_Surface*);

    static RealColor estimateRealColor(const HSV&);

private:
    CaptureGameState detectGameState(const SDL_Surface*) override;
    std::unique_ptr<DetectedField> detectField(int pi,
                                               const SDL_Surface* current,
                                               const SDL_Surface* prev,
                                               const DetectedField* prevDetectedField) override;
    bool detectOjamaDrop(const SDL_Surface* current, const SDL_Surface* prev, const Box&);

    bool isLevelSelect(const SDL_Surface*);
    bool isGameFinished(const SDL_Surface*);

    bool isMatchEnd(const SDL_Surface*);
    bool isDead(int playerId, const SDL_Surface*);

    void drawBoxWithAnalysisResult(SDL_Surface*, const Box&);
};

#endif
