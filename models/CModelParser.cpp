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
        // 索引个数
        meshData.m_nNumIndices = pMesh->mNumFaces * 3;
        unsigned* pIndexBuffer = new unsigned[meshData.m_nNumIndices];
        // 默认每个面都是三角形的，不是三角就报错！
        for(int j = 0;j<pMesh->mNumFaces;++j)
        {
            assert(pMesh->mFaces[j].mNumIndices == 3);
            memcpy(pIndexBuffer + j * 3,pMesh->mFaces[j].mIndices,sizeof(unsigned) * 3);
        }
        // 创建VAO 绑定VAO
        glGenVertexArrays(1,&meshData.m_VAO);
        glBindVertexArray(meshData.m_VAO);
        // 创建IBO ,绑定IBO,填充IBO
        glGenBuffers(1,&meshData.m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,meshData.m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,meshData.m_nNumIndices * sizeof(unsigned),pIndexBuffer,GL_STATIC_DRAW);
        delete []pIndexBuffer;

        // 创建顶点坐标 m_VBO_vertex
        glGenBuffers(1,&meshData.m_VBO_vertex);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_vertex);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mVertices,GL_STATIC_DRAW);
        // 创建tangents
        glGenBuffers(1,&meshData.m_VBO_tangent);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_tangent);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mTangents,GL_STATIC_DRAW);
        // 创建法线
        glGenBuffers(1,&meshData.m_VBO_normal);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_normal);
        //                           法线数量和坐标数量一样！
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pMesh->mNormals,GL_STATIC_DRAW);
        // 创建UV
        aiVector3D * pUVBuffer = new aiVector3D[pMesh->mNumVertices];
        for(int j = 0;j<pMesh->mNumVertices;++j)
        {
            memcpy(pUVBuffer + j,&pMesh->mTextureCoords[0][j],sizeof(aiVector3D));
        }
        glGenBuffers(1,&meshData.m_VBO_uv);
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_uv);
        glBufferData(GL_ARRAY_BUFFER,pMesh->mNumVertices * sizeof(aiVector3D),pUVBuffer,GL_STATIC_DRAW);

        // 将顶点数据 和 VAO 绑定
        // 绑定IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,meshData.m_IBO);
        // 顶点坐标
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_vertex);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
        // 法线
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_normal);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
        // uv
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_uv);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,NULL);
        // tangent
        glBindBuffer(GL_ARRAY_BUFFER,meshData.m_VBO_tangent);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);
        // 开启绑定槽
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        // 取消绑定
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // 添加数据到队列
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
