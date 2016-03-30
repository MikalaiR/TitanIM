#include "servicemsgitem.h"

ServiceMsgItemPrivate::ServiceMsgItemPrivate()
{
    setMessageType(Service);
    setIsFake(true);
}

QString ServiceMsgItemPrivate::body() const
{
    return _body;
}

void ServiceMsgItemPrivate::setBody(const QString &body)
{
    if (_body != body)
    {
        _body = body;
        emitPropertyChanged("body");
    }
}
