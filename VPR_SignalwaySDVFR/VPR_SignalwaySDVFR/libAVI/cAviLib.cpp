#include "stdafx.h"
#include "CAviLib.h"

extern "C"{
	#include "avilib.h"
}




CAviLib::CAviLib()
{
	m_outFd = NULL;
}

CAviLib::~CAviLib()
{

}

void CAviLib::close()
{
	if(m_outFd != NULL)
    {
		AVI_close((avi_t*)m_outFd);
		m_outFd = NULL;
	 }
}

bool CAviLib::IsNULL()
{
	if(m_outFd == NULL){
		return true;
	}else{
		return false;
	}
}


void CAviLib::setAviInfo(char* fileName, int Wide, int Hight, int iframerate, char *code)
{
    m_outFd = (avi_t*)AVI_open_output_file(fileName); //把文件描述符绑定到此文件上
    if(m_outFd == NULL)
    {
		printf("open file erro");
        return;
    }
    AVI_set_video((avi_t*)m_outFd,Wide,Hight, iframerate, code);//设置视频文件的格式 code = "H264"
}


int CAviLib::writeFrame(char *framebuff, int buffSize, int keyframe)
{
	if (m_outFd != NULL && framebuff!= NULL && buffSize!= 0)
    {
		
        if(AVI_write_frame((avi_t*)m_outFd,framebuff,buffSize,keyframe)==0)//向视频文件中写入一帧图像
        {
            // qDebug()<<"write success";
			return 0;
        }
		
    }
	else if(m_outFd == NULL)
	{
		return 1; // 未初始化
	}
	// printf("write erro");
	return 2;
}
