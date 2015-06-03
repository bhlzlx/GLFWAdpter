//
// Created by phantom on 2015/5/26 0026.
//

#ifndef GRAPHICSLIBRARY_CTEXTURE_H
#define GRAPHICSLIBRARY_CTEXTURE_H

#include <gl/glew.h>

#include "../Graphics/GraphicsCore.h"

iTex * CreateTexFromImage(iImage * image,int autoMip);
iImage * ImageFromPNG(iBuffer * buff);

#endif //GRAPHICSLIBRARY_CTEXTURE_H
