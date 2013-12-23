#ifndef DIALOGPARSER_H
#define DIALOGPARSER_H

#include <QObject>
#include "dialogitem.h"
#include "dialoglist.h"
#include "messageparser.h"
#include "utils.h"

class DialogParser : public QObject
{
public:
    static DialogItem parser(const QVariantMap &item, const ProfileList &profiles);
    static DialogList parser(const QVariantList &items, const ProfileList &profiles);
};

#endif // DIALOGPARSER_H
