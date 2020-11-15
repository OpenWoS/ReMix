
//Class includes.
#include "mastermixthread.hpp"

//ReMix includes.
#include "serverinfo.hpp"
#include "settings.hpp"
#include "randdev.hpp"
#include "logger.hpp"
#include "helper.hpp"
#include "user.hpp"

//Qt Includes.
#include <QNetworkInterface>
#include <QInputDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>
#include <QSettings>
#include <QtCore>

//Initialize our QSettings Object globally to make things more responsive.
//Should be Thread safe.
QSettings* MasterMixThread::masterMixPref{ new QSettings( "synreal.ini", QSettings::IniFormat ) };
bool MasterMixThread::downloaded;
QMutex MasterMixThread::mutex;

MasterMixThread::MasterMixThread()
{
    updateInfoTimer.setInterval( 86400 * 1000 );

    QObject::connect( &updateInfoTimer, &QTimer::timeout, this, [=]()
    {
        QString msg{ "Automatically refreshing the Master Mix Information." };
        emit this->insertLogSignal( "MasterMixThread", msg, LogTypes::MASTERMIX, true, true );

        this->updateMasterMixInfoSlot();
    } );

    QObject::connect( this, &MasterMixThread::insertLogSignal, Logger::getInstance(), &Logger::insertLogSlot );
}

MasterMixThread::~MasterMixThread()
{
    updateInfoTimer.stop();
    masterMixPref->deleteLater();

    this->disconnect();
}

void MasterMixThread::startUpdateInfoTimer(const bool& start)
{
    if ( start )
        updateInfoTimer.start();
    else
        updateInfoTimer.stop();
}

void MasterMixThread::updateMasterMixInfoSlot()
{
    downloaded = false;

    QString host{ Settings::getSetting( SKeys::Setting, SSubKeys::OverrideMasterHost ).toString() };

    QString message{ "Fetching Master Info from [ %1 ]." };
            message = message.arg( host );

    emit this->insertLogSignal( "MasterMixThread", message, LogTypes::MASTERMIX, true, true );

    QFileInfo synRealFile( "synReal.ini" );

    //The file was older than 48 hours or did not exist. Request a fresh copy.

    QTcpSocket* socket{ new QTcpSocket() };
    QUrl url{ host };

    socket->connectToHost( url.host(), 80 );
    QObject::connect( socket, &QTcpSocket::connected, socket,
    [=]()
    {
        socket->write( QString( "GET %1\r\n" )
                        .arg( host ).toLatin1() );
    } );

    QObject::connect( socket, &QTcpSocket::readyRead, socket,
    [=]()
    {
        QFile synreal( "synReal.ini" );
        if ( synreal.open( QIODevice::WriteOnly | QIODevice::Append ) )
        {
            synreal.seek( 0 ); //Erase the file by overwriting previous data.
            socket->waitForReadyRead();
            synreal.write( socket->readAll() );
        }

        synreal.flush();
        synreal.close();

        emit this->masterMixInfoSignal(); //Inform Listening Objects of a completed download.
    } );
    QObject::connect( socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater );
}

void MasterMixThread::obtainMasterData(ServerInfo* server)
{
    QString str{ masterMixPref->value( server->getGameName() % "/master" ).toString() };

    int index{ str.indexOf( ":" ) };
    if ( index > 0 )
    {
        QString ip{ str.left( index ) };
        quint16 port{ static_cast<quint16>( str.midRef( index + 1 ).toInt() ) };

        QString msg{ "Got Master Server [ %1:%2 ] for Game [ %3 ]." };
                msg = msg.arg( ip )
                         .arg( port )
                         .arg( server->getGameName() );
        emit this->insertLogSignal( server->getServerName(), msg, LogTypes::MASTERMIX, true, true );

        server->setMasterIP( ip, port );
    }
}

void MasterMixThread::run()
{
    masterMixPref->moveToThread( this );

    this->exec();
}

void MasterMixThread::getMasterMixInfo(ServerInfo* server)
{
    QMutexLocker locker( &mutex ); //Ensure thread safety.
    if ( !downloaded )
    {
        QObject::connect( this, &MasterMixThread::masterMixInfoSignal, this, [=]()
        {
            qDebug() << server;
            this->obtainMasterData( server );
        }, Qt::ConnectionType::UniqueConnection );

        this->updateMasterMixInfoSlot();
    }
    else
    {
        this->obtainMasterData( server );
    }
}

MasterMixThread* MasterMixThread::getInstance()
{
    static MasterMixThread* instance;
    if ( instance == nullptr )
        instance = new MasterMixThread();

    return instance;
}