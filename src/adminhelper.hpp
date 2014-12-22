
#ifndef ADMINHELPER_HPP
#define ADMINHELPER_HPP

#include "prototypes.hpp"

namespace AdminHelper
{
    enum Keys{ RANK = 0, HASH = 1, SALT = 2 };
    extern const QString adminKeys[ 3 ];
    extern const QStringList ranks;

    void setAdminData(const QString& key, const QString& subKey, QVariant& value);
    QVariant getAdminData(const QString& key, const QString& subKey);

    void setReqAdminAuth(QVariant& value);
    bool getReqAdminAuth();

    bool getIsRemoteAdmin(QString& serNum);
    bool cmpRemoteAdminPwd(QString& serNum, QVariant& value);

    qint32 getRemoteAdminRank(QString& sernum);
    void setRemoteAdminRank(QString& sernum, qint32 rank);

    qint32 changeRemoteAdminRank(QWidget* parent, QString& sernum);
    bool deleteRemoteAdmin(QWidget* parent, QString& sernum);
    bool createRemoteAdmin(QWidget* parent, QString& sernum);
}

#endif // ADMINHELPER_HPP
