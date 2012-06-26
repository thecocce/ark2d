/*
 * Resource.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: ashleygwinnell
 */

#include "../Namespaces.h"
#include "../ARK2D.h"
#include "Resource.h"

#include "../Graphics/Image.h"
#include "../Graphics/SpriteSheetDescription.h"
#include "../Audio/Sound.h"
#include "../Font/FTFont.h"
#include "../Font/BMFont.h"
#include "../Font/Font.h"
#include "../Path/PathGroup.h"
#include "../Path/PathIO.h"
#include "../Util/LocalHighscores.h"
#include "../Tiled/TiledMap.h"
#include "GameContainer.h"
#include "../Util/StringUtil.h"
#include "String.h"

namespace ARK {
	namespace Core {
		string Resource::s_latestName;

		#if defined(ARK2D_ANDROID)
			zip* Resource::apkZip = NULL;
			string Resource::apkZipName = "";
		#endif

		string Resource::latestName() {
			return s_latestName;
		}

		Resource* Resource::get(string ref) {
			return get(ref, true);
		}

		Resource* Resource::get(string ref, bool appendPath) {
			s_latestName = ref;

			GameContainer* container = ARK2D::getContainer();
			string oldref = ref;

			if (ref.substr(1,1).compare(":") == 0 || ref.substr(0,1).compare("/") == 0) { appendPath = false; }
			if (appendPath) {
				ref = container->getResourcePath() + ref;
			}

			ARK2D::getLog()->i(StringUtil::append("Loading Resource: ", ref));

			Resource* resource = NULL;
			string extension = StringUtil::getExtension(ref);
			if (extension == "fnt")
			{
				//string pngref = ref.substr(0, ref.find_last_of(".")) + ".png";
				string pngref = oldref.substr(0, oldref.find_last_of(".")) + ".png";
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);

					Image* imgData = get(pngref)->asImage();
					resource = new ARK::Font::BMFont(rt->data, imgData);

					delete rt;
				#else
					resource = new ARK::Font::BMFont(ref, pngref);
				#endif
			}
			else if (extension == "tmx")
			{
				ARK::Tiled::TiledMap* map = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);
					map = new ARK::Tiled::TiledMap(ref, rt->data);
				#else
					map = new ARK::Tiled::TiledMap(ref);
				#endif
				resource = map;
			}
			else if (extension == "localhighscores")
			{
				LocalHighscores* scores = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);
					scores = new LocalHighscores(ref, rt->data);
				#else
					scores = new LocalHighscores(oldref);
				#endif
				resource = scores;
			}
			else if (extension == "spritesheetdescription")
			{
				SpriteSheetDescription* desc = NULL;
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);
					desc = new SpriteSheetDescription(ref, rt->data);
				#else
					desc = new SpriteSheetDescription(oldref);
				#endif
				resource = desc;
			}
			else if (extension == "png" || extension == "bmp" || extension == "tga")
			{ // Image
				#if defined(ARK2D_ANDROID)
					ARK2D::getLog()->i("Creating raw resource data... ");
					RawDataReturns* rt = getRawData(ref);
					void* fileData = rt->data;

					ARK2D::getLog()->i("Creating resource type from data... ");
					resource = new Image(fileData, getResourceTypeByExtension(extension));

					ARK2D::getLog()->i("Freeing raw resource data... ");
					delete rt;
				#else
					resource = new Image(ref);
				#endif
			}
			else if (extension == "wav" || extension == "ogg")
			{ // Sound
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);
					resource = new Sound(ref, rt->data, rt->size);
					delete rt;
				#else
					resource = new Sound(ref);
				#endif
			}
			else if (extension == "path") 
			{
				#if defined(ARK2D_ANDROID)
					ErrorDialog::createAndShow("Path implementation not on Android.");
				#else
					resource = PathIO::createFromFile(ref);
				#endif
			}
			else
			{ // Assume plain text.
				String* arkstr = new String();
				#if defined(ARK2D_ANDROID)
					RawDataReturns* rt = getRawData(ref);
					char* newtextbuffer = (char*) malloc(rt->size+1);
					memcpy(newtextbuffer, rt->data, rt->size);
					newtextbuffer[rt->size] = '\0';
					arkstr->append(newtextbuffer);
					delete rt;
					free(newtextbuffer);

				#else
					string s = StringUtil::file_get_contents(oldref.c_str());
					arkstr->append(s);
				#endif
				resource = arkstr;
			}

			ARK2D::getLog()->i(StringUtil::append("Loaded Resource: ", ref));

			return resource;
		}
		RawDataReturns* Resource::getRawData(string ref) {
			#if defined(ARK2D_ANDROID)
				if (apkZip == NULL) {
					apkZip = zip_open(apkZipName.c_str(), 0, NULL);
					if (apkZip == NULL) {
						ARK2D::getLog()->i(StringUtil::append("Error Loading APK: ", apkZipName));
						return NULL;
					}
					//Just for debug, print APK contents
					int numFiles = zip_get_num_files(apkZip);
					for (int i=0; i<numFiles; i++)
					{
						const char* name = zip_get_name(apkZip, i, 0);
						if (name == NULL)
						{
							String thisErr("Error reading zip file name at index ");
							thisErr += i;
							thisErr += " : ";
							thisErr += zip_strerror(apkZip);
							ARK2D::getLog()->i(thisErr.get());
							return NULL;
						}
						String thisErr("File ");
						thisErr += i;
						thisErr += " : ";
						thisErr += name;
						ARK2D::getLog()->i(thisErr.get());
					}
				}
				zip_file* file;
				file = zip_fopen(apkZip, ref.c_str(), 0); // ref = filename
				if (!file) {
					ARK2D::getLog()->i(StringUtil::append("Could not open file in zip: ", ref));
					return NULL;
				}
				struct zip_stat fileStats;
				zip_stat(apkZip, ref.c_str(), 0, &fileStats); // ZIP_FL_NOCASE | ZIP_FL_NODIR

				String fileStatsStr("Resource: ");
				fileStatsStr += ref;
				fileStatsStr += ". uncompressed size: ";
				fileStatsStr += (int) fileStats.size;
				fileStatsStr += ". compressed size: ";
				fileStatsStr += (int) fileStats.comp_size;
				ARK2D::getLog()->i(fileStatsStr.get());

				char* fileUncompressed = (char*) malloc((int) fileStats.size);
				int fileUncompressedPointer = 0;
				unsigned int fileUncompressedBufferSize = 4096;
				if (fileUncompressedBufferSize > (int) fileStats.size) {
					fileUncompressedBufferSize = (int) fileStats.size;
				}
				for(signed int i = 0; i < (int) fileStats.size; i += fileUncompressedBufferSize) {
					char fileUncompressedBuffer[fileUncompressedBufferSize];
					signed int done = zip_fread(file, &fileUncompressedBuffer, fileUncompressedBufferSize);
					if (done == -1) { break; }
					memcpy(fileUncompressed + i, &fileUncompressedBuffer[0], done);
				}
				zip_fclose(file);

				RawDataReturns* rt = new RawDataReturns();
				rt->data = (void*) fileUncompressed;
				rt->size = (int) fileStats.size;
				return rt;
			#else
				return NULL;
			#endif
		}
		unsigned int Resource::getResourceTypeByExtension(string extension) {
			if (extension == "tga") {
				return ARK2D_RESOURCE_TYPE_TGA;
			} else if (extension == "bmp") {
				return ARK2D_RESOURCE_TYPE_BMP;
			} else if (extension == "png") {
				return ARK2D_RESOURCE_TYPE_PNG;
			} else if (extension == "fnt") {
				return ARK2D_RESOURCE_TYPE_FNT;
			} else if (extension == "localhighscores") {
				return ARK2D_RESOURCE_TYPE_LOCAL_HIGHSCORES;
			} else if (extension == "tmx") {
				return ARK2D_RESOURCE_TYPE_TILED_MAP;
			} else if (extension == "spritesheetdescription") {
				return ARK2D_RESOURCE_TYPE_SPRITESHEET_DESCRIPTION;
			} else if (extension == "txt") {
				return ARK2D_RESOURCE_TYPE_TXT;
			}
			return 0;
		}

		Resource::Resource() {

		}

		Image* Resource::asImage() {
			return dynamic_cast<Image*>(this);
		}
		Sound* Resource::asSound() {
			return dynamic_cast<ARK::Audio::Sound*>(this);
		}
		ARK::Font::Font* Resource::asFont() {
			return dynamic_cast<ARK::Font::Font*>(this);
		}
		LocalHighscores* Resource::asLocalHighscores() {
			return dynamic_cast<ARK::Util::LocalHighscores*>(this);
		}
		PathGroup* Resource::asPathGroup() {
			return dynamic_cast<ARK::Path::PathGroup*>(this);
		}
		TiledMap* Resource::asTiledMap() {
			return dynamic_cast<ARK::Tiled::TiledMap*>(this);
		}
		SpriteSheetDescription* Resource::asSpriteSheetDescription() {
			return dynamic_cast<ARK::Graphics::SpriteSheetDescription*>(this);
		}
		String* Resource::asString() {
			return dynamic_cast<String*>(this);
		}

		Resource::~Resource() {

		}

	}
}