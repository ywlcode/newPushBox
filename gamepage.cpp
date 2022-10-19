#include "gamepage.h"
#include "ui_gamepage.h"
#include "leveldata.h"
#include "beginpage.h"
#include <QIcon>
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QScreen>

GamePage::GamePage(QString user, int num, int style, bool sound, bool tip, QWidget *parent) : QWidget(parent),
                                                                                              ui(new Ui::GamePage)
{
    ui->setupUi(this);
    //初始化变量
    this->username = user;
    this->level_num = num;
    this->style = style;
    this->is_sound = sound;
    this->is_tip = tip;
    this->screen = QGuiApplication::primaryScreen()->size();
    QSqlQuery query;
    query.exec("select * from account");
    while (query.next())
    {
        if (query.value(0).toString() == username)
        {
            levelest_num = query.value(2).toInt();
        }
    }
    //界面设置
    setFixedSize(1600, 900);
    setWindowTitle("推箱子");
    setWindowIcon(QIcon(":/res/pic/front.png"));
    setWindowFlags(Qt::FramelessWindowHint);
    //贴背景
    QPalette palette;
    QPixmap background;
    switch (style)
    {
    case 1:
        background = QPixmap(":/res/pic/backin-1.png");
        break;
    case 2:
        background = QPixmap(":/res/pic/backin-2.png");
        break;
    case 3:
        background = QPixmap(":/res/pic/backin-3.png");
        break;
    case 4:
        background = QPixmap(":/res/pic/backin-4.png");
        break;
    }
    background = background.scaled(screen.width(), screen.height());
    palette.setBrush(this->backgroundRole(), QBrush(background));
    this->setPalette(palette);
    //计时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]()
            {
		time_used++;
		update(); });
    timer->start(1000);

    initData();
}

GamePage::~GamePage()
{
    delete ui;
}

