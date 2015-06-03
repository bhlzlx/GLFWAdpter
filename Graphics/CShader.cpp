//
// Created by phantom on 2015/5/18.
//
#include <cassert>
#include <cstring>
#include <cstdio>
#include "CShader.h"
#include "../log/cmnlog.h"


GLuint load_shader(const char * szShader,GLuint mode)
{
    GLuint id ;
    id = glCreateShader(mode);
    glShaderSource(id,1,&szShader,NULL);
    glCompileShader(id);
    // �������ǲ���������
    GLint compileState;
    glGetShaderiv(id,GL_COMPILE_STATUS,&compileState);
    if(compileState == GL_FALSE)
    {
        char error[1024];
        glGetShaderInfoLog(id,1024,NULL,error);
        cmnlog::writelog(error);
        glDeleteShader(id);
        return 0;
    }
    else
    {
        return id;
    }
}

void CShader::SetSampler(unsigned _index,unsigned tex)
{
    assert(_index < SAMPLER2D_NUM_MAX);
    char samplerName[32];
    sprintf(samplerName,"TEXTURE%02d",_index);
    GLuint texture_slot = GL_TEXTURE0 + _index;

    for (const ShaderVariable& var:this->m_uniforms)
    {
        if(strcmp(var.name ,samplerName) == 0)
        {
            glActiveTexture(texture_slot);
            glBindTexture(GL_TEXTURE_2D,tex);
            glUniform1i(var.id,_index);
            return;
        }
    }
}

void CShader::SetDepthSampler(unsigned _index,unsigned tex)
{
    assert(_index < SAMPLER2DSHADOW_NUM_MAX);
    char samplerName[32];
    sprintf(samplerName,"DEPTH_TEXTURE%02d",_index);
    GLuint texture_slot = GL_TEXTURE0 + _index + SAMPLER2D_NUM_MAX;

    for (const ShaderVariable& var:this->m_uniforms)
    {
        if(strcmp(var.name ,samplerName) == 0)
        {
            glActiveTexture(texture_slot);
            glBindTexture(GL_TEXTURE_2D,tex);
            glUniform1i(var.id,_index + SAMPLER2D_NUM_MAX);
            return;
        }
    }
}

CShader *CShader::ShaderFromStrings(const char *szVertex, const char *szFragment)
{
    CShader * pShader = new CShader();
    pShader->m_vertexFunc = shaderFuncFromStringWithMode(szVertex,GL_VERTEX_SHADER);
    pShader->m_framentFunc = shaderFuncFromStringWithMode(szFragment,GL_FRAGMENT_SHADER);
    pShader->m_program = glCreateProgram();
    glAttachShader(pShader->m_program,pShader->m_vertexFunc);
    glAttachShader(pShader->m_program,pShader->m_framentFunc);
    glLinkProgram(pShader->m_program);

    GLint nUniform;
    glGetProgramiv( pShader->m_program , GL_ACTIVE_UNIFORMS , &nUniform );

    static char namebuff[1024];

    GLsizei name_len;
    GLenum type;
    GLint size;
    GLint loc;

    pShader->m_uniforms.clear();

    for( GLuint idx = 0 ; idx < nUniform ; ++idx )
    {
        glGetActiveUniform(pShader->m_program, idx, 1024, &name_len, &size, &type, namebuff);
        loc = glGetUniformLocation(pShader->m_program, namebuff);
        ShaderVariable var;
        var.id = loc;
        var.type = type;
        var.size = size;
        strcpy(var.name,namebuff);
        pShader->m_uniforms.push_back(var);
    }
    return pShader;
}

void CShader::Active()
{
    glUseProgram(this->m_program);
}

void CShader::Release()
{
    glDetachShader(this->m_program,this->m_vertexFunc);
    glDetachShader(this->m_program,this->m_framentFunc);
    glDeleteShader(this->m_vertexFunc);
    glDeleteShader(this->m_framentFunc);
    glDeleteProgram(this->m_program);
    delete this;
}

GLuint CShader::shaderFuncFromStringWithMode(const char *szShader, GLuint mode)
{
    GLuint id ;
    id = glCreateShader(mode);
    glShaderSource(id,1,&szShader,NULL);
    glCompileShader(id);
    // �������ǲ���������
    GLint compileState;
    glGetShaderiv(id,GL_COMPILE_STATUS,&compileState);
    if(compileState == GL_FALSE)
    {
        static char error[1024];
        glGetShaderInfoLog(id,1024,NULL,error);
        cmnlog::writelog(error);
        glDeleteShader(id);
        return 0;
    }
    else
    {
        return id;
    }
}

void CShader::UseNullShader()
{
    glUseProgram(0);
}

void CShader::SetTexture(GLuint texture,GLuint index)
{

}

void CShader::SetUniformData(void * _pData,const char * szName)
{
    // ����uniform��
    for (const ShaderVariable& var:this->m_uniforms   )
    {
        if(strcmp(var.name ,szName) == 0)
        {
            /*
             * GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4, GL_BOOL, GL_BOOL_VEC2, GL_BOOL_VEC3, GL_BOOL_VEC4, GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4, GL_SAMPLER_2D, or GL_SAMPLER_CUBE
             */
            switch (var.type)
            {
                case GL_FLOAT:
                {
                    glUniform1fv(var.id,1,(float *)_pData);
                    break;
                }
                case GL_FLOAT_VEC2:
                {
                    glUniform2fv(var.id,1,(float *)_pData);
                    break;
                }
                case GL_FLOAT_VEC3:
                {
                    glUniform3fv(var.id,1,(float *)_pData);
                    break;
                }
                case GL_FLOAT_VEC4:
                {
                    glUniform4fv(var.id,1,(float *)_pData);
                    break;
                }
                case GL_INT:
                {
                    glUniform1iv(var.id,1,(int*)_pData);
                    break;
                }
                case GL_INT_VEC2:
                {
                    glUniform2iv(var.id,1,(int*)_pData);
                    break;
                }
                case GL_INT_VEC3:
                {
                    glUniform3iv(var.id,1,(int*)_pData);
                    break;
                }
                case GL_INT_VEC4:
                {
                    glUniform4iv(var.id,1,(int*)_pData);
                    break;
                }
                case GL_FLOAT_MAT2:
                {
                    glUniformMatrix2fv(var.id,1,GL_FALSE,(float*)_pData);
                    break;
                }
                case GL_FLOAT_MAT3:
                {
                    glUniformMatrix3fv(var.id,1,GL_FALSE,(float*)_pData);
                    break;
                }
                case GL_FLOAT_MAT4:
                {
                    glUniformMatrix4fv(var.id,1,GL_FALSE,(float*)_pData);
                    break;
                }
                case GL_SAMPLER_2D:
                {
                    glUniform1i(var.id,*(GLint*)_pData);
                    break;
                }
                case GL_SAMPLER_2D_SHADOW:
                {
                    glUniform1i(var.id,*(GLint*)_pData);
                    break;
                }
            }
        }
    }
}
