
//Class includes.
#include "helper.hpp"

//ReMix includes.
#include "serverinfo.hpp"
#include "settings.hpp"
#include "randdev.hpp"
#include "user.hpp"

//Qt Includes.
#include <QNetworkInterface>
#include <QInputDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>
#include <QtCore>

const QString Helper::logType[ LOG_TYPE_COUNT ] =
{
    "AdminUsage.txt",
    "Comments.txt",
    "UsageLog.txt",
    "UPNPLog.txt",
    "BanLog.txt",
    "DCLog.txt",
    "MuteLog.txt",
    "Ignored.txt",
};

QInputDialog* Helper::createInputDialog(QWidget* parent, QString& label,
                                        QInputDialog::InputMode mode,
                                        int width, int height)
{
    QInputDialog* dialog = new QInputDialog( parent );
                  dialog->setInputMode( mode );
                  dialog->setLabelText( label );
                  dialog->resize( width, height );
    return dialog;
}

qint32 Helper::strToInt(QString& str, int base)
{
    bool base16 = ( base != 10 );
    bool ok{ false };

    qint32 val = str.toInt( &ok, base );
    if ( !ok && !base16 )
        val = str.toInt( &ok, 16 );

    if ( !ok )
        val = -1;

    return val;
}

QString Helper::intSToStr(QString& val, int base, int fill, QChar filler)
{
    /* This overload is mainly used to reformat a QString's numeric format
     * if the source is in an unknown format.
     *
     * base --- What numeric format the string will be in.
     * fill --- How much padding will be prepended to the string.
     * filler --- The char used to pad the string
     */

    int val_i = val.toInt();
    if ( val_i > 0 )
        return QString( "%1" ).arg( val_i, fill, base, filler ).toUpper();
    else
        return QString( "%1" ).arg( val.toInt( 0, 16 ), fill, base, filler )
                   .toUpper();
}

QString Helper::getStrStr(const QString& str, QString indStr,
                          QString mid, QString left)
{
    /* Search an input string and return a sub-string based on the input strings.
     * indStr --- Sub-string to search for.
     * mid --- Obtain data after this sub-string.
     * left --- Obtain data before this sub-string.
     */

    QString tmp{ "" };
    int index{ 0 };

    if ( !str.isEmpty() )
    {
        if ( !indStr.isEmpty() )
        {
            index = getStrIndex( str,indStr );
            if ( index >= 0 )   //-1 if str didn't contain indStr.
            {
                if ( !mid.isEmpty() )
                    tmp = str.mid( index + indStr.length() );
                else
                    tmp = str.mid( index ); //Get the actual search string.
            }
        }

        if ( !mid.isEmpty()
          || !left.isEmpty() )
        {
            if ( indStr.isEmpty() )
                tmp = str;

            if ( !mid.isEmpty() )
            {
                index = getStrIndex( tmp, mid );
                if ( index >= 0 )   //-1 if str didn't contain mid.
                {
                    //Append the lookup string's length if it's greater than 1
                    if ( mid.length() >= 1 )
                        tmp = tmp.mid( index + mid.length() );
                    else
                        tmp = tmp.mid( index );
                }
            }

            if ( !left.isEmpty() )
            {
                index = getStrIndex( tmp, left );
                if ( index >= 0 )   //-1 if str didn't contain left.
                    tmp = tmp.left( index );
            }
        }

        if ( !tmp.isEmpty() )
            return tmp.trimmed();
    }
    return QString();
}

void Helper::stripNewlines(QString& string)
{
    if ( string.contains( "\r\n" ) ) //Replace Unix NewLines with Spaces.
        string = string.replace( "\r\n", " " );
    else if ( string.contains( "\r" ) ) //Replace Carriage Returns with Spaces.
        string = string.replace( "\r", " " );
    else if ( string.contains( "\n" ) ) //Replace NewLines with Spaces.
        string = string.replace( "\n", " " );
}

void Helper::stripSerNumHeader(QString& sernum)
{
    if ( strContainsStr( sernum, "SOUL" ) )
    {
        sernum = sernum.remove( "SOUL", Qt::CaseInsensitive )
                       .trimmed();
    }

    if ( strContainsStr( sernum, "WP" ) )
    {
        sernum = sernum.remove( "WP", Qt::CaseInsensitive )
                       .trimmed();
    }
}

