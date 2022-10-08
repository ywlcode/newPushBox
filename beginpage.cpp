#include "beginpage.h"
#include "ui_beginpage.h"
#include <QDir>
#include <QIcon>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QProcess>
#include <QMediaPlaylist>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

BeginPage::BeginPage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::BeginPage)
{
    ui->setupUi(this);
    bool dbFile = !QFile::exists(QDir::currentPath() + "/test.db");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString("test.db"));
    //连接数据库
    if (!db.open()) {
        QMessageBox::information(this, "infor", "link failed");
        qDebug() << "error open database because" << db.lastError().text();
    }
    if (dbFile)
    {
        QSqlQuery k1;
        QString create_sql = "create table account(username varchar(63), password varchar(255),level int,l1 int, l2 int, l3 int, l4 int, l5 int, l6 int, l7 int, l8 int, l9 int, l10 int, l11 int, l12 int, l13 int, l14 int, l15 int, score int);";
        k1.prepare(create_sql);
        if (!k1.exec())
        {
            QMessageBox::information(this, "infor", "Error: Fail to create table.");
        }
        else
        {
            QMessageBox::information(this, "infor", "Table created!");
        }
    }
    ui->label_user_name->setScaledContents(true); //图片自适应label大小
    ui->label_pwd->setScaledContents(true); //图片自适应label大小
    ui->lineE_pwd->setEchoMode(QLineEdit::Password); //密码框设置为小黑点

    connect(ui->btn_1, SIGNAL(clicked(bool)), this, SLOT(setStyle()));
    connect(ui->btn_2, SIGNAL(clicked(bool)), this, SLOT(setStyle()));
    connect(ui->btn_3, SIGNAL(clicked(bool)), this, SLOT(setStyle()));
    connect(ui->btn_4, SIGNAL(clicked(bool)), this, SLOT(setStyle()));
    connect(ui->btn_forget, &QPushButton::clicked, this, &BeginPage::btn_forget_clicked);
    connect(ui->btn_login, &QPushButton::clicked, this, &BeginPage::setLogin);
    connect(ui->btn_exit, &QPushButton::clicked, []() {
        QApplication* app;
        app->exit(0);
        });
    connect(ui->btn_mini, &QPushButton::clicked, [=]() {
        this->showMinimized();
        });

    setFixedSize(1600, 900);
    setWindowTitle("推箱子");
    setWindowIcon(QIcon(":/res/pic/front.png"));
    setWindowFlags(Qt::FramelessWindowHint);
    this->style = 1;
    this->isMusic = true;
    this->isSound = true;
    this->isCount = false;
    this->isCheckLose = true;
    btn_style = QString("QPushButton{color:#000000;border:none;background:#f1f1f1;border-radius:5px;}");
    this->btn_back = new QPushButton("返回", this);
    btn_back->setFont(QFont("幼圆", 14));
    btn_back->setGeometry(400, 660, 300, 50);
    btn_back->setStyleSheet(btn_style);
    btn_back->hide();
}

BeginPage::~BeginPage()
{
    delete ui;
}

