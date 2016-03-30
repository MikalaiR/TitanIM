/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "macnotification.h"

MacNotification *MacNotification::aInstance = 0;

@interface NotifyHandler : NSObject<NSUserNotificationCenterDelegate> {
}
- (void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification;
- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification;
@end

@implementation NotifyHandler {
}
- (instancetype)init
{
    self = [super init];

    if (self)
    {
        [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
    }

    return self;
}

- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
    NSDictionary *dict = [notification userInfo];

    NSNumber *midObj = [dict objectForKey:@"mid"];
    int mid = midObj ? [midObj intValue] : 0;

    NSNumber *peerObj = [dict objectForKey:@"peer"];
    int peer = peerObj ? [peerObj intValue] : 0;

    if (notification.activationType == NSUserNotificationActivationTypeReplied)
    {
        QString msg = QString([[[notification response] string] UTF8String]);
        MacNotification::instance()->notificationReplied(peer, mid, msg);
    }
    else if (notification.activationType == NSUserNotificationActivationTypeContentsClicked)
    {
        MacNotification::instance()->notificationClicked(peer, mid);
    }

    [center removeDeliveredNotification: notification];
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
    return YES;
}
@end

MacNotification *MacNotification::instance()
{
    if (!aInstance) aInstance = new MacNotification();
    return aInstance;
}

void MacNotification::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

MacNotification::MacNotification()
{
    NotifyHandler *notifyHandler = [[[NotifyHandler alloc] init] autorelease];
}

void MacNotification::showNotification(const int peer, const int mid, const QString &title, const QString &msg, const bool withReply, const QPixmap &pixmap)
{
    if (hasUserNotificationCenterSupport()) {
        NSUserNotification *userNotification = [[[NSUserNotification alloc] init] autorelease];

        userNotification.title = title.toNSString();
        userNotification.informativeText = msg.toNSString();
        if (!pixmap.isNull()) userNotification.contentImage = QtMac::toNSImage(pixmap);
        if (withReply) userNotification.hasReplyButton = YES;
        userNotification.userInfo = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:peer], @"peer", [NSNumber numberWithInt:mid], @"mid", nil];

        [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:userNotification];
    }
}

void MacNotification::clearAllNotifications()
{
    [[NSUserNotificationCenter defaultUserNotificationCenter] removeAllDeliveredNotifications];
}

bool MacNotification::hasUserNotificationCenterSupport(void)
{
    Class possibleClass = NSClassFromString(@"NSUserNotificationCenter");

    if (possibleClass != nil)
    {
        return true;
    }

    return false;
}

