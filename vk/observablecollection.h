/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef OBSERVABLECOLLECTION_H
#define OBSERVABLECOLLECTION_H

#include <QObject>
#include <QList>

class ObservableCollectionPrivate : public QObject
{
    Q_OBJECT

public:
    ObservableCollectionPrivate(QObject *parent = 0);
    virtual int indexOf(const int id) const = 0;

protected slots:
    void onItemChanged(const int id, const QString &propertyName);

signals:
    void itemChanged(const int i);
};


template <typename T>
class ObservableCollection : public ObservableCollectionPrivate
{
public:
    ObservableCollection(QObject *parent = 0) :
        ObservableCollectionPrivate(parent) {}

    void add(T item);
    void add(const QList<T> &items);
    void prepend(T item);
    bool replace(const T item);
    void replace(const int i, const T item);
    int indexOf(const int id) const;
    inline T at(const int i) const { return _items.at(i); }
    inline T first() const { return _items.first(); }
    inline T last() const { return _items.last(); }
    T item(const int id) const;
    void removeAt(const int i);
    void clear();
    inline int count() const { return _items.count(); }
    inline QList<T> toList() const { return _items; }
    inline QVector<T> toVector() const { return _items.toVector(); }

private:
    QList<T> _items;
};


template <typename T>
void ObservableCollection<T>::add(T item)
{
    _items.append(item);
    connect(item.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

template <typename T>
void ObservableCollection<T>::add(const QList<T> &items)
{
    for (int i = 0; i < items.count(); i++)
    {
        add(items.at(i));
    }
}

template <typename T>
void ObservableCollection<T>::prepend(T item)
{
    _items.prepend(item);
    connect(item.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

template <typename T>
bool ObservableCollection<T>::replace(const T item)
{
    int i = indexOf(item->id());

    if (i > -1)
    {
        replace(i, item);
        return true;
    }

    return false;
}

template <typename T>
void ObservableCollection<T>::replace(const int i, const T item)
{
    disconnect(_items.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _items.replace(i, item);
    connect(item.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));

    emit itemChanged(i);
}

template <typename T>
int ObservableCollection<T>::indexOf(const int id) const
{
    for (int i = 0; i < _items.count(); i++)
    {
        if (_items.at(i)->id() == id)
            return i;
    }

    return -1;
}

template <typename T>
T ObservableCollection<T>::item(const int id) const
{
    foreach (T i, _items)
    {
        if (i->id() == id)
            return i;
    }

    T temp = T::create();
    temp->setId(id);
    return temp;
}

template <typename T>
inline void ObservableCollection<T>::removeAt(const int i)
{
    disconnect(_items.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _items.removeAt(i);
}

template <typename T>
void ObservableCollection<T>::clear()
{
    while (_items.count())
    {
        removeAt(0);
    }
}

#endif // OBSERVABLECOLLECTION_H
