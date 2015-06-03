#ifndef CMODELPARSER_H
#define CMODELPARSER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gl/glew.h>

#include <vector>

class CModelParser
{
    public:
        struct ModelData
        {
            GLuint m_VAO;
            GLuint m_VBO_vertex;
            GLuint m_VBO_normal;
            GLuint m_VBO_uv;
            GLuint m_VBO_tangent;
            GLuint m_IBO;

            GLuint m_nNumIndices;
        };

        std::vector<ModelData> m_models;

        CModelParser();

        void Render();

        virtual ~CModelParser();

        bool LoadModel(const char * szFilepath);
    protected:
    private:
};

#endif // CMODELPARSER_H
