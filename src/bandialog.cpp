
#include "includes.hpp"
#include "bandialog.hpp"
#include "ui_bandialog.h"

BanDialog::BanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BanDialog)
{
    ui->setupUi(this);

    //Remove the "Help" button from the window title bars.
    {
        QIcon icon = this->windowIcon();
        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;

        this->setWindowFlags( flags );
        this->setWindowIcon( icon );
        //this->setWindowModality( Qt::WindowModal );
    }

    ipBanWidget = new IPBanWidget( this );
    ui->tabWidget->addTab( ipBanWidget, "IP Bans" );

    snBanWidget = new SNBanWidget( this );
    ui->tabWidget->addTab( snBanWidget, "SerNum Bans" );

    dvBanWidget = new DVBanWidget( this );
    ui->tabWidget->addTab( dvBanWidget, "Device Bans" );

    daBanWidget = new DABanWidget( this );
    ui->tabWidget->addTab( daBanWidget, "Date Bans" );
}

BanDialog::~BanDialog()
{
    daBanWidget->deleteLater();
    dvBanWidget->deleteLater();
    ipBanWidget->deleteLater();
    snBanWidget->deleteLater();

    delete ui;
}

//IP Ban Tab forward functions.
void BanDialog::remoteAddIPBan(Player* admin, Player* target, QString& reason)
{
    if ( admin == nullptr || target == nullptr )
    {
        return;
    }

    QString ip{ target->getPublicIP() };

    QString msg{ reason };
    if ( msg.isEmpty() )
    {
        msg = "Remote-Banish by [ %1 ]; Unknown Reason: [ %2 ]";
        msg = msg.arg( admin->getSernum_s() )
                 .arg( ip );
    }
    ipBanWidget->addIPBan( ip, msg );
}

void BanDialog::addIPBan(QString ip, QString& reason)
{
    ipBanWidget->addIPBan( ip, reason );
}

void BanDialog::removeIPBan(QString ip)
{
    ipBanWidget->removeIPBan( ip );
}

//Sernum-Ban Tab.
void BanDialog::remoteAddSerNumBan(Player* admin, Player* target, QString& reason)
{
    if ( admin == nullptr || target == nullptr )
    {
        return;
    }

    QString msg{ reason };
    if ( msg.isEmpty() )
    {
        msg = "Remote-Banish by [ %1 ]; Unknown reason: [ %2 ]";
        msg = msg.arg( admin->getSernum_s() )
                 .arg( target->getSernum_s() );
    }

    QString sernum{ target->getSernumHex_s() };
    snBanWidget->addSerNumBan( sernum, msg );
}

void BanDialog::addSerNumBan(QString& sernum, QString& reason)
{
    snBanWidget->addSerNumBan( sernum, reason );
}

void BanDialog::removeSerNumBan(QString& sernum)
{
    snBanWidget->removeSerNumBan( sernum );
}
