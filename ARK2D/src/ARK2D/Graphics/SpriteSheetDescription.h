/*
 * SpriteSheetDescription.h
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#ifndef SPRITESHEETDESCRIPTION_H_
#define SPRITESHEETDESCRIPTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>

#include "SpriteSheetDescriptionItem.h"

using namespace std;

/*!
 * \brief Read a spritesheet description file produce by a Sprite Sheet tool.
 * It can read JSON or CSV.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class SpriteSheetDescription {
	public:
		SpriteSheetDescription(const char* path);
		SpriteSheetDescription(string path);
		const SpriteSheetDescriptionItem& getItemByName(const char* name);
		~SpriteSheetDescription();

	private:
		string m_path;
		map<string, SpriteSheetDescriptionItem> m_items;
		void load();
};

#endif /* SPRITESHEETDESCRIPTION_H_ */