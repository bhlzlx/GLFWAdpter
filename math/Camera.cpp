//
// Created by phantom on 2015/5/27 0027.
//

#include "Camera.h"
#include <cstring>

#define M_PI		3.14159265358979323846


const vec4 default_normal = vec4(0,0,-1,0);
const vec4 default_top = vec4(0,1,0,0);
const vec4 default_left = vec4(-1,0,0,0);

void CCamera::RotateAxisX(float angle)
{
    // x轴方向控制在 -90~90之间
    this->m_axis_x += angle;
    if(m_axis_x > 90)
        m_axis_x = 90;
    else if(m_axis_x < -90)
        m_axis_x = -90;
}

void CCamera::RotateAxisY(float angle)
{
    // 控制好y轴旋转角度
    this->m_axis_y += angle;
    if(m_axis_y > 360)
        m_axis_y -= 360;
    else if(m_axis_y < -360)
        m_axis_y += 360;
}

void CCamera::Forward(int iden)
{
    vec4 forward_vec = glm::inverse(m_rotateMatrix) * default_normal;
    m_position += forward_vec * (float)iden;
    // 计算模型的位移矩阵
    vec4 modelOffset = m_position * -1.0f;
    m_offsetMatrix = glm::translate(mat4(1.0),vec3(modelOffset.x,modelOffset.y,modelOffset.z));
    // 应用位移
    m_viewMatrix = m_rotateMatrix * m_offsetMatrix;
}

void CCamera::Leftward(int iden)
{
    vec4 leftward_vec = glm::inverse(m_rotateMatrix) * default_left;
    m_position += leftward_vec * (float)iden;
    // 计算模型的位移矩阵
    vec4 modelOffset = m_position * -1.0f;
    m_offsetMatrix = glm::translate(mat4(1.0),vec3(modelOffset.x,modelOffset.y,modelOffset.z));
    // 应用位移
    m_viewMatrix = m_rotateMatrix * m_offsetMatrix;
}
void CCamera::Rightward(int iden)
{
    vec4 leftward_vec = glm::inverse(m_rotateMatrix) * default_left;
    m_position -= leftward_vec * (float)iden;
    // 计算模型的位移矩阵
    vec4 modelOffset = m_position * -1.0f;
    m_offsetMatrix = glm::translate(mat4(1.0),vec3(modelOffset.x,modelOffset.y,modelOffset.z));
    // 应用位移
    m_viewMatrix = m_rotateMatrix * m_offsetMatrix;
}

void CCamera::Translate(vec4 &offset)
{
   /* m_position += offset;
    // 应用位移
    vec4 modelOffset = -m_position;
    memcpy(&m_viewMatrix[3][0],&modelOffset[0],sizeof(modelOffset));
    */
}

void CCamera::ApplyRotate()
{
    // 应用旋转
    // 世界物体坐标变换和相机的变换相反
    mat4 rotatex = glm::rotate<float>(glm::mat4(1.0),-m_axis_x / 180.0f * M_PI,vec3(1,0,0));
    m_rotateMatrix = glm::rotate<float>(rotatex,-m_axis_y / 180.0f * M_PI,vec3(0,1,0));
    // 应用位移
    m_viewMatrix = m_rotateMatrix * m_offsetMatrix;
   // memcpy(&m_viewMatrix[3][0],&modelOffset[0],sizeof(float)*3);
}

mat4& CCamera::GetMatrix()
{
    return m_viewMatrix;
}

vec4& CCamera::GetPosition()
{
    return m_position;
}

CCamera::CCamera()
{
    m_position = vec4(0,0,0,1);
}
