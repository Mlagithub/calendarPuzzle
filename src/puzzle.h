#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <QThread>

#include "piece.h"
#include "board.h"

struct comp
{
    template <typename T>
    bool operator()(const T &l, const T &r) const
    {
        return l.second <= r.second;
    }
};

class Puzzle : public QThread
{
    Q_OBJECT

public:
    Puzzle();

public:
    struct PuzzleDate
    {
        int month, day, week;
    };

public:
    void solve();

    PuzzleDate date() const;

    void setDate(Board::Month month, Board::Day day, Board::Week week);

    void setMonth(Board::Month month);

    void setDay(Board::Day day);

    void setWeek(Board::Week week);

    vector<vector<int>> board() const;

    const shape& result(const int i) const;

    const vector<shape>& result() const;

signals:
    void solving(QString val);
    void solveFinisthed();

protected:
    void run() override;

private:
    void init(Piece piece, Board& board);

    std::pair<bool, Board> dropShapeAt(shape curShape, int x, int y, const Board& board);

    int minArea(Board& board);

    int dfs(Board& board, int i, int j);

    void dumpResult();


private:
    Board board_;
    std::vector<shape> result_;

    std::vector<Piece> pieces_;
    std::queue<Board> queue_;

    Board::Month month_ = Board::Jan;
    Board::Day day_ = 1;
    Board::Week week_ = Board::Fri;
};

#endif // PUZZLE_H
