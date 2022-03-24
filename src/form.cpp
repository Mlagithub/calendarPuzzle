#include <QGraphicsRectItem>
#include <iostream>
#include <QDebug>
#include <QDate>

#include "pieceshader.h"
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) : QWidget(parent),
                              ui(new Ui::Form)
{
    ui->setupUi(this);

    connect(&puzzle_, &Puzzle::solveFinisthed, this, &Form::handleSolvedSignal);
    connect(&puzzle_, &Puzzle::solving, this, &Form::logging);
    connect(this, &Form::dateChanged, this, &Form::getAnswerFromDB);
    connect(&db_, &DataStorge::endUpdateDB, this, &Form::handleUpdateDB);

    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    shader_ = new PuzzleShader(puzzle_.board());
    scene_->addItem(shader_);

    date_ = ui->calendarWidget->selectedDate();
    if (this->getAnswerFromDB(date_.month(), date_.day(), date_.dayOfWeek()))
    {
        this->updateUI(State::Solved);
        this->showResult(1);
    }
    else
    {
        this->updateUI(State::ToSolve);
        puzzle_.setDate(static_cast<Board::Month>(date_.month()), date_.day(), static_cast<Board::Week>(date_.dayOfWeek()));
    }
}

Form::~Form()
{
    delete ui;
}

void Form::showResult(const int n)
{
    auto nResult_ = result_.size();
    shader_->render(result_[n]);

    ui->horizontalSlider_result->setMaximum(nResult_);
    ui->label_result->setText(QString("第 %1 种方法，共 %2 种").arg(n).arg(nResult_));
}

void Form::updateUI(const State state)
{
    switch (state)
    {
    case State::ToSolve:
        ui->calendarWidget->setEnabled(true);
        ui->horizontalSlider_result->setValue(0);
        ui->horizontalSlider_result->setEnabled(true);
        ui->label_result->setText("");
        ui->pushButton_solve->setEnabled(true);
        shader_->reset();
        break;
    case State::Solving:
        ui->calendarWidget->setEnabled(false);
        ui->horizontalSlider_result->setEnabled(true);
        ui->pushButton_solve->setEnabled(false);
        break;
    case State::SolveFinish:
        ui->calendarWidget->setEnabled(true);
        ui->pushButton_solve->setEnabled(false);
        ui->horizontalSlider_result->setMaximum(result_.size());
        ui->horizontalSlider_result->setEnabled(true);
        break;
    case State::Solved:
        ui->calendarWidget->setEnabled(true);
        ui->pushButton_solve->setEnabled(false);
        ui->horizontalSlider_result->setValue(0);
        ui->horizontalSlider_result->setMaximum(result_.size());
        ui->horizontalSlider_result->setEnabled(true);
        break;
    default:
        break;
    }
}

bool Form::isDataBaseSupport()
{
    return db_.good();
}

void Form::on_pushButton_solve_clicked()
{
    if (puzzle_.isRunning())
    {
        puzzle_.wait();
    }

    vector<shape>{}.swap(result_);
    this->updateUI(State::Solving);

    shader_->reset();
    puzzle_.setDate(static_cast<Board::Month>(date_.month()), date_.day(), static_cast<Board::Week>(date_.dayOfWeek()));
    puzzle_.start();
}

void Form::on_horizontalSlider_result_valueChanged(int value)
{
    auto nResult_ = result_.size();
    if (nResult_ > 0 && nResult_ >= value)
    {
        shader_->render(result_[value - 1]);
    }
}

void Form::on_horizontalSlider_result_sliderMoved(int position)
{
    auto nResult_ = result_.size();
    if (nResult_ > 0)
    {
        ui->label_result->setText(QString("第 %1 种方法，共 %2 种").arg(position).arg(nResult_));
    }
}

void Form::logging(QString val)
{
    ui->plainTextEdit_log->appendPlainText(val);
}

void Form::on_calendarWidget_selectionChanged()
{
    date_ = ui->calendarWidget->selectedDate();

    if (this->getAnswerFromDB(date_.month(), date_.day(), date_.dayOfWeek()))
    {
        this->updateUI(State::Solved);
        qDebug(QString("method number of today is %1").arg(result_.size()).toStdString().c_str());
        this->showResult(1);
    }
    else
    {
        this->updateUI(State::ToSolve);
    }
}

void Form::handleSolvedSignal()
{
    if (!isDataBaseSupport())
        return;

    auto pdt = puzzle_.date();
    result_ = puzzle_.result();
    db_.insert(pdt.month, pdt.day, pdt.week, result_);
    if(db_.isRunning()) db_.wait();
    db_.start();
    ui->plainTextEdit_log->appendPlainText("Insert answer into database...");
}

bool Form::getAnswerFromDB(const int month, const int day, const int week)
{
    if (!isDataBaseSupport())
        return false;

    // cur date has been solved
    if (db_.methodNumberOfDate(month, day, week) > 0)
    {
        result_ = db_.get(month, day, week);
        return result_.size() > 0;
    }
    else
    {
        return false;
    }

    return true;
}

void Form::handleUpdateDB()
{
    ui->plainTextEdit_log->appendPlainText("Insert answer finished");
    this->showResult(1);
    this->updateUI(State::SolveFinish);
}
