#ifndef VPR_COMMENTDEF_H
#define VPR_COMMENTDEF_H
/*
ע��
1��	ͼƬ�洢Ŀ¼Ϊ����̬������Ŀ¼��. / idevlp / xxxx.jpg
2��	ͼƬ��ʽ��JPEG����չ����JPG
3��	��Ƶ��ʽ��MPEG - 4����չ����MP4
4��	��ͼƬ�ļ�����������
��ͷ����ͼ�ļ�����. / idevlp / vlp_0_xx.jpg
��ͷ����ͼ�ļ�����. / idevlp / vlp_1_xx.jpg
��ͷ��ֵ��ͼ�ļ�����. / idevlp / vlp_2_xx.jpg
����ͼ�ļ�����. / idevlp / vlp_3_xx.jpg
��β����ͼ�ļ�����. / idevlp / vlp_4_xx.jpg
��β����ͼ�ļ�����. / idevlp / vlp_5_xx.jpg
��β��ֵ��ͼ�ļ�����. / idevlp / vlp_6_xx.jpg
������Ƶ�ļ�����. / idevlp / vlp_7_xx.jpg
���У�XXΪ00 - 99ѭ��ʹ�ã�ÿ��һ������1����99���00���¿�ʼ��

5.13	������ɫ��
ֵ	����
00	��ɫ
01	��ɫ
02	��ɫ
03	��ɫ
04	������ɫ
05	����˫ƴɫ
06	���׽���ɫ
09	δ֪

5.14	����ʶ��ӿڴ�����
������	��������
0	����
-100	�豸����Ӧ
-1000	�����������
-1001	�豸��ռ��
-1002	��ʧ��
-2000	��������
-2000����	Ԥ��

*/


//typedef struct _vlp_info
//{
//	int vlpInfoSize;					//ʶ��ṹ���С
//	unsigned char vlpColor[2];			//��ͷ������ɫ(�������5.14);
//	unsigned char vlpText[16];			//��ͷ�������֣�GBK����
//	char vlpTime[20];          		//��ͷʶ��ʱ�䣬��ʽ��yyyyMMddHHmmsszzz��
//	unsigned char vlpBackColor[2];		//��β������ɫ(�������5.14);
//	unsigned char vlpBackText[16];		//��β�������֣�GBK����
//	char vlpBackTime[20];          	//��βʶ��ʱ�䣬��ʽ��yyyyMMddHHmmsszzz��
//	int vlpCarClass;					//����
//	int vehLength;					//ʶ�𳵳�
//	int vehWidth;						//ʶ�𳵿�
//	int vehHigh;						//ʶ�𳵸�
//	int vehAxis;						//ʶ������
//	unsigned int vlpReliability;			//���Ŷ�(������λ��ʾ9999��ʾΪ99.99%);
//	unsigned char imageFile[8][128];		//ʶ��ͼƬ����:[0]=��ͷ����ͼ�ļ�����[1]=��ͷ����ͼ�ļ�����[2]=��ͷ��ֵ��ͼ�ļ�����[3]=����ͼ�ļ�����[4]=��β����ͼ�ļ�����[5]=��β����ͼ�ļ�����[6]=��β��ֵ��ͼ�ļ�����[7]=������Ƶ�ļ���
//} T_VLPINFO;

//typedef void (*CBFun_GetRegResult)(int nHandle,T_VLPINFO* pVlpResult, void *pUser);


typedef struct _vlp_front_info
{
	int vlpInfoSize;					//ʶ��ṹ���С
	unsigned char vlpColor[2];			//��ͷ������ɫ(�������5.14)
	unsigned char vlpText[16];			//��ͷ�������֣�GBK����
	char vlpTime[20];          		//��ͷʶ��ʱ�䣬��ʽ��yyyyMMddHHmmsszzz��
	unsigned int vlpReliability;			//���Ŷ�(������λ��ʾ9999��ʾΪ99.99%)
	unsigned char imageFile[3][128];		//ʶ��ͼƬ����:[0]=��ͷ����ͼ�ļ�����[1]=��ͷ����ͼ�ļ�����[2]=��ͷ��ֵ��ͼ�ļ���
} T_VLPFRONTINFO;


typedef struct _vlp_back_info
{
	int vlpInfoSize;					//ʶ��ṹ���С
	unsigned char vlpBackColor[2];		//��β������ɫ(�������5.14)
	unsigned char vlpBackText[16];		//��β�������֣�GBK����
	char vlpBackTime[20];          	//��βʶ��ʱ�䣬��ʽ��yyyyMMddHHmmsszzz��
	int vlpCarClass;					//����
	int vehLength;					//ʶ�𳵳�
	int vehWidth;						//ʶ�𳵿�
	int vehHigh;						//ʶ�𳵸�
	int vehAxis;						//ʶ������
	unsigned int vlpReliability;			//���Ŷ�(������λ��ʾ9999��ʾΪ99.99%)
	unsigned char imageFile[5][128];		//ʶ��ͼƬ����: [0]=����ͼ�ļ�����[1]=��β����ͼ�ļ�����[2]=��β����ͼ�ļ�����[3]=��β��ֵ��ͼ�ļ�����[4]=������Ƶ�ļ���
} T_VLPBACKINFO;


typedef void(*CBFun_GetFrontResult)(
	int nHandle,
	T_VLPFRONTINFO* pVlpResult,
	void *pUser
	);


typedef void(*CBFun_GetBackResult)(
	int nHandle,
	T_VLPBACKINFO* pVlpResult,
	void *pUser
	);


typedef void(*CBFun_GetDevStatus)(	int nHandle,	int nStatus,	void* pUser	);
#endif
