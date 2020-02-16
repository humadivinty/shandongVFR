#include "stdafx.h"
#include "CusH264Struct.h"
#include <Windows.h>

#define SAFE_DELETE_ARRAY(obj) \
{   \
    if (obj != NULL)    \
    {   \
        delete[] obj;   \
        obj = NULL; \
    }   \
}

#define  SAFE_DELETE_OBJ(obj)   \
{   \
if (obj != NULL)    \
{   \
delete obj;   \
obj = NULL; \
}   \
}

CustH264Struct::CustH264Struct():
m_pbH264FrameData(NULL),
 m_iDataSize(0),
 m_iWidth(0),
 m_iHeight(0),
 m_isIFrame(0),
 m_isHistory(0),
m_llFrameTime(0),
index(0)
{

}

CustH264Struct::CustH264Struct(unsigned char* pbH264FrameData,
    long iDataSize, 
    long iWidth,
    long iHeight,
    long isIFrame, 
    long IsHistory,
    long long FrameTime,int nindex):
    m_pbH264FrameData(NULL),
    m_iDataSize(iDataSize),
    m_iWidth(iWidth),
    m_iHeight(iHeight),
    m_isIFrame(isIFrame),
    m_isHistory(IsHistory),
    m_llFrameTime(FrameTime),
	index(nindex)
{
    if (pbH264FrameData != NULL
        && iDataSize > 0)
    {
        m_pbH264FrameData = new unsigned char[iDataSize];
        memcpy(m_pbH264FrameData, pbH264FrameData, iDataSize);
    }
}

CustH264Struct::~CustH264Struct()
{
    SAFE_DELETE_ARRAY(m_pbH264FrameData);
}
