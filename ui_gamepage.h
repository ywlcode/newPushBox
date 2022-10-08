/********************************************************************************
** Form generated from reading UI file 'gamepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEPAGE_H
#define UI_GAMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GamePage
{
public:

    void setupUi(QWidget *GamePage)
    {
        if (GamePage->objectName().isEmpty())
            GamePage->setObjectName(QString::fromUtf8("GamePage"));
        GamePage->resize(1600, 900);
        GamePage->setFocusPolicy(Qt::StrongFocus);
        GamePage->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(GamePage);

        QMetaObject::connectSlotsByName(GamePage);
    } // setupUi

    void retranslateUi(QWidget *GamePage)
    {
        GamePage->setWindowTitle(QApplication::translate("GamePage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GamePage: public Ui_GamePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEPAGE_H
