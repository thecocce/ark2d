/*
 * FileDialog.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef FILEDIALOG_H_
#define FILEDIALOG_H_

#include "../ARK2D.h"
#include <string>
using namespace std;

#if defined (ARK2D_WINDOWS)
	#include <windows.h>
	#include "../vendor/FileInterface.h"
#elif defined(ARK2D_MACINTOSH)
	#import <Cocoa/Cocoa.h>
#endif

namespace ARK {
	namespace UI {

		class ARK2D_API FileDialogPlatformData {
			public:
				#ifdef ARK2D_WINDOWS
					OPENFILENAME opf;
					string title;
					char buffer[256];
				#elif defined(ARK2D_MACINTOSH)

				#endif
		};

		/*!
		 * \brief File dialogs such as open file and save file
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API FileDialog {
			private:
				static void init(FileDialogPlatformData* fdpd);

			public:
				static string openFileDialog();
				static string openFileDialog(string title);

				static string saveFileDialog();
				static string saveFileDialog(string title);
		};
	}
}

#endif /* FILEDIALOG_H_ */
