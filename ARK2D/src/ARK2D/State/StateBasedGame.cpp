/*
 * StateBasedGame.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include <vector> 
#include "StateBasedGame.h"
#include "GameState.h"
#include "LoadingState.h"
#include "Transition/Transition.h"
#include "Transition/EmptyTransition.h"
#include "../UI/ErrorDialog.h"

namespace ARK {
	namespace State {

		StateBasedGame::StateBasedGame(string title):
			Game(title),
			m_states(),
			m_from_state(NULL),
			m_current_state(NULL),
			m_next_state(NULL), 
			m_container(NULL),
			m_loading_state(NULL), 
			m_enterTransition(NULL),
			m_leaveTransition(NULL),
			m_initialised(false),
			m_autoDeleteTransitions(false) {

		}

		string StateBasedGame::getTitle() {
			return Game::getTitle();
		}
		void StateBasedGame::addState(GameState* state) {
			m_states.push_back(state);
			m_from_state = state;
			//if (m_current_state != NULL && m_current_state == m_loading_state) {
				
			//}
			if (m_current_state == NULL) {
				m_current_state = state;
			}
		}
		void StateBasedGame::setLoadingState(LoadingState* state) {
			m_loading_state = state;
		}
		LoadingState* StateBasedGame::getLoadingState() {
			return m_loading_state;
		}
		void StateBasedGame::enterState(unsigned int id) {
			for (unsigned int i = 0; i < m_states.size(); i++) {
				if (m_states.at(i)->id() == id) {
					enterState(m_states.at(i));
					break;
				}
			}
		}
		void StateBasedGame::enterState(GameState* state) {
			enterState(state, new EmptyTransition(), new EmptyTransition());
		}

		void StateBasedGame::enterState(unsigned int id, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter) {
			GameState* state = NULL;
			for (unsigned int i = 0; i < m_states.size(); i++) {
				if (m_states.at(i)->id() == id) {
					state = m_states.at(i);
					break;
				}
			}
			if (state == NULL) {
				ErrorDialog::createAndShow(StringUtil::append("State with id not found: ", id));
				exit(0);
			} else {
				enterState(state, leave, enter);
			}
		}
		void StateBasedGame::enterState(GameState* state, ARK::State::Transition::Transition* leave, ARK::State::Transition::Transition* enter) {

			m_from_state = m_current_state;
			//m_current_state = state;
			m_next_state = state;

			if (leave == NULL) { // || m_leaveTransition == NULL) {
				ARK2D::getLog()->v("StateBasedGame::enterState - enter was null");
				leave = new EmptyTransition();
			}
			if (enter == NULL) { // || m_enterTransition == NULL) {
				ARK2D::getLog()->v("StateBasedGame::enterState - leave was null");
				enter = new EmptyTransition();
			}

			m_leaveTransition = leave;
			m_enterTransition = enter;

			m_leaveTransition->init(m_container, this, m_from_state, state);
		}
		void StateBasedGame::setCurrentState(GameState* state) {
			m_current_state = state;
		}
		void StateBasedGame::setCurrentState(unsigned int id) {
			for (unsigned int i = 0; i < m_states.size(); i++) {
				if (m_states.at(i)->id() == id) {
					m_current_state = m_states.at(i);
					break;
				}
			}
		}

		GameState* StateBasedGame::getCurrentState() {
			return m_current_state;
		}
		unsigned int StateBasedGame::getCurrentStateID() {
			return m_current_state->id();
		}

		vector<GameState*> StateBasedGame::getStates() {
			return m_states;
		}
		GameState* StateBasedGame::getState(unsigned int id) {
			for(unsigned int i = 0; i < m_states.size(); ++i) {
				GameState* s = m_states.at(i);
				if (s->id() == id) {
					return s;
				}
			}
			return NULL;
		}

		void StateBasedGame::setFromState(GameState* state) {
			m_from_state = state;
		}

		void StateBasedGame::init(GameContainer* container) {
			m_container = container;

			initStates(container);

			for (unsigned int i = 0; i < m_states.size(); i++) {
				GameState* s = m_states.at(i);
				ARK2D::getLog()->i(StringUtil::append("initialising state: ", s->id()));
				s->init(container, this);
			}

			m_initialised = true;
		}

		void StateBasedGame::preUpdate(GameContainer* container, GameTimer* timer) {

		}
		void StateBasedGame::postUpdate(GameContainer* container, GameTimer* timer) {

		}
		void StateBasedGame::update(GameContainer* container, GameTimer* timer) {

			//preUpdate(container, timer);

			Game::update(container, timer);

			if (m_leaveTransition != NULL) {
				//std::cout << "leave wasn't null once! :(" << std::endl;
				m_leaveTransition->update(container, this, timer);
				if (m_leaveTransition->isComplete()) {
					m_from_state->leave(container, this, m_next_state);
					m_leaveTransition->postLeave(container, this, m_from_state, m_next_state);

					if (m_autoDeleteTransitions) { delete m_leaveTransition; }
					m_leaveTransition = NULL;

					if (m_enterTransition == NULL) {
						m_current_state = m_next_state;
						m_current_state->enter(container, this, m_from_state);
					} else {
						m_current_state = m_next_state;
						m_enterTransition->init(container, this, m_from_state, m_next_state);
					}
				} else {
					return;
				}
			}

			if (m_enterTransition != NULL) {
				//std::cout << "enter wasn't null once! :(" << std::endl;
				m_enterTransition->update(container, this, timer);
				if (m_enterTransition->isComplete()) {
					m_current_state->enter(container, this, m_from_state);
					m_enterTransition->postEnter(container, this, m_from_state, m_current_state);

					if (m_autoDeleteTransitions) { delete m_enterTransition; }
					m_enterTransition = NULL;
				} else {
					return;
				}
			}

			//ARK2D::getLog()->v("sbg::update");
			//Image::showAnyGlErrorAndExit();
			if (m_current_state == NULL) {
				ARK2D::getLog()->e("StateBasedGame::update(): Current State is null");
				return;
			}

			m_current_state->update(container, this, timer);

			//postUpdate(container, timer);
		}

		void StateBasedGame::preRender(GameContainer* container, Renderer* r) {
			Game::preRender(container, r);
		}
		void StateBasedGame::postRender(GameContainer* container, Renderer* r) {
			Game::postRender(container, r);
		}
		void StateBasedGame::render(GameContainer* container, Renderer* g) {
			//preRender(container, g);
			//ARK2D::getLog()->v("sbg::render");

			preRenderTransitions();
			
			// Render Current State.
			if (m_current_state != NULL) {
				//ARK2D::getLog()->v("StateBasedGame::render - render current state");
				m_current_state->render(container, this, g);
			}

			postRenderTransitions();
 
			//postRender(container, g);
		}
		void StateBasedGame::preRenderTransitions() {
			// Render post-enter transition 
			GameContainer* container = ARK2D::getContainer();
			Renderer* r = ARK2D::getRenderer();
			if (m_leaveTransition != NULL) {
				m_leaveTransition->preRender(container, this, r);
			} else if (m_enterTransition != NULL) {
				m_enterTransition->preRender(container, this, r);
			}
		}
		void StateBasedGame::postRenderTransitions() {
			// Render post-enter transition 
			GameContainer* container = ARK2D::getContainer();
			Renderer* r = ARK2D::getRenderer();
			if (m_leaveTransition != NULL) {
				m_leaveTransition->postRender(container, this, r);
			} else if (m_enterTransition != NULL) {
				m_enterTransition->postRender(container, this, r);
			}
		}

		bool StateBasedGame::isTransitioning() {
			return (m_enterTransition != NULL || m_leaveTransition != NULL);
		}
		bool StateBasedGame::isInitialised() {
			return m_initialised;
		}

		GameContainer* StateBasedGame::getContainer() {
			return m_container;
		}

		void StateBasedGame::resize(GameContainer* container, int width, int height) 
		{
			Game::resize(container, width, height);
		}

		void StateBasedGame::pause() 
		{
			Game::pause();
		}
		void StateBasedGame::resume()
		{
			Game::resume();
		}


		void StateBasedGame::keyPressed(unsigned int key) {
			Game::keyPressed(key);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }
			
			m_current_state->keyPressed(key);
		}
		void StateBasedGame::keyReleased(unsigned int key) {
			Game::keyReleased(key);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			m_current_state->keyReleased(key);
		}
		void StateBasedGame::mouseMoved(int x, int y, int oldx, int oldy) {
			Game::mouseMoved(x, y, oldx, oldy);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			m_current_state->mouseMoved(x, y, oldx, oldy);
		} 

		void StateBasedGame::gamepadConnected(Gamepad* gamepad) {
			Game::gamepadConnected(gamepad);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			GamepadListener* l = dynamic_cast<GamepadListener*>(m_current_state);
			if (l != NULL) { l->gamepadConnected(gamepad); }
		}
		void StateBasedGame::gamepadDisconnected(Gamepad* gamepad) {
			Game::gamepadDisconnected(gamepad);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			GamepadListener* l = dynamic_cast<GamepadListener*>(m_current_state);
			if (l != NULL) { l->gamepadDisconnected(gamepad); }
		}
		void StateBasedGame::buttonPressed(Gamepad* gamepad, unsigned int button) {
			Game::buttonPressed(gamepad, button);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			GamepadListener* l = dynamic_cast<GamepadListener*>(m_current_state);
			if (l != NULL) { l->buttonPressed(gamepad, button); }
		}
		void StateBasedGame::buttonReleased(Gamepad* gamepad, unsigned int button) {
			Game::buttonReleased(gamepad, button);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			GamepadListener* l = dynamic_cast<GamepadListener*>(m_current_state);
			if (l != NULL) { l->buttonReleased(gamepad, button); }
		}
		void StateBasedGame::axisMoved(Gamepad* gamepad, unsigned int axis, float value) {
			Game::axisMoved(gamepad, axis, value);
			if (m_loading_state != NULL && m_loading_state->isLoading()) { return; }

			GamepadListener* l = dynamic_cast<GamepadListener*>(m_current_state);
			if (l != NULL) { l->axisMoved(gamepad, axis, value); }
		}

		void StateBasedGame::orientationChanged(int orientation) {
			for(unsigned int i = 0; i < m_states.size(); ++i) {
				GameState* state = m_states[i];
				if (state == NULL) { continue; }
				state->orientationChanged(orientation);
			}
		}

		StateBasedGame::~StateBasedGame() { 
			for(unsigned int i = 0; i < m_states.size(); i++) {
				//try { 
					GameState* state = m_states.at(i);
					if (state != NULL) { 
						delete state; 
					}
				//} catch(...) {
				//	ARK2D::getLog()->e(StringUtil::append("Could not delete GameState at index: ", i));
				//	exit(0);
				//}
			}
			m_states.clear();
		}
	}
}
