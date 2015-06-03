#ifndef IBUFFER_H
#define IBUFFER_H

struct iBuffer
{
    virtual unsigned long GetLength()= 0;
    virtual unsigned char * GetBuffer()= 0;
    virtual unsigned long Write(unsigned char * src,unsigned long size)= 0;
    virtual unsigned long Read(unsigned char * out,unsigned long size)= 0;
    virtual bool Seek(int pos,long offset)= 0;

    virtual void Release() = 0;
};

iBuffer * CreateStandardBuffer(unsigned long);
iBuffer * BufferFromFile(const char * szFile);

#endif // IBUFFER_H
