/*
 * Slider.cpp
 *
 *  Created on: 16 Apr 2014
 *      Author: Ashley
 */

#include "Slider.h"   
#include "../Tween/Easing.h"  
#include "../Geometry/Shape.h"  

namespace ARK { 
	namespace UI { 

		Slider::Slider(): 
			AbstractUIComponent(),
			m_buttonLocation(40, 4),
			
			bar(NULL),
			button(NULL),
			
			m_value(0.0f),
			m_actualValue(0.0f),
			lowValue(0.0f),
			highValue(1.0f),

			m_dragging(false),
			m_alpha(1.0f),

			m_snapping(false),
			m_snapTo(0.0f)
		{
			setBounds(40, 0, 240, 8);
		}

		void Slider::setRange(float low, float high) {
			lowValue = low; 
			highValue = high; 
		}

		void Slider::updateValue()
		{
			float intermediary = Easing::ease(Easing::LINEAR, m_buttonLocation.getX() - getMinX(), 0.0f, 1.0f, getMaxX() - getMinX());
			m_value = intermediary;

			if (m_snapping) {
				signed int divisions = round(intermediary / m_snapTo);
				m_value = divisions * m_snapTo; 
			}

			m_actualValue = lowValue + (m_value * highValue);
		}

		void Slider::setButtonPosition(float percentage) {
			m_value = percentage;
			m_actualValue = lowValue + (m_value * highValue);

			refreshButtonPosition();
		}
		void Slider::refreshButtonPosition() {
			float val = getMinX() + (m_value * getWidth());
			m_buttonLocation.setX(val);
			m_buttonLocation.setY(getCenterY());
		}
		void Slider::setBarImage(Image* img)
		{
			bar = img; 
		}
		void Slider::setButtonImage(Image* img) 
		{
			button = img;
		}
		void Slider::setBounds(float x, float y, float w, float h) 
		{
			setSize(w, h);
			setLocation(x, y);
			
			refreshButtonPosition();
		}
		void Slider::setLocation(float x, float y) {
			AbstractUIComponent::setLocation(x, y);
			refreshButtonPosition();
		}
		void Slider::setLocationByCenter(float x, float y) {
			AbstractUIComponent::setLocationByCenter(x, y);
			refreshButtonPosition();
		}
		void Slider::render() 
		{
			Renderer* r = ARK2D::getRenderer();


			

			if (bar != NULL) {
				bar->setAlpha(m_alpha);
				//bar->draw(m_bounds.getMinX(), m_bounds.getMinY(), m_bounds.getWidth(), m_bounds.getHeight());
				bar->drawCentered(getCenterX(), getCenterY());
			} else {
				r->setDrawColor(Color::white);
				r->fillRect(getMinX(), getMinY(), (int) getWidth(), (int) getHeight());
			}

			if (button != NULL) {
				button->setAlpha(m_alpha);
				button->drawCentered(m_buttonLocation.getX(), m_buttonLocation.getY());
			} else {
				r->setDrawColor(Color::white_50a);
				r->fillCircle(m_buttonLocation.getX(), m_buttonLocation.getY(), 10, 20);
				r->setLineWidth(2);
				r->drawCircle(m_buttonLocation.getX(), m_buttonLocation.getY(), 10, 20);
				r->setLineWidth(1);
			}
		 }

		void Slider::keyPressed(unsigned int key) 
		{
			if (key != (unsigned int) Input::MOUSE_BUTTON_LEFT) { return; }

			Input* i = ARK2D::getInput();
			if (!m_dragging) 
			{
				bool collides = Shape<float>::collision_circleCircle(i->getMouseX(), i->getMouseY(), 15.0f, m_buttonLocation.getX(), m_buttonLocation.getY(), 15.0f);
				if (collides) {
					m_dragging = true;
					updateValue();
				}
			}
		}

		void Slider::keyReleased(unsigned int key) 
		{
			if (key != (unsigned int) Input::MOUSE_BUTTON_LEFT) { return; }

			m_dragging = false;
			updateValue();
		}

		void Slider::mouseMoved(int x, int y, int oldx, int oldy)
		{
			if (m_dragging) 
			{
				
				int newx = x;
				if (m_snapping) {
					int snapx = (getMaxX() - getMinX()) * m_snapTo;
					signed int divisions = round(newx / snapx);
					newx = divisions * snapx; 
				}

				if (newx < getMinX()) { newx = (int) getMinX(); }
				else if (newx > getMaxX()) { newx = (int) getMaxX(); }

				m_buttonLocation.setX(newx);

				updateValue();
			}
		}

		Slider::~Slider() 
		{

		}

	}
}



