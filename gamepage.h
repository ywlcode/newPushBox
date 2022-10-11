#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QKeyEvent>

#define MAXN 15

namespace Ui {
	class GamePage;
}

class GamePage : public QWidget
{
	Q_OBJECT

public:
	explicit GamePage(QString user, int num, int style, bool sound, bool tip, QWidget* parent = nullptr);
	~GamePage();

	void initData();
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
	void tryMove(QPoint p1, QPoint p2);
	void isWin();
	void isLose();

signals:
	void toBeginPage(int l);

private:
	Ui::GamePage* ui;
	QString username; //用户名
	int levelest_num; //用户达到的最大关卡
	int level_num; //处于第几关
	int chessboard[MAXN][MAXN]; //实时棋盘数据
	int move_times; //步数
	int time_used; //所用的时间
	int score; //得分
	QPoint pos; //人的坐标
	int towards; //人的朝向
	QSize screen;//屏幕尺寸
	bool is_lose;
	//设置选项
	int style;
	bool is_sound;
	bool is_tip;
};

#endif // GAMEPAGE_H
