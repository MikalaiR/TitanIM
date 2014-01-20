#include "attachmentsparser.h"

AttachmentList AttachmentsParser::parser(const QVariantList &items)
{
    AttachmentList attachments = AttachmentList::create();

    const QMetaObject &metaObject = AttachmentItemPrivate::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AttachmentType"));

    AttachmentItemPrivate::AttachmentType type;

    foreach (QVariant item, items)
    {
        QString strType = item.toMap()["type"].toString();
        QVariantMap attachment = item.toMap()[strType].toMap();

        type = (AttachmentItemPrivate::AttachmentType)metaEnum.keyToValue(strType.toUtf8());

        switch (type) {
        case AttachmentItemPrivate::photo:
        {
            PhotoItem photo = PhotoParser::parser(attachment);
            attachments->add(photo);
            break;
        }

        case AttachmentItemPrivate::unknown:
        {
            qWarning() << "AttachmentsParser: unknown attachment" << strType;
            break;
        }

        default:
        {
            qWarning() << "AttachmentsParser: not implemented parser" << strType;
            break;
        }
        }
    }

    return attachments;
}
