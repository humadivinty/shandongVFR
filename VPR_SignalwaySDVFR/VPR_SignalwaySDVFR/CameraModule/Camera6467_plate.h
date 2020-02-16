#pragma once
#include "BaseCamera.h"
#include "CameraModule/ResultListManager.h"


#define MAX_THREAD_COUNT 5

class Camera6467_plate :    public BaseCamera
{
public:
    Camera6467_plate();
    Camera6467_plate(const char* chIP, HWND  hWnd, int Msg);
    virtual ~Camera6467_plate();

    bool GetOneJpegImg(CameraIMG &destImg);    

    //获取发送状态回调的时间间隔
    int GetTimeInterval();
    void SetDisConnectCallback(void* funcDisc, void* pUser);
    void SetReConnectCallback(void* funcReco, void* pUser);
    void SetConnectStatus_Callback(void* func, void* pUser, int TimeInterval);
    void SetResultCallbackFunc(void* func);
    void SendConnetStateMsg(bool isConnect);

    bool GetResultComplete();
    void SetResultComplete(bool bfinish);

    CameraResult* GetOneResult();
    CameraResult* GetOneResultFromBuffer();
    void CopyResultToBuffer(CameraResult* pResult);
    std::shared_ptr<CameraResult> GetOneResultByCarId(DWORD dwCarID);


    void NotifyResultReady(DWORD index);

    void SaveResult();
    void SetExtraDataSource(void* pDataSource);
    std::shared_ptr<CameraResult>  GetResultFromExtraSourceByPlateNumber(const char* pChPlateNumber);

private:
    void AnalysisAppendXML(CameraResult* CamResult);
    int AnalysisVelchType(const char* vehType);
    void ReadConfig();

    void SetCheckThreadExit(bool  bExit);
    bool GetCheckThreadExit();

private:

    int m_iTimeInvl;
    int m_iCompressBigImgSize;
    int m_iCompressSamllImgSize;

    DWORD m_dwLastCarID;

    bool m_bResultComplete;
    bool m_bJpegComplete;
    bool m_bSaveToBuffer;
    bool m_bOverlay;
    bool m_bCompress;
    bool m_bStatusCheckThreadExit;

    char m_chResultPath[MAX_PATH];

    CameraResult* m_CameraResult;
    CameraResult* m_BufferResult;
    //std::shared_ptr<CameraResult> m_BufferResult;
    ResultListManager m_lsResultList;
    void* m_pExternalDataSource;


    CameraIMG m_CIMG_StreamJPEG;

    CRITICAL_SECTION m_csResult;
    CameraIMG m_Bin_IMG_Temp;
    CameraIMG m_Small_IMG_Temp;

    void* g_pUser;
    void* g_func_ReconnectCallback;
    void* g_ConnectStatusCallback;
    void* g_func_DisconnectCallback;
    void* g_func_ResultCallback;

    HANDLE m_hStatusCheckThread;			//状态检测线程

private:
    Camera6467_plate* m_pSideCamera;
    Camera6467_plate* m_pTailCamera;

public:
    enum CameraType{type_FrontCam, type_SideCam, type_TailCam};
    bool SetSubCamera(int Type, const char* camIP);
    void* GetSubCamera(int Type);

    void CloseSubCamera(int Type);
private:

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
};

