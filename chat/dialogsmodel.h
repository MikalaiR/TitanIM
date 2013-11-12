#ifndef DIALOGSMODEL_H
#define DIALOGSMODEL_H

#include <QAbstractListModel>
#include "vk/client.h"
#include "vk/dialogspacket.h"

class DialogsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum dialogsRole
    {
        bodyRole = Qt::UserRole,
        dateRole,
        dateStrRole,
        uidRole,
        midRole,
        isUnreadRole,
        isOutRole,
        onlineRole
    };

    explicit DialogsModel(QObject *parent = 0);
    ~DialogsModel();
    void loadDialogs(const int count=20);
    void loadNextDialogs(const int count=20);
    void appendDialogs(const MessageList *dialogs);
    void replaceDialogs(const MessageList *dialogs);
    bool removeDialog(int row, int count);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    MessageList *_dialogs;
    DialogsPacket *_dialogsPacket;

protected slots:
    void onDialogsLoaded(const DialogsPacket *sender, const MessageList *dialogs);
};

#endif // DIALOGSMODEL_H
