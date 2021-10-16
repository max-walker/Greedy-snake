#include "Greedy_snake.h"

#include <QPointF>

Greedy_snake::Greedy_snake(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->resize(480, 500);

    setStyleSheet("QWidget{background:black}");
    setWindowOpacity(0.8);

    snake.append(QRectF(200, 500, snakeNodeWidth, snakeNodeHeight));
    addTopRectF();

    rewardNode.append(QRectF(100, 100, snakeNodeWidth, snakeNodeWidth));
    timer = new QTimer(this);
	timer->start(snakeTime);
    connect(timer, &QTimer::timeout, this, &Greedy_snake::timeOut);

    rewardTimer = new QTimer(this);
	rewardTimer->start(rewardTime);
    connect(rewardTimer, &QTimer::timeout, this, &Greedy_snake::rewardTimeOut);
}

//界面刷新
void Greedy_snake::timeOut()
{
	int flage = 1;
	for (int i = 0; i < rewardNode.size(); i++)
	{
		if (rewardNode.at(i).contains(snake.at(0).topLeft() + QPointF(snakeNodeWidth / 2, snakeNodeHeight / 2))) 
		{
			if (rewardNode.at(i).width() > snakeNodeWidth) 
			{//额外奖励
				flage += 2;
			}
			flage++;//正常奖励
			rewardNode.removeAt(i);
			break;
		}
	}

	//这里flag如果只是1，就正常移动，因为每次移动都会删除最后一个点。
	//如果吃到奖励就会一直移动的时候，最后一个位置会添加新的点
	while (flage--)
	{
		switch (moveFlag)
		{
		case Greedy_snake::Move::Left:
			addLeftRectF();
			break;
		case Greedy_snake::Move::Right:
			addRightRectF();
			break;
		case Greedy_snake::Move::Up:
			addTopRectF();
			break;
		case Greedy_snake::Move::Down:
			addDownRectF();
			break;
		default:
			break;
		}
	}
	deleteLastRectF();
	update();
}

void Greedy_snake::rewardTimeOut()
{
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	if (rewardNode.size() > 5) {
		rewardNode.removeAt(qrand() % 5);
	}
	rewardNode.append(QRectF(qrand() % (this->width() / 20) * 20, qrand() % (this->height() / 20) * 20, snakeNodeWidth, snakeNodeWidth));
	if (qrand() % 5 == 3) {
		rewardNode.append(QRectF(qrand() % (this->width() / 20) * 20 - 5, qrand() % (this->height() / 20) * 20 - 5, snakeNodeWidth * 2, snakeNodeWidth * 2));
	}
	update();
}


//判断蛇头和蛇身有没有相撞
bool Greedy_snake::snakeStrike()
{
	for (int i = 0; i < snake.size(); i++) {
		for (int j = i + 1; j < snake.size(); j++) {
			if (snake.at(i) == snake.at(j)) {
				return true;
			}
		}
	}
	return false;
}

void Greedy_snake::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen;
	QBrush brush;
	QFont font("方正舒体", 12, QFont::ExtraLight, false);

	//反锯齿
	painter.setRenderHint(QPainter::Antialiasing);

	pen.setColor(Qt::black);
	brush.setColor(Qt::green);
	brush.setStyle(Qt::SolidPattern);
	painter.setPen(pen);
	painter.setBrush(brush);

	for (int i = 0; i < snake.size(); i++) {
		painter.drawRect(snake.at(i));
	}
	brush.setColor(Qt::red);
	painter.setBrush(brush);
	for (int i = 0; i < rewardNode.size(); i++) {
		painter.drawEllipse(rewardNode.at(i));
	}
	pen.setColor(Qt::white);
	painter.setPen(pen);
	painter.setFont(font);
	painter.drawText(20, 20, QString(QString::fromLocal8Bit("当前得分：")) + QString("%1").arg(snake.size()));

	if (snakeStrike()) {
		QFont font("方正舒体", 30, QFont::ExtraLight, false);
		painter.setFont(font);
		painter.drawText((this->width() - 300) / 2, (this->height() - 30) / 2, QString("GAME OVER!"));
		timer->stop();
		rewardTimer->stop();
		gameOver = true;
	}

	QWidget::paintEvent(event);
}

//按键
void Greedy_snake::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
		if (moveFlag != Move::Down) {
			moveFlag = Move::Up;
		}
		break;
	case Qt::Key_Down:
		if (moveFlag != Move::Up) {
			moveFlag = Move::Down;
		}
		break;
	case  Qt::Key_Right:
		if (moveFlag != Move::Left) {
			moveFlag = Move::Right;
		}
		break;
	case Qt::Key_Left:
		if (moveFlag != Move::Right) {
			moveFlag = Move::Left;
		}
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:
		if (gameOver) {
			snake.clear();
			rewardNode.clear();
			moveFlag = Move::Up;

			snake.append(QRectF(200, 500, snakeNodeWidth, snakeNodeHeight));
			addTopRectF();
			addTopRectF();

			rewardNode.append(QRectF(100, 100, snakeNodeWidth, snakeNodeHeight));
		}
	default:
		break;
	}
}

void Greedy_snake::addTopRectF()
{
	if (snake.at(0).y() - snakeNodeHeight < 0) 
	{
		snake.insert(0, QRectF(QPointF(snake.at(0).x(), this->height() - snakeNodeHeight),
			QPointF(snake.at(0).x() + snakeNodeWidth, this->height())));
	}
	else 
	{
		snake.insert(0, QRectF(snake.at(0).topLeft() + QPointF(0, -snakeNodeHeight), snake.at(0).topRight()));
	}
}
//向下移动
void Greedy_snake::addDownRectF()
{
	if (snake.at(0).y() + snakeNodeHeight * 2 > this->height()) 
	{
		snake.insert(0, QRectF(QPointF(snake.at(0).x(), snakeNodeHeight),
			QPointF(snake.at(0).x() + snakeNodeWidth, 0)));
	}
	else {
		snake.insert(0, QRectF(snake.at(0).bottomLeft(), snake.at(0).bottomRight() + QPointF(0, snakeNodeHeight)));
	}
}
//向左移动
void Greedy_snake::addLeftRectF()
{
	if (snake.at(0).x() - snakeNodeWidth < 0) 
	{
		snake.insert(0, QRectF(QPointF(this->width() - snakeNodeWidth, snake.at(0).y()),
			QPointF(this->width(), snake.at(0).y() + snakeNodeHeight)));
	}
	else 
	{
		snake.insert(0, QRectF(snake.at(0).topLeft() + QPointF(-snakeNodeWidth, 0), snake.at(0).bottomLeft()));
	}
}
//向右移动
void Greedy_snake::addRightRectF()
{
	if (snake.at(0).x() + snakeNodeWidth * 2 > this->width()) 
	{
		snake.insert(0, QRectF(QPointF(0, snake.at(0).y()),
			QPointF(snakeNodeWidth, snake.at(0).y() + snakeNodeHeight)));
	}
	else 
	{
		snake.insert(0, QRectF(snake.at(0).topRight(), snake.at(0).bottomRight() + QPointF(snakeNodeWidth, 0)));
	}
}
