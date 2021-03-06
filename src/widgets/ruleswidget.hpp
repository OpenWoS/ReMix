#ifndef RULESWIDGET_HPP
#define RULESWIDGET_HPP

#include "prototypes.hpp"

//Required Qt Includes..
#include <QWidget>
#include <QMutex>

namespace Ui {
    class RulesWidget;
}

class RulesWidget : public QWidget
{
    Q_OBJECT

//    enum Toggles{ svrPassword = 0, autoRestart = 1, world = 2, url = 3, allPK = 4, maxP = 5, maxIdle = 6,
//                  minV = 7, ladder = 8, noBleep = 9, noCheat = 10,
//                  noEavesdrop = 11, noMigrate = 12, noMod = 13, noPets = 14,
//                  noPK = 15, arenaPK = 16 };

    static QHash<ServerInfo*, RulesWidget*> ruleWidgets;

    SelectWorld* selectWorld{ nullptr };
    bool maxPlayersCheckState{ false };
    bool minVersionCheckState{ false };
    bool maxIdleCheckState{ false };
    bool worldCheckState{ false };
    bool urlCheckState{ false };
    bool pwdCheckState{ false };

    QString serverName{ "" };
    QString gameInfo{ "" };

    public:
        explicit RulesWidget();
        ~RulesWidget() override;

        static RulesWidget* getWidget(ServerInfo* server);
        static void deleteWidget(ServerInfo* server);

        void setServerName(const QString& name);
        void setCheckedState(const RToggles& option, const bool& val);
        bool getCheckedState(const RToggles& option);

        void setSelectedWorld(const QString& worldName, const bool& state);

        void setGameInfo(const QString& gInfo);
        const QString& getGameInfo() const;

    private:
        void toggleRulesModel(const RToggles& row);
        void toggleRules(const qint32& row, const Qt::CheckState& value);

    private slots:
        void on_rulesView_doubleClicked(const QModelIndex& index);
        void on_rulesView_itemClicked(QTableWidgetItem *item);

    signals:
        void gameInfoChangedSignal(const QString& gameInfo);
        void setMaxIdleTimeSignal();

    private:
        Ui::RulesWidget* ui;
        static QMutex mutex;
};

#endif // RULESWIDGET_HPP
