/*
 * StencilShader.cpp
 *
 *  Created on: 5 Jun 2015
 *      Author: Ashley
 */

#include "../Shader.h"
#include "StencilShader.h"

namespace ARK { 
	namespace Graphics { 

		StencilShader::StencilShader(): 
			ARK::Graphics::Shader()
		{
			
		}
		void StencilShader::load() {

			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/stencil-glsles100-vertex.txt");
				addFragmentShader("ark2d/shaders/stencil-glsles100-fragment.txt");
			} else {
				addVertexShader("ark2d/shaders/stencil-glsl150-vertex.txt");
				bool err1 = hasError();
				addFragmentShader("ark2d/shaders/stencil-glsl150-fragment.txt");
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/stencil-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/stencil-glsl130-fragment.txt");
				}
			}

			bindAttributeLocation(0, "ark_TextureId");
			bindAttributeLocation(1, "ark_VertexPosition");
			bindAttributeLocation(2, "ark_VertexTexCoordIn");
			bindAttributeLocation(3, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			RendererState::start(RendererState::SHADER, getId());

			_ModelViewMatrix = getUniformVariable("ark_ModelViewMatrix");
			_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			
			_TextureId = getUniformVariable("ark_TextureId");
			_VertexPositionIn = 1;
			_VertexTexCoordIn = 2;
			_VertexColorIn = 3;
			
			RendererState::start(RendererState::GEOMETRY);
		}

		void StencilShader::start() {
			//Shader::bind();

			// Replace existing texture shader... how novel! 
			m_shaderBasicTexture 					= Renderer::s_shaderBasicTexture;
			m_shaderBasicTexture_ModelViewMatrix	= Renderer::s_shaderBasicTexture_ModelViewMatrix;
			m_shaderBasicTexture_ProjectionMatrix 	= Renderer::s_shaderBasicTexture_ProjectionMatrix;
			m_shaderBasicTexture_VertexPositionIn 	= Renderer::s_shaderBasicTexture_VertexPosition;
			m_shaderBasicTexture_VertexTexCoordIn 	= Renderer::s_shaderBasicTexture_VertexTexCoordIn;
			m_shaderBasicTexture_VertexColorIn 		= Renderer::s_shaderBasicTexture_VertexColorIn;
			m_shaderBasicTexture_TextureId 			= Renderer::s_shaderBasicTexture_TextureId;

			Renderer::s_shaderBasicTexture 					= this;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 	= _ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = _ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= _VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = _VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= _VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= _TextureId;


		} 

		void StencilShader::stop() {
			//Shader::unbind();

			Renderer::s_shaderBasicTexture 					= m_shaderBasicTexture;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 	= m_shaderBasicTexture_ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = m_shaderBasicTexture_ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= m_shaderBasicTexture_VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = m_shaderBasicTexture_VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= m_shaderBasicTexture_VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= m_shaderBasicTexture_TextureId;
		}
		  
		StencilShader::~StencilShader() { 
			 
		}
	}
}




