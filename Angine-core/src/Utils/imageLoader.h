#pragma once
#include "../common.h"
#include <FreeImage\FreeImage.h>
#include <iostream>
namespace Angine
{
	namespace Utils
	{

		static BYTE *  imageLoader(const char * filename, unsigned int &width, unsigned int &height, unsigned int& BPP) {
			//	, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border
			//image format
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			//pointer to the image, once loaded
			FIBITMAP *dib(0);
			//pointer to the image data
			BYTE* bits(0);
			//image width and height



			//check the file signature and deduce its format
			fif = FreeImage_GetFileType(filename, 0);
			//if still unknown, try to guess the file format from the file extension
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(filename);
			//if still unkown, return failure
			if (fif == FIF_UNKNOWN) {
				std::cout << "couldn't load image: " << filename << '\n';
				return nullptr;
			}

			//check that the plugin has reading capabilities and load the file
			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, filename);
			//if the image failed to load, return failure
			if (!dib)
			{
				std::cout << "couldn't load image: " << filename << '\n';
				return nullptr;
			}
			//flip it 
			FreeImage_FlipVertical(dib);

			//retrieve the image data
			bits = FreeImage_GetBits(dib);
			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			//if this somehow one of these failed (they shouldn't), return failure
			if ((bits == 0) || (width == 0) || (height == 0))
			{
				std::cout << "couldn't load image: " << filename << '\n';
				return nullptr;
			}
			/*	if this texture ID is in use, unload the current texture
			if (m_texID.find(texID) != m_texID.end())
			glDeleteTextures(1, &(m_texID[texID]));

			generate an OpenGL texture ID for this texture
			glGenTextures(1, &gl_texID);
			store the texture ID mapping
			m_texID[texID] = gl_texID;
			bind to the new texture ID
			glBindTexture(GL_TEXTURE_2D, gl_texID);
			store the texture data for OpenGL use
			glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
			border, image_format, GL_UNSIGNED_BYTE, bits);
			*/
			//Free FreeImage's copy of the data
	//		FreeImage_Unload(dib);

		
			BPP = FreeImage_GetBPP(dib);
			return bits;
		}
	}
}
