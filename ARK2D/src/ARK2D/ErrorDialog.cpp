/*
 * ErrorDialog.cpp
 *
 *  Created on: 23-Dec-2009
 *      Author: Ashley
 */

#include <stdlib.h>
#include <string>

#include "ErrorDialog.h"
#include "ARK2D.h"

#if defined(ARK2D_WINDOWS)
	#include <windows.h>
#elif defined(ARK2D_MACINTOSH)
	#include <Cocoa/Cocoa.h>
#endif

void ErrorDialog::createAndShow(std::string message) {
	#ifdef _WIN32
		MessageBox(NULL, message.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
	#elif defined(ARK2D_MACINTOSH)

		CFStringRef header_ref    = CFStringCreateWithCString( NULL, "",   strlen("")    );
		CFStringRef message_ref  = CFStringCreateWithCString( NULL, message.c_str(),  strlen(message.c_str()) );

		CFOptionFlags result;
		CFUserNotificationDisplayAlert(
			0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL,
			header_ref, message_ref,
			NULL, // default button
			NULL, // alternative button 1
			NULL, // anternative button 2
			&result
		);

		CFRelease( header_ref );
		CFRelease( message_ref );

		// kCFUserNotificationDefaultResponse // default button pressed
		// kCFUserNotificationAlternateResponse // alternative button 1 pressed
		// kCFUserNotificationOtherResponse // alternative button 2 pressed.
		// kCFUserNotificationCancelResponse // notification timed out.

		/*
		 * confirm dialog box
		 *
		//convert the strings from char* to CFStringRef
		CFStringRef header_ref    = CFStringCreateWithCString( NULL, "",   strlen("")    );
		CFStringRef message_ref  = CFStringCreateWithCString( NULL, message.c_str(),  strlen(message.c_str()) );

		CFOptionFlags result;  //result code from the message box

		//launch the message box
		CFUserNotificationDisplayAlert(
			0, // no timeout
			kCFUserNotificationNoteAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
			NULL, //icon url, use default, you can change it depending message_type flags
			NULL, //not used
			NULL, //localization of strings
			header_ref, //header text
			message_ref, //message text
			NULL, //default "ok" text in button
			CFSTR("Cancel"), //alternate button title
			NULL, //other button title, null--> no other button
			&result //response flags
		);

		//Clean up the strings
		CFRelease( header_ref );
		CFRelease( message_ref );
		*/

	#endif
}
