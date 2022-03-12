#include "puzzle.h"


Puzzle::Puzzle(){
    pieces_.push_back(Piece{"Spill", {
        {1, 0, 1},
        {1, 1, 1}}});
    pieces_.push_back(Piece{"LongL", {
        {2, 0},
        {2, 0},
        {2, 0},
        {2, 2}}});
    pieces_.push_back(Piece{"ShortL", {
        {3, 0},
        {3, 0},
        {3, 3}}});
    pieces_.push_back(Piece{"EqualSide", {
        {4, 0, 0},
        {4, 0, 0},
        {4, 4, 4}}});
    pieces_.push_back(Piece{"RectCutAngle", {
        {5, 5, 5},
        {5, 5, 0}}});
    pieces_.push_back(Piece{"TShape", {
        {6, 6, 6},
        {0, 6, 0},
        {0, 6, 0}}});
    pieces_.push_back(Piece{"FlashLong", {
        {7, 7, 0, 0},
        {0, 7, 7, 7}}});
    pieces_.push_back(Piece{"FlashShort", {
        {8, 8, 0},
        {0, 8, 8}}});
    pieces_.push_back(Piece{"ZShape", {
        {9, 0, 0},
        {9, 9, 9},
        {0, 0, 9}}});
    pieces_.push_back(Piece{"Bar", {
        {10, 10, 10, 10}}});
}


void Puzzle::solve()
{
    board_.setup(month_, day_, week_);

    solving(QString("\nSolve question: [%1 月 %2 日， 周 %3]").arg(month_).arg(day_).arg(week_));

    // init
    {
        std::set<std::pair<Piece, int>, comp> sortedPiece;
        for(auto it : pieces_)
        {
            std::queue<Board>{}.swap(queue_);
            auto tmp = board_;
            this->init(it, tmp);
            sortedPiece.insert({it, queue_.size()});
            // spdlog::info("Total case of piece {}, is {}", it.name(), queue_.size());
        }

        pieces_.clear();
        for(auto it : sortedPiece) pieces_.push_back(it.first);
        std::queue<Board>{}.swap(queue_);
        this->init(pieces_[0], board_);
        solving(QString("    Place piece %1, current methods %2").arg(pieces_[0].name().c_str()).arg(queue_.size()));
        qInfo("Place piece %s, current methods %d", pieces_[0].name().c_str(), queue_.size());
    }

    // loop all piece
    for(size_t iPiece=1; iPiece<pieces_.size(); ++iPiece)
    {
        // case number of curBoard
        int n = queue_.size();
        for(int i=0; i<n; ++i)
        {
            auto curBoard = queue_.front();

            // loop all case of each piece
            int id = 0;
            for(auto ishape: pieces_[iPiece].poes())
            {
                // check if ishape is drop-able
                for(int i=0; i<board_.height()-1; ++i)
                {
                    for(int j=0; j<board_.width()-1; ++j)
                    {
                        auto rst = dropShapeAt(ishape, i, j, curBoard);
                        if(rst.first == false) continue;

                        queue_.push(rst.second);
                    }
                }
                ++id;
            }

            // pop one
            queue_.pop();
        }

        solving(QString("    Place piece %1, current methods %2").arg(pieces_[iPiece].name().c_str()).arg(queue_.size()));
        qInfo("Place piece %s, current methods %d", pieces_[iPiece].name().c_str(), queue_.size());
    }
    // dumpResult();

    result_.clear();
    result_.reserve(queue_.size());
    while(!queue_.empty())
    {
        result_.push_back(queue_.front().data());
        queue_.pop();
    }

    emit solveFinisthed();
}

Puzzle::PuzzleDate Puzzle::date() const
{
    return PuzzleDate{month_, day_, week_};
}


void Puzzle::run()
{
    qDebug()<<"Puzzle thread ID: "<<QThread::currentThreadId()<<'\n';
    solve();
}


void Puzzle::init(Piece piece, Board& board)
{
    for(int i=0; i<board.height()-1; ++i)
    {
        for(int j=0; j<board.width()-1; ++j)
        {
            for(auto shape : piece.poes())
            {
                auto rst = dropShapeAt(shape, i, j, board);
                if(rst.first)
                {
                    queue_.push(rst.second);
                }
            }
        }
    }
}

std::pair<bool, Board> Puzzle::dropShapeAt(shape curShape, int x, int y, const Board& board)
{
    Board newBoard = board;

    for(size_t i=0; i<curShape.size(); ++i)
    {
        for(size_t j=0; j<curShape[i].size(); ++j)
        {
            if (curShape[i][j] == 0) {
                continue;
            }

            auto newX = x + i;
            auto newY = y + j;

            // 越界
            if (newX >= board.height() || newY >= board.width()) {
                return {false, newBoard};
            }

            // 重合
            if (!newBoard.empty(newX, newY)) {
                return {false, newBoard};
            }

            newBoard.fill(newX, newY, curShape[i][j]);
        }
    }

    // 剪枝, 优化的很明显
    int minA = minArea(newBoard);
    newBoard.update();
    if (minA < 4) {
        return {false, newBoard};
    }

    return {true, newBoard};
}

int Puzzle::minArea(Board& board)
{
    int minVal = 1000000;

    for(int i=0; i<board.height(); ++i)
    {
        for(int j=0; j<board.width(); ++j)
        {
            if(board.empty(i, j))
            {
                int area = this->dfs(board, i, j);
                minVal = std::min(minVal, area);
                if(area<4) return minVal;
            }
        }
    }

    return minVal;
}

int Puzzle::dfs(Board& board, int i, int j)
{
    // 越界
    if (!(0 <= i && i < 8 && 0 <= j && j < 7)) {
        return 0;
    }

    //
    if (!board.empty(i, j)) {
        return 0;
    }

    // flag
    board.fill(i, j, Board::CheckFlag);

    return 1 +
        + this->dfs(board, i - 1, j)
        + this->dfs(board, i + 1, j)
        + this->dfs(board, i, j - 1)
        + this->dfs(board, i, j + 1);
}

void Puzzle::dumpResult()
{
    qInfo("Result of %d月 %d日 周%d : %d", month_, day_, week_, queue_.size());

    int n = 0;
    while (!queue_.empty())
    {
        std::cout << ++n << '\n' << queue_.front().data() << '\n';
        queue_.pop();
    }
}


void Puzzle::setDate(Board::Month month, Board::Day day, Board::Week week)
{
    month_ = month;
    day_ = day;
    week_ = week;
}

void Puzzle::setMonth(Board::Month month)
{
    month_ = month;
}

void Puzzle::setDay(Board::Day day)
{
    day_ = day;
}

void Puzzle::setWeek(Board::Week week)
{
    week_ = week;
}

vector<vector<int>> Puzzle::board() const {return board_.data();}

const shape& Puzzle::result(const int i) const { return result_[i]; }

const vector<shape> &Puzzle::result() const
{
    return result_;
}
