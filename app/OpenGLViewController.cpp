//
// Created by phantom on 2015/5/21.
//

#include "OpenGLViewController.h"
#include "../models/models.h"
#include <cstdio>
#include <cstdlib>
#include "../Graphics/CTexture.h"

#include "../models/CModelParser.h"
#include "../math/Camera.h"


using namespace common_model;

#define MOVE_STEP 10

void OpenGLViewController::OnInit()
{
    glEnable(GL_TEXTURE_2D);

    m_rotation = 0.0f;


    m_pCamera = new CCamera();
    m_lightPosition = vec4(0,250,175,1);

    m_model = glm::scale(mat4(1.0f),vec3(0.125,0.125,0.125));
    // InitTexture

    iBuffer * pBuff = BufferFromFile("./textures/texture.png");
    iImage * image = ImageFromPNG(pBuff);
    m_texture = CreateDefaultTexture(0);
    //m_texture = CreateTexFromImage(image,1);
    image->Release();
    pBuff->Release();

    pBuff = BufferFromFile("./textures/normal.png");
    image = ImageFromPNG(pBuff);
    m_normalTex = CreateTexFromImage(image,0);
    image->Release();
    pBuff->Release();

    pBuff = BufferFromFile("./textures/specular.png");
    image = ImageFromPNG(pBuff);
    m_highTex = CreateTexFromImage(image,0);
    image->Release();
    pBuff->Release();

    // Init Shaders
    static char vertBuff[1024 * 5];
    static char fragBuff[1024 * 5];
    FILE * file;
    long filelen;

    file = fopen("./glsl/depth.vert","rb");
    if(!file)
        return;
    fseek(file,0,SEEK_END);
    filelen = ftell(file);
    fseek(file,0,SEEK_SET);
    fread(vertBuff,1,filelen,file);
    vertBuff[filelen] = 0;
    fclose(file);

    file = fopen("./glsl/depth.frag","rb");
    if(!file)
        return;
    fseek(file,0,SEEK_END);
    filelen = ftell(file);
    fseek(file,0,SEEK_SET);
    fread(fragBuff,1,filelen,file);
    fragBuff[filelen] = 0;
    fclose(file);

    m_pDepthShader = CShader::ShaderFromStrings(vertBuff,fragBuff);

    file = fopen("./glsl/cube.vert","rb");
    if(!file)
        return;
    fseek(file,0,SEEK_END);
    filelen = ftell(file);
    fseek(file,0,SEEK_SET);
    fread(vertBuff,1,filelen,file);
    vertBuff[filelen] = 0;
    fclose(file);

    file = fopen("./glsl/cube.frag","rb");
    if(!file)
        return;
    fseek(file,0,SEEK_END);
    filelen = ftell(file);
    fseek(file,0,SEEK_SET);
    fread(fragBuff,1,filelen,file);
    fragBuff[filelen] = 0;
    fclose(file);

    m_pShader = CShader::ShaderFromStrings(vertBuff,fragBuff);

   // m_templeModel.LoadModel("Temple.obj");
    m_templeModel.LoadModel("Temple.obj");

    m_view_light = lookAt(
            vec3(m_lightPosition.x,m_lightPosition.y,m_lightPosition.z),
            vec3(0,0,0),
            vec3(0,1,0)
                          );

    this->m_width = 640;
    this->m_height = 480;
    glViewport(0,0,640, 480);
    m_projection = perspective(45.0f,(float)m_width/(float)m_height,0.01f,500.0f);
}

void OpenGLViewController::OnTimer()
{
 /*   m_model_rotation -= 1;
    m_model = glm::rotate(mat4(1.0),m_model_rotation / 180 * 3.1415926f,vec3(0,1,0));
*/
    m_lightRotation += 2;
    static vec4 initLightPos(0,125,85,1);
    m_lightPosition = glm::rotate(mat4(1.0),m_lightRotation / 180 * 3.1415926f,vec3(0,1,0)) * initLightPos;
    m_view_light = lookAt(
            vec3(m_lightPosition.x,m_lightPosition.y,m_lightPosition.z),
            vec3(0,0,0),
            vec3(0,1,0)
    );
}

