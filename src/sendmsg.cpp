
#include "includes.hpp"
#include "sendmsg.hpp"
#include "ui_sendmsg.h"

SendMsg::SendMsg(QWidget *parent, ServerInfo* svr, Player* trg) :
    QDialog(parent),
    ui(new Ui::SendMsg)
{
    ui->setupUi(this);

    {
        QIcon icon = this->windowIcon();
        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;

        this->setWindowFlags( flags );
        this->setWindowIcon( icon );
    }

    if ( Settings::getSaveWindowPositions() )
    {
        QByteArray geometry{ Settings::getWindowPositions(
                                    this->metaObject()->className() ) };
        if ( !geometry.isEmpty() )
        {
            this->restoreGeometry( Settings::getWindowPositions(
                                       this->metaObject()->className() ) );
        }
    }

    target = trg;
    server = svr;

    //Install EventFilters.
    this->installEventFilter( this );
    ui->msgEditor->installEventFilter( this );
}

SendMsg::~SendMsg()
{
    if ( Settings::getSaveWindowPositions() )
    {
        Settings::setWindowPositions( this->saveGeometry(),
                                      this->metaObject()->className() );
    }
    delete ui;
}

void SendMsg::on_sendMsg_clicked()
{
    if ( server == nullptr )
        return;

    QString message{ ui->msgEditor->toPlainText() };
    if ( message.isEmpty() )
    {
        this->close();
        return;
    }

    message = message.prepend( "Owner: " );

    if ( message.contains( "\r\n" ) ) //Replace Unix NewLines with Spaces.
        message = message.replace( "\r\n", " " );
    else if ( message.contains( "\r" ) ) //Replace Carriage Returns with Spaces.
        message = message.replace( "\r", " " );
    else if ( message.contains( "\n" ) ) //Replace NewLines with Spaces.
        message = message.replace( "\n", " " );

    if ( ui->checkBox->isChecked() )
        server->sendMasterMessage( message, nullptr, true );
    else
    {
        if ( target == nullptr )
            return;

        server->sendMasterMessage( message, target, false );
    }

    this->close();
}

bool SendMsg::eventFilter(QObject* obj, QEvent* event)
{
    if ( obj == nullptr || event == nullptr  )
        return false;

    QKeyEvent* key = static_cast<QKeyEvent*>( event );
    if ( key != nullptr
      && key->type() == QEvent::KeyPress )
    {
        switch ( key->key() )
        {
            case Qt::Key_Escape:
                {
                    this->close();
                    event->accept();
                }
                return true;
            break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
                {
                    emit ui->sendMsg->clicked();
                    event->accept();
                }
                return true;
            break;
            default:
                event->ignore();
            break;
        }
    }
    return QObject::eventFilter( obj, event );
}
