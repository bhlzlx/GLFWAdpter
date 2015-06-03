//
// Created by phantom on 2015/5/21.
//

#ifndef GRAPHICSLIBRARY_OPENGLVIEWCONTROLLER_H
#define GRAPHICSLIBRARY_OPENGLVIEWCONTROLLER_H

#include "../common/type.h"
#include "../Graphics/CShader.h"
#include "../Graphics/CTexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../models/CModelParser.h"
#include "../event/MouseEvent.h"
#include "../math/Camera.h"

using namespace glm;

struct OpenGLViewController
{
    CShader*            m_pShader;
    CShader*            m_pDepthShader;

    float               m_rotation;
    float               m_model_rotation;

    uint32              m_width;
    uint32              m_height;

    mat4                m_projection;
    mat4                m_view;
    mat4                m_model;


    CModelParser        m_templeModel;

    GLuint              m_shadowFBO;
    GLuint              m_depthTexture;

    iTex*               m_texture;
    iTex*               m_normalTex;
    iTex*               m_highTex;

    mat4                m_view_light;
    vec4                m_lightPosition;
    float               m_lightRotation;

    aiScene *           m_pModel;

    CCamera *           m_pCamera;

    OpenGLViewController();

    void OnMouseEvent(MouseEvent * event);
    void OnResize(unsigned width,unsigned height);
    void OnInit();

    void OnUpdate();
    void OnTimer();
    void Release();


    void OnKeyPressed(unsigned char key, int x, int y);
};


#endif //GRAPHICSLIBRARY_OPENGLVIEWCONTROLLER_H
