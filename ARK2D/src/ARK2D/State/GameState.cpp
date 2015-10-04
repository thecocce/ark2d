/*
 * GameState.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include "GameState.h"
#include "StateBasedGame.h"

#include "../Namespaces.h"

namespace ARK {
	namespace State {

		GameState::GameState(): 
			SceneNode("GameState",TYPE_NODE)
		{

		}
		GameState::GameState(string nm):
			SceneNode(nm,TYPE_NODE)
		{

		}

		void GameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
			ARK2D::getLog()->v("GameState::enter");
		}
		void GameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
			ARK2D::getLog()->v("GameState::leave");
		}

		void GameState::pause() { 
			ARK2D::getLog()->v("GameState::pause");
		}
		void GameState::resume() { 
			ARK2D::getLog()->v("GameState::resume");
		}

		void GameState::render() {
			if (!visible) { return; }
            StateBasedGame* sbg = dynamic_cast<StateBasedGame*>(ARK2D::getGame());
			render(ARK2D::getContainer(), sbg, ARK2D::getRenderer());
		}


		bool GameState::keyPressed(unsigned int key) {
			return SceneNode::keyPressed(key);
		}
		bool GameState::keyReleased(unsigned int key) {
			return SceneNode::keyReleased(key);
		}
		bool GameState::mouseMoved(int x, int y, int oldx, int oldy) {
			return SceneNode::mouseMoved(x,y,oldx,oldy);
		}
		void GameState::orientationChanged(int orientation) {

		}
		GameState::~GameState() {
			// hmm
		}

	}
}
