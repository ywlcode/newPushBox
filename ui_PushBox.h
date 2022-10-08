/********************************************************************************
** Form generated from reading UI file 'PushBox.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUSHBOX_H
#define UI_PUSHBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PushBoxClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PushBoxClass)
    {
        if (PushBoxClass->objectName().isEmpty())
            PushBoxClass->setObjectName(QString::fromUtf8("PushBoxClass"));
        PushBoxClass->resize(600, 400);
        menuBar = new QMenuBar(PushBoxClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        PushBoxClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PushBoxClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PushBoxClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PushBoxClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        PushBoxClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PushBoxClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PushBoxClass->setStatusBar(statusBar);

        retranslateUi(PushBoxClass);

        QMetaObject::connectSlotsByName(PushBoxClass);
    } // setupUi

    void retranslateUi(QMainWindow *PushBoxClass)
    {
        PushBoxClass->setWindowTitle(QApplication::translate("PushBoxClass", "PushBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PushBoxClass: public Ui_PushBoxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHBOX_H