OpenGLViewController::OpenGLViewController()
{
    m_lightRotation = 0;
    m_model_rotation = 0;
}

void OpenGLViewController::OnMouseEvent(MouseEvent * event)
{
    float diffX = event->currX - event->lastX;
    float diffY = event->currY - event->lastY;
    if(event->eventType == eMouseMove)
    {
        if(event->button == eMouseButtonLeft)
        {
            m_pCamera->RotateAxisY(-diffX);
            m_pCamera->RotateAxisX(-diffY);
            m_pCamera->ApplyRotate();
        }
        else if(event->button == eMouseButtonRight)
        {
            m_pCamera->Forward(diffY);
        }
    }
}

void OpenGLViewController::OnKeyPressed(unsigned char key, int x, int y)
{
    //printf("key : %c pressed...\n",key);
    switch(key)
    {
    case 'W':
        {
            m_pCamera->Forward(MOVE_STEP);break;
        }
    case 'S':
        {
            m_pCamera->Forward(-MOVE_STEP);break;
        }
    case 'A':
        {
            m_pCamera->Leftward(MOVE_STEP);break;
        }
    case 'D':
        {
            m_pCamera->Rightward(MOVE_STEP);break;
        }
    }
}

void OpenGLViewController::OnUpdate()
{

    m_view = m_pCamera->GetMatrix();
    // ��Ⱦ��Դ����FBO
    glBindFramebuffer(GL_FRAMEBUFFER,m_shadowFBO);
    glClearDepth(1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // ����shader
    m_pDepthShader->Active();
    m_pDepthShader->SetUniformData(&m_projection[0][0],"PROJECTION");
    m_pDepthShader->SetUniformData(&m_view_light[0][0],"VIEW");
    m_pDepthShader->SetUniformData(&m_lightPosition[0],"LIGHT_POSITION0");
    glDepthFunc(GL_LEQUAL);
    // ����ͼ��
    m_pDepthShader->SetUniformData(&m_model[0][0],"MODEL");
    glViewport(0,0,(float)m_width * 2.0f,(float)m_height * 2.0f);
    m_templeModel.Render();

    // ��ȾĬ��FBO
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClearColor(.5,.5,.5,1.0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // ����shader
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.5, 2);

    m_pShader->Active();
    m_pShader->SetUniformData(&m_projection[0][0],"PROJECTION");
    m_pShader->SetUniformData(&m_view[0][0],"VIEW");
    m_pShader->SetUniformData(&m_view_light[0][0],"SHADOW_VIEW");

    m_pShader->SetUniformData(&m_lightPosition[0],"LIGHT_POSITION0");
    m_pShader->SetUniformData(&m_pCamera->GetPosition(),"CAMERA_POSITION");

    m_pShader->SetDepthSampler(0,m_depthTexture);

    m_pShader->SetSampler(0,m_texture->GetHandle());
    m_pShader->SetSampler(1,m_normalTex->GetHandle());
    m_pShader->SetSampler(2,m_highTex->GetHandle());
    // ����ͼ��
    glViewport(0,0,this->m_width,this->m_height);
    m_pShader->SetUniformData(&m_model[0][0],"MODEL");
    m_templeModel.Render();


    glViewport(0,0,this->m_width/4,this->m_height/4);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_pShader->SetUniformData(&m_view_light[0][0],"VIEW");
    m_templeModel.Render();

}

void OpenGLViewController::OnResize(unsigned width,unsigned height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width,height);

    m_projection = perspective(45.0f,(float)width/(float)height,0.01f,250.0f);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteTextures(1,&this->m_depthTexture);
        glDeleteFramebuffers(1,&this->m_shadowFBO);
    }
    glGenTextures(1,&this->m_depthTexture);
    glBindTexture(GL_TEXTURE_2D,this->m_depthTexture);

    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,(float)width * 2,(float)height * 2,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,0);

    glGenFramebuffers(1,&this->m_shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,this->m_shadowFBO);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,m_depthTexture,0);
    glDrawBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        assert(false);
}

void OpenGLViewController::Release()
{
    delete this;
}
