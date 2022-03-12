#include "piece.h"


namespace std
{
std::ostream& operator<<(ostream& os, shape s)
{
    for(auto it : s)
    {
        os << '[';
        for(auto jt : it)
        {
            os << ' ' << jt;
        }
        os << " ]\n";
    }

    return os;
}
}


Piece::Piece(string name, shape s) : name_(name) {
    auto& baseShape = s;
    shapes.insert(baseShape);
    shapes.insert(Rotate90{}(baseShape));
    shapes.insert(Rotate180{}(baseShape));
    shapes.insert(Rotate270{}(baseShape));

    auto flipShape = FlipLR{}(baseShape);
    shapes.insert(flipShape);
    shapes.insert(Rotate90{}(flipShape));
    shapes.insert(Rotate180{}(flipShape));
    shapes.insert(Rotate270{}(flipShape));
}

string Piece::name() const { return name_; }

vector<shape> Piece::poes()
{
    vector<shape> rst;
    for(auto it : shapes)
    {
        rst.push_back(it);
    }
    return rst;
}
