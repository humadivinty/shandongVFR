#ifndef CUSH264STRUCT_H
#define  CUSH264STRUCT_H

class CustH264Struct
{
public:
    CustH264Struct();
    CustH264Struct(unsigned char* pbH264FrameData,
    long iDataSize,
    long iWidth,
    long iHeight,
    long isIFrame,
    long IsHistory,
	long long FrameTime, int nindex);

    ~CustH264Struct();

public:
    unsigned char* m_pbH264FrameData;
    long m_iDataSize;
    long m_iWidth;
    long m_iHeight;
    long m_isIFrame;    
    long m_isHistory;
    long long m_llFrameTime;
	int index;
};


#endif // !CUSH264STRUCT_H