QString Helper::sanitizeSerNum(const QString& value)
{
    QString sernum{ value };
    stripSerNumHeader( sernum );

    quint32 sernum_i{ sernum.toUInt( 0, 16 ) };
    if ( sernum_i & MIN_HEX_SERNUM )
        return value;

    return serNumToHexStr( sernum, 8 );
}

QString Helper::serNumToHexStr(QString sernum, int fillAmt)
{
    stripSerNumHeader( sernum );

    quint32 sernum_i{ sernum.toUInt( 0, 16 ) };
    QString result{ "" };

    if ( !( sernum_i & MIN_HEX_SERNUM ) )
    {
        bool ok{ false };
        sernum.toUInt( &ok, 10 );

        if ( !ok )
        {
            result = intToStr( sernum.toUInt( &ok, 16 ), 16, fillAmt );
            if ( !ok )
                result = intToStr( sernum.toInt( &ok, 16 ), 16, fillAmt );
        }
        else
        {
            result = intToStr( sernum.toUInt( &ok, 10 ), 16, fillAmt );
            if ( !ok )
                result = intToStr( sernum.toInt( &ok, 10 ), 16, fillAmt );
        }
    }
    else
        result = intToStr( sernum_i, 16, fillAmt );

    if ( result.length() > 8 )
        result = result.mid( result.length() - 8 );

    return result;
}

QString Helper::serNumToIntStr(QString sernum)
{
    quint32 sernum_i{ sernum.toUInt( 0, 16 ) };
    QString retn{ "" };

    if ( !( sernum_i & MIN_HEX_SERNUM ) )
        retn = QString( "SOUL %1" ).arg( intToStr( sernum_i, 10 ) );
    else
        retn = QString( "%1" ).arg( intToStr( sernum_i, 16 ) );

    if ( !strStartsWithStr( retn, "SOUL" )
      && retn.length() > 8 )
    {
        retn = retn.mid( retn.length() - 8 );
    }
    return retn;
}

qint32 Helper::serNumtoInt(QString& sernum)
{
    stripSerNumHeader( sernum );

    qint32 sernum_i{ sernum.toInt( 0, 16 ) };
    if ( sernum_i & MIN_HEX_SERNUM )
        sernum_i = strToInt( sernum, 16 );
    else
        sernum_i = strToInt( sernum, 10 );

    return sernum_i;
}

void Helper::logToFile(const LogTypes& type, const QString& text,
                       const bool& timeStamp,
                       const bool& newLine)
{
    if ( Settings::getLogFiles() )
    {
        QString logTxt = text;

        QFile log( "logs/"
                 % QDate::currentDate().toString( "[yyyy-MM-dd]/")
                 % logType[ type ] );

        QFileInfo logInfo( log );
        if ( !logInfo.dir().exists() )
            logInfo.dir().mkdir( "." );

        if ( log.open( QFile::WriteOnly | QFile::Append ) )
        {
            if ( timeStamp )
                logTxt.prepend( "[ " % getTimeAsString() % " ] " );

            if ( newLine )
                logTxt.prepend( "\r\n" );

            log.write( logTxt.toLatin1() );

            log.close();
        }
    }
}

bool Helper::confirmAction(QWidget* parent, QString& title, QString& prompt)
{
    qint32 value = QMessageBox::question( parent, title, prompt,
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No );
    return value == QMessageBox::Yes;
}

qint32 Helper::warningMessage(QWidget* parent, const QString& title,
                              const QString& prompt)
{
    return QMessageBox::warning( parent, title, prompt,
                                 QMessageBox::NoButton,
                                 QMessageBox::Ok );
}

QString Helper::getTextResponse(QWidget* parent, const QString& title,
                                const QString& prompt,
                                const QString& defaultInput,
                                bool* ok, int type)
{
    QString response{ "" };
    if ( type == 0 )    //Single-line message.
    {
        response = QInputDialog::getText( parent, title, prompt,
                                          QLineEdit::Normal,
                                          defaultInput, ok );
    }
    else if ( type == 1 )   //Multi-line message.
    {
        response = QInputDialog::getMultiLineText( parent, title, prompt,
                                                   defaultInput, ok );
    }
    return response;
}

QString Helper::getBanishReason(QWidget* parent)
{
    QString label{ "Ban Reason ( Sent to User ):" };
    QInputDialog* dialog{
        createInputDialog( parent, label,
                           QInputDialog::TextInput,
                           355, 170 ) };

    dialog->exec();
    dialog->deleteLater();

    return dialog->textValue();
}

