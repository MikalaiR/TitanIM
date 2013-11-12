#include "dialogsmodel.h"

DialogsModel::DialogsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _dialogs = new MessageList();

    _dialogsPacket = new DialogsPacket(Client::instance()->connection());
    connect(_dialogsPacket, SIGNAL(dialogs(const DialogsPacket*,const MessageList*)), SLOT(onDialogsLoaded(const DialogsPacket*,const MessageList*)));
}

DialogsModel::~DialogsModel()
{
    delete _dialogsPacket;
    delete _dialogs;
}

void DialogsModel::loadDialogs(const int count)
{
    _dialogsPacket->load(0, count);
}

void DialogsModel::loadNextDialogs(const int count)
{
    _dialogsPacket->load(_dialogs->count(), count);
}

void DialogsModel::appendDialogs(const MessageList *dialogs)
{
    if (!dialogs->count())
        return;

    beginInsertRows(QModelIndex(), _dialogs->count(), _dialogs->count() + dialogs->count() - 1);
    _dialogs->add(dialogs);
    endInsertRows();
}

void DialogsModel::replaceDialogs(const MessageList *dialogs)
{
    removeDialog(0, rowCount());
    appendDialogs(dialogs);
}

bool DialogsModel::removeDialog(int row, int count)
{
    if (count <= 0 || row < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        _dialogs->remove(row);
    }

    endRemoveRows();

    return true;
}

QHash<int, QByteArray> DialogsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[bodyRole] = "body";
    roles[dateRole] = "date";
    roles[dateStrRole] = "dateStr";
    roles[uidRole] = "uid";
    roles[midRole] = "mid";
    roles[isUnreadRole] = "isUnread";
    roles[isOutRole] = "isOut";
    roles[onlineRole] = "online";

    return roles;
}

QVariant DialogsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _dialogs->count())
    {
        return QVariant();
    }

    MessageItem *dialog = _dialogs->at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return dialog->displayName().isEmpty() ? QString::number(dialog->uid()) : dialog->displayName();

    case Qt::DecorationRole:
        return dialog->photoMediumRect();

    case bodyRole:
        return dialog->body();

    case dateRole:
        return dialog->date();

    case dateStrRole:
        return Utils::dateToText(dialog->date());

    case uidRole:
        return dialog->uid();

    case midRole:
        return dialog->mid();

    case isUnreadRole:
        return dialog->isUnread();

    case isOutRole:
        return dialog->isOut();

    case onlineRole:
        return dialog->online();
    }

    return QVariant();
}

bool DialogsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() && !value.isValid() && index.row() >= _dialogs->count())
    {
        return false;
    }

    return false;
}

int DialogsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return _dialogs->count();
}

Qt::ItemFlags DialogsModel::flags(const QModelIndex &index) const
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

void DialogsModel::onDialogsLoaded(const DialogsPacket *sender, const MessageList *dialogs)
{
    if (!sender->offset())
    {
        replaceDialogs(dialogs);
    }
    else
    {
        appendDialogs(dialogs);
    }
}
