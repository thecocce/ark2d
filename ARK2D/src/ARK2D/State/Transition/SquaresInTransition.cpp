/*
 * SquaresInTransition.cpp
 *
 *  Created on: 13 Oct 2012
 *      Author: Ashley
 */

#include "SquaresInTransition.h"

#include "../../Graphics/Color.h"
#include "../GameState.h"
#include "../StateBasedGame.h"
#include "../../Tween/Easing.h"
#include <algorithm>

namespace ARK {
	namespace State {
		namespace Transition {

			SquaresInTransition::SquaresInTransition(float duration, float squareSize):
				Transition(),
				m_timer(0.0f),
				m_duration(duration),
				m_squareSize(squareSize),
				m_squares(),
				m_squaresFilled(),
				m_squaresEmptied(0),
				m_squaresTotal(0),
				m_color(NULL),
				m_squaresX(5),
				m_squaresY(5)
				{
 				m_color = const_cast<Color*>(&Color::black);
			}
			SquaresInTransition::SquaresInTransition(float duration, float squareSize, Color* c):
				Transition(),
				m_timer(0.0f),
				m_duration(duration), 
				m_squareSize(squareSize),
				m_squares(),
				m_squaresFilled(),
				m_squaresEmptied(0),
				m_squaresTotal(0),
				m_color(c),
				m_squaresX(5),
				m_squaresY(5)
				{
		
			}
			void SquaresInTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				Transition::init(container, game, from, to);

				m_timer = 0.0f; 

				m_squaresX = (int) (float(container->getDynamicWidth()) / m_squareSize);
				m_squaresY = (int) (float(container->getDynamicHeight()) / m_squareSize);

				for(int i = 0; i < m_squaresX; i++) {
					for(int j = 0; j < m_squaresY; j++) {
						m_squares.push_back(true);
					}
				}

				for(unsigned int i = 0; i < m_squares.size(); i++) {
					m_squaresFilled.push_back(i);
				}
				std::random_shuffle(m_squaresFilled.begin(), m_squaresFilled.end());

				m_squaresEmptied = 0;
				m_squaresTotal = m_squaresX * m_squaresY;
				
			}
			void SquaresInTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) 
			{
				m_timer += timer->getDelta();
				if (m_timer > m_duration) {
					m_timer = m_duration;
				}

				while (m_timer / m_duration > float(m_squaresEmptied) / float(m_squaresTotal)) 
				{
					int index = m_squaresFilled.back();
					m_squaresFilled.pop_back();
					m_squares[index] = false;
					m_squaresEmptied++;
				}
 
			}
			void SquaresInTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

			}
			void SquaresInTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) 
			{
				g->setDrawColorf(m_color->getRedf(), m_color->getGreenf(), m_color->getBluef(), m_color->getAlphaf());

				g->getBatch()->setEnabled(true);

				for(signed int x = 0; x < m_squaresX; x++) 
				{
					for(signed int y = 0; y < m_squaresY; y++) 
					{
						unsigned int gid = (y * m_squaresX) + x;
						if (m_squares.at(gid)) {
							g->fillRect(x*m_squareSize, y*m_squareSize, (int) m_squareSize, (int) m_squareSize);
						}
					}
					
				}

				g->getBatch()->render(); 
				g->getBatch()->setEnabled(false);
				
			}
			bool SquaresInTransition::isComplete() {
				if (m_timer >= m_duration) {
					return true;
				}
				return false;
			}

			SquaresInTransition::~SquaresInTransition() {

			}
		}
	}
}