QString Helper::getDisconnectReason(QWidget* parent)
{
    QString label{ "Disconnect Reason ( Sent to User ):" };
    QInputDialog* dialog{
        createInputDialog( parent, label,
                           QInputDialog::TextInput,
                           355, 170 ) };

    dialog->exec();
    dialog->deleteLater();

    return dialog->textValue();
}

QString Helper::hashPassword(QString& password)
{
    QCryptographicHash hash( QCryptographicHash::Sha3_512 );
                       hash.addData( password.toLatin1() );

    return QString( hash.result().toHex() );
}

QString Helper::genPwdSalt(const qint32& length)
{
    RandDev* randGen{ RandDev::getDevice() };

    QString salt{ "" };
    QString charList
    {
        "0123456789"
        "`~!@#$%^&*-_=+{([])}|;:'\"\\,./?<>"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
    };

    qint32 chrPos{ -1 };
    while ( salt.length() < length )
    {
        chrPos = randGen->genRandNum( 0, charList.length() - 1 );
        salt.append( charList.at( chrPos ) );
    }

    if ( !validateSalt( salt ) )
        salt = genPwdSalt( length );

    return salt;
}

bool Helper::validateSalt(QString& salt)
{
    QSettings* userData = User::getUserData();
    QStringList groups = userData->childGroups();
    QString j{ "" };

    for ( int i = 0; i < groups.count(); ++i )
    {
        j =  userData->value( groups.at( i ) % "/salt" )
                         .toString();

        if ( j == salt )
            return false;
    }
    return true;
}

bool Helper::naturalSort(QString& left, QString& right, bool& result)
{
    do
    {
        int posL = left.indexOf( QRegExp( "[0-9]" ) );
        int posR = right.indexOf( QRegExp( "[0-9]" ) );
        if ( posL == -1 || posR == -1 )
            break;
        else if ( posL != posR )
            break;
        else  if ( left.left( posL ) != right.left( posR ) )
            break;

        QString temp;
        while ( posL < left.size( ) )
        {
            if ( left.at( posL ).isDigit( ) )
                temp += left.at( posL );
            else
                break;
            posL++;
        }
        int numL = temp.toInt( );
        temp.clear( );

        while ( posR < right.size( ) )
        {
            if ( right.at( posR ).isDigit( ) )
                temp += right.at( posR );
            else
                break;
            posR++;
        }
        int numR = temp.toInt( );

        if ( numL != numR )
        {
            result = ( numL < numR );
            return true;
        }
        left.remove( 0, posL );
        right.remove( 0, posR );

    } while ( true );
    return false;
}

