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

#ifndef NOTIFYPROPERTYBASE_H
#define NOTIFYPROPERTYBASE_H

#include <QObject>

class NotifyPropertyBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id CONSTANT) //todo

public:
    NotifyPropertyBase();
    inline int id() const { return _id; }
    void setId(const int id);
    void beginChangeGroupProperties();
    void endChangeGroupProperties();

private:
    bool _blockSignalPropertyChanged;

protected:
    int _id;

public slots:
    inline void emitPropertyChanged(const QString &propertyName)
    {
        if (!_blockSignalPropertyChanged)
            emit propertyChanged(_id, propertyName);
    }

signals:
    void propertyChanged(const int id, const QString &propertyName);
};

#endif // NOTIFYPROPERTYBASE_H
