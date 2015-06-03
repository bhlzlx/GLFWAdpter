//
// Created by phantom on 2015/5/18.
//

#ifndef OPENGL_CSHADER_H
#define OPENGL_CSHADER_H

#include <gl/glew.h>
#include <vector>

#define SAMPLER2D_NUM_MAX           4
#define SAMPLER2DSHADOW_NUM_MAX     2

struct ShaderVariable
{
    char        name[32];
    GLuint      type;
    GLint       size;
    GLint       id;
};

struct CShader
{
    std::vector<ShaderVariable> m_uniforms;

    GLuint m_vertexFunc;
    GLuint m_framentFunc;
    GLuint m_program;

    void Active();
    void Release();

    void SetUniformData(void * _pData,const char * szName);

    void SetSampler(unsigned _index,unsigned tex);
    void SetDepthSampler(unsigned _index,unsigned tex);

    void SetTexture(GLuint texture,GLuint index);

    // ¾²Ì¬·½·¨
    static CShader *        ShaderFromStrings(const char * szVertex,const char * szFragment);
    static GLuint           shaderFuncFromStringWithMode(const char * szShader,GLuint mode);
    static void             UseNullShader();
};


#endif //OPENGL_CSHADER_H
