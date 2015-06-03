//
// Created by phantom on 2015/5/19.
//

#ifndef GRAPHICSLIBRARY_GRAPHICSCORE_H
#define GRAPHICSLIBRARY_GRAPHICSCORE_H

#include "../common/type.h"

struct IGXBO
{
    virtual  void SetData(const void * _pData,uint32 _nSize);
    virtual  void Release();
};

#include "../buffer/iBuffer.h"

enum IMG_FORMAT
{
    FMT_RGBA8888 = 0,
    FMT_RGB888 = 1,
    FMT_RGB565 = 2
};

struct iImage
{
    IMG_FORMAT      m_format;
    unsigned int    m_iWidth;
    unsigned int    m_iHeight;
    unsigned char * m_pData;

    virtual void Release() = 0;
};

struct iTex
{
    virtual unsigned GetHandle() = 0;
    virtual void Release() = 0;
};

iImage * ImageFromPNG(iBuffer * buff);
iTex * CreateTexFromImage(iImage * image,int autoMip);

#endif //GRAPHICSLIBRARY_GRAPHICSCORE_H
