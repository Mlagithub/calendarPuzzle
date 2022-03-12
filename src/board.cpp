#include "board.h"

namespace std
{
std::size_t vecHash(std::vector<int> const& vec) {
    std::size_t seed = vec.size();
    for(auto& i : vec) {
        seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

bool operator<(const Board& lhs, const Board& rhs)
{
    std::size_t thisSeed = 0, otherSeed = 0;
    for(auto it : lhs.data()) thisSeed += vecHash(it);
    for(auto it : rhs.data()) otherSeed += vecHash(it);

    return thisSeed > otherSeed;
}
}

const int Board::CheckFlag = 11;


Board::Board()
{
    this->initState();
}

void Board::initState()
{
    // Board: height x width
    shape(height_, std::vector<int>(width_, 0)).swap(data_);

    // Handle special position
    fill(0,6);
    fill(1,6);
    fill(7,0);
    fill(7,1);
    fill(7,2);
    fill(7,3);
}

int Board::width() const {return width_;}

int Board::height() const {return height_;}

void Board::setup(const Month month, const Day day, const Week week)
{
    this->initState();

    month_ = month;
    day_ = day;
    week_ = week;

    // place month
    int month_h = int(month/6.1);
    int month_w = int((month-month_h*6)%7)-1;
    fill(month_h, month_w);

    // place day
    int day_h = int(day/7.1);
    int day_w = int((day-day_h*7)%8)-1;
    day_h += 2;
    fill(day_h, day_w);

    // place week
    std::unordered_map<Week, Position> data{
        {Week::Mon,  {6, 4}},
        {Week::Tues, {6, 5}},
        {Week::Wed,  {6, 6}},
        {Week::Thur, {7, 4}},
        {Week::Fri,  {7, 5}},
        {Week::Sat,  {7, 6}},
        {Week::Sun,  {6, 3}}
    };
    auto pos = data[week];
    fill(pos.x, pos.y);
}

bool Board::inBoard(const int i, const int j) const
{
    if(i>=0 && i<height_ && j>=0 && j<width_) return true;
    qInfo("overange with postion [%d, %d]", i, j);

    return false;
}

bool Board::empty(const int i, const int j) const { return inBoard(i, j) && data_[i][j] == 0;}

bool Board::fill(int height, int width, int val)
{
    if(!inBoard(height, width)) return false;
    data_[height][width] = val;

    return true;
}

int Board::value(const int i, const int j)
{
    return data_[i][j];
}

void Board::update()
{
    for(int i=0; i<height_; ++i)
    {
        for(int j=0; j<width_; ++j)
        {
            if(data_[i][j] == CheckFlag) data_[i][j] = 0;
        }
    }
}

const std::vector<std::vector<int>>& Board::data() const {return data_;}
