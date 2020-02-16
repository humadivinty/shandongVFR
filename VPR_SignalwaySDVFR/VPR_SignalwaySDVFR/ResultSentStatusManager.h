#ifndef RESULT_SENT_STATUS_LIST_H
#define RESULT_SENT_STATUS_LIST_H
#include <memory>
#include <mutex>
#include <list>

enum sentStatus{ sta_undefine, sta_receiveDone, sta_waitGet, sta_sentFinish };
enum imgItem{ item_deiviceID, item_frontImg, item_sideImg, item_tailImg, item_video, item_smallImg, item_binImg, item_carID, item_receiveTime };

typedef struct  _ResultSentStatus
{
    int DeviceID;
    unsigned long dwCarID;
    int iFrontImgSendStatus;
    int iSidImgSendStatus;
    int iTailImgSendStatus;
    int iVideoSendStatus;
    int iSmallImgSendStatus;
    int iBinaryImgSendStatus;

    unsigned long uTimeReceive;

    _ResultSentStatus() :
        DeviceID(0)
        ,dwCarID(0)
        , iFrontImgSendStatus(sta_undefine)
        , iSidImgSendStatus(sta_undefine)
        , iTailImgSendStatus(sta_undefine)
        , iVideoSendStatus(sta_undefine)
        , iSmallImgSendStatus(sta_undefine)
        , iBinaryImgSendStatus(sta_undefine)
        , uTimeReceive(0)
    {

    }

}ResultSentStatus;

class ResultSentStatusManager
{
public:
    ResultSentStatusManager();
    ~ResultSentStatusManager();

    void UpdateStatus(int DeviceID, unsigned long carID, int itemID, int iStatus);
    bool GetRsultStatus(int DeviceID, unsigned long carID, int itemID, long& iStatus);
    void RemoveElement(int DeviceID, unsigned long carID);

    bool getFirstElement(ResultSentStatus& element);
    bool RemoveFirstElement();

    void ClearAll();

    size_t size();

private:
    std::mutex m_mtx;
    std::list<ResultSentStatus> m_lsStatusList;
};

#endif