
#ifndef _ET199_C51_H_
#define _ET199_C51_H_

#ifdef __C51__
#pragma NOREGPARMS
#endif

#ifdef __C51__
typedef unsigned char	BOOL;
typedef unsigned short	word;
typedef unsigned short	WORD;
typedef unsigned long	dword;
typedef unsigned long	DWORD;
typedef unsigned char	byte;
typedef unsigned char	BYTE;
typedef BYTE			DOUBLE[8];
typedef BYTE 			HANDLE;
#endif



#define SHA_DIGEST_LENGTH 20
#define MD5_DIGEST_LENGTH 16
#define DES_KEY_LENGTH	  8
#define TDES_KEY_LENGTH	  16

#define ET199_SHA_CBLOCK	64
#define ET199_MD5_CBLOCK	64


#define INPUT_DATA_OFFSET 		0x0800

#ifdef __C51__
#define wInLen		   	(*(WORD xdata *)INPUT_DATA_OFFSET )
#define	pbInBuff		((BYTE xdata *)INPUT_DATA_OFFSET + 2 )
#endif

#ifdef _WIN32
#define INPUT_DATA_LEN_OFFSET		INPUT_DATA_OFFSET 
#define	INPUT_DATA_BUFFER_OFFSET	INPUT_DATA_OFFSET + 2 
#endif 

typedef struct _tagSHA_CONTEXT {
    DWORD	h[5];
    DWORD	dwTotalLength;
    BYTE	bRemainLength;
    BYTE	pbRemainBuf[ET199_SHA_CBLOCK];
}
SHA_CONTEXT,*PSHA_CONTEXT;

typedef SHA_CONTEXT ShaContext;
typedef ShaContext* PShaContext;

typedef struct _tagMD5_CONTEXT {
    DWORD h[4];
    DWORD dwTotalLength;
    BYTE  bRemainLength;
    BYTE  pbRemainBuf[ET199_MD5_CBLOCK];
}
MD5_CONTEXT,*PMD5_CONTEXT;

typedef MD5_CONTEXT Md5Context;
typedef Md5Context* PMd5Context;

#ifndef ET199_FILEINFO
#define ET199_FILEINFO

#ifdef _WIN32
#pragma pack(push, 1)
#endif 

typedef struct _FILE_INFO {
    WORD	wFileID;							//�ļ�ID
    BYTE	bFileType;							//�ļ�����
    WORD	wFileSize;							//�ļ���С
}
EFINFO,*PEFINFO;

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif

enum
{
    GLOBAL_SERIAL_NUMBER	= 0x00,				//ȡ8 �ֽ����к�
    GLOBAL_CLIENT_NUMBER,						//ȡ4 �ֽڿ�����ID
    GLOBAL_COS_VERSION							//ȡ2 �ֽ�COS�汾
};

enum
{
    RSA_CALC_NORMAL			= 0x00,				//ֱ�����㣬�ޱ���
    RSA_CALC_HASH,								//���������ݽ���HASH ���㣬�ٽ��������˽Կ����
    RSA_CALC_PKCS								//PKCS#1��׼����
};

#define RSA_CALC_BIT_512			0x10
#define RSA_CALC_BIT_1024			0x00
#define RSA_CALC_BIT_2048			0x20

#define RSA_CRYPT_NOPKCS			RSA_CALC_NORMAL
#define RSA_CRYPT_PKCS				RSA_CALC_PKCS

#define RSA_SIGN_NOPH				RSA_CALC_NORMAL
#define RSA_SIGN_HASH				RSA_CALC_HASH
#define RSA_SIGN_PKCS				RSA_CALC_PKCS
#define RSA_SIGN_PH					(RSA_CALC_HASH|RSA_CALC_PKCS)

#define RSA_VERI_NOPKCS				RSA_CALC_NORMAL
#define RSA_VERI_PKCS				RSA_CALC_PKCS

#ifndef ET199_FILE_TYPE
#define ET199_FILE_TYPE
enum
{
    FILE_TYPE_EXE			= 0x00, 			//��ִ���ļ�
    FILE_TYPE_DATA,								//�����ļ�
    FILE_TYPE_RSA_PUBLIC,						//RSA ��Կ�ļ�
    FILE_TYPE_RSA_PRIVATE,						//RSA ˽Կ�ļ�
    FILE_TYPE_INTERNAL_EXE						//��ִ���ļ������ɶ�д��
};
#endif

