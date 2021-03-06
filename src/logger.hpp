#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "prototypes.hpp"

//Required Qt Includes.
#include <QGraphicsPixmapItem>
#include <QDateTime>
#include <QCollator>
#include <QDialog>
#include <QTimer>
#include <QFile>
#include <QMap>

namespace Ui{
    class Logger;
}

class Logger : public QDialog
{
    Q_OBJECT

    WriteThread* writeThread{ nullptr };
    QThread* thread{ nullptr };

    static const QStringList logType;
    static const QString website;
    static QStandardItemModel* tblModel;
    static Logger* logInstance;

    QGraphicsPixmapItem* iconViewerItem{ nullptr };
    QGraphicsScene* iconViewerScene{ nullptr };

    public:
        explicit Logger(QWidget *parent = nullptr);
        ~Logger() override;

        static Logger* getInstance();
        static void setInstance(Logger* logger = nullptr);

        void scrollToBottom();
        void insertLog(const QString& source, const QString& message, const LogTypes& type, const bool& logToFile, const bool& newLine);
        void updateRowData(const qint32& row, const qint32& col, const QVariant& data);

    public slots:
        void insertLogSlot(const QString& source, const QString& message, const LogTypes& type, const bool& logToFile, const bool& newLine);

    private slots:
        void on_websiteLabel_linkActivated(const QString&);
        void on_autoScroll_clicked();
        void resizeColumnsSlot(const LogCols& column);

    signals:
        void insertLogSignal(const LogTypes& type, const QString& text, const QString& timeStamp, const bool& newLine);
        void resizeColumnsSignal(const LogCols& column);

    private:
        Ui::Logger *ui;
};

#endif // LOGGER_HPP
