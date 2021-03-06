#ifndef CORE_PLAIN_FIELD_H_
#define CORE_PLAIN_FIELD_H_

#include <string>

#include "core/field_constant.h"
#include "core/puyo_color.h"

struct Position;
class FieldChecker;

class PlainField : public FieldConstant {
public:
    PlainField();
    explicit PlainField(const std::string& url);

    // Gets a color of puyo at the specified position.
    PuyoColor color(int x, int y) const { return field_[x][y]; }
    // Returns true if puyo on (x, y) is c.
    bool isColor(int x, int y, PuyoColor c) const { return field_[x][y] == c; }
    // Returns true if puyo on (x, y) is empty.
    bool isEmpty(int x, int y) const { return isColor(x, y, PuyoColor::EMPTY); }

    // Sets a color of puyo at the specified position.
    void setColor(int x, int y, PuyoColor c) { field_[x][y] = c; }

    // Returns true if the field does not have any puyo.
    // Puyo on the 14th row will be ignored.
    bool isZenkeshi() const;

    // Returns true if color(x, y) is connected in some direction.
    bool isConnectedPuyo(int x, int y) const;

    // Returns the number of puyos connected to (x, y).
    // 13th row (and 14th row of course) won't be counted.
    int countConnectedPuyos(int x, int y) const;
    // Same as countConnectedPuyos(x, y), but with checking using |checked|.
    int countConnectedPuyos(int x, int y, FieldChecker* checked) const;
    // Same as countConnectedPuyos(x, y).
    // If # of connected puyos is >= 4, the result is any value >= 4.
    // For example, if the actual number of connected is 6, result is 4, 5, or 6.
    // This is faster than countConnectedPuyos, so this will be useful when checking
    // puyo is vanished or not.
    int countConnectedPuyosMax4(int x, int y) const;

    // Drops all the puyos in the air. Puyos in the 14th row won't be dropped.
    void drop();

    // Inserts positions whose puyo color is the same as |c|, and connected to (x, y).
    // The checked cells will be marked in |checked|.
    // PositionQueueHead should have enough capacity.
    Position* fillSameColorPosition(int x, int y, PuyoColor c, Position* positionQueueHead, FieldChecker* checked) const;

    // Returns true if there is an empty neighbor of (x, y).
    bool hasEmptyNeighbor(int x, int y) const;

    std::string toString(char charIfEmpty = ' ') const;
    std::string toDebugString() const;

    // Vanishes normal color puyos if 4 or more puyos are connected.
    // Score is returned.
    int vanish(int currentChain);

    // Returns the column data. The result pointer should be 16-byte aligned.
    const PuyoColor* column(int x) const { return field_[x]; }

    friend bool operator==(const PlainField&, const PlainField&);

private:
    void initialize();

    alignas(16) PuyoColor field_[MAP_WIDTH][MAP_HEIGHT];
};

inline bool PlainField::hasEmptyNeighbor(int x, int y) const
{
    return isEmpty(x, y + 1) || isEmpty(x, y - 1) || isEmpty(x + 1, y) || isEmpty(x - 1, y);
}

inline bool PlainField::isConnectedPuyo(int x, int y) const
{
    PuyoColor c = color(x, y);
    return color(x, y - 1) == c || color(x, y + 1) == c || color(x - 1, y) == c || color(x + 1, y) == c;
}

#endif
