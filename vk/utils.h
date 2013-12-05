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

#ifndef UTILS_H
#define UTILS_H

#include <QCryptographicHash>
#include <QStringBuilder>
#include <QMap>
#include <QUrl>
#include <QDateTime>
#include <QVector>
#include "k8json.h"

class Utils
{
public:
    static QVariant parseJSON(const QByteArray &data);
    static QString getMd5String(const QString &source);
    static QString join(const QMap<QString, QString> &list, const QChar &separator);
    static QString joinAndEscape(const QMap<QString, QString> &list, const QChar &separator);
    static QString decode(const QString &str);
    static QString dateToText(const QDateTime &date);
    static QVector<int> toVectorInt(const QVariantList &list);
};

#endif // UTILS_H
