#pragma once

#include <QtWidgets/QWidget>
#include "ui_Greedy_snake.h"

#include <QkeyEvent>
#include <QRectF>
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QPen>

class Greedy_snake : public QWidget
{
	Q_OBJECT

public:
	explicit Greedy_snake(QWidget* parent = nullptr);
	~Greedy_snake() {};

protected:
	void paintEvent(QPaintEvent*event) override;
	void keyPressEvent(QKeyEvent* event) override;
private:
	void addTopRectF();
	void addDownRectF();
	void addLeftRectF();
	void addRightRectF();
	void deleteLastRectF() { snake.removeLast(); }
	bool snakeStrike();
	enum class Move
	{
		Left,
		Right,
		Up,
		Down
	};
	
	Ui::Greedy_snakeClass ui;
	QList<QRectF> snake;
	int snakeNodeWidth = 10;
	int snakeNodeHeight = 10;
	QTimer* timer;
	QTimer* rewardTimer;
	int snakeTime = 500;
	int rewardTime = 3000;
	bool gameOver = false;
	Move moveFlag = Move::Up;
	QList<QRectF> rewardNode;  //½±Àø½Úµã

protected slots:
	void timeOut();
	void rewardTimeOut();
};
