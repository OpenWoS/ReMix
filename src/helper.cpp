
#include "includes.hpp"
#include "helper.hpp"

QInputDialog* Helper::createInputDialog(QWidget* parent, QString& label,
                                        QInputDialog::InputMode mode,
                                        int width, quint32 height)
{
    QInputDialog* dialog = new QInputDialog( parent );
                  dialog->setInputMode( mode );
                  dialog->setLabelText( label );
                  dialog->resize( width, height );
    return dialog;
}

QString Helper::intSToStr(QString val, int base, int fill, QChar filler)
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

int Helper::strToInt(QString str, int base)
{
    bool base16 = ( base != 10 );
    bool ok{ false };

    int val = str.toInt( &ok, base );
    if ( !ok && !base16 )
        val = str.toInt( &ok, 16 );

    if ( !ok )
        val = -1;

    return val;
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
            index = str.indexOf( indStr, 0, Qt::CaseInsensitive );
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
                index = tmp.indexOf( mid, 0, Qt::CaseInsensitive );
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
                index = tmp.indexOf( left, 0, Qt::CaseInsensitive );
                if ( index >= 0 )   //-1 if str didn't contain left.
                    tmp = tmp.left( index );
            }
        }

        if ( !tmp.isEmpty() )
            return tmp.trimmed();
    }
    return QString();
}

QString Helper::serNumToHexStr(QString sernum, int fillAmt)
{
    qint32 sernum_i{ sernum.toInt( 0, 16 ) };

    QString result{ "" };
    if ( sernum.contains( "SOUL" ) )
    {
        result = intToStr( getStrStr( sernum, "SOUL", "SOUL", "" )
                                 .toInt(), 16, fillAmt );
    }
    else
    {
        if ( sernum_i & MIN_HEX_SERNUM )
            result = intToStr( sernum_i, 16, fillAmt );
        else
            result = intToStr( sernum.toInt( 0, 10 ), 16, fillAmt );
    }

    //Remove Spaces.
    return result.trimmed();
}

QString Helper::serNumToIntStr(QString sernum)
{
    qint32 sernum_i{ sernum.toInt( 0, 16 ) };
    if ( sernum_i <= 0 )
        sernum_i = sernum.toUInt( 0, 16 );

    QString retn{ "" };
    if ( !( sernum_i & MIN_HEX_SERNUM ) )
        retn = QString( "SOUL %1" ).arg( intToStr( sernum_i, 10 ) );
    else
        retn = QString( "%1" ).arg( intToStr( sernum_i, 16 ) );

    if ( !retn.startsWith( "SOUL", Qt::CaseInsensitive )
      && retn.length() > 8 )
    {
        retn = retn.mid( retn.length() - 8 );
    }
    return retn;
}

qint32 Helper::serNumtoInt(QString& sernum)
{
    int sernum_i{ 0 };
    if ( sernum.contains( "SOUL" ) )
        sernum_i = strToInt( getStrStr( sernum, "SOUL", " ", "" ), 10 );
    else
        sernum_i = strToInt( sernum, 16 );

    return sernum_i;
}

void Helper::logToFile(QString& file, QString& text, bool timeStamp,
                       bool newLine)
{
    QFile log( file );
    QFileInfo logInfo( log );
    if ( !logInfo.dir().exists() )
        logInfo.dir().mkdir( "." );

    if ( log.open( QFile::WriteOnly | QFile::Append ) )
    {
        if ( timeStamp )
        {
            quint64 date = QDateTime::currentDateTime().toTime_t();
            text.prepend( QDateTime::fromTime_t( date )
                               .toString( "[ ddd MMM dd HH:mm:ss yyyy ] " ) );
        }

        if ( newLine )
            text.prepend( "\r\n" );

        log.write( text.toLatin1() );

        log.close();
    }
}

bool Helper::confirmAction(QWidget* parent, QString& title, QString& prompt)
{
    qint32 value = QMessageBox::question( parent, title, prompt,
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No );
    return value == QMessageBox::Yes;
}

qint32 Helper::warningMessage(QWidget* parent, QString& title, QString& prompt )
{
    return QMessageBox::warning( parent, title, prompt,
                                 QMessageBox::NoButton,
                                 QMessageBox::Ok );
}

QString Helper::getTextResponse(QWidget* parent, QString& title,
                                QString& prompt, bool* ok, int type)
{
    QString response{ "" };
    if ( type == 0 )    //Single-line message.
    {
        response = QInputDialog::getText( parent, title, prompt,
                                          QLineEdit::Normal,
                                          "", ok );
    }
    else if ( type == 1 )   //Multi-line message.
    {
        response = QInputDialog::getMultiLineText( parent, title, prompt,
                                                   "", ok );
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

QString Helper::hashPassword(QVariant& password)
{
    QCryptographicHash hash( QCryptographicHash::Sha3_512 );
                       hash.addData( password.toString().toLatin1() );

    return QString( hash.result().toHex() );
}

QString Helper::genPwdSalt(RandDev* randGen, qint32 length)
{
    bool newRNG{ false };
    if ( randGen == nullptr )
    {
        randGen = new RandDev();
        newRNG = true;
    }

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

    if ( newRNG )
        delete randGen;

    return salt;
}

void Helper::logBIOData(QString& serNum, QHostAddress& ip,
                        quint16 port, QString& bio)
{
    QString sernum{ serNum };
    if ( sernum.contains( "SOUL", Qt::CaseInsensitive ) )
        sernum = Helper::serNumToHexStr( serNum, 8 );

    QString ipAddr{ ip.toString() % ":" % Helper::intToStr( port, 10, 0 ) };
    qint64 date{ QDateTime::currentDateTime().toTime_t() };

    QSettings bioData( "userInfo.ini", QSettings::IniFormat );
              bioData.setValue( sernum % "/bioInfo", bio.mid( 1 ) );
              bioData.setValue( sernum % "/ipInfo", ipAddr );
              bioData.setValue( sernum % "/date", QDateTime::fromTime_t( date )
                                                       .toString( "ddd MMM dd"
                                                                  " HH:mm:ss "
                                                                  "yyyy" ));
}

bool Helper::naturalSort(QString left, QString right, bool& result)
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
