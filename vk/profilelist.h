#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QObject>
#include <QMap>
#include "profileitem.h"

class ProfileList : public QObject
{
    Q_OBJECT

public:
    ProfileList();
    void add(ProfileItem *profile);
    void add(ProfileList *profiles);
    ProfileItem* at(const int uid) const;
    int count() const;

private:
    QMap<int, ProfileItem*> _profiles;
};

#endif // PROFILELIST_H
