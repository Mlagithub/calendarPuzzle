#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>

#include "pieceshader.h"

PieceShader::PieceShader(QColor c, int length, int border)
    : colorInner_(c), length_(length), border_(border)
{
}

void PieceShader::setColor(QColor c)
{
    colorInner_ = c;
}

QColor PieceShader::color() const
{
    return colorInner_;
}




PuzzleShader::PuzzleShader(const vector<vector<int> >& landMatrix, QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    landMatrix_ = landMatrix;
    nHeight_ = landMatrix.size();
    nWidth_ = landMatrix.at(0).size();
    land_.assign(nHeight_, vector<PieceShader*>(nWidth_, nullptr));
    height_ = pieceLength_*nHeight_;
    width_ = pieceLength_*nWidth_;
    for(int i=0; i<nHeight_; ++i)
    {
        for(int j=0; j<nWidth_; ++j)
        {
            if(landMatrix_[i][j]==0)
            {
                land_[i][j] = new PieceShader(Qt::gray, pieceLength_);
            }
            else
            {
                land_[i][j] = new PieceShader(Qt::white, pieceLength_);
            }
        }
    }
}

void PuzzleShader::render(const Shape& result)
{
    auto const H = result.size(), W = result.at(0).size();

    for(size_t i=0; i<H; ++i)
    {
        for(size_t j=0; j<W; ++j)
        {
            auto cur = result[i][j];
            if(cur == -1) continue;
            land_[i][j]->setColor(pieceColor_.at(pieceFlag_.at(cur)));
        }
    }
    this->update(this->boundingRect());
}

QRectF PuzzleShader::boundingRect() const
{
    return QRectF(0, 0, width_+1, height_+1);
}

void PuzzleShader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    for(int i=0; i<nHeight_; ++i)
    {
        for(int j=0; j<nWidth_; ++j)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(land_[i][j]->color());
            painter->drawRect(j*pieceLength_, i*pieceLength_, pieceLength_, pieceLength_);
//            auto c = land_[i][j]->color();
//            qDebug("updateLand [%d, %d] with color[%d, %d, %d]", i, j, c.red(), c.green(), c.blue());
        }
    }
}

void PuzzleShader::reset()
{
    for(int i=0; i<nHeight_; ++i)
    {
        for(int j=0; j<nWidth_; ++j)
        {
            if(landMatrix_[i][j]==0)
            {
                land_[i][j]->setColor(Qt::gray);
            }
            else
            {
                land_[i][j]->setColor(Qt::white);
            }
        }
    }
}

void PuzzleShader::updateLand(const vector<vector<int> > &shape, QPoint pos, QColor c)
{
    auto const H = shape.size(), W = shape.at(0).size();

    for(size_t i=0; i<H; ++i)
    {
        for(size_t j=0; j<W; ++j)
        {
            if(shape[i][j]==1){
//                qDebug("updateLand [%d, %d] with color[%d, %d, %d]", i+pos.x(), j+pos.y(), c.red(), c.green(), c.blue());
                land_[i+pos.x()][j+pos.y()]->setColor(c);
            }
        }
    }
}
