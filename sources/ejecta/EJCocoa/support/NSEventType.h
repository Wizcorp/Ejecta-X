
#ifndef __NSEVENT_TYPE_H__
#define __NSEVENT_TYPE_H__

/**
 * This header is used for defining event types using in NSNotificationCenter
 */

// The application will come to foreground.
// This message is used for reloading resources before come to foreground on Android.
// This message is posted in main.cpp.
#define EVNET_COME_TO_FOREGROUND    "event_come_to_foreground"

// The application will come to background.
// This message is used for doing something before coming to background, such as save NSRenderTexture.
#define EVENT_COME_TO_BACKGROUND    "event_come_to_background"

#endif // __NSEVENT_TYPE_H__
