#include "profilelist.h"

ProfileList::ProfileList()
{
}

void ProfileList::add(ProfileItem *profile)
{
    _profiles[profile->uid()] = profile;
}

void ProfileList::add(ProfileList *profiles)
{
    //    _profiles << profiles; //todo
}

ProfileItem* ProfileList::at(const int uid) const
{
    return _profiles.value(uid);
}

int ProfileList::count() const
{
    return _profiles.count();
}
