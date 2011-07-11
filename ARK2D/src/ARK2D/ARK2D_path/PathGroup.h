/*
 * PathGroup.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef PATHGROUP_H_
#define PATHGROUP_H_

#include "../ARK2D.h"
#include "../Graphics.h"
#include "../MathUtil.h"
#include "../GameTimer.h"
#include "../Vector2.h"
#include <vector>

#include "Path.h"

using namespace std;

class PathGroup {
	private:
		vector<Path*> paths;
		Vector2<float> calcVector;
		unsigned int current;
		float timer;
		string name;
		string description;

	public:
		PathGroup();
		void addPath(Path* p);
		vector<Path*> getPaths();
		Path* getPath(unsigned int index);

		void setName(string n);
		void setDescription(string d);
		void setTimer(float t);
		float getTimer();
		void incrementTimer(float t);

		float getDuration();

		void update(GameTimer* gt);
		void updateCurrent();

		/*float getStartTime(unsigned int index);*/
		Vector2<float>* getLocation();
		unsigned int getIndex();

		virtual void render();
		virtual void renderPoint(float x, float y, bool linkPoint);
		virtual void renderLine(float x, float y, float x2, float y2);
		virtual ~PathGroup();
};

#endif /* PATHGROUP_H_ */