enum
{
    CREATE_OPEN_ALWAYS		= 0x00, 			//����ļ��Ѵ��ڣ��򿪸��ļ�����֮�򴴽����ļ�����
    CREATE_FILE_NEW,							//�����������ļ�������ļ��Ѿ������򷵻ش���
    CREATE_OPEN_EXISTING						//���Ѵ����ļ�������ͬ_open()����
};

//et199 error code

#define ET_SUCCESS				0x00			//	�ɹ�
#define ET_UNKNOWN				0x01			//	δ֪����
#define ET_INVALID_PARAMETER	0x02			//	��Ч�Ĳ���
#define ET_INVALID_ADDRESS		0x03			//	��Ч�ĵ�ַ,�������ַԽ��
#define ET_INVALID_SIZE			0x04			//	��Ч�ĳ���
#define ET_FILE_NOT_FOUND		0x05			//	�ļ�û�ҵ�
#define ET_ACCESS_DENIED		0x06			//	�����ļ�ʧ��
#define ET_FILE_SELECT			0x07			//	�ļ��򿪸����Ѵ�����
#define ET_INVALID_HANDLE		0x08			//	��Ч���ļ����
#define ET_FILE_OUT_OF_RANGE	0x09			//	�ļ���дԽ��
#define ET_FILE_TYPE_MISMATCH	0x0A			//	�ļ����ڵ����Ͳ�ƥ��
#define ET_FILE_SIZE_MISMATCH	0x0B			//	�ļ����ڵ����Ȳ�ƥ��
#define ET_NO_SPACE				0x0C			//	�ļ��пռ䲻��
#define ET_FILE_EXIST			0x0D			//	�ļ�����Ŀ¼�Ѵ���
#define ET_INVALID_KEY_FORMAT	0x0E			//	��Ч��RSA��Կ�ļ���ʽ
#define ET_KEY_LEN_MISMATCH		0x0F			//  �û��������Կ������ʵ�ʳ��Ȳ�ƥ��
#define ET_RSA_INVALID_KEY_FILE	0x10			//	�ļ����Ͳ�����Ҫ��
#define ET_RSA_ENC_DEC_FAILED	0x11			//	RSA���ܽ���ʧ��
#define ET_RSA_SIGN_VERI_FAILED	0x12			//	RSAǩ����֤ʧ��
#define ET_SHA1					0x13			//  SHA1�������
#define ET_MD5					0x14			//  MD5�������
#define ET_INVALID_ADDERSS		0x15			//  ��Ч���ڴ�ָ��
#define ET_EEPROM				0x16			//  дEEPROM����

#ifdef __C51__


//���̿���
extern void _exit();

//�������
extern BYTE _set_response(WORD wLen, void *pvData);

//�ļ�����
extern BYTE _create(WORD wFileID, WORD wSize, BYTE bFileType, BYTE bFlag, HANDLE *pHandle);
extern BYTE _open(WORD wFileID, HANDLE *pHandle);
extern BYTE _close(HANDLE handle);
extern BYTE _read(HANDLE handle, WORD wOffset, BYTE bLen, void *pvData);
extern BYTE _write(HANDLE handle, WORD wOffset, BYTE bLen,const void *pvData);
extern BYTE _get_file_infor(PEFINFO pFileInfo);
extern BYTE _enable_exe(WORD wFileID);

//���ܽ���
extern BYTE _des_enc(const void *pvKey, BYTE bLen, void *pvData);
extern BYTE _des_dec(const void *pvKey, BYTE bLen, void *pvData);
extern BYTE _tdes_enc(const void *pvKey, BYTE bLen, void *pvData);
extern BYTE _tdes_dec(const void *pvKey, BYTE bLen, void *pvData);
extern BYTE _sha1_init(PSHA_CONTEXT pCtx);
extern BYTE _sha1_update(PSHA_CONTEXT pCtx,const void *pvData, BYTE bLen);
extern BYTE _sha1_final(PSHA_CONTEXT pCtx, void *pvResult);
extern BYTE _rsa_enc(BYTE bMode, WORD wFileID, WORD wLen, void *pvData);
extern BYTE _rsa_dec(BYTE bMode, WORD wFileID, WORD wLen, void *pvData);
extern BYTE _rsa_sign(BYTE bMode, WORD wFileID, WORD wLen, void *pvData);
extern BYTE _rsa_verify(BYTE bMode, WORD wFileID, WORD wLen, void *pvData);
extern BYTE _rsa_gen_key(WORD wPubID, WORD wPriID,WORD wKeyBitLen);
extern BYTE _md5_init(PMD5_CONTEXT pCtx);
extern BYTE _md5_update(PMD5_CONTEXT pCtx,const void *pvData, BYTE bLen);
extern BYTE _md5_final(PMD5_CONTEXT pCtx, void *pvResult);

