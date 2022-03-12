#ifndef PIECESHADER_H
#define PIECESHADER_H

#include <QGraphicsRectItem>
#include <vector>
#include <map>

#include "piece.h"

using std::vector;

class PieceShader
{
public:
    PieceShader(QColor c, int length, int border = 0);

    void setColor(QColor c);
    QColor color() const;
private:
    QColor colorInner_ = Qt::gray;
    int length_ = 30;
    int border_ = 0;
    QColor colorBorder_ = Qt::black;
};


class PuzzleShader : public QGraphicsItem
{
public:
    PuzzleShader(const vector<vector<int>>& landMatrix, QGraphicsItem *parent = nullptr);

    void render(const Shape&);
    void reset();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:

    void updateLand(const vector<vector<int>>& shape, QPoint pos, QColor c);

private:
    const std::map<std::string, QColor> pieceColor_ = {
        {"Spill", QColor("#DD7126")},
        {"LongL", QColor("#38629A")},
        {"ShortL", QColor("#F8BF28")},
        {"EqualSide", QColor("#DF3794")},
        {"RectCutAngle", QColor("#FEF6AD")},
        {"TShape", QColor("#9D84BD")},
        {"FlashLong", QColor("#F5AF60")},
        {"FlashShort", QColor("#F79A8B")},
        {"ZShape", QColor("#D568A2")},
        {"Bar", QColor("#84D5F7")}
    };
    const std::map<int, std::string> pieceFlag_ = {
        {1, "Spill"},
        {2, "LongL"},
        {3, "ShortL"},
        {4, "EqualSide"},
        {5, "RectCutAngle"},
        {6, "TShape"},
        {7, "FlashLong"},
        {8, "FlashShort"},
        {9, "ZShape"},
        {10,"Bar"}
    };
    int pieceLength_ = 50;
    int width_ = 0, height_ = 0;
    int nWidth_ = 0, nHeight_ = 0;
    QColor backgroundColor_ = Qt::gray;

    vector<vector<PieceShader*>> land_;
    vector<vector<int>> landMatrix_;
};

#endif // PIECESHADER_H