//切换皮肤
void BeginPage::setStyle()
{
    QPushButton* btn;
    btn = qobject_cast<QPushButton*>(sender());//获取发射信号的对象
    QString filePath;
    if ("btn_1" == btn->objectName()) {
        filePath = ":/res/qss/style-1.qss";
        this->style = 1;
    }
    else if ("btn_2" == btn->objectName()) {
        filePath = ":/res/qss/style-2.qss";
        this->style = 2;
    }
    else if ("btn_3" == btn->objectName()) {
        filePath = ":/res/qss/style-3.qss";
        this->style = 3;
    }
    else if ("btn_4" == btn->objectName()) {
        filePath = ":/res/qss/style-4.qss";
        this->style = 4;
    }
    QFile file(filePath); //QSS文件所在的路径
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

//点击跳转到注册页面
void BeginPage::btn_forget_clicked()
{
    ui->btn_qq->hide();
    ui->btn_wx->hide();
    ui->btn_wb->hide();
    ui->lineE_pwd->clear();
    ui->lineE_user_name->clear();
    ui->btn_forget->setText("已有账号？返回登录");
    ui->btn_login->setText("注册");
    ui->btn_forget->disconnect();
    ui->btn_login->disconnect();
    connect(ui->btn_login, &QPushButton::clicked, this, &BeginPage::setRegister);
    connect(ui->btn_forget, &QPushButton::clicked, this, &BeginPage::btn_forget_clicked_2);
}
//点击返回到登录界面
void BeginPage::btn_forget_clicked_2()
{
    ui->btn_qq->show();
    ui->btn_wx->show();
    ui->btn_wb->show();
    ui->lineE_pwd->clear();
    ui->lineE_user_name->clear();
    ui->btn_forget->setText("没有账号？点此注册");
    ui->btn_login->setText("登录");
    ui->btn_forget->disconnect();
    ui->btn_login->disconnect();
    connect(ui->btn_forget, &QPushButton::clicked, this, &BeginPage::btn_forget_clicked);
    connect(ui->btn_login, &QPushButton::clicked, this, &BeginPage::setLogin);
}

//注册账号
void BeginPage::setRegister()
{
    QString username = ui->lineE_user_name->text();
    QString password = ui->lineE_pwd->text();
    if (username == "" || password == "") {
        return;
    }
    bool exist = false;
    QSqlQuery query;
    query.exec("select * from account");
    while (query.next()) {
        if (query.value(0).toString() == username) {
            QMessageBox msgbox(QMessageBox::Information, "提示", "用户名已存在  ", QMessageBox::Ok, this);
            msgbox.button(QMessageBox::Ok)->setText("确定");
            msgbox.exec();
            exist = true;
        }
    }
    if (!exist) {
        query.prepare("insert into account(username,password,level) values(:username,:password,:level)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":level", 1);
        query.exec();
        QMessageBox msgbox(QMessageBox::Information, "提示", "注册成功  ", QMessageBox::Ok, this);
        msgbox.button(QMessageBox::Ok)->setText("确定");
        msgbox.exec();
        btn_forget_clicked_2();
    }
}
//登录账号
void BeginPage::setLogin()
{
    QString username = ui->lineE_user_name->text();
    QString password = ui->lineE_pwd->text();
    bool correct = false;
    if (username == "" || password == "") {
        return;
    }
    QSqlQuery query;
    query.exec("select * from account");
    while (query.next()) {
        if (query.value(0).toString() == username && query.value(1).toString() == password) {
            correct = true;
            this->levelest_num = query.value(2).toInt();
        }
    }
    if (correct) {
        this->username = username;
        loginSuccess();
    }
    else {
        QMessageBox msgbox(QMessageBox::Information, "提示", "用户名或密码错误  ", QMessageBox::Ok, this);
        msgbox.button(QMessageBox::Ok)->setText("确定");
        msgbox.exec();
    }

}

//登录成功
void BeginPage::loginSuccess()
{
    //音乐
    back_music = new QSoundEffect;
    back_music->setSource(QUrl("qrc:/res/dio/back_music.wav"));
    back_music->setLoopCount(-1);  //循环次数
    back_music->setVolume(0.02); //音量  0~1之间
    back_music->play();
    //按钮
    begin_game = new QPushButton("开始游戏", this);
    begin_game->setGeometry(390, 360, 320, 50);
    begin_game->setStyleSheet(btn_style);
    begin_game->setFont(QFont("幼圆", 14));
    begin_game->show();
    leaderboard = new QPushButton("排行榜", this);
    leaderboard->setGeometry(390, 460, 320, 50);
    leaderboard->setStyleSheet(btn_style);
    leaderboard->setFont(QFont("幼圆", 14));
    leaderboard->show();
    game_set = new QPushButton("游戏设置", this);
    game_set->setGeometry(390, 560, 320, 50);
    game_set->setStyleSheet(btn_style);
    game_set->setFont(QFont("幼圆", 14));
    game_set->show();
    logout = new QPushButton("退出登录", this);
    logout->setGeometry(390, 660, 320, 50);
    logout->setStyleSheet(btn_style);
    logout->setFont(QFont("幼圆", 14));
    logout->show();
    connect(begin_game, &QPushButton::clicked, this, &BeginPage::chooseLevel);
    connect(leaderboard, &QPushButton::clicked, this, &BeginPage::seeLeader);
    connect(game_set, &QPushButton::clicked, this, &BeginPage::gameSet);
    connect(logout, &QPushButton::clicked, this, &BeginPage::setLogout);
    ui->lineE_pwd->hide();
    ui->lineE_user_name->hide();
    ui->label_user_name->hide();
    ui->label_pwd->hide();
    ui->btn_forget->hide();
    ui->btn_qq->hide();
    ui->btn_wx->hide();
    ui->btn_wb->hide();
    ui->frame_user_name->hide();
    ui->frame_pwd->hide();
    ui->btn_login->hide();
    begin_game->show();
    leaderboard->show();
}

//选择关卡
void BeginPage::chooseLevel()
{
    begin_game->hide();
    leaderboard->hide();
    game_set->hide();
    logout->hide();
    //关卡按钮
    QString opened_style = QString("QPushButton{color:#000000;border:none;background-color:#f4f4f4;"
        "border-radius: 5px;font-family: 幼圆;}");
    QString closed_style = QString("QPushButton{color: #000000;border: none;background-color: #747474;"
        "border-radius: 5px;font-family: 幼圆;}");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            int num = 5 * i + j;
            btn_level[num] = new QPushButton(QString::asprintf("%d", num + 1), this);
            btn_level[num]->setGeometry(360 + j * 80, 350 + i * 100, 50, 50);
            btn_level[num]->show();
            if (num + 1 <= levelest_num) {
                btn_level[num]->setStyleSheet(opened_style);
                connect(btn_level[num], &QPushButton::clicked, [=]() {
                    beginGame(username, num + 1);
                    });
            }
            else {
                btn_level[num]->setStyleSheet(closed_style);
            }
        }
    }
    //返回按钮
    this->btn_back->show();
    btn_back->disconnect();
    connect(btn_back, &QPushButton::clicked, [=]() {
        for (int i = 0; i < 15; i++) {
            this->btn_level[i]->hide();
        }
        begin_game->show();
        leaderboard->show();
        game_set->show();
        logout->show();
        btn_back->hide();
        });
}
//查看排行榜
void BeginPage::seeLeader()
{
    begin_game->hide();
    leaderboard->hide();
    game_set->hide();
    logout->hide();
    
    //数据库获取数据
    QSqlQuery query;
    query.exec("select * from account order by score desc");
    QLabel* title = new QLabel(" 玩家         分数 ", this);
    title->setFont(QFont("黑体", 13));
    title->setGeometry(390, 320, 320, 50);
    title->setAlignment(Qt::AlignCenter);
    title->show();
    QLabel* leaders[5];
    int i = 0;
    while (query.next()) {
        leaders[i] = new QLabel(this);
        leaders[i]->setFont(QFont("幼圆", 11));
        leaders[i]->setGeometry(390, 370 + 50 * i, 320, 50);
        leaders[i]->setText(QString(QString("%1").arg(query.value(0).toString(), -15 - query.value(0).toString().length() / 4, ' ')
            + QString("%1").arg(query.value(18).toString(), -3, ' ')));
        leaders[i]->setAlignment(Qt::AlignCenter);
        leaders[i]->show();
        i++;
        if (i >= 5) {
            break;
        }
    }
    //返回按钮
    this->btn_back->show();
    btn_back->disconnect();
    connect(btn_back, &QPushButton::clicked, [=]() {
        title->hide();
        for (int j = 0; j < i; j++) {
            leaders[j]->hide();
        }
        begin_game->show();
        leaderboard->show();
        game_set->show();
        logout->show();
        btn_back->hide();
     });
}
//进入游戏设置页面
void BeginPage::gameSet()
{
    begin_game->hide();
    leaderboard->hide();
    game_set->hide();
    logout->hide();
    this->btn_back->show();
    btn_back->disconnect();
    QPushButton* music = new QPushButton("音乐：开", this);
    QPushButton* sound = new QPushButton("音效：开", this);
    QPushButton* check_lose = new QPushButton("失败提示：开", this);
    if (!isMusic) {
        music->setText("音乐：关");
    }
    if (!isSound) {
        sound->setText("音效：关");
    }
    if (!isCheckLose) {
        check_lose->setText("失败提示：关");
    }
    music->setGeometry(390, 360, 320, 50);
    music->setStyleSheet(btn_style);
    music->setFont(QFont("幼圆", 14));
    music->show();
    sound->setGeometry(390, 460, 320, 50);
    sound->setStyleSheet(btn_style);
    sound->setFont(QFont("幼圆", 14));
    sound->show();
    check_lose->setGeometry(390, 560, 320, 50);
    check_lose->setStyleSheet(btn_style);
    check_lose->setFont(QFont("幼圆", 14));
    check_lose->show();
    connect(music, &QPushButton::clicked, [=]() {
        if (isMusic) {
            back_music->stop();
            music->setText("音乐：关");
        }
        else {
            back_music->play();
            music->setText("音乐：开");
        }
        isMusic = !isMusic;
        });
    connect(sound, &QPushButton::clicked, [=]() {
        if (isSound) {
            sound->setText("音效：关");
        }
        else {
            sound->setText("音效：开");
        }
        isSound = !isSound;
        });
    connect(check_lose, &QPushButton::clicked, [=]() {
        if (isCheckLose) {
            check_lose->setText("失败提示：关");
        }
        else {
            check_lose->setText("失败提示：开");
        }
        isCheckLose = !isCheckLose;
        });
    connect(btn_back, &QPushButton::clicked, [=]() {
        music->hide();
        sound->hide();
        check_lose->hide();
        btn_back->hide();
        begin_game->show();
        leaderboard->show();
        game_set->show();
        logout->show();
        });

}
//退出登录
void BeginPage::setLogout()
{
    qApp->closeAllWindows();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

//开始游戏
void BeginPage::beginGame(QString username, int num)
{
    g = new GamePage(username, num, style, isSound, isCheckLose);
    this->connect(g, &GamePage::toBeginPage, [=](int l) {
        this->levelest_num = l;
        for (int i = 0; i < 15; i++) {
            this->btn_level[i]->hide();
        }
        begin_game->show();
        leaderboard->show();
        game_set->show();
        logout->show();
        btn_back->hide();
        this->show();
        g->close();
        });
    g->showFullScreen();
    this->hide();
}
