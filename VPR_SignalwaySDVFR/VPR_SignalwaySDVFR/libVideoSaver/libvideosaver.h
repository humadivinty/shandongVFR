#ifndef LIBVIDEOSAVER_H
#define LIBVIDEOSAVER_H

#ifndef LIBVIDEOSHARED_EXPORT

#ifdef WIN32
#define LIBVIDEOSHARED_EXPORT __declspec(dllexport)
#define DELSPEC /*__stdcall*/
#else
#define LIBVIDEOSHARED_EXPORT __attribute__((visibility("default")))
#define DELSPEC
#endif

#else

#ifdef WIN32
#define LIBVIDEOSHARED_EXPORT __declspec(dllimport)
#define DELSPEC __stdcall
#else
#define LIBVIDEOSHARED_EXPORT __attribute__((visibility("default")))
#define DELSPEC
#endif

#endif

#define FRAMETYPE_MP4_VIDEO 0
#define FRAMETYPE_MP4_AUDIO 1
#define FRAMETYPE_AVI_I_FRAME 2
#define FRAMETYPE_AVI_P_FRAME 3

#define  VIDEOTYPE_AVI 0
#define  VIDEOTYPE_MP4 1

//#include "libvideosaver_global.h"
#ifdef __cplusplus
extern "C"
{
#endif

LIBVIDEOSHARED_EXPORT void* DELSPEC Video_CreateProcessHandle(int VideoType);
LIBVIDEOSHARED_EXPORT int DELSPEC Video_CreateVideoFile(void* pHandle, const char* chFileName, int iVideoWidth, int iVideoHeight, int iFrameRate);
LIBVIDEOSHARED_EXPORT int DELSPEC Video_WriteH264Frame(void* pHandle, int iFrameType, unsigned char* pbFrameData, int iFrameSize);
LIBVIDEOSHARED_EXPORT int DELSPEC Video_CloseVideoFile(void* pHandle);
LIBVIDEOSHARED_EXPORT int DELSPEC Video_CloseProcessHandle(void* pHandle);


#ifdef __cplusplus
}
#endif

#endif // LIBVIDEOSAVER_H
