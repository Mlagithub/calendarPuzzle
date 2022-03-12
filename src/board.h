#ifndef BOARD_H
#define BOARD_H

#include <QDebug>
#include <vector>
#include <unordered_map>

#include "piece.h"

class Board
{
public:
    enum Month
    {
        Jan = 1,
        Feb = 2,
        Mar = 3,
        Apr = 4,
        May = 5,
        Jun = 6,
        Jul = 7,
        Aug = 8,
        Seq = 9,
        Oct = 10,
        Nov = 11,
        Dec = 12
    };
    enum Week
    {
        Mon = 1,
        Tues = 2,
        Wed = 3,
        Thur = 4,
        Fri = 5,
        Sat = 6,
        Sun = 7
    };
    using Day = int;
    static const int CheckFlag;

public:
    Board();

    void initState();

    int width() const;

    int height() const;

    void setup(const Month month, const Day day, const Week week);

    bool inBoard(const int i, const int j) const;

    bool empty(const int i, const int j) const;

    bool fill(int height, int width, int val=-1);

    int value(const int i, const int j);

    void update();

    const std::vector<std::vector<int>>& data() const;

private:
    struct Position
    {
        int x;
        int y;
    };

private:
    std::vector<std::vector<int>> data_;
    int width_ = 7;
    int height_ = 8;
    int month_ = 1, day_ = 1, week_ = 1;
};

namespace std
{
    std::size_t vecHash(std::vector<int> const& vec);
    bool operator<(const Board& lhs, const Board& rhs);
}

#endif // BOARD_H
