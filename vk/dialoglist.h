#ifndef DIALOGLIST_H
#define DIALOGLIST_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "dialogitem.h"

class DialogListPrivate : public QObject
{
    Q_OBJECT

public:
    DialogListPrivate();
    ~DialogListPrivate();
    void add(DialogItem dialog);
    void add(const QVector<DialogItem> &dialogs);
    int indexOf(const int id) const;
    DialogItem at(const int i) const;
    DialogItem item(const int id) const;
    void remove(const int i);
    int count() const;
    QVector<DialogItem> toVector() const;

private:
    QVector<DialogItem> _dialogs;

protected slots:
    void onItemChanged(const int id, const QString &propertyName);

signals:
    void itemChanged(const int i);
};

typedef QSharedPointer<DialogListPrivate> DialogList;

#endif // DIALOGLIST_H
