/*
 * AlphaMaskShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_ALPHAMASKSHADER_H_
#define ARK_GRAPHICS_SHADERS_ALPHAMASKSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API AlphaMaskGeometryShader : public Shader {
			public:
				Shader* m_shaderBasicGeometry;
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
                unsigned int _NormalMatrix;
				unsigned int _VertexPositionIn;
                unsigned int _VertexNormalIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicGeometry_ModelMatrix;
				unsigned int m_shaderBasicGeometry_ViewMatrix;
				unsigned int m_shaderBasicGeometry_ProjectionMatrix;
				unsigned int m_shaderBasicGeometry_NormalMatrix;
				unsigned int m_shaderBasicGeometry_VertexPositionIn;
				unsigned int m_shaderBasicGeometry_VertexNormalIn;
				unsigned int m_shaderBasicGeometry_VertexColorIn;

				AlphaMaskGeometryShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
				static void startStatic(AlphaMaskGeometryShader* obj);
				static void stopStatic(AlphaMaskGeometryShader* obj);
				virtual ~AlphaMaskGeometryShader();
		};
		class ARK2D_API AlphaMaskTextureShader : public Shader {
			public:
				Shader* m_shaderBasicTexture;
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _NormalMatrix;

				unsigned int _TextureId;
				unsigned int _VertexPositionIn;
				unsigned int _VertexNormalIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicTexture_ModelMatrix;
				unsigned int m_shaderBasicTexture_ViewMatrix;
				unsigned int m_shaderBasicTexture_ProjectionMatrix;
				unsigned int m_shaderBasicTexture_NormalMatrix;
				unsigned int m_shaderBasicTexture_VertexPositionIn;
				unsigned int m_shaderBasicTexture_VertexNormalIn;
				unsigned int m_shaderBasicTexture_VertexTexCoordIn;
				unsigned int m_shaderBasicTexture_VertexColorIn;
				unsigned int m_shaderBasicTexture_TextureId;

				AlphaMaskTextureShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
				static void startStatic(AlphaMaskTextureShader* obj);
				static void stopStatic(AlphaMaskTextureShader* obj);
				virtual ~AlphaMaskTextureShader();
		};
		class ARK2D_API AlphaMaskShader {
			public:
				AlphaMaskGeometryShader* geometry;
				AlphaMaskTextureShader* texture;
				
			public:
				AlphaMaskShader();
				void load();
				void start();
				void stop();
				virtual ~AlphaMaskShader();
		};
	}
}

#endif
