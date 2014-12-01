
#include "helper.hpp"

QInputDialog* Helper::createInputDialog(QWidget* parent, QString& label, QInputDialog::InputMode mode, int width, quint32 height)
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
        return QString( "%1" ).arg( val.toInt( 0, 16 ), fill, base, filler ).toUpper();
}

int Helper::strToInt(QString& str, int base)
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

QString Helper::serNumToHexStr(QString sernum, int fillAmt)
{
    if ( sernum.contains( "SOUL " ) )
        return intToStr( sernum.mid( sernum.indexOf( " " ) + 1 ).toInt(), 16, fillAmt );
    else
        return intToStr( sernum.toInt( 0, 16 ), 16, fillAmt );
}

QString Helper::serNumToIntStr(QString sernum)
{
    int serNum = sernum.toInt( 0, 16 );
    if ( !( serNum & 0x40000000 ) )
        return QString( "SOUL %1" ).arg( intToStr( serNum, 10 ) );
    else
        return QString( "%1" ).arg( intToStr( serNum, 16 ) );
}

void Helper::logToFile(QString& file, QString& text, bool timeStamp, bool newLine)
{
    QFile log( file );
    if ( log.open( QFile::WriteOnly | QFile::Append ) )
    {
        if ( timeStamp )
        {
            quint64 date = QDateTime::currentDateTime().toTime_t();
            text.prepend( QDateTime::fromTime_t( date ).toString( "[ ddd MMM dd HH:mm:ss yyyy ] " ) );
        }

        if ( newLine )
            text.prepend( "\r\n" );

        log.write( text.toLatin1() );

        log.close();
    }
}

bool Helper::confirmAction(QWidget* parent, QString& title, QString& prompt)
{
    quint32 value = QMessageBox::question( parent, title, prompt,
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No );
    return value == QMessageBox::Yes;
}

QString Helper::getTextResponse(QWidget* parent, QString& title, QString& prompt, bool* ok, int type)
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

void Helper::setSetting(const QString& key, const QString& subKey, QVariant& value)
{
    QSettings setting( "preferences.ini", QSettings::IniFormat );

    if ( key == "General" )
        setting.setValue( subKey, value );
    else
        setting.setValue( key + "/" + subKey, value );
}

QVariant Helper::getSetting(const QString& key, const QString& subKey)
{
    QSettings setting( "preferences.ini", QSettings::IniFormat );

    if ( key == "General" )
        return setting.value( subKey );

    return setting.value( key + "/" + subKey );
}

void Helper::setMOTDMessage(QVariant& value)
{
    setSetting( keys[ Keys::General ], subKeys[ SubKeys::MOTD ], value );
}

QString Helper::getMOTDMessage()
{
    return getSetting( keys[ Keys::General ], subKeys[ SubKeys::MOTD ] ).toString();
}

void Helper::setBanishMesage(QVariant& value)
{
    setSetting( keys[ Keys::General ], subKeys[ SubKeys::BanishMsg ], value );
}

QString Helper::getBanishMesage()
{
    return getSetting( keys[ Keys::General ], subKeys[ SubKeys::BanishMsg ] ).toString();
}

QString Helper::getBanishReason(QWidget* parent)
{
    QString label{ "Ban Reason ( Sent to User ):" };
    QInputDialog* dialog{ createInputDialog( parent, label, QInputDialog::TextInput, 355, 170 ) };

    dialog->exec();
    dialog->deleteLater();

    QString reason{ dialog->textValue() };
    if ( reason.isEmpty() )
        reason = "Manual Banish.";

    return reason;
}

QString Helper::getDisconnectReason(QWidget* parent)
{
    QString label{ "Disconnect Reason ( Sent to User ):" };
    QInputDialog* dialog{ createInputDialog( parent, label, QInputDialog::TextInput, 355, 170 ) };

    dialog->exec();
    dialog->deleteLater();

    QString reason{ dialog->textValue() };
    if ( reason.isEmpty() )
        reason = "Manual Disconnect.";

    return reason;
}

bool Helper::setPassword(QVariant& value, bool isHashed)
{
    //Convert the password to a SHA3_512 hash.
    if ( !value.toString().isEmpty() && !isHashed )
    {
        value = hashPassword( value );
        isHashed = true;
    }
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::Password ], value );

    return isHashed;
}

QString Helper::getPassword()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::Password ] ).toString();
}

void Helper::setRequirePassword(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::ReqPassword ], value );
}

bool Helper::getRequirePassword()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::ReqPassword ] ).toBool();
}

bool Helper::cmpServerPassword(QVariant& value)
{
    //Convert the password to a SHA3_512 hash.
    if ( !value.toString().isEmpty() )
        value = hashPassword( value );

    return ( getPassword() == value.toString() );
}

QString Helper::hashPassword(QVariant& password)
{
    return QString( QCryptographicHash::hash( password.toString().toLatin1(),
                                              QCryptographicHash::Sha3_512 ).toHex() );
}

void Helper::setServerRules(QVariant& value)
{
    setSetting( keys[ Keys::General ], subKeys[ SubKeys::Rules ], value );
}

QString Helper::getServerRules()
{
    QVariant pending = getSetting( keys[ Keys::General ], subKeys[ SubKeys::Rules ] );

    QString rules;
    if ( pending.type() == QVariant::StringList )
    {
        QStringList ruleList = pending.toStringList();
        for ( int i = 0; i < ruleList.count(); ++i )
        {
            if ( i > 0 )
                rules.append( ", " );

            rules.append( ruleList.at( i ) );
        }
    }
    else if ( pending.type() == QVariant::String )
        rules = pending.toString();

    return rules;
}

void Helper::setAllowDupedIP(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowDupe ], value );
}

bool Helper::getAllowDupedIP()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowDupe ] ).toBool();
}

void Helper::setBanDupedIP(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::BanDupes ], value );
}

bool Helper::getBanDupedIP()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::BanDupes ] ).toBool();
}

void Helper::setBanHackers(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::AutoBan ], value );
}

bool Helper::getBanHackers()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::AutoBan ] ).toBool();
}

void Helper::setReqSernums(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::ReqSernum ], value );
}

bool Helper::getReqSernums()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::ReqSernum ] ).toBool();
}

void Helper::setDisconnectIdles(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowIdle ], value );
}

bool Helper::getDisconnectIdles()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowIdle ] ).toBool();
}

void Helper::setAllowSSV(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowSSV ], value );
}

bool Helper::getAllowSSV()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::AllowSSV ] ).toBool();
}

void Helper::setLogComments(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::LogComments ], value );
}

bool Helper::getLogComments()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::LogComments ] ).toBool();
}

void Helper::setServerID(QVariant& value)
{
    setSetting( keys[ Keys::Options ], subKeys[ SubKeys::Extension ], value );
}

int Helper::getServerID()
{
    return getSetting( keys[ Keys::Options ], subKeys[ SubKeys::Extension ] ).toInt();
}

bool Helper::isInvalidIPAddress(const QString& value)
{
    return getSetting( keys[ Keys::WrongIP ], value ).toBool();
}