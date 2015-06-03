//
// Created by phantom on 2015/5/27 0027.
//

#ifndef GRAPHICSLIBRARY_CAMERA_H
#define GRAPHICSLIBRARY_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

extern const vec4 default_normal;
extern const vec4 default_top;
extern const vec4 default_left;


struct CCamera
{
    float m_axis_x;
    float m_axis_y;

    vec4  m_position;

    mat4  m_offsetMatrix;
    mat4  m_rotateMatrix;

    mat4  m_viewMatrix;

    void RotateAxisX(float angle);
    void RotateAxisY(float angle);
    void Translate(vec4 &offset);
    void ApplyRotate();

    void Forward( int iden);
    void Leftward( int iden);
    void Rightward( int iden);

    mat4& GetMatrix();
    vec4& GetPosition();

    CCamera();
};

#endif //GRAPHICSLIBRARY_CAMERA_H

