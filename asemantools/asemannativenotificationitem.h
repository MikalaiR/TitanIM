#ifndef ASEMANNATIVENOTIFICATIONITEM_H
#define ASEMANNATIVENOTIFICATIONITEM_H

#include <QWidget>
#include <QStringList>

class AsemanNativeNotificationItemPrivate;
class AsemanNativeNotificationItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    AsemanNativeNotificationItem(QWidget *parent = 0);
    ~AsemanNativeNotificationItem();

    void setColor(const QColor &color);
    QColor color() const;

    void setActions(const QStringList & actions);
    void setTitle(const QString &title);
    void setBody(const QString &body);
    void setIcon(const QVariant &icon);
    void setTimeOut(int timeOut);

signals:
    void actionTriggered(const QString & act);
    void colorChanged();

protected:
    void resizeEvent(QResizeEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void refreshSize();
    void setRaised();
    void buttonClicked();

private:
    AsemanNativeNotificationItemPrivate *p;
};

#endif // ASEMANNATIVENOTIFICATIONITEM_H
