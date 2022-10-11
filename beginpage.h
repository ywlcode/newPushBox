#ifndef BEGINPAGE_H
#define BEGINPAGE_H

#include <QWidget>
#include "gamepage.h"
#include <QPushButton>
#include <QSoundEffect>
#include <QSound>

namespace Ui {
	class BeginPage;
}

class BeginPage : public QWidget
{
	Q_OBJECT

public:
	explicit BeginPage(QWidget* parent = nullptr);
	~BeginPage();

public slots:
	void setStyle(); //设置皮肤
	void btn_forget_clicked(); //切换登录注册
	void btn_forget_clicked_2(); //切换登录注册
	void setRegister(); //注册
	void setLogin(); //登录
	void chooseLevel(); //进入选择关卡界面
	void seeLeader(); //查看排行榜
	void gameSet(); //进入游戏设置页面
	void setLogout(); //退出登录
	void beginGame(QString username, int num);

private:
	Ui::BeginPage* ui;
	GamePage* g;
	int style;
	QPushButton* begin_game;
	QPushButton* leaderboard;
	QPushButton* game_set;
	QPushButton* logout;
	QPushButton* btn_level[15];
	QPushButton* btn_back;
	QString btn_style;
	QSoundEffect* back_music;
	QString username; //用户名
	int levelest_num; //账号所处最大关卡
	void loginSuccess(); //登录成功
	//游戏设置
	bool isMusic;
	bool isSound;
	bool isCheckLose;
	bool isCount;
	int time_count;
};

#endif // BEGINPAGE_H
