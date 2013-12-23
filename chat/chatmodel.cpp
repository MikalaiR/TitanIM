#include "chatmodel.h"

ChatModel::ChatModel(const DialogItem dialog, const ProfileItem ownProfile, QObject *parent) :
    QAbstractListModel(parent)
{
    _dialog = dialog;
    _ownProfile = ownProfile;

    _chat = MessageList::create();
    connect(_chat.data(), SIGNAL(itemChanged(int)), this, SLOT(onItemChanged(int)));
}

ChatModel::~ChatModel()
{

}

void ChatModel::append(const MessageList items)
{
    if (!items->count())
        return;

    beginInsertRows(QModelIndex(), _chat->count(), _chat->count() + items->count() - 1);
    _chat->add(items->toVector());
    endInsertRows();
}

void ChatModel::replace(const MessageList items)
{
    remove(0, rowCount());
    append(items);
}

bool ChatModel::remove(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _chat->remove(row);
    }

    endRemoveRows();

    return true;
}

MessageItem ChatModel::at(const int row)
{
    return _chat->at(row);
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[bodyRole] = "body";
    roles[dateRole] = "date";
    roles[dateStrRole] = "dateStr";
    roles[uidRole] = "uid";
    roles[midRole] = "mid";
    roles[isUnreadRole] = "isUnread";
    roles[isOutRole] = "isOut";

    return roles;
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _chat->count())
    {
        return QVariant();
    }

    MessageItem message = _chat->at(index.row());
    ProfileItem profile = _dialog->isGroupChat() ? _dialog->groupChatHandler()->user(message->uid()) : _dialog->profile();

    switch (role)
    {
    case Qt::DisplayRole:
        return message->body();

    case Qt::DecorationRole:
        return message->isOut() ? _ownProfile->photoMediumRect() : profile->photoMediumRect();

    case bodyRole:
        return message->body();

    case dateRole:
        return message->date();

    case dateStrRole:
        return Utils::dateToText(message->date());

    case uidRole:
        return message->uid();

    case midRole:
        return message->mid();

    case isUnreadRole:
        return message->isUnread();

    case isOutRole:
        return message->isOut();
    }

    return QVariant();
}

bool ChatModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _chat->count())
    {
        return false;
    }

    return false;
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _chat->count();
}

Qt::ItemFlags ChatModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    else
    {
        return Qt::NoItemFlags;
    }
}

void ChatModel::onItemChanged(const int i)
{
    QModelIndex idx = index(i, 0);
    emit dataChanged(idx, idx);
}
