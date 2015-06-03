#include "CModelParser.h"

CModelParser::CModelParser()
{
    //ctor
}

CModelParser::~CModelParser()
{
    //dtor
}

bool CModelParser::LoadModel(const char * szFilepath)
{
    Assimp::Importer importer;
    const aiScene * pModel = importer.ReadFile(
                                    szFilepath,
                                    aiProcess_CalcTangentSpace       |
                                    aiProcess_Triangulate            |
                                    aiProcess_JoinIdenticalVertices  |
                                    aiProcess_SortByPType
                                );
    if(NULL == pModel)
        return false;
    aiMesh * pMesh;
    for(int i = 0;i<pModel->mNumMeshes;++i)
    {
        pMesh = pModel->mMeshes[i];
        ModelData meshData;
        // ��������
        meshData.m_nNumIndices = pMesh->mNumFaces * 3;
        unsigned* pIndexBuffer = new unsigned[meshData.m_nNumIndices];
        // Ĭ��ÿ���涼�������εģ��������Ǿͱ���
        for(int j = 0;j<pMesh->mNumFaces;++j)
        {
            assert(pMesh->mFaces[j].mNumIndices == 3);
            memcpy(pIndexBuffer + j * 3,pMesh->mFaces[j].mIndices,sizeof(unsigned) * 3);
        }
        // ����VAO ��VAO
        glGenVertexArrays(1,&meshData.m_VAO);
        glBindVertexArray(meshData.m_VAO);
        // ����IBO ,��IBO,���IBO
        glGenBuffers(1,&meshData.m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,meshData.m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,meshData.m_nNumIndices * sizeof(unsigned),pIndexBuffer,GL_STATIC_DRAW);
        delete []pIndexBuffer;

        // ������������ m_VBO_vertex
        glGenBuffers(1,&meshData.m_VBO_vertex);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_vertex);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mVertices,GL_STATIC_DRAW);
        // ����tangents
        glGenBuffers(1,&meshData.m_VBO_tangent);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_tangent);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mTangents,GL_STATIC_DRAW);
        // ��������
        glGenBuffers(1,&meshData.m_VBO_normal);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_normal);
        //                           ������������������һ����
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mNormals,GL_STATIC_DRAW);
        // ����UV
        aiVector3D * pUVBuffer = new aiVector3D[pMesh->mNumVertices];
        for(int j = 0;j<pMesh->mNumVertices;++j)
        {
            memcpy(pUVBuffer + j,&pMesh->mTextureCoords[0][j],sizeof(aiVector3D));
        }
        glGenBuffers(1,&meshData.m_VBO_uv);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_uv);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pUVBuffer,GL_STATIC_DRAW);

        // ���������� �� VAO ��
        // ��IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,meshData.m_IBO);
        // ��������
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_vertex);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
        // ����
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_normal);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
        // uv
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_uv);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,NULL);
        // tangent
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_tangent);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);
        // �����󶨲�
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        // ȡ����
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // ������ݵ�����
        this->m_models.push_back(meshData);
    }
}

void CModelParser::Render()
{
    for( const ModelData & data : m_models)
    {
        glBindVertexArray(data.m_VAO);
        glDrawElements(GL_TRIANGLES,data.m_nNumIndices,GL_UNSIGNED_INT,NULL);
    }
}
