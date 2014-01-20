#ifndef ATTACHMENTSPARSER_H
#define ATTACHMENTSPARSER_H

#include <QObject>
#include <QVariant>
#include <QMetaEnum>
#include <QDebug>
#include "attachmentlist.h"
#include "photoparser.h"

class AttachmentsParser : public QObject
{
public:
    static AttachmentList parser(const QVariantList &items);
};

#endif // ATTACHMENTSPARSER_H
