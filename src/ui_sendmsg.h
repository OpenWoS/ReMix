/********************************************************************************
** Form generated from reading UI file 'sendmsg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDMSG_H
#define UI_SENDMSG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SendMsg
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox;
    QPushButton *sendMsg;
    QTextEdit *msgEditor;

    void setupUi(QDialog *SendMsg)
    {
        if (SendMsg->objectName().isEmpty())
            SendMsg->setObjectName(QStringLiteral("SendMsg"));
        SendMsg->resize(330, 200);
        gridLayout = new QGridLayout(SendMsg);
        gridLayout->setSpacing(5);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        frame = new QFrame(SendMsg);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(5);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(5, 5, 5, 5);
        checkBox = new QCheckBox(frame);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout_2->addWidget(checkBox, 2, 0, 1, 1);

        sendMsg = new QPushButton(frame);
        sendMsg->setObjectName(QStringLiteral("sendMsg"));
        sendMsg->setFocusPolicy(Qt::ClickFocus);

        gridLayout_2->addWidget(sendMsg, 2, 1, 1, 1);

        msgEditor = new QTextEdit(frame);
        msgEditor->setObjectName(QStringLiteral("msgEditor"));
        msgEditor->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(msgEditor, 0, 0, 2, 2);


        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(SendMsg);

        QMetaObject::connectSlotsByName(SendMsg);
    } // setupUi

    void retranslateUi(QDialog *SendMsg)
    {
        SendMsg->setWindowTitle(QApplication::translate("SendMsg", "Admin Message:", Q_NULLPTR));
        checkBox->setText(QApplication::translate("SendMsg", "Send to Everyone", Q_NULLPTR));
        sendMsg->setText(QApplication::translate("SendMsg", "Send Message", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SendMsg: public Ui_SendMsg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDMSG_H
