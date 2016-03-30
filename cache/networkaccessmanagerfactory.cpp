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

#include "networkaccessmanagerfactory.h"

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
    QNetworkDiskCache *cache = new QNetworkDiskCache(nam);
    cache->setCacheDirectory(Settings::instance()->cacheDir());
    cache->setMaximumCacheSize(Settings::instance()->loadMain("main/maxCacheSize", 1024 * 1024 * 200).toInt());
    nam->setCache(cache);
    return nam;
}
