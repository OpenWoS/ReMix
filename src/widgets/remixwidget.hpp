#ifndef REMIXWIDGET_HPP
#define REMIXWIDGET_HPP

#include "prototypes.hpp"

//Required Qt Includes..
#include <QModelIndex>
#include <QWidget>

namespace Ui {
    class ReMixWidget;
}

class ReMixWidget : public QWidget
{
    Q_OBJECT

    MessagesWidget* messages{ nullptr };
    RulesWidget* rules{ nullptr };

    PlrListWidget* plrWidget{ nullptr };

    Server* tcpServer{ nullptr };
    RandDev* randDev{ nullptr };
    Settings* settings{ nullptr };

    ServerInfo* server{ nullptr };
    User* user{ nullptr };

    QPalette defaultPalette;
    bool nightMode{ false };

    QString serverID{ "" };

    enum CMDLArgs{ GAME = 0, MASTER = 1, PUBLIC = 2, LISTEN = 3, NAME = 4,
                   FUDGE = 5, RELOAD = 6 };
    enum Themes{ LIGHT = 0, DARK = 1 };
    static const QStringList cmdlArgs;

    public:
        explicit ReMixWidget(QWidget* parent = nullptr, User* usr = nullptr,
                             QStringList* argList = nullptr,
                             QString svrID = "0" );
        ~ReMixWidget();

        void sendServerMessage(QString msg, Player* plr, bool toAll);
        qint32 getPlayerCount();
        QString getServerName() const;

        Settings* getSettings() const;
        Server* getTcpServer() const;

        QString& getServerID();

    private:
        void parseCMDLArgs(QStringList* argList);
        void initUIUpdate();

    private slots:
        void on_openSettings_clicked();
        void on_openUserComments_clicked();
        void on_enableNetworking_clicked();
        void on_openUserInfo_clicked();
        void on_isPublicServer_stateChanged(int arg1);

        void on_serverPort_textChanged(const QString &arg1);
        void on_serverName_textChanged(const QString &arg1);

    signals:
        void serverNameChanged(const QString& name);
        void reloadOldServersSignal();

    private:
        Ui::ReMixWidget *ui;
};

#endif // REMIXWIDGET_HPP
