/*
 * FadeTransition.cpp
 *
 *  Created on: 22nd Nov 2010
 *      Author: Ashley
 */

#include "FadeTransition.h"

#include "../GameState.h"
#include "../StateBasedGame.h"
#include "../../Graphics/Renderer.h"

namespace ARK {
	namespace State {
		namespace Transition {

			FadeTransition::FadeTransition(unsigned int timeMillis):
				Transition(),
				m_time(timeMillis),
				m_time_current(0),
				m_alpha(1.0f)
				{

			}
			void FadeTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				Transition::init(container, game, from, to);

				int width = container->getWidth();
				int height = container->getHeight();

				m_time_current = 0;
				m_alpha = 1.0f;

				

				#ifdef ARK2D_WINDOWS_VS 
					void* data = (void*) malloc(width * height);
				#else 
					void* data[width * height];
				#endif 
				
				Renderer* r = ARK2D::getRenderer();
				r->readPixels(&data, 0, 0, width, height);

				
				unsigned Object(0);

				#if defined(ARK2D_RENDERER_OPENGL)
				
					// Generate one texture (we're creating only one).
					glGenTextures(1, &Object);

					// bind?
					glBindTexture(GL_TEXTURE_2D, Object);

					// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					// Create the actual texture object.
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
					RendererStats::s_textureAllocatedBytes += (width * height * 8 * 4);

				#elif defined(ARK2D_RENDERER_DIRECTX) 

				#endif

				#ifdef ARK2D_WINDOWS_VS 
					free(data);
				#endif 
				
				m_textureId = Object;

			}
			void FadeTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				m_alpha = (float) Easing::ease(m_easing, double(m_time_current), 1.0, -1.0, double(m_time));
				m_time_current += int(timer->getDelta() * 1000.0f);
			}
			void FadeTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

			}
			void FadeTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

				#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
					// ...
				#else

					g->setDrawColorf(1.0f, 1.0f, 1.0f, m_alpha); //glColor4f(1.0f, 1.0f, 1.0f, m_alpha);

					int m_Width = container->getWidth();
					int m_Height = container->getHeight();

					/*glBindTexture(GL_TEXTURE_2D, m_textureId);
					glPushMatrix();
					glBegin( GL_QUADS );
						glTexCoord2f( 0, 1.0f);
						glVertex2f(0, 0);

						glTexCoord2f( 0, 0 ); 
						glVertex2f(0, m_Height);

						glTexCoord2f( 1.0f, 0.0f );
						glVertex2f(m_Width, m_Height);

						glTexCoord2f(1.0f, 1.0f );
						glVertex2f(m_Width, 0);
					glEnd();
					glPopMatrix();
					glBindTexture(GL_TEXTURE_2D, 0);*/

					RendererState::start(RendererState::TEXTURE, m_textureId);
					g->fillRect(0, 0, m_Width, m_Height);

				#endif

			}
			bool FadeTransition::isComplete() {
				if (m_time_current >= m_time) {
					return true;
				}
				return false;
			}

			FadeTransition::~FadeTransition() {

			}
		}
	}
}
