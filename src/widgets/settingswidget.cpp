
#include "includes.hpp"
#include "settingswidget.hpp"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    //Load Settings from file.
    pwdCheckState = Settings::getRequirePassword();
    this->setCheckedState( Toggles::REQPWD,
                           pwdCheckState );

    this->setCheckedState( Toggles::REQADMINPWD,
                           Settings::getReqAdminAuth() );

    this->setCheckedState( Toggles::ALLOWDUPEDIP,
                           Settings::getAllowDupedIP() );

    this->setCheckedState( Toggles::BANDUPEDIP,
                           Settings::getBanDupedIP() );

    this->setCheckedState( Toggles::BANHACKERS,
                           Settings::getBanDeviants() );

    this->setCheckedState( Toggles::REQSERNUM,
                           Settings::getReqSernums() );

    this->setCheckedState( Toggles::DISCONNECTIDLES,
                           Settings::getDisconnectIdles() );

    this->setCheckedState( Toggles::ALLOWSSV,
                           Settings::getAllowSSV() );

    this->setCheckedState( Toggles::LOGCOMMENTS,
                           Settings::getLogComments() );

    this->setCheckedState( Toggles::FWDCOMMENTS,
                           Settings::getFwdComments() );

    this->setCheckedState( Toggles::INFORMADMINLOGIN,
                           Settings::getInformAdminLogin() );

    this->setCheckedState( Toggles::ECHOCOMMENTS,
                           Settings::getEchoComments() );

    this->setCheckedState( Toggles::MINIMIZETOTRAY,
                           Settings::getMinimizeToTray() );

    this->setCheckedState( Toggles::SAVEWINDOWPOSITIONS,
                           Settings::getSaveWindowPositions() );

    this->setCheckedState( Toggles::LOGFILES,
                           Settings::getLogFiles() );

    QString dir{ Settings::getWorldDir() };
    QString rowText{ "World Dir: [ %1 ]" };
            rowText = rowText.arg( dir );

    ui->settingsView->item( Toggles::WORLDDIR, 0 )->setText( rowText );
    this->setCheckedState( Toggles::WORLDDIR, !dir.isEmpty() );
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::setCheckedState(Toggles option, bool val)
{
    Qt::CheckState state;
    if ( val )
        state = Qt::Checked;
    else
        state = Qt::Unchecked;

    ui->settingsView->item( option, 0 )->setCheckState( state );
}

void SettingsWidget::on_settingsView_itemClicked(QTableWidgetItem *item)
{
    if ( item != nullptr )
    {
        if ( item->checkState() == Qt::Checked
          || item->checkState() == Qt::Unchecked )
        {
            this->toggleSettings( item->row(), item->checkState() );
        }
    }
}

void SettingsWidget::on_settingsView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    Qt::CheckState val = ui->settingsView->item( row, 0 )->checkState();
    ui->settingsView->item( row, 0 )->setCheckState( val == Qt::Checked
                                                     ? Qt::Unchecked
                                                     : Qt::Checked );

    val = ui->settingsView->item( row, 0 )->checkState();
    this->toggleSettings( row, val );
}

