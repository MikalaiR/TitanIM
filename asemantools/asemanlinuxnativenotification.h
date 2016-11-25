#ifndef ASEMANLINUXNATIVENOTIFICATION_H
#define ASEMANLINUXNATIVENOTIFICATION_H

#include <QObject>
#include <QStringList>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QtDBus>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusArgument>
#include <QDBusInterface>
#include <QCoreApplication>
#include <QDebug>

struct iiibiiay
{
    iiibiiay(const QPixmap& img);
    iiibiiay();
    static const int id;
    int width;
    int height;
    int rowstride;
    int bitsPerSample;
    int channels;
    bool hasAlpha;
    QByteArray image;
};

Q_DECLARE_METATYPE(iiibiiay);

QDBusArgument &operator<<(QDBusArgument &a, const iiibiiay &i);
const QDBusArgument & operator >>(const QDBusArgument &a, iiibiiay &i);


class QDBusMessage;
class AsemanLinuxNativeNotificationPrivate;
class AsemanLinuxNativeNotification : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    AsemanLinuxNativeNotification(QObject *parent = 0);
    ~AsemanLinuxNativeNotification();

    void setColor(const QColor &color);
    QColor color() const;

public slots:
    uint sendNotify(const QString & title, const QString & body, const QVariant &icon, uint replace_id = 0, int timeOut = 3000 , const QStringList &actions = QStringList());
    void closeNotification( uint id );

signals:
    void notifyClosed( uint id );
    void notifyTimedOut( uint id );
    void notifyAction( uint id, const QString & action );
    void colorChanged();

private slots:
    void notificationClosed( const QDBusMessage & dmsg );
    void actionInvoked( const QDBusMessage & dmsg );

private:
    AsemanLinuxNativeNotificationPrivate *p;
};

#endif // ASEMANLINUXNATIVENOTIFICATION_H
