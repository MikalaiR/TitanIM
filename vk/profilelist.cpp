#include "profilelist.h"

ProfileList::ProfileList()
{
}

void ProfileList::add(ProfileItem *profile)
{
    _profiles.append(profile);
}

void ProfileList::add(const ProfileList *profiles)
{
    _profiles << profiles->toVector();
}

int ProfileList::indexOf(const int uid) const
{
    for (int i = 0; i < _profiles.count(); i++)
    {
        if (_profiles.at(i)->uid() == uid)
            return i;
    }
}

ProfileItem* ProfileList::at(const int i) const
{
    return _profiles.at(i);
}

ProfileItem *ProfileList::item(const int uid) const
{
    foreach (ProfileItem *item, _profiles)
    {
        if (item->uid() == uid)
            return item;
    }
}

void ProfileList::remove(const int i)
{
    _profiles.remove(i);
}

int ProfileList::count() const
{
    return _profiles.count();
}

QVector<ProfileItem *> ProfileList::toVector() const
{
    return _profiles;
}
