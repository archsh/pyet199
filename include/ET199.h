
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
    WORD	wFileID;							//文件ID
    BYTE	bFileType;							//文件类型
    WORD	wFileSize;							//文件大小
}
EFINFO,*PEFINFO;

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif

enum
{
    GLOBAL_SERIAL_NUMBER	= 0x00,				//取8 字节序列号
    GLOBAL_CLIENT_NUMBER,						//取4 字节开发商ID
    GLOBAL_COS_VERSION							//取2 字节COS版本
};

enum
{
    RSA_CALC_NORMAL			= 0x00,				//直接运算，无编码
    RSA_CALC_HASH,								//对输入数据进行HASH 运算，再将结果进行私钥加密
    RSA_CALC_PKCS								//PKCS#1标准加密
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
    FILE_TYPE_EXE			= 0x00, 			//可执行文件
    FILE_TYPE_DATA,								//数据文件
    FILE_TYPE_RSA_PUBLIC,						//RSA 公钥文件
    FILE_TYPE_RSA_PRIVATE,						//RSA 私钥文件
    FILE_TYPE_INTERNAL_EXE						//可执行文件（不可读写）
};
#endif

enum
{
    CREATE_OPEN_ALWAYS		= 0x00, 			//如果文件已存在，打开该文件；反之则创建新文件并打开
    CREATE_FILE_NEW,							//创建并打开新文件，如果文件已经存在则返回错误
    CREATE_OPEN_EXISTING						//打开已存在文件，功能同_open()函数
};

//et199 error code

#define ET_SUCCESS				0x00			//	成功
#define ET_UNKNOWN				0x01			//	未知错误
#define ET_INVALID_PARAMETER	0x02			//	无效的参数
#define ET_INVALID_ADDRESS		0x03			//	无效的地址,虚拟机地址越界
#define ET_INVALID_SIZE			0x04			//	无效的长度
#define ET_FILE_NOT_FOUND		0x05			//	文件没找到
#define ET_ACCESS_DENIED		0x06			//	访问文件失败
#define ET_FILE_SELECT			0x07			//	文件打开个数已达上限
#define ET_INVALID_HANDLE		0x08			//	无效的文件句柄
#define ET_FILE_OUT_OF_RANGE	0x09			//	文件读写越界
#define ET_FILE_TYPE_MISMATCH	0x0A			//	文件存在但类型不匹配
#define ET_FILE_SIZE_MISMATCH	0x0B			//	文件存在但长度不匹配
#define ET_NO_SPACE				0x0C			//	文件夹空间不足
#define ET_FILE_EXIST			0x0D			//	文件或子目录已存在
#define ET_INVALID_KEY_FORMAT	0x0E			//	无效的RSA密钥文件格式
#define ET_KEY_LEN_MISMATCH		0x0F			//  用户传入的密钥长度与实际长度不匹配
#define ET_RSA_INVALID_KEY_FILE	0x10			//	文件类型不符合要求
#define ET_RSA_ENC_DEC_FAILED	0x11			//	RSA加密解密失败
#define ET_RSA_SIGN_VERI_FAILED	0x12			//	RSA签名验证失败
#define ET_SHA1					0x13			//  SHA1计算错误
#define ET_MD5					0x14			//  MD5计算错误
#define ET_INVALID_ADDERSS		0x15			//  无效的内存指针
#define ET_EEPROM				0x16			//  写EEPROM错误

#ifdef __C51__


//流程控制
extern void _exit();

//输入输出
extern BYTE _set_response(WORD wLen, void *pvData);

//文件操作
extern BYTE _create(WORD wFileID, WORD wSize, BYTE bFileType, BYTE bFlag, HANDLE *pHandle);
extern BYTE _open(WORD wFileID, HANDLE *pHandle);
extern BYTE _close(HANDLE handle);
extern BYTE _read(HANDLE handle, WORD wOffset, BYTE bLen, void *pvData);
extern BYTE _write(HANDLE handle, WORD wOffset, BYTE bLen,const void *pvData);
extern BYTE _get_file_infor(PEFINFO pFileInfo);
extern BYTE _enable_exe(WORD wFileID);

//加密解密
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

//系统功能
extern BYTE _rand(void *pvData, BYTE bLen);
extern BYTE _get_version(BYTE bFlag,void *pvData,BYTE bLen);

//双精度浮点函数
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

//单精度浮点函数
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

//浮点转换
extern BYTE _dbltof(float *result, DOUBLE x);
extern BYTE _dbltol(long *result, DOUBLE x);
extern BYTE _ftodbl(DOUBLE result, float x);
extern BYTE _ltodbl(DOUBLE result, long x);

//错误信息
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

//数据倒转
void _swap(void* pvData,unsigned short wLen);

#endif //_ET199_C51_H_


