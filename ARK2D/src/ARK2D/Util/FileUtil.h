
/*
 * FileUtil.h
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

namespace ARK {
	namespace Util {
		/*!
		 * \brief File operations functionality. Framework File IO goes through the Resource class.
		 *
		 * This remains public for custom file types.
		 *
		 * @see Resource
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */

		struct file_get_contents_binary_result {
			const char* data;
			unsigned int len;
		};
		struct file_get_contents_text_result {
			const char* data;
			unsigned int len;
		};

		class ARK2D_API FileUtil {
			private:
			public:
				static string getResourcePath();
				static string prependPlatform(string filename);
				static bool file_put_contents(string filename, string contents);
				static bool file_put_contents(string filename, string contents, bool doPrependPlatform);
				static bool file_put_contents(string filename, const char* data, unsigned int len);



				static file_get_contents_binary_result file_get_contents_utf8binary(string filename);
				static file_get_contents_binary_result file_get_contents_binary(string filename);
				static file_get_contents_text_result file_get_contents_text(string filename);


				static string getCurrentDirectory();
				static string getSeparator();
				static string getOSDirectory();

				static void openBrowserToURL(string url);
				static void openGalleryToImageUrl(string url);
				static void openGooglePlayStore(string packageName);
				static bool openSoftwareKeyboard();

				static string getOSUsername(); // TODO: not sure where to put this. would seem odd in a class by itself, no?


		};
	}
}

#endif /* FILEUTIL_H_ */