//ϵͳ����
extern BYTE _rand(void *pvData, BYTE bLen);
extern BYTE _get_version(BYTE bFlag,void *pvData,BYTE bLen);

//˫���ȸ��㺯��
extern BYTE _add(DOUBLE result, DOUBLE x, DOUBLE y);
extern BYTE _sub(DOUBLE result, DOUBLE x, DOUBLE y);
extern BYTE _mul(DOUBLE result, DOUBLE x, DOUBLE y);
extern BYTE _div(DOUBLE result, DOUBLE x, DOUBLE y);
extern BYTE _sin(DOUBLE result, DOUBLE x);
extern BYTE _cos(DOUBLE result, DOUBLE x);
extern BYTE _tan(DOUBLE result, DOUBLE x);
extern BYTE _asin(DOUBLE result, DOUBLE x);
extern BYTE _acos(DOUBLE result, DOUBLE x);
extern BYTE _atan(DOUBLE result, DOUBLE x);
extern BYTE _atan2(DOUBLE result, DOUBLE x, DOUBLE y);
extern BYTE _sinh(DOUBLE result, DOUBLE x);
extern BYTE _cosh(DOUBLE result, DOUBLE x);
extern BYTE _tanh(DOUBLE result, DOUBLE x);
extern BYTE _mod(DOUBLE result, DOUBLE x, DOUBLE pMod);
extern BYTE _pow(DOUBLE result, DOUBLE x, DOUBLE pExp);
extern BYTE _ceil(DOUBLE result, DOUBLE x);
extern BYTE _floor(DOUBLE result, DOUBLE x);
extern BYTE _modf(DOUBLE remain, DOUBLE x, DOUBLE intpart);
extern BYTE _frexp(DOUBLE result, DOUBLE x, WORD *pExp);
extern BYTE _ldexp(DOUBLE result, DOUBLE x, WORD exp);
extern BYTE _abs(DOUBLE result, DOUBLE x);
extern BYTE _exp(DOUBLE result, DOUBLE x);
extern BYTE _log(DOUBLE result, DOUBLE x);
extern BYTE _log10(DOUBLE result, DOUBLE x);
extern BYTE _sqrt(DOUBLE result, DOUBLE x);
extern char _cmp(DOUBLE x, DOUBLE y);

//�����ȸ��㺯��
extern float _addf(float x, float y);
extern float _subf(float x, float y);
extern float _mulf(float x, float y);
extern float _divf(float x, float y);
extern float _sinf(float x);
extern float _cosf(float x);
extern float _tanf(float x);
extern float _asinf(float x);
extern float _acosf(float x);
extern float _atanf(float x);
extern float _sinhf(float x);
extern float _coshf(float x);
extern float _tanhf(float x);
extern float _atan2f(float x, float y);
extern float _fmodf(float x, float y);
extern float _powf(float x, float y);
extern char  _cmpf(float x, float y);
extern float _ceilf(float x);
extern float _floorf(float x);
extern float _absf(float x);
extern float _expf(float x);
extern float _logf(float x);
extern float _log10f(float x);
extern float _sqrtf(float x);

//����ת��
extern BYTE _dbltof(float *result, DOUBLE x);
extern BYTE _dbltol(long *result, DOUBLE x);
extern BYTE _ftodbl(DOUBLE result, float x);
extern BYTE _ltodbl(DOUBLE result, long x);

//������Ϣ
extern const char* _get_error_info(BYTE bErr);

#define _dtof(result,x)	_dbltof(result,x)
#define _dtol(result,x)	_dbltol(result,x)
#define _ftod(result,x)	_ftodbl(result,*(x))
#define _ltod(result,x)	_ltodbl(result,*(x))

#define _swap_u16(pvData)	_swap(pvData,2)
#define _swap_u32(pvData)	_swap(pvData,4)

#define DOUBLE_INIT(x,y) _ftodbl(x,y)

#endif


#ifdef __C51__
#pragma REGPARMS
#endif	

//���ݵ�ת
void _swap(void* pvData,unsigned short wLen);

#endif //_ET199_C51_H_


