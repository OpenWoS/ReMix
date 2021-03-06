#ifndef SELECTWORLD_H
#define SELECTWORLD_H

#include "prototypes.hpp"


//Required Qt Includes.
#include <QDialog>

namespace Ui {
    class SelectWorld;
}

class SelectWorld : public QDialog
{
        Q_OBJECT

        QStringListModel* worldModel{ nullptr };
        QString world{ "" };
        bool requireWorld{ false };

    public:
        explicit SelectWorld(QWidget* parent = nullptr);
        ~SelectWorld() override;

        QString& getSelectedWorld();
        void setRequireWorld(const bool& value);

    private slots:
        void on_worldViewer_activated(const QModelIndex& index);
        void on_worldViewer_clicked(const QModelIndex& index);
        void on_worldViewer_entered(const QModelIndex& index);
        void on_cancelButton_clicked();
        void on_okButton_clicked();

    private:
        Ui::SelectWorld* ui;
};

#endif // SELECTWORLD_H
