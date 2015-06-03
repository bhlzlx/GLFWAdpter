#ifndef __MODELS_H__
#define __MODELS_H__

namespace common_model
{
	void GenerateTangent(const float* _pData, unsigned _uv_offset, unsigned _vertex_offset, unsigned _stride, unsigned _face_count,float * _buff);

	extern const float __cube[8 * 36];
	extern const float __plane[8 * 6];
}

#endif