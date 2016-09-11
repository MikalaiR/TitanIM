#define DBUS_SERVICE "org.freedesktop.Notifications"
#define DBUS_PATH    "/org/freedesktop/Notifications"
#define DBUS_OBJECT  "org.freedesktop.Notifications"
#define DBUS_CLOSED  "NotificationClosed"
#define DBUS_ACTION  "ActionInvoked"
#define DBUS_NOTIFY  "Notify"
#define DBUS_NCLOSE  "CloseNotification"

#include "asemanlinuxnativenotification.h"

iiibiiay::iiibiiay(QPixmap& pixmap)
{
    QImage img = pixmap.toImage();
    if(img.format() != QImage::Format_ARGB32)
        img = img.convertToFormat(QImage::Format_ARGB32);

    width = img.width();
    height = img.height();
    rowstride = img.bytesPerLine();
    hasAlpha = img.hasAlphaChannel();
    channels = img.isGrayscale() ? 1 : (hasAlpha ? 4 : 3);
    bitsPerSample = img.depth()/channels;
    image.append( reinterpret_cast<char*>(img.rgbSwapped().bits()), img.byteCount() );
}
iiibiiay::iiibiiay(){}
const int iiibiiay::id(qDBusRegisterMetaType<iiibiiay>());

QDBusArgument &operator<<(QDBusArgument &a, const iiibiiay &i)
{
    a.beginStructure();
    a << i.width << i.height << i.rowstride << i.hasAlpha << i.bitsPerSample << i.channels << i.image;
    a.endStructure();
    return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a,  iiibiiay &i)
{
    a.beginStructure();
    a >> i.width>> i.height >> i.rowstride >> i.hasAlpha >> i.bitsPerSample >> i.channels >> i.image;
    a.endStructure();
    return a;
}


class AsemanLinuxNativeNotificationPrivate
{
public:
    QDBusConnection *connection;

    QSet<uint> notifies;
    QColor color;
};

AsemanLinuxNativeNotification::AsemanLinuxNativeNotification(QObject *parent) :
    QObject(parent)
{
    p = new AsemanLinuxNativeNotificationPrivate;

    p->connection = new QDBusConnection( QDBusConnection::sessionBus() );
    p->connection->connect( DBUS_SERVICE , DBUS_PATH , DBUS_OBJECT , DBUS_CLOSED , this , SLOT(notificationClosed(QDBusMessage)) );
    p->connection->connect( DBUS_SERVICE , DBUS_PATH , DBUS_OBJECT , DBUS_ACTION , this , SLOT(actionInvoked(QDBusMessage))      );
}

void AsemanLinuxNativeNotification::setColor(const QColor &color)
{
    if(p->color == color)
        return;

    p->color = color;
    emit colorChanged();
}

QColor AsemanLinuxNativeNotification::color() const
{
    return p->color;
}

uint AsemanLinuxNativeNotification::sendNotify(const QString &title, const QString &body, const QVariant &icon, uint replace_id, int timeOut, const QStringList &actions)
{
    QVariantList args;
    args << QCoreApplication::applicationName();
    args << replace_id;
    args << "TitanIM64";
    args << title;
    args << body;
    args << QVariant::fromValue<QStringList>(actions);

    QVariantMap hints;
    hints.insert("category", "im");
    if (icon.type() == QVariant::Type::Pixmap)
    {
        QPixmap px(qvariant_cast<QPixmap>(icon).scaledToWidth(50, Qt::FastTransformation));
        hints.insert("image-data", QVariant(iiibiiay::id).fromValue(iiibiiay(px)));
    }

    args << QVariant::fromValue<QVariantMap>(hints);

    args << timeOut;

    QDBusMessage omsg = QDBusMessage::createMethodCall( DBUS_SERVICE , DBUS_PATH , DBUS_OBJECT , DBUS_NOTIFY );
    omsg.setArguments( args );

    const QDBusMessage & imsg = p->connection->call( omsg , QDBus::BlockWithGui );
    const QVariantList & res = imsg.arguments();
    if( res.isEmpty() )
        return 0;

    uint id_res = res.first().toUInt();
    p->notifies.insert( id_res );
    return id_res;
}

void AsemanLinuxNativeNotification::closeNotification(uint id)
{
    if( !p->notifies.contains(id) )
        return;

    QVariantList args;
    args << id;

    QDBusMessage omsg = QDBusMessage::createMethodCall( DBUS_SERVICE , DBUS_PATH , DBUS_OBJECT , DBUS_NCLOSE );
    omsg.setArguments( args );

    p->connection->call( omsg , QDBus::NoBlock );
}

void AsemanLinuxNativeNotification::notificationClosed(const QDBusMessage &dmsg)
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    const QVariantList & args = dmsg.arguments();
    if( args.isEmpty() )
        return ;

    uint id = args.at(0).toUInt();
    if( !p->notifies.contains(id) )
        return;

    if( args.count() == 1 )
    {
        emit notifyClosed(id);
        p->notifies.remove(id);
        return;
    }

    int type = args.at(1).toInt();
    switch (type) {
    case 1:
        emit notifyTimedOut( id );
        break;

    case 2:
    default:
        emit notifyClosed( id );
        p->notifies.remove(id);
        break;
    }
}

void AsemanLinuxNativeNotification::actionInvoked(const QDBusMessage &dmsg)
{
    if( dmsg.type() != QDBusMessage::SignalMessage )
        return ;

    const QVariantList & args = dmsg.arguments();
    if( args.count() != 2 )
        return ;

    uint id = args.at(0).toUInt();
    if( !p->notifies.contains(id) )
        return;

    QString action = args.at(1).toString();
    emit notifyAction(id, action);
}

AsemanLinuxNativeNotification::~AsemanLinuxNativeNotification()
{
    delete p;
}

