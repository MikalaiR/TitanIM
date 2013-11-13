#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QObject>
#include <QVector>
#include "profileitem.h"

class ProfileList : public QObject
{
    Q_OBJECT

public:
    ProfileList();
    void add(ProfileItem *profile);
    void add(const ProfileList *profiles);
    int indexOf(const int uid) const;
    ProfileItem* at(const int i) const;
    ProfileItem* item(const int uid) const;
    void remove(const int i);
    int count() const;
    QVector<ProfileItem*> toVector() const;

private:
    QVector<ProfileItem*> _profiles;
};

#endif // PROFILELIST_H
