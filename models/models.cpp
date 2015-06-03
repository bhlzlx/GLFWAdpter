#include "models.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glm/glm.hpp>

namespace common_model
{

	struct TexCoord
	{
		float u;
		float v;
	};

	struct Tangent
	{
		float x,y,z;
	};

	void GenerateTangent(const float* _pData, unsigned _uv_offset, unsigned _vertex_offset, unsigned _stride, unsigned _face_count,float * _buff)
	{
		const float * uv_ptr = _pData + _uv_offset/sizeof(float);
		const float * vert_ptr = _pData + _vertex_offset/sizeof(float);
		glm::vec3 vertices_vec[2];
		glm::vec2 uv_vec[2];
		//float vertices_vec[2][3];
		//float uv_vec[2][2]
		for (int i = 0; i < _face_count; ++i)
		{
		    //
		    //system("cls");
			glm::vec2 uv1;
			glm::vec3 vertex1;
			memcpy(&uv1[0],uv_ptr,sizeof(float)*2);
			memcpy(&vertex1[0],vert_ptr,sizeof(float)*3);

			//printf("prime uv: %f %f \n",uv1.x,uv1.y);
			//printf("prime vertex: %f %f %f \n",vertex1.x,vertex1.y,vertex1.z);

			uv_ptr  =(float*)((char *)uv_ptr +  _stride);
			vert_ptr = (float*)((char *)vert_ptr + _stride);

			for (int j = 0; j <2; ++j)
			{
				//printf("uv: %f %f \n",((glm::vec2 *)uv_ptr)->x,((glm::vec2 *)uv_ptr)->y);
				//printf("vertex: %f %f %f \n",((glm::vec3 *)vert_ptr)->x,((glm::vec3 *)vert_ptr)->y,((glm::vec3 *)vert_ptr)->z);

				vertices_vec[j] = *(glm::vec3 *)vert_ptr - vertex1;
				vert_ptr = (float*)((char *)vert_ptr + _stride);
				uv_vec[j]	= *(glm::vec2 *)uv_ptr - uv1;
				uv_ptr  =(float*)((char *)uv_ptr +  _stride);
			}
			float scale1,scale2;
			scale1 = scale2 = 1.0f;
			// �Ѿ�ȡ������ �� UV ����������������UV���������

			if(uv_vec[0].y == 0 && uv_vec[1].y == 0)
			{
				// �������������v������0�������uv����������!
				scale1 = scale2 = 1;
			}
			else if(uv_vec[0].y == 0)
			{
				scale2 = 0;
			}
			else if(uv_vec[1].y == 0)
			{
				scale1 = 0;
			}
			else
			{
				scale2 = -uv_vec[0].y/uv_vec[1].y;
			}
			float finalU = uv_vec[0].x * scale1 + uv_vec[1].x * scale2;
			// ���õ���uv����һ���� u ����~
			if(finalU < 0)
			{
				scale1 = -scale1;
				scale2 = -scale2;
			}
			glm::vec3 tangent1 = vertices_vec[0] * scale1 + vertices_vec[1] * scale2;

			tangent1 = glm::normalize(tangent1);

			//printf("tangent : %f %f %f \n",tangent1.x,tangent1.y,tangent1.z);
			memcpy(_buff,&tangent1[0],sizeof(tangent1));
			_buff += 3;
		}
	}

	const float __cube[8 * 36] =
	{
        // 1 ��
		-1,1,-1,	0,1,0, 0,1,
		-1,1,1,		0,1,0, 0,0,
		1,1,1,		0,1,0, 1,0,

		-1,1,-1,	0,1,0, 0,1,
		1,1,1,		0,1,0, 1,0,
		1,1,-1,		0,1,0, 1,1,
        // 2 ǰ
		-1,1,1,		0,0,1, 0,1,
		-1,-1,1,	0,0,1, 0,0,
		1,-1,1,		0,0,1, 1,0,

		-1,1,1,		0,0,1, 0,1,
		1,-1,1,		0,0,1, 1,0,
		1,1,1,		0,0,1, 1,1,
        // 3 ��
		-1,-1,1,	0,-1,0, 0,1,
		-1,-1,-1,	0,-1,0, 0,0,
		1,-1,-1,	0,-1,0, 1,0,

		-1,-1,1,	0,-1,0, 0,1,
		1,-1,-1,	0,-1,0, 1,0,
		1,-1,1,     0,-1,0, 1,1,
        // 4 ��
		1,1,-1,     0,0,-1, 0,1,
		1,-1,-1,	0,0,-1, 0,0,
		-1,-1,-1,	0,0,-1, 1,0,

		1,1,-1,     0,0,-1, 0,1,
		-1,-1,-1,	0,0,-1, 1,0,
		-1,1,-1,	0,0,-1, 1,1,
        // 5 ��
		1,1,1,		1,0,0,	0,1,
		1,-1,1,		1,0,0,	0,0,
		1,-1,-1,	1,0,0,	1,0,

		1,1,1,		1,0,0,	0,1,
		1,-1,-1,	1,0,0,	1,0,
		1,1,-1,		1,0,0,	1,1,
        // 6 ��
		-1,1,-1,	-1,0,0,	0,1,
		-1,-1,-1,	-1,0,0,	0,0,
		-1,-1,1,	-1,0,0,	1,0,

		-1,1,-1,	-1,0,0,	0,1,
		-1,-1,1,	-1,0,0,	1,0,
		-1,1,1,		-1,0,0,	1,1
	};

	const float __plane[8 * 6] =
	{
	    -1,0,-1,	0,1,0, 0,1,
		-1,0,1,		0,1,0, 0,0,
		1,0,1,		0,1,0, 1,0,

		-1,0,-1,	0,1,0, 0,1,
		1,0,1,		0,1,0, 1,0,
		1,0,-1,		0,1,0, 1,1
	};
};