void SettingsWidget::toggleSettings(quint32 row, Qt::CheckState value)
{
    QVariant state = value == Qt::Checked;

    QString title{ "" };
    QString prompt{ "" };

    switch ( row )
    {
        case Toggles::REQPWD: //0
            {
                QString pwd{ Settings::getPassword() };

                bool reUse{ false };
                bool ok{ false };

                Settings::setRequirePassword( state );
                if ( state.toBool() != pwdCheckState )
                {
                    if ( Settings::getRequirePassword() )
                    {
                        //Recycyle the Old password. Assuming it wasn't deleted.
                        if ( !pwd.isEmpty() )
                        {
                            title = "Re-Use Password:";
                            prompt = "Do you wish to re-use the stored Password?";

                            reUse = Helper::confirmAction( this, title, prompt );

                        }

                        if ( pwd.isEmpty()
                             || !reUse )
                        {
                            title = "Server Password:";
                            prompt = "Password:";
                            pwd = Helper::getTextResponse( this, title,
                                                           prompt, &ok, 0 );
                        }

                        if (( !pwd.isEmpty()
                           && ok )
                          || reUse )
                        {
                            if ( !reUse )
                                Settings::setPassword( pwd );
                        }
                        else
                        {
                            ui->settingsView->item( row, 0 )->setCheckState(
                                        Qt::Unchecked );

                            state = false;
                            Settings::setRequirePassword( state );
                        }
                    }
                    else if ( !Settings::getRequirePassword()
                              && !pwd.isEmpty() )
                    {
                        title = "Remove Password:";
                        prompt = "Do you wish to erase the stored Password?";

                        if ( Helper::confirmAction( this, title, prompt ) )
                        {
                            pwd.clear();
                            Settings::setPassword( pwd );
                        }
                    }
                }
                pwdCheckState = state.toBool();
            }
        break;
        case Toggles::REQADMINPWD: //1
            Settings::setReqAdminAuth( state );
        break;
        case Toggles::ALLOWDUPEDIP: //2
            Settings::setAllowDupedIP( state );
        break;
        case Toggles::BANDUPEDIP: //3
            Settings::setBanDupedIP( state );
        break;
        case Toggles::BANHACKERS: //4
            Settings::setBanHackers( state );
        break;
        case Toggles::REQSERNUM: //5
            Settings::setReqSernums( state );
        break;
        case Toggles::DISCONNECTIDLES: //6
            Settings::setDisconnectIdles( state );
        break;
        case Toggles::ALLOWSSV: //7
            Settings::setAllowSSV( state );
        break;
        case Toggles::LOGCOMMENTS: //8
            Settings::setLogComments( state );
        break;
        case Toggles::FWDCOMMENTS: //9
            Settings::setFwdComments( state );
        break;
        case Toggles::ECHOCOMMENTS: //10
            Settings::setEchoComments( state );
        break;
        case Toggles::INFORMADMINLOGIN: //11
            Settings::setInformAdminLogin( state );
        break;
        case Toggles::MINIMIZETOTRAY: //12
            Settings::setMinimizeToTray( state );
        break;
        case Toggles::SAVEWINDOWPOSITIONS: //13
            Settings::setSaveWindowPositions( state );
        break;
        case Toggles::LOGFILES: //14
            Settings::setLogFiles( state );
        break;
        case Toggles::WORLDDIR: //15
            {
                QString directory{ Settings::getWorldDir() };
                QString rowText{ "World Dir: [ %1 ]" };

                bool reUse{ false };
                if ( !directory.isEmpty() )
                {
                    title = "Re-Select Directory:";
                    prompt = "Do you wish to re-select the world directory?";

                    reUse = Helper::confirmAction( this, title, prompt );
                }

                if ( directory.isEmpty() || reUse )
                {
                    QString title{ "Select WoS Directory" };
                    directory = QFileDialog::getExistingDirectory(
                                    this, title, "/worlds",
                                    QFileDialog::ShowDirsOnly |
                                    QFileDialog::DontResolveSymlinks );

                    state = false;
                    if ( directory.contains( "worlds", Qt::CaseInsensitive ) )
                    {
                        if ( directory.endsWith( "/worlds" ) )
                        {
                            state = true;
                            ui->settingsView->item( Toggles::WORLDDIR, 0 )->
                                                setCheckState( Qt::Checked );
                        }
                    }

                    if ( !state.toBool() )
                    {
                        directory = "";
                        title = "Invalid Directory:";
                        prompt = "You have selected an invalid world directory."
                                 " Please try again.";

                        ui->settingsView->item( Toggles::WORLDDIR, 0 )->
                                            setCheckState( Qt::Unchecked );

                        Helper::warningMessage( this, title, prompt );
                    }
                    rowText = rowText.arg( directory );
                    ui->settingsView->item( Toggles::WORLDDIR, 0 )->
                                        setText( rowText );
                    Settings::setWorldDir( directory );
                }
            }
        break;
        default:
            qDebug() << "Unknown Option, doing nothing!";
        break;
    }
}
