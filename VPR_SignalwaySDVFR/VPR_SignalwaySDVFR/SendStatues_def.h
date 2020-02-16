#ifndef SEND_STATUES_H
#define SEND_STATUES_H

enum sendType{ send_begin, send_Finish };

typedef struct _SendStatues
{
    long long iSendID;
    long long iCurrentTimeTick;
    long long iCarID;
    int iSendStatues;
    int iVideoBeginTime;
    int iVideoEndeTime;
    int iSendPlate;
    int iSendBestSnapShot;
    int iSendLastSnapShot;
    int iSendBeginCapture;
    int iSendBestCapture;
    int iSendLastCapture;
    int iSendSmallImage;
    int iSendBinaryImage;

    _SendStatues() :
        iSendID(0),
        iCurrentTimeTick(0),
        iCarID(-1),
        iSendStatues(0),
        iVideoBeginTime(0),
        iVideoEndeTime(0),
        iSendPlate(0),
        iSendBestSnapShot(0),
        iSendLastSnapShot(0),
        iSendBeginCapture(0),
        iSendBestCapture(0),
        iSendLastCapture(0),
        iSendSmallImage(0),
        iSendBinaryImage(0)
    {

    }
}SendStatues;

#endif // !SEND_STATUES_H
