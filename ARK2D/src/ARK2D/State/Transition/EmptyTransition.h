/*
 * EmptyTransition.h
 *
 *  Created on: 1 Aug 2010
 *      Author: Ashley
 */

#ifndef EMPTYTRANSITION_H_
#define EMPTYTRANSITION_H_

#include "Transition.h"

class EmptyTransition : public Transition {
	public:
		EmptyTransition();
		void init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* delta);
		void preRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		void postRender(GameContainer* container, StateBasedGame* game, Graphics* g);
		bool isComplete();
		virtual ~EmptyTransition();
};

#endif /* EMPTYTRANSITION_H_ */