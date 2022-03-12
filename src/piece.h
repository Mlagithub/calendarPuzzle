#ifndef PIECE_H
#define PIECE_H


#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::vector;
using std::cout;
using std::cout;
using std::string;
using std::set;
using shape = vector<vector<int>>;
using Shape = vector<vector<int>>;

namespace std
{
    std::ostream& operator<<(ostream& os, shape s);
} // namespace std

struct Rotate90
{
    shape operator()(shape& s)
    {
        int h = s.size();
        int w = s[0].size();
        shape rst(w, vector<int>(h, 0));
        for(int i = 1; i<=h; ++i)
        {
            for(int j=1; j<=w; ++j)
            {
                rst[j-1][h+1-i-1] = s[i-1][j-1];
            }
        }
        return rst;
    }
};

struct Rotate180
{
    shape operator()(shape& s)
    {
        int h = s.size();
        int w = s[0].size();
        shape rst(h, vector<int>(w, 0));
        for(int i = 1; i<=h; ++i)
        {
            for(int j=1; j<=w; ++j)
            {
                rst[h+1-i-1][w+1-j-1] = s[i-1][j-1];
            }
        }
        return rst;
    }
};

struct Rotate270
{
    shape operator()(shape& s)
    {
        int h = s.size();
        int w = s[0].size();
        shape rst(w, vector<int>(h, 0));
        for(int i = 1; i<=h; ++i)
        {
            for(int j=1; j<=w; ++j)
            {
                rst[w+1-j-1][i-1] = s[i-1][j-1];
            }
        }
        return rst;
    }
};

struct FlipLR
{
    shape operator()(shape& s)
    {
        int h = s.size();
        int w = s[0].size();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w/2; ++j) {
                std::swap(s[i][j], s[i][w-1 - j]);
            }
        }
        return s;
    }
};

struct Piece
{
    Piece(string name, shape s);

    string name() const;

    vector<shape> poes();

private:
    string name_;
    set<shape> shapes;
};


#endif // PIECE_H
