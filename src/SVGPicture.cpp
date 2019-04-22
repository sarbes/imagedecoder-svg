/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <kodi/addon-instance/ImageDecoder.h>
#include <iostream>

/*#include <stdio.h>
#include <string.h>
#include <math.h>*/
#define NANOSVG_ALL_COLOR_KEYWORDS	// Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION		// Expands implementation
#define NANOSVGRAST_IMPLEMENTATION		// Expands implementation
extern "C" {
#include "../lib/nanosvg/nanosvg.h"
#include "../lib/nanosvg/nanosvgrast.h"
}

class MPOPicture : public kodi::addon::CInstanceImageDecoder
{
public:
  MPOPicture(KODI_HANDLE instance)
    : CInstanceImageDecoder(instance)
  {
  }

  virtual ~MPOPicture()
  {
	  nsvgDelete(image);
    nsvgDeleteRasterizer(rast);
  }

  virtual bool LoadImageFromMemory(unsigned char* buffer, unsigned int bufSize, unsigned int& width, unsigned int& height) override
  {
    image = nsvgParse(reinterpret_cast<char*>(buffer), "px", 96);
    width = image->width;
    height = image->height;
    return true;
  }

  virtual bool Decode(unsigned char *pixels,
                      unsigned int width, unsigned int height,
                      unsigned int pitch, ImageFormat format) override
  {
    unsigned char* img = (unsigned char*)malloc(width*height*4);
    nsvgRasterize(rast, image, 0,0,1, img, width, height, width*4);
    
    //ADDON_IMG_FMT_A8R8G8B8
    //What about others?
    //This is a bit of a clutch, but I have no better idea ATM
    for (int i = 0; i < width*height*4; i+=4) {
      pixels[i+0] = img[i+2];
      pixels[i+1] = img[i+1];
      pixels[i+2] = img[i+0];
      pixels[i+3] = img[i+3];
    };
    free (img); //is malloc and free the right way?!
  
    return true;
  }

private:
  struct NSVGimage* image;
	struct NSVGrasterizer* rast = nsvgCreateRasterizer();
};

class CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() { }
  virtual ADDON_STATUS CreateInstance(int instanceType, std::string instanceID, KODI_HANDLE instance, KODI_HANDLE& addonInstance) override
  {
    addonInstance = new MPOPicture(instance);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
