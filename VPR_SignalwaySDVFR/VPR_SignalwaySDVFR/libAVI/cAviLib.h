#ifndef CAviLib_H
#define CAviLib_H 

class CAviLib
{
public:
    CAviLib();
    ~CAviLib();

	   /**
     * @brief setAviInfo  设置avi文件头信息
     * @param fileName    文件名
     * @param Wide        宽
     * @param Hight       高
     * @param iframerate  帧率
     * @param code        编码格式 如 "H264"
     */
    void setAviInfo(char* fileName, int Wide, int Hight, int iframerate, char *code);

	/**
     * @brief writeFrame    写入帧
     * @param framebuff     数据缓冲区
     * @param buffSize      数据长
     * @param keyframe      是否关键帧
     * @return 0 成功  1 失败
     */
    int writeFrame(char *framebuff,int buffSize,int keyframe);

	/**
     * @brief IsNULL 是否已经创建文件
     */
	bool IsNULL();

	/**
     * @brief close  关闭文件
     */
    void close();
private:
    void* m_outFd;
};





#endif // CAviLib_H
