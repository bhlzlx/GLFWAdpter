#include "iBuffer.h"
#include <stdio.h>
#include <string.h>

struct StdBuffer:public iBuffer
{
    unsigned char * m_pData;
    unsigned char * m_pCurr;
    unsigned char * m_pEnd;
    unsigned long m_iCapacity;

    StdBuffer(const char * src,unsigned long size)
    {
        m_pData = new unsigned char[size];
        m_pCurr = m_pData;
        m_pEnd = m_pData + size;
        memcpy(m_pData,src,size);
        m_iCapacity = size;
    }

    StdBuffer(unsigned long capacity
              )
    {
        m_pData = new unsigned char[capacity];
        m_iCapacity = capacity;
        m_pEnd = m_pData + m_iCapacity;
        m_pCurr = m_pData;
    }

    unsigned long Write(unsigned char * src,unsigned long size)
    {
        unsigned long sizeLeft = m_iCapacity - (m_pCurr - m_pData);
        if(!sizeLeft)
            return 0;
        if(size > sizeLeft)
            size = sizeLeft;
        memcpy(m_pCurr,src,size);
        m_pCurr += size;
        return size;
    }

    unsigned long Read(unsigned char * out,unsigned long size)
    {
        unsigned long sizeLeft = m_iCapacity - (m_pCurr - m_pData);
        if(!sizeLeft)
            return 0;
        if(size > sizeLeft)
            size = sizeLeft;
        memcpy(out,m_pCurr,size);
        m_pCurr += size;
        return size;
    }

    bool Seek(int pos,long offset)
    {
        switch(pos)
        {
        case SEEK_SET:
            m_pCurr = m_pData + offset;
            break;
        case SEEK_CUR:
            m_pCurr += offset;
            break;
        case SEEK_END:
            m_pCurr = m_pEnd + offset;
            break;
        }
        if((m_pEnd - m_pCurr) < 0)
            m_pCurr = m_pEnd;
        else if(m_pCurr < m_pData)
            m_pCurr = m_pData;
    }

    unsigned long GetLength()
    {
        return m_iCapacity;
    }

    unsigned char * GetBuffer()
    {
        return m_pData;
    }

    void Release()
    {
        m_iCapacity = 0;
        if(m_pData)
            delete m_pData;
        m_pData = NULL;
        m_pCurr = NULL;
        m_pEnd = NULL;
    }
};

iBuffer * CreateStandardBuffer(unsigned long size)
{
    return new StdBuffer(size);
}

iBuffer * BufferFromFile(const char * szFile)
{
    FILE * file = fopen(szFile,"rb");
    if(!file)
        return NULL;
    fseek(file,0,SEEK_END);
    long fileLen = ftell(file);
    fseek(file,0,SEEK_SET);
    StdBuffer * buffer = new StdBuffer(fileLen);
    fread(buffer->m_pData,1,fileLen,file);
    fclose(file);
    return buffer;
}
