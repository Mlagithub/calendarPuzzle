#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGraphicsScene>
#include <QDate>

#include "puzzle.h"
#include "pieceshader.h"
#include "datastorge.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

    enum State{
        ToSolve = 1,
        Solving = 2,
        Solved = 3,
        SolveFinish = 4
    };

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

signals:
    void dateChanged(const int month, const int day, const int week);

private slots:
    void on_pushButton_solve_clicked();

    void on_horizontalSlider_result_valueChanged(int value);

    void on_horizontalSlider_result_sliderMoved(int position);

    void on_calendarWidget_selectionChanged();

    void handleSolvedSignal();

    bool getAnswerFromDB(const int month, const int day, const int week);


private:
    void logging(QString val);
    void showResult(const int n);
    void updateUI(const State state);
    bool isDataBaseSupport();

private:
    Ui::Form *ui;
    QGraphicsScene* scene_;
    Puzzle puzzle_;
    PuzzleShader* shader_;
    QDate date_;
    DataStorge db_;
    vector<shape> result_;

};

#endif // FORM_H
