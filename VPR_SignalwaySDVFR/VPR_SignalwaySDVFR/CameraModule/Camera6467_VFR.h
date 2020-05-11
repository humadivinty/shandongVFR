#pragma once
//#include "G:\GitHub\VCR_YN\VCR\VCR\Camera\BaseCamera.h"
#include "CameraModule/BaseCamera.h"
#include "ResultListManager.h"
#include "CameraModule/Camera6467_plate.h"
#include "SendStatues_def.h"
#include "utilityTool/CCusSemaohore.h"
#include "ResultSentStatusManager.h"

class Camera6467_VFR :
    public BaseCamera
{
public:
    Camera6467_VFR();
    Camera6467_VFR(const char* chIP, HWND  hWnd, int Msg);
    virtual ~Camera6467_VFR();

private:
    void ReadConfig();
    void AnalysisAppendXML(CameraResult* CamResult);
    int AnalysisVelchType(const char* vehType);
    int AnalysisVelchAxleType(const char* AxleType, int  iAxleGroupCount);
    int GetAlexType(int ivalue);
    //************************************
    // Method:        CheckIfSuperLength
    // Describe:       查看当前车是否为超长车，如果是，则后续的逻辑需清理车型队列
    // FullName:      Camera6467_VFR::CheckIfSuperLength
    // Access:          private 
    // Returns:        bool
    // Returns Describe:
    // Parameter:    CameraResult * CamResult
    //************************************
    bool CheckIfSuperLength(CameraResult* CamResult);

    //************************************
    // Method:        CheckIfBackUpVehicle
    // Describe:        检查该结果是否为倒车结果
    // FullName:      Camera6467_VFR::CheckIfBackUpVehicle
    // Access:          private 
    // Returns:        bool
    // Returns Describe:
    // Parameter:    CameraResult * CamResult
    //************************************
    bool CheckIfBackUpVehicle(CameraResult* CamResult);

public:
    //获取发送状态回调的时间间隔
    int GetTimeInterval();
    void SetDisConnectCallback(void* funcDisc, void* pUser);
    void SetReConnectCallback(void* funcReco, void* pUser);
    void SetConnectStatus_Callback(void* func, void* pUser, int TimeInterval);
    void SendConnetStateMsg(bool isConnect);

    void SetResultCallback(void* func, void* pUser);
    bool CheckIfSetResultCallback();
	void SetFrontResultCallback(void* func, void* pUser);
	bool CheckFrontResultCallback();
	void SetTailResultCallback(void* pfunc, void* pUser);
	bool CheckTailResultcallback();
    void SendResultByCallback(std::shared_ptr<CameraResult> pResult);

    void SetMsgHandleAngMsg(void* handle, int msg);

    void SetCheckThreadExit(bool  bExit);
    bool GetCheckThreadExit();

    bool OpenPlateCamera(const char* ipAddress);
    std::shared_ptr<CameraResult> GetFrontResult();
    std::shared_ptr<CameraResult> GetFrontResultByPlateNo(const char* plateNo);
    std::shared_ptr<CameraResult> GetFrontResultByPosition(int position);
    std::shared_ptr<CameraResult> GetLastResult();
    std::shared_ptr<CameraResult> GetResultByCarID(unsigned long  dwCarID);

    bool GetLastResultIfReceiveComplete();
    void SetLastResultIfReceiveComplete(bool bValue);

    void DeleteFrontResult(const char* plateNo);
    void ClearALLResult();
    size_t GetResultListSize();
    void TryWaitCondition();

public:

    virtual int RecordInfoBegin(DWORD dwCarID);

    virtual int RecordInfoEnd(DWORD dwCarID);

    virtual int RecordInfoPlate(DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoBigImage(DWORD dwCarID,
        WORD  wImgType,
        WORD  wWidth,
        WORD  wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoSmallImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int RecordInfoBinaryImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    virtual int DeviceJPEGStream(
        PBYTE pbImageData,
        DWORD dwImageDataLen,
        DWORD dwImageType,
        LPCSTR szImageExtInfo);

    virtual void CheckStatus();

public:
    bool checkIfHasThreePic(std::shared_ptr<CameraResult> result);
    int getResultWaitTime();
    void SetResultWaitTime(int iValue);

    //unsigned int SendResultThreadFunc();	
    static unsigned int WINAPI s_SendResultThreadFunc(void* parameter)
    {
        Camera6467_VFR* pThis = (Camera6467_VFR*)parameter;
        if (pThis)
        {
//#ifdef USE_LAST_RESULT
//            return pThis->SendResultThreadFunc_lastResult();
//#else
            //return pThis->SendResultThreadFunc();
//#endif
            //return pThis->SendResultThreadFunc_WithNoSignal();

			return pThis->SendResultThreadFunc_Separate();
        }
        return 0;
    }
    void copyStringToBuffer(char* bufer,size_t bufLen, const char * srcStr);
	bool SaveImgStructFunc(CameraIMG* pImg, int imgType, int iIndex, const char* imgPath);

    //unsigned int SendResultThreadFunc_WithNoSignal();

	unsigned int SendResultThreadFunc_Separate();
	void SendFrontResultByCallback(std::shared_ptr<CameraResult> pResult, int index);
	void SendTailResultByCallback(std::shared_ptr<CameraResult> pResult, int index);
	void FontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath);
	void BeginFontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath);
	void LastFontAdd(std::shared_ptr<CameraResult> pResult, int index, const char* imgPath);


    /************************************************************************/
    /* 循环删除日志线程                                                                     */
    /************************************************************************/
    static unsigned int WINAPI s_DeleteLogThreadFunc(void* parameter)
    {
        Camera6467_VFR* pThis = (Camera6467_VFR*)parameter;
        if (pThis)
        {
            return pThis->DeleteLogThreadFunc();
        }
        return 0;
    }
    unsigned int DeleteLogThreadFunc();

    
    /************************************************************************/
    /* 循环删除缓存线程                                                                     */
    /************************************************************************/
    static unsigned int WINAPI s_DeleteResultThreadFunc(void* parameter)
    {
        Camera6467_VFR* pThis = (Camera6467_VFR*)parameter;
        if (pThis)
        {
            return pThis->DeleteResultThreadFunc();
        }
        return 0;
    }
    unsigned int DeleteResultThreadFunc();

    void UpdateSentStatus(CameraResult* pResult);

    bool GetIfSendResult();
    void SetIfSendResult(bool bValue);

    bool GetFrontSendStatus(ResultSentStatus& status);
    void UpdateSendStatus(int loginId, unsigned long dwCarID, int iTem, long value);

    int GetResultMode();
private:
    DWORD m_dwLastCarID;
    int m_iTimeInvl;
    int m_iSuperLenth;
    int m_iResultTimeOut;
    int m_iResultMsg;
    int m_iWaitVfrTimeOut;
    int m_iResultModule;
	int m_addplate;
	int m_addtime;
	int m_iFontSize;
	int m_iFontAddcolorR;
	int m_iFontAddcolorG;
	int m_iFontAddcolorB;

    void* g_pUser;
    void* g_func_ReconnectCallback;
    void* g_ConnectStatusCallback;
    void* g_func_DisconnectCallback;

    void* g_pFuncResultCallback;
    void* g_pResultUserData;

	void* g_pFuncFrontResultCallback;
	void* g_pFrontResultUserData;

	void* g_pFuncTailResultCallback;
	void* g_pTailResultUserData;

    void* m_hMsgHanldle;
    
    bool m_bStatusCheckThreadExit;
    bool m_bJpegComplete;
    bool m_bLastResultComplete;
    bool m_bSentResult;

    HANDLE m_hStatusCheckThread;			//状态检测线程
    HANDLE m_hSendResultThread;			//结果发送线程
    HANDLE m_hDeleteLogThread;              //日志清除线程
    HANDLE m_hDeleteResultThread;              //结果清除线程

    CameraIMG m_Small_IMG_Temp;
    CameraIMG m_CIMG_StreamJPEG;

    CRITICAL_SECTION m_csResult;

    std::shared_ptr<Camera6467_plate> m_Camera_Plate;

    CameraResult* m_pResult;
    ResultListManager m_VfrResultList;
    std::list<std::string> m_lsVideoName;
    std::list<ResultSentStatus> m_lsResultSentStatusList;

    std::shared_ptr<CameraResult> m_pLastResult;
    //CCusSemaphore m_MySemaphore;

    ResultSentStatusManager m_lsStatusList;

	enum imgType{ type_frontImg, type_SideImg, type_tailImg, type_frontPlate, type_frontBin, type_tailPlate, type_taileBin };
};

