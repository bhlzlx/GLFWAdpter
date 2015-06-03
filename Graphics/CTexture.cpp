//
// Created by phantom on 2015/5/26 0026.
//

#include "CTexture.h"

#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>
#include <string.h>
#include "../buffer/iBuffer.h"


static void iBufferPNGReadFunc(png_structp s_ptr,png_bytep data,png_size_t size)
{
    iBuffer * pBuff = (iBuffer *)png_get_io_ptr(s_ptr);
    pBuff->Read(data,size);
}

struct Image:public iImage
{
    void Release()
    {
        if(this->m_pData)
        {
            delete[] m_pData;
            m_pData = NULL;
        }
        delete this;
    }
};

struct TexImp : public iTex
{
    GLuint m_iTexture;
    unsigned GetHandle()
    {
        return m_iTexture;
    }
    void Release()
    {
        glDeleteTextures(1,&m_iTexture);
    }
};

iImage * ImageFromPNG(iBuffer * buff)
{
    if(!buff)
        return NULL;
    png_structp s_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    png_infop   i_ptr = png_create_info_struct(s_ptr);
    setjmp(png_jmpbuf(s_ptr));
    png_set_read_fn(s_ptr,buff,iBufferPNGReadFunc);
    png_read_png(s_ptr,i_ptr, PNG_TRANSFORM_EXPAND , 0);

    int bit_depth = i_ptr->bit_depth;
    int color_type = i_ptr->color_type;
    int format = -1;
    int channels = i_ptr->channels;
    int pixel_depth = i_ptr->pixel_depth;
    if(bit_depth == 8)
    {
        switch(color_type)
        {
            case PNG_COLOR_TYPE_RGBA:
            {
                format = FMT_RGBA8888;
                break;
            }
            case PNG_COLOR_TYPE_RGB:
            {
                format = FMT_RGB888;
                break;
            }
        }
    }
    else
    {
        return NULL;
    }

    if(format == -1 || channels == 0)
    {
        return NULL;
    }

    Image * pImage = new Image();
    pImage->m_format = (IMG_FORMAT)format;
    pImage->m_iHeight = i_ptr->height;
    pImage->m_iWidth = i_ptr->width;

    unsigned long buff_size = pImage->m_iHeight * pImage->m_iWidth * pixel_depth/8;
    png_bytep* row_pointers = png_get_rows(s_ptr,i_ptr);

    pImage->m_pData = new unsigned char[buff_size];

    for(int i = 0;i<pImage->m_iHeight;i++)
    {
        memcpy(pImage->m_pData+(pImage->m_iWidth*pixel_depth/8*i),
               row_pointers[i],
               pImage->m_iWidth*pixel_depth/8
        );
    }
    png_destroy_read_struct(&s_ptr, &i_ptr, 0);
    return pImage;
}

iTex * CreateTexFromImage(iImage * image,int autoMip)
{
    int error;
    error = glGetError();
    TexImp * pTex = new TexImp();
    glGenTextures(1,&pTex->m_iTexture);
    error = glGetError();
    glBindTexture(GL_TEXTURE_2D,pTex->m_iTexture);
    switch(image->m_format)
    {
        case FMT_RGBA8888:
        {
            glTexImage2D(GL_TEXTURE_2D,0,4,image->m_iWidth,image->m_iHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,image->m_pData);
            error = glGetError();
            break;
        }
        case FMT_RGB888:
        {
            glTexImage2D(GL_TEXTURE_2D,0,3,image->m_iWidth,image->m_iHeight,0,GL_RGB,GL_UNSIGNED_BYTE,image->m_pData);
            break;
        }
    }

    if(autoMip)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,5);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }
    return pTex;
}
