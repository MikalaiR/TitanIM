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

#ifndef ATTACHMENTSMODEL_H
#define ATTACHMENTSMODEL_H

#include <QAbstractListModel>
#include "vk/attachmentlist.h"
#include "vk/photoitem.h"

class AttachmentsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY attachmentsCountChanged)

public:
    enum chatRole
    {
        TypeRole = Qt::UserRole
    };

    AttachmentsModel(QObject *parent = 0);
    Q_INVOKABLE bool remove(int row, int count=1);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    AttachmentList *_attachments;

public slots:
    void setAttachments(AttachmentList *attachments);

protected slots:
    void onItemChanged(const int i);

signals:
    void attachmentsCountChanged();
};

#endif // ATTACHMENTSMODEL_H
