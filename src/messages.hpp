#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <QDialog>
#include <QString>

namespace Ui {
    class Messages;
}

class Messages : public QDialog
{
    Q_OBJECT
    Ui::Messages *ui;

    public:
        explicit Messages(QWidget *parent = 0);
        ~Messages();

        QString getServerMOTD();
        QString getBanishMsg();

        bool isPasswordEnabled();
        bool cmpPassword(QString& value);

    private slots:
        void on_saveSettings_clicked();
        void on_reloadSettings_clicked();
};

#endif // MESSAGES_HPP