void Helper::delay(const qint32& time)
{
    //Delay the next Port refresh by /time/ seconds.
    QTime delayedTime = QTime::currentTime().addSecs( time );
    while ( QTime::currentTime() < delayedTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

QHostAddress Helper::getPrivateIP()
{
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();

    //Default to our localhost address if nothing valid is found.
    QHostAddress ipAddress{ QHostAddress::LocalHost };

    for ( int i = 0; i < ipList.size(); ++i )
    {
        QString tmp = ipList.at( i ).toString();
        //Remove localhost addresses.
        if ( ipList.at( i ) != QHostAddress::LocalHost
          //Remove any ipv6 addresses.
          && ipList.at( i ).toIPv4Address()
          //Remove any addresses the User manually marked invalid.
          && !Settings::getIsInvalidIPAddress( tmp )
          //Remove Windows generated APIPA addresses.
          && !strStartsWithStr( tmp, "169" ) )
        {
            //Use first non-local IP address.
            ipAddress = ipList.at(i).toString();
            break;
        }
    }

    //Null ip address was selected, default to '0.0.0.0'.
    if ( ipAddress.isNull() )
        ipAddress = QHostAddress::AnyIPv4;

    return ipAddress;
}

void Helper::getSynRealData(ServerInfo* svr)
{
    if ( svr == nullptr )
        return;

    QFileInfo synRealFile( "synReal.ini" );

    bool downloadFile = true;
    if ( synRealFile.exists() )
    {
        qint64 curTime = static_cast<qint64>(
                             QDateTime::currentDateTime()
                                  .toMSecsSinceEpoch() / 1000 );
        qint64 modTime = static_cast<qint64>(
                             synRealFile.lastModified()
                                  .toMSecsSinceEpoch() / 1000 );

        //Check if the file is 48 hours old and set our bool.
        downloadFile = ( curTime - modTime >= 172800 );
    }

    //The file was older than 48 hours or did not exist. Request a fresh copy.
    if ( downloadFile )
    {
        QTcpSocket* socket = new QTcpSocket;
        QUrl url( svr->getMasterInfoHost() );

        socket->connectToHost( url.host(), 80 );
        QObject::connect( socket, &QTcpSocket::connected, socket,
        [=]()
        {
            socket->write( QString( "GET %1\r\n" )
                               .arg( svr->getMasterInfoHost() )
                                             .toLatin1() );
        });

        QObject::connect( socket, &QTcpSocket::readyRead, socket,
        [=]()
        {
            QFile synreal( "synReal.ini" );
            if ( synreal.open( QIODevice::WriteOnly ) )
            {
                socket->waitForReadyRead();
                synreal.write( socket->readAll() );
            }

            synreal.close();

            QSettings settings( "synReal.ini", QSettings::IniFormat );
            QString str = settings.value( svr->getGameName()
                                        % "/master" ).toString();
            int index = str.indexOf( ":" );
            if ( index > 0 )
            {
                svr->setMasterIP( str.left( index ) );
                svr->setMasterPort(
                            static_cast<quint16>(
                                str.mid( index + 1 ).toInt() ) );
            }
        });

        QObject::connect( socket, &QTcpSocket::disconnected,
                          socket, &QTcpSocket::deleteLater );
    }
    else
    {
        QSettings settings( "synReal.ini", QSettings::IniFormat );
        QString str = settings.value( svr->getGameName()
                                    % "/master" ).toString();
        if ( !str.isEmpty() )
        {
            int index = str.indexOf( ":" );
            if ( index > 0 )
            {
                svr->setMasterIP( str.left( index ) );
                svr->setMasterPort(
                            static_cast<quint16>(
                                str.mid( index + 1 ).toInt() ) );
            }
        }
    }
}

bool Helper::strStartsWithStr(const QString& strA, const QString& strB)
{
    //Returns true if strA starts with strB.
    return strA.startsWith( strB, Qt::CaseInsensitive );
}

bool Helper::strContainsStr(const QString& strA, const QString& strB)
{
    //Returns true if strA contains strB.
    return strA.contains( strB, Qt::CaseInsensitive );
}

bool Helper::cmpStrings(const QString& strA, const QString& strB)
{
    //Returns true if strA is equal to strB.
    return ( strA.compare( strB, Qt::CaseInsensitive ) == 0 );
}

qint32 Helper::getStrIndex(const QString& strA, const QString& strB)
{
    //Returns the position of strB within strA.
    return strA.indexOf( strB, 0, Qt::CaseInsensitive );
}

QString Helper::getTimeAsString(const quint64& time)
{
    quint64 date{ time };
    if ( date == 0 )
        date = QDateTime::currentDateTime().toTime_t();

    return QDateTime::fromTime_t( date )
            .toString( "ddd MMM dd HH:mm:ss yyyy" );
}

QString Helper::getTimeFormat(const qint64& time)
{
    return QString( "%1:%2:%3" )
            .arg( getTimeIntFormat( time, TimeFormat::Hours ),
                  2, 10, QChar( '0' ) )
            .arg( getTimeIntFormat( time, TimeFormat::Minutes ),
                  2, 10, QChar( '0' ) )
            .arg( getTimeIntFormat( time, TimeFormat::Seconds ),
                  2, 10, QChar( '0' ) );
}

qint64 Helper::getTimeIntFormat(const qint64& time, const TimeFormat& format)
{
    switch ( format )
    {
        case TimeFormat::Hours:
        return ( time / static_cast<int>( TimeFormat::HoursDiv ) );
        break;
        case TimeFormat::Minutes:
        return ( ( time / static_cast<int>( TimeFormat::MinsDiv ) )
                 % static_cast<int>( TimeFormat::SecDiv ) );
        break;
        case TimeFormat::Seconds:
        return ( time % static_cast<int>( TimeFormat::SecDiv ) );
        break;
        case TimeFormat::Default:
        default:
        return time;
        break;
    }
}