//初始化数据
void GamePage::initData()
{
    LevelData leveldata(level_num);
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            chessboard[i][j] = leveldata.m[i][j];
            if (chessboard[i][j] == 4)
            {
                pos.setX(j);
                pos.setY(i);
            }
        }
    }
    move_times = 0;
    towards = 1;
    time_used = 0;
    score = 0;
    is_lose = false;
}
//绘图事件
void GamePage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = screen.height() / 15;
    int H = 0;
    int W = (screen.width() - screen.height()) / 2;
    QPixmap block(":/res/pic/frame.png");
    block = block.scaled(d, d);
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            painter.drawPixmap(i * d + W, j * d + H, block);
        }
    }
    QPixmap wall(":/res/pic/wall.png");
    wall = wall.scaled(d, d);
    QPixmap box(":/res/pic/box.png");
    box = box.scaled(d, d);
    QPixmap dot;
    switch (style)
    {
    case 1:
        dot = QPixmap(":/res/pic/dot-1.png");
        painter.setPen(QColor("#ffffff"));
        break;
    case 2:
        dot = QPixmap(":/res/pic/dot-2.png");
        painter.setPen(QColor("#ffffff"));
        break;
    case 3:
        dot = QPixmap(":/res/pic/dot-3.png");
        painter.setPen(QColor("#ffffff"));
        break;
    case 4:
        dot = QPixmap(":/res/pic/dot-4.png");
        painter.setPen(QColor("#808080"));
        break;
    }
    dot = dot.scaled(d, d);
    QPixmap player;
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (chessboard[i][j] == 1)
            {
                painter.drawPixmap(j * d + W, i * d + H, wall);
            }
            else if (chessboard[i][j] == 2)
            {
                painter.drawPixmap(j * d + W, i * d + H, dot);
            }
            else if (chessboard[i][j] == 3 || chessboard[i][j] == 5)
            {
                painter.drawPixmap(j * d + W, i * d + H, box);
            }
            else if (chessboard[i][j] == 4 || chessboard[i][j] == 6)
            {
                switch (towards)
                {
                case 1:
                    player.load(":/res/pic/front.png");
                    break;
                case 2:
                    player.load(":/res/pic/back.png");
                    break;
                case 3:
                    player.load(":/res/pic/right.png");
                    break;
                case 4:
                    player.load(":/res/pic/left.png");
                    break;
                }
                player = player.scaled(d, d);
                painter.drawPixmap(j * d + W, i * d + H, player);
            }
        }
    }
    painter.setFont(QFont("幼圆", 14));
    painter.drawText(QPoint(screen.width() - 250, screen.height() - 50), QString("用时: " + QString::number(time_used)));
    painter.drawText(QPoint(screen.width() - 250, screen.height() - 100), QString("步数: " + QString::number(move_times)));
    if (is_lose)
    {
        QRect rect[4];
        for (int i = 0; i < 4; i++)
        {
            rect[i] = QRect((i + 2) * d * 2 + W, 0, d, d);
        }
        painter.setPen(Qt::red);
        painter.setFont(QFont("幼圆", 18));
        painter.drawText(rect[0], Qt::AlignCenter, "游");
        painter.drawText(rect[1], Qt::AlignCenter, "戏");
        painter.drawText(rect[2], Qt::AlignCenter, "失");
        painter.drawText(rect[3], Qt::AlignCenter, "败");
    }
}
//键盘事件
void GamePage::keyPressEvent(QKeyEvent *event)
{
    int i = pos.x();
    int j = pos.y();
    QPoint p1, p2;
    switch (event->key())
    {
    case Qt::Key_Down:
        towards = 1;
        p1 = QPoint(i, j + 1);
        p2 = QPoint(i, j + 2);
        break;
    case Qt::Key_Up:
        towards = 2;
        p1 = QPoint(i, j - 1);
        p2 = QPoint(i, j - 2);
        break;
    case Qt::Key_Right:
        towards = 3;
        p1 = QPoint(i + 1, j);
        p2 = QPoint(i + 2, j);
        break;
    case Qt::Key_Left:
        towards = 4;
        p1 = QPoint(i - 1, j);
        p2 = QPoint(i - 2, j);
        break;
    case Qt::Key_P:
        initData();
        update();
        return;
    case Qt::Key_Escape:
        emit this->toBeginPage(this->levelest_num);
        return;
    default:
        return;
    }
    tryMove(p1, p2);
    update();
}
//尝试移动
void GamePage::tryMove(QPoint p1, QPoint p2)
{
    if (chessboard[p1.y()][p1.x()] == 0 || chessboard[p1.y()][p1.x()] == 2)
    {
        chessboard[pos.y()][pos.x()] -= 4;
        chessboard[p1.y()][p1.x()] += 4;
        pos.setX(p1.x());
        pos.setY(p1.y());
        move_times++;
        if (this->is_sound)
        {
            QSoundEffect *effect = new QSoundEffect;
            effect->setSource(QUrl("qrc:/res/dio/move.wav"));
            effect->setLoopCount(1); //循环次数
            effect->setVolume(0.1);  //音量  0~1之间
            effect->play();
        }
    }
    else if (chessboard[p1.y()][p1.x()] == 3 || chessboard[p1.y()][p1.x()] == 5)
    {
        if (chessboard[p2.y()][p2.x()] == 0 || chessboard[p2.y()][p2.x()] == 2)
        {
            chessboard[pos.y()][pos.x()] -= 4;
            chessboard[p1.y()][p1.x()] += 1;
            chessboard[p2.y()][p2.x()] += 3;
            pos.setX(p1.x());
            pos.setY(p1.y());
            move_times++;
            if (this->is_sound)
            {
                QSoundEffect *effect = new QSoundEffect;
                effect->setSource(QUrl("qrc:/res/dio/move.wav"));
                effect->setLoopCount(1); //循环次数
                effect->setVolume(0.1);  //音量  0~1之间
                effect->play();
            }
            isWin();
            isLose();
        }
    }
}
//判断胜利
void GamePage::isWin()
{
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (chessboard[i][j] == 2 || chessboard[i][j] == 6)
            {
                return;
            }
        }
    }
    score = 100 - time_used;
    if (score < 10)
    {
        score = 10;
    }
    if (this->is_sound)
    {
        QSoundEffect *effect = new QSoundEffect;
        effect->setSource(QUrl("qrc:/res/dio/win.wav"));
        effect->setLoopCount(1); //循环次数
        effect->setVolume(0.1);  //音量  0~1之间
        effect->play();
    }
    //提示框
    QMessageBox msgbox(QMessageBox::Information, "恭喜过关", "得分：" + QString::number(score) + "    ", QMessageBox::Ok, this);
    msgbox.button(QMessageBox::Ok)->setText("下一关");
    msgbox.exec();
    //修改本局分数
    QSqlQuery query;
    query.exec("select * from account");
    while (query.next())
    {
        if (query.value(0).toString() == username)
        {
            if (query.value(level_num + 2) < score)
            {
                switch (level_num)
                {
                case 1:
                    query.prepare("update account set l1=? where username=?");
                    break;
                case 2:
                    query.prepare("update account set l2=? where username=?");
                    break;
                case 3:
                    query.prepare("update account set l3=? where username=?");
                    break;
                case 4:
                    query.prepare("update account set l4=? where username=?");
                    break;
                case 5:
                    query.prepare("update account set l5=? where username=?");
                    break;
                case 6:
                    query.prepare("update account set l6=? where username=?");
                    break;
                case 7:
                    query.prepare("update account set l7=? where username=?");
                    break;
                case 8:
                    query.prepare("update account set l8=? where username=?");
                    break;
                case 9:
                    query.prepare("update account set l9=? where username=?");
                    break;
                case 10:
                    query.prepare("update account set l10=? where username=?");
                    break;
                case 11:
                    query.prepare("update account set l11=? where username=?");
                    break;
                case 12:
                    query.prepare("update account set l12=? where username=?");
                    break;
                case 13:
                    query.prepare("update account set l13=? where username=?");
                    break;
                case 14:
                    query.prepare("update account set l14=? where username=?");
                    break;
                case 15:
                    query.prepare("update account set l15=? where username=?");
                    break;
                }
                query.addBindValue(score);
                query.addBindValue(username);
                query.exec();
            }
        }
    }
    //如果通关
    if (level_num == 15)
    {
        emit toBeginPage(level_num);
    }
    //如果通过了账号最大关卡，修改最大关卡数
    else if (level_num == levelest_num)
    {
        levelest_num++;
        query.prepare("update account set level=? where username=?");
        query.addBindValue(levelest_num);
        query.addBindValue(username);
        query.exec();
    }
    //修改总分
    query.exec("select * from account");
    while (query.next())
    {
        if (query.value(0).toString() == username)
        {
            int totle = query.value(18).toInt();
            int t = 0;
            for (int i = 3; i < 18; i++)
            {
                t += query.value(i).toInt();
            }
            if (t > totle)
            {
                query.prepare("update account set score=? where username=?");
                query.addBindValue(t);
                query.addBindValue(username);
                query.exec();
            }
        }
    }
    if (level_num != -1)
        level_num++;
    initData();
}
//判断失败
void GamePage::isLose()
{
    if (is_tip == false || is_lose == true)
    {
        return;
    }
    for (int i = 0; i < MAXN; i++)
    {
        for (int j = 0; j < MAXN; j++)
        {
            if (chessboard[i][j] == 3)
            {
                bool lr = false, fb = false;
                if (chessboard[i][j + 1] == 1 || chessboard[i][j + 1] == -1)
                    lr = true;
                if (chessboard[i][j - 1] == 1 || chessboard[i][j - 1] == -1)
                    lr = true;
                if (chessboard[i + 1][j] == 1 || chessboard[i + 1][j] == -1)
                    fb = true;
                if (chessboard[i - 1][j] == 1 || chessboard[i - 1][j] == -1)
                    fb = true;
                if (lr && fb)
                {
                    is_lose = true;
                    if (this->is_sound)
                    {
                        QSoundEffect *effect = new QSoundEffect;
                        effect->setSource(QUrl("qrc:/res/dio/defeat.wav"));
                        effect->setLoopCount(1); //循环次数
                        effect->setVolume(0.1);  //音量  0~1之间
                        effect->play();
                    }
                }
            }
        }
    }
}
