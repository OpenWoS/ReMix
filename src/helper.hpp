
#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include "prototypes.hpp"

//Required Qt Includes.
#include <QInputDialog>
//#include <QtCore>

class Helper
{
    static const QList<qint32> blueCodedList;

    public:
        static QInputDialog* createInputDialog(QWidget* parent, QString& label, QInputDialog::InputMode mode, int width, int height);

        template<typename T>
        static QString intToStr(const T& val, const int& base = 10, const int& fill = 0, const QChar& filler = '0')
        {
            QString str{ "%1" };
                    str = str.arg( val, fill, base, filler ).toUpper();
            if ( str.length() > 8 )
                str = str.mid( str.length() - 8 );
            return str;
        }
        static qint32 strToInt(const QString& str, const int& base = 16);
        static QString intSToStr(QString& val, int base = 16, int fill = 0, QChar filler = '0');
        static QString getStrStr(const QString& str, const QString& indStr, const QString& mid, const QString& left);

        static void stripNewlines(QString& string);
        static QString stripSerNumHeader(const QString& sernum);
        static QString sanitizeSerNum(const QString& value);
        static QString serNumToHexStr(QString sernum, int fillAmt = 8);
        static QString serNumToIntStr(const QString& sernum, const bool& isHex = false);
        static qint32 serNumtoInt(const QString& sernum, const bool& isHex = false);

        static bool isBlueCodedSerNum(const qint32& sernum);

        static bool confirmAction(QWidget* parent, QString& title, QString& prompt);
        static qint32 warningMessage(QWidget* parent, const QString& title, const QString& prompt);
        static QString getTextResponse(QWidget* parent, const QString& title, const QString& prompt, const QString& defaultInput, bool* ok,
                                       const MessageBox& type);

        static QString getDisconnectReason(QWidget* parent = nullptr);

        static QString hashPassword(QString& password);
        static QString genPwdSalt(const qint32& length = SALT_LENGTH);

        static bool validateSalt(QString& salt);

        static bool naturalSort(QString& left, QString& right, bool& result);
        static void delay(const qint32& time);

        static QHostAddress getPrivateIP();
        static void getSynRealData(ServerInfo* svr);

        static bool strStartsWithStr(const QString& strA, const QString& strB);
        static bool strContainsStr(const QString& strA, const QString& strB);
        static bool cmpStrings(const QString& strA, const QString& strB );
        static qint32 getStrIndex(const QString& strA, const QString& strB);
        static QString getTimeAsString(const quint64& time = 0);
        static QString getTimeFormat(const qint64& time);
        static qint64 getTimeIntFormat(const qint64& time, const TimeFormat& format);
};

#endif // PREFERENCES_HPP
