
#ifndef REMIX_HPP
#define REMIX_HPP

#include "prototypes.hpp"

//Required Qt Includes..
#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
    class ReMix;
}

class ReMix : public QMainWindow
{
    Q_OBJECT

    static Settings* settings;
    static ReMix* instance;
    static User* user;

    QSystemTrayIcon* trayObject{ nullptr };
    ReMixTabWidget* serverUI{ nullptr };
    QMenu* trayMenu{ nullptr };
    QIcon trayIcon;

    bool hasSysTray{ false };
    bool exiting{ false };

    private:
    #if !defined( Q_OS_LINUX ) && !defined( Q_OS_OSX )
        void initSysTray();
    #endif
        void initUIUpdate();

    public:
        explicit ReMix(QWidget *parent = 0);
        ~ReMix();

        static ReMix* getInstance();
        static void setInstance(ReMix* value);

        static void updateTitleBars(ServerInfo* server);
        static Settings* getSettings();
        static User* getUser();

        static void getSynRealData(ServerInfo* svr);
        static QPalette& setupPalette(qint32 type);
        static QPalette& getPalette(qint32 type);

    private slots:
        //Handle Minimize events.
        #if !defined( Q_OS_LINUX ) && !defined( Q_OS_OSX )
            void changeEvent(QEvent* event);
        #endif

        void closeEvent(QCloseEvent* event);
        bool rejectCloseEvent();

    private:
        Ui::ReMix *ui;
};

#endif // REMIX_HPP
