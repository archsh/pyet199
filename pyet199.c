#include <Python.h>
#include "ET199_32.h"
#define _STR(x) #x
#define STR(x)  _STR(x)
#define DEBUG 1
/***************************************************************************************************************************/
static PyObject *InvalidParameter;
static PyObject *NormalError;

#define INVALID_PARAMS(msg)\
  do{\
    PyErr_SetString(InvalidParameter,msg); \
    return NULL;\
  }while(0)

#define NORMAL_ERROR(dwRet)\
  do{\
    char msgbuf[128]={0}; \
    if(0==ETFormatErrorMessage(dwRet,msgbuf,128)){ \
      PyErr_SetString(NormalError,msgbuf); \
      return NULL;\
    }else{\
      PyErr_SetString(NormalError,"Unknow"); \
      return NULL;\
    }\
  }while(0)

#define DWRET_VALIDATE(dwRet)\
  do{\
    if(dwRet!=0){\
      NORMAL_ERROR(dwRet);\
    }\
 }while(0)


/**
 * Enum connected ET199 devices.  -> @ref: ETEnum
 * Args: No.
 * Return: A list of ETContext objects. NONE if no ET199 was connected or failed.
 */
static PyObject *pETEnum(PyObject *self){
  /** Get connected keys*/
  DWORD keyCount=0,dwRet=0;
  int i=0;
  ET_CONTEXT *pETContextList = NULL;
  PyObject *result = PyList_New(0);
  dwRet = ETEnum(NULL,&keyCount);
  if(ET_E_INSUFFICIENT_BUFFER!=dwRet && !dwRet){
    NORMAL_ERROR(dwRet);
  }
  if(keyCount==0){
    return result;
  }
  pETContextList = malloc(sizeof(ET_CONTEXT)*keyCount);
  memset(pETContextList,0,sizeof(ET_CONTEXT)*keyCount);
  dwRet=ETEnum(pETContextList,&keyCount);
  if(0!=dwRet){
    free(pETContextList);
    NORMAL_ERROR(dwRet);
  }
  for(i=0;i<keyCount;i++){
    PyObject *dict = PyDict_New();
    ET_CONTEXT *pETC = &pETContextList[i];
    /*
    DWORD dwOut=0;
    
    dwRet = ETOpen(pETC);
    dwRet = ETControl(pETC,ET_GET_SERIAL_NUMBER, NULL, 0, pETC->bID, 8, &dwOut);
    dwRet = ETControl(pETC,ET_GET_DEVICE_ATR,NULL,0,pETC->bAtr,16,&pETC->dwAtrLen);
    dwRet = ETControl(pETC,ET_GET_CUSTOMER_NAME,NULL,0,&pETC->dwCustomer,4,&dwOut);
    */
    PyDict_SetItemString (dict, "Index", Py_BuildValue("i",pETContextList[i].dwIndex));
    PyDict_SetItemString (dict, "Version", Py_BuildValue("i",pETContextList[i].dwVersion));
    PyDict_SetItemString (dict, "Lock", Py_BuildValue("i",pETContextList[i].hLock));
    PyDict_SetItemString (dict, "reserve", Py_BuildValue("s",pETContextList[i].reserve));
    PyDict_SetItemString (dict, "Customer", Py_BuildValue("i",pETContextList[i].dwCustomer));
    PyDict_SetItemString (dict, "Atr", PyByteArray_FromStringAndSize(pETContextList[i].bAtr,pETC->dwAtrLen));
    PyDict_SetItemString (dict, "AtrLen", Py_BuildValue("i",pETContextList[i].dwAtrLen));
    PyDict_SetItemString (dict, "ID", PyByteArray_FromStringAndSize(pETContextList[i].bID,8));
    PyList_Append(result, dict);
    /*
    dwRet = ETClose(pETC);
    */
  }
  //Py_XDECREF(pETContextList);
  free(pETContextList);
  return result;
}

/**
 * Open an ET199 key.
 * Args:
 *  ETContext object which returned from ETEnum. -> @ref: ETOpen
 *  ETOpenInfo: optional argument for open with specific mode. -> @ref: ETOpenEx
 * Return: ETContext object or None for failure.
 */
static PyObject *pETOpen(PyObject *self, PyObject *args, PyObject *kwargs){
  /*(
        IN OUT  ET_CONTEXT			*pETCtx
        ETOpenInfo
    );*/
  if (!PyArg_ParseTuple(args, "ids", &i, &d, &s)) {
    return NULL;
  }
  
  Py_RETURN_NONE;
}

/**
 * Close an opened ET199 key.
 * Args: ETContext object of opened.
 * Return: True or False for failure.
 */
static PyObject *pETClose(PyObject *self, PyObject *args){
  /*(
        IN OUT  ET_CONTEXT			*pETCtx
    );*/
  Py_RETURN_NONE;
}

/**
 * Send Control command to an ET199 key.
 * Args:
 *    ETContext object of opened.
 *    
 * Return:
 */
static PyObject *pETControl(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      DWORD				dwCtlCode,
        IN      CONST VOID			*pInBuffer,
        IN      DWORD				dwInBufferLen,
        OUT     VOID				*pOutBuffer,
        IN      DWORD				dwOutBufferLen,
        OUT     DWORD				*pdwBytesReturned
    );*/
  Py_RETURN_NONE;
}

/**
 * Create a directory
 * Args:
 *   ETContext object of opened.
 *   dirId:
 *   dirSize:
 *   flags:
 *   dirInfo:   -> @ref: pETCreateDirEx
 * Return:
 */
static PyObject *pETCreateDir(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT    *pETCtx,
        IN      LPCSTR				lpszDirID,
        IN      DWORD				dwDirSize,
        IN      DWORD				dwFlags
    );*/
  Py_RETURN_NONE;
}

/**
 * Change current working directory
 * Args:
 *    ETContext object of opened.
 *    dirPath:
 * Return:
 */
static PyObject *pETChangeDir(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT    *pETCtx,
        IN      LPCSTR				lpszPath
    );*/
  Py_RETURN_NONE;
}

/**
 * Erase a directory.
 * Args:
 *    ETContext object of opened.
 *    dirPath:
 * Return:
 */
static PyObject *pETEraseDir(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      LPCSTR				lpszDirID
    );*/
  Py_RETURN_NONE;
}

/**
 * Verify Pin
 * Args:
 *    ETContext object of opened.
 *    pin
 *    pinType
 * Return:
 */
static PyObject *pETVerifyPin(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      CONST BYTE          *pbPin,
        IN      DWORD				dwPinLen,
        IN      DWORD				dwPinType
    );*/
  Py_RETURN_NONE;
}

/**
 * Change Pin
 * Args:
 *    ETContext object of opened.
 *    oldPin,
 *    newPin,
 *    pinType,
 *    pinTryCount,
 * Return:
 */
static PyObject *pETChangePin(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      CONST BYTE			*pbOldPin,
        IN      DWORD				dwOldPinLen,
        IN      CONST BYTE			*pbNewPin,
        IN      DWORD				dwNewPinLen,
        IN      DWORD				dwPinType,
        IN		BYTE				byPinTryCount
    );*/
  Py_RETURN_NONE;
}

/**
 * Write file to ET199 key.
 * Args:
 *    ETContext object of opened.
 *    fileId,
 *    offset,
 *    data,
 *    flags,   -> @ref: ETWriteFileEx
 *    filetype,-> @ref: ETWriteFileEx
 * Return:
 */
static PyObject *pETWriteFile(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      LPCSTR				lpszFileID,
        IN      DWORD				dwOffset,
        IN      CONST VOID			*pBuffer,
        IN      DWORD				dwBufferSize
    );*/
  Py_RETURN_NONE;
}

/**
 * Execute a program in ET199 key.
 * Args:
 *     ETContext object of opened.
 *     fileId
 *     inputData
 * Return:
 */
static PyObject *pETExecute(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      LPCSTR				lpszFileID,
        IN      CONST VOID			*pInBuffer,
        IN      DWORD				dwInbufferSize,
        OUT     VOID				*pOutBuffer,
        IN      DWORD				dwOutBufferSize,
        OUT     DWORD				*pdwBytesReturned
    );*/
  Py_RETURN_NONE;
}

/**
 * Create a file.
 * Args:
 *    ETContext object of opened.
 *    fileId:
 *    fileSize:
 *    fileType:
 * Return:
 */
static PyObject *pETCreateFile(PyObject *self, PyObject *args){
  /*(
        IN      CONST ET_CONTEXT	*pETCtx,
        IN      LPCSTR				lpszFileID,
        IN      DWORD				dwFileSize,
        IN      BYTE				bFileType
    );*/
  Py_RETURN_NONE;
}

/**
 * Generate RSA Key.
 * Args:
 *    ETContext object of opened.
 *    keySize:
 *    eVal:
 *    pubFileId:
 *    priFileId:
 * Return:
 *    (pubKeyData,priKeyData) -> tuple with bytearrays. Or None for failure.
 */
static PyObject *pETGenRsaKey(PyObject *self, PyObject *args){
  /*(
        IN		CONST ET_CONTEXT	*pETCtx,
        IN		WORD				wKeySize,
        IN		DWORD				dwE,
        IN		LPCSTR				lpszPubFileID,
        IN		LPCSTR				lpszPriFileID,
        OUT		PVOID				pbPubKeyData,
        IN OUT	DWORD				*dwPubKeyDataSize,
        OUT		PVOID				pbPriKeyData,
        IN OUT	DWORD				*dwPriKeyDataSize
    );*/
  Py_RETURN_NONE;
}

/***************************************************************************************************************************/
static PyMethodDef methods[] = {
  { "ETEnum", (PyCFunction)pETEnum, METH_NOARGS,
      "ETEnum: " },
  { "ETOpen", (PyCFunction)pETOpen, METH_VARARGS|METH_KEYWORDS,
      "ETOpen: " },
  { "ETClose", (PyCFunction)pETClose, METH_VARARGS,
      "ETClose: " },
  { "ETControl", (PyCFunction)pETControl, METH_VARARGS,
      "ETControl: " },
  { "ETCreateDir", (PyCFunction)pETCreateDir, METH_VARARGS,
      "ETCreateDir: " },
  { "ETChangeDir", (PyCFunction)pETChangeDir, METH_VARARGS,
      "ETChangeDir: " },
  { "ETEraseDir", (PyCFunction)pETEraseDir, METH_VARARGS,
      "ETEraseDir: " },
  { "ETVerifyPin", (PyCFunction)pETVerifyPin, METH_VARARGS|METH_KEYWORDS,
      "ETVerifyPin: " },
  { "ETChangePin", (PyCFunction)pETChangePin, METH_VARARGS,
      "ETChangePin: " },
  { "ETWriteFile", (PyCFunction)pETWriteFile, METH_VARARGS,
      "ETWriteFile: " },
  { "ETExecute", (PyCFunction)pETExecute, METH_VARARGS|METH_KEYWORDS,
      "ETExecute: " },
  { "ETCreateFile", (PyCFunction)pETCreateFile, METH_VARARGS,
      "ETCreateFile: " },
  { "ETGenRsaKey", (PyCFunction)pETGenRsaKey, METH_VARARGS,
      "ETGenRsaKey: " },
  { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
initpyet199(void)
{
  PyObject *m;
  m = Py_InitModule ("pyet199", methods);
  PyModule_AddStringConstant(m, "version", STR(PYET199_VERSION));
  
  //PyModule_AddIntConstant()
  PyModule_AddIntConstant (m,"ET_S_SUCCESS",ET_S_SUCCESS);
      //0x00000000						/** 操作成功*/
  PyModule_AddIntConstant (m,"ET_E_KEY_REMOVED",ET_E_KEY_REMOVED);
      //0xF0000001						/** 设备未连接，或者被移除*/
  PyModule_AddIntConstant (m,"ET_E_INVALID_PARAMETER",ET_E_INVALID_PARAMETER);
      //0xF0000002						/** 参数错误*/
  PyModule_AddIntConstant (m,"ET_E_COMM_ERROR",ET_E_COMM_ERROR);
      //0xF0000003						/** 通讯错误，例如数据传输超时*/
  PyModule_AddIntConstant (m,"ET_E_INSUFFICIENT_BUFFER",ET_E_INSUFFICIENT_BUFFER);
      //0xF0000004						/** 缓冲区空间不足*/
  PyModule_AddIntConstant (m,"ET_E_NO_LIST",ET_E_NO_LIST);
      //0xF0000005						/** 没有找到设备列表*/
  PyModule_AddIntConstant (m,"ET_E_DEVPIN_NOT_CHECK",ET_E_DEVPIN_NOT_CHECK);
      //0xF0000006						/** 开发商口令没有验证*/
  PyModule_AddIntConstant (m,"ET_E_USERPIN_NOT_CHECK",ET_E_USERPIN_NOT_CHECK);
      //0xF0000007						/** 用户口令没有验证*/
  PyModule_AddIntConstant (m,"ET_E_RSA_FILE_FORMAT_ERROR",ET_E_RSA_FILE_FORMAT_ERROR);
      //0xF0000008						/** RSA文件格式错误*/
  PyModule_AddIntConstant (m,"ET_E_DIR_NOT_FOUND",ET_E_DIR_NOT_FOUND);
      //0xF0000009						/** 目录没有找到*/
  PyModule_AddIntConstant (m,"ET_E_ACCESS_DENIED",ET_E_ACCESS_DENIED);
      //0xF000000A						/** 访问被拒绝*/
  PyModule_AddIntConstant (m,"ET_E_ALREADY_INITIALIZED",ET_E_ALREADY_INITIALIZED);
      //0xF000000B						/** 产品已经初始化*/
  PyModule_AddIntConstant (m,"ET_E_INCORRECT_PIN",ET_E_INCORRECT_PIN);
      //0xF0000C00						/** 密码不正确*/
  PyModule_AddIntConstant (m,"ET_E_DF_SIZE",ET_E_DF_SIZE);
      //0xF000000D						/** 指定的目录空间大小不够*/
  PyModule_AddIntConstant (m,"ET_E_FILE_EXIST",ET_E_FILE_EXIST);
      //0xF000000E						/** 文件已存在*/
  PyModule_AddIntConstant (m,"ET_E_UNSUPPORTED",ET_E_UNSUPPORTED);
      //0xF000000F						/** 功能不支持或尚未建立文件系统*/
  PyModule_AddIntConstant (m,"ET_E_FILE_NOT_FOUND",ET_E_FILE_NOT_FOUND);
      //0xF0000010						/** 未找到指定的文件*/
  PyModule_AddIntConstant (m,"ET_E_ALREADY_OPENED",ET_E_ALREADY_OPENED);
      //0xF0000011						/** 卡已经被打开*/
  PyModule_AddIntConstant (m,"ET_E_DIRECTORY_EXIST",ET_E_DIRECTORY_EXIST);
      //0xF0000012						/** 目录已存在*/
  PyModule_AddIntConstant (m,"ET_E_CODE_RANGE",ET_E_CODE_RANGE);
      //0xF0000013						/** 虚拟机内存地址溢出*/
  PyModule_AddIntConstant (m,"ET_E_INVALID_POINTER",ET_E_INVALID_POINTER);
      //0xF0000014						/** 虚拟机错误的指针 */
  PyModule_AddIntConstant (m,"ET_E_GENERAL_FILESYSTEM",ET_E_GENERAL_FILESYSTEM);
      //0xF0000015						/** 常规文件系统错误 */
  PyModule_AddIntConstant (m,"ET_E_OFFSET_BEYOND",ET_E_OFFSET_BEYOND);
      //0xF0000016						/** 文件偏移量超出文件大小*/
  PyModule_AddIntConstant (m,"ET_E_FILE_TYPE_MISMATCH",ET_E_FILE_TYPE_MISMATCH);
      //0xF0000017						/** 文件类型不匹配*/
  PyModule_AddIntConstant (m,"ET_E_PIN_BLOCKED",ET_E_PIN_BLOCKED);
      //0xF0000018						/** PIN码锁死*/
  PyModule_AddIntConstant (m,"ET_E_INVALID_CONTEXT",ET_E_INVALID_CONTEXT);
      //0xF0000019						/** ETContext 参数错误*/
  PyModule_AddIntConstant (m,"ET_E_SHARING_VIOLATION",ET_E_SHARING_VIOLATION);
      //0XF000001A						/** 另一个程序正在使用此文件，进程无法访问*/
  PyModule_AddIntConstant (m,"ET_E_ERROR_UNKNOWN",ET_E_ERROR_UNKNOWN);
      //0xFFFFFFFF						/** 未知的错误*/
  
  PyModule_AddIntConstant (m,"MAX_ATR_LEN",MAX_ATR_LEN);
      //16                              /** 最大ATR长度 */
  PyModule_AddIntConstant (m,"MAX_ID_LEN",MAX_ID_LEN);
      //8                               /** 最大硬件ID长度*/
  
  PyModule_AddIntConstant (m,"ET_USER_PIN",ET_USER_PIN);
      //0x00000000                      /** 用户PIN*/
  PyModule_AddIntConstant (m,"ET_DEV_PIN",ET_DEV_PIN);
      //0x00000001                      /** 开发商PIN*/
  
  PyModule_AddIntConstant (m,"ET_CREATE_NEW",ET_CREATE_NEW);
      //0x00000000                      /** 创建新文件*/
  PyModule_AddIntConstant (m,"ET_UPDATE_FILE",ET_UPDATE_FILE);
      //0x00000001                      /** 更新数据文件*/
  
  PyModule_AddIntConstant (m,"ET_CREATE_ROOT_DIR",ET_CREATE_ROOT_DIR);
      //0x00000000                      /** 创建根目录*/
  PyModule_AddIntConstant (m,"ET_CREATE_SUB_DIR",ET_CREATE_SUB_DIR);
      //0x00000001                      /** 创建当前目录的子目录*/
  
  PyModule_AddIntConstant (m,"ET_LED_UP",ET_LED_UP);
      //0x00000001						/** LED灯亮*/
  PyModule_AddIntConstant (m,"ET_LED_DOWN",ET_LED_DOWN);
      //0x00000002						/** LED灯灭*/
  PyModule_AddIntConstant (m,"ET_LED_WINK",ET_LED_WINK);
      //0x00000003						/** LED灯闪烁*/
  
  PyModule_AddIntConstant (m,"ET_GET_DEVICE_TYPE",ET_GET_DEVICE_TYPE);
      //0x00000011                      /** 获得设备类型*/
  PyModule_AddIntConstant (m,"ET_GET_SERIAL_NUMBER",ET_GET_SERIAL_NUMBER);
      //0x00000012                      /** 获取硬件序列号*/
  PyModule_AddIntConstant (m,"ET_GET_DEVICE_USABLE_SPACE",ET_GET_DEVICE_USABLE_SPACE);
      //0x00000013                      /** 获得设备空间大小*/
  PyModule_AddIntConstant (m,"ET_GET_DEVICE_ATR",ET_GET_DEVICE_ATR);
      //0x00000014                      /** 获得设备ATR*/
  PyModule_AddIntConstant (m,"ET_GET_CUSTOMER_NAME",ET_GET_CUSTOMER_NAME);
      //0x00000015                      /** 获得客户号*/
  PyModule_AddIntConstant (m,"ET_GET_MANUFACTURE_DATE",ET_GET_MANUFACTURE_DATE);
      //0x00000016                      /** 获得生产日期*/
  PyModule_AddIntConstant (m,"ET_GET_DF_AVAILABLE_SPACE",ET_GET_DF_AVAILABLE_SPACE);
      //0x00000017                      /** 获得当前目录的剩余空间*/
  PyModule_AddIntConstant (m,"ET_GET_EF_INFO",ET_GET_EF_INFO);
      //0x00000018                      /** 获得指定文件信息*/
  PyModule_AddIntConstant (m,"ET_GET_COS_VERSION",ET_GET_COS_VERSION);
      //0x00000019						/** 获得COS版本信息*/
  
  PyModule_AddIntConstant (m,"ET_SET_DEVICE_ATR",ET_SET_DEVICE_ATR);
      //0x00000021                      /** 设置设备ATR*/
  PyModule_AddIntConstant (m,"ET_SET_DEVICE_TYPE",ET_SET_DEVICE_TYPE);
      //0x00000022						/** 设置设备类型*/
  PyModule_AddIntConstant (m,"ET_SET_SHELL_KEY",ET_SET_SHELL_KEY);
      //0x00000023						/** 设置8字节外壳加密种子码*/
  PyModule_AddIntConstant (m,"ET_SET_CUSTOMER_NAME",ET_SET_CUSTOMER_NAME);
      //0x00000024						/** 输入一个种子, 产生客户号*/
  
  PyModule_AddIntConstant (m,"ET_RESET_DEVICE",ET_RESET_DEVICE);
      //0x00000031                      /** 复位设备*/
  
  PyModule_AddIntConstant (m,"ET_DEVICE_TYPE_PKI",ET_DEVICE_TYPE_PKI);
      //0x01							/** 身份验证锁类型*/
  PyModule_AddIntConstant (m,"ET_DEVICE_TYPE_DONGLE",ET_DEVICE_TYPE_DONGLE);
      //0x02							/** 加密锁类型*/
  PyModule_AddIntConstant (m,"ET_DEVICE_TYPE_EMPTY",ET_DEVICE_TYPE_EMPTY);
      //0x04							/** 空锁类型*/
  
  PyModule_AddIntConstant (m,"ET_DEFAULT_TRY",ET_DEFAULT_TRY);
      //0xff							/** 默认重试次数（无限次）*/
  
  PyModule_AddStringConstant (m,"ET_DEFAULT_DEV_PIN",ET_DEFAULT_DEV_PIN);
      //(BYTE *);"123456781234567812345678"
  PyModule_AddStringConstant (m,"ET_DEFAULT_USER_PIN",ET_DEFAULT_USER_PIN);
      //(BYTE *);"12345678"
  PyModule_AddIntConstant (m,"ET_DEV_PIN_LEN",ET_DEV_PIN_LEN);
      //24
  PyModule_AddIntConstant (m,"ET_USER_PIN_LEN",ET_USER_PIN_LEN);
      //8
  
  PyModule_AddIntConstant (m,"ET_EXCLUSIVE_MODE",ET_EXCLUSIVE_MODE);
      //0                               /** 独占模式*/
  PyModule_AddIntConstant (m,"ET_SHARE_MODE",ET_SHARE_MODE);
      //1                               /** 共享模式*/
      
  PyModule_AddIntConstant (m,"FILE_TYPE_EXE",FILE_TYPE_EXE);
      //0								/** 可执行文件*/
  PyModule_AddIntConstant (m,"FILE_TYPE_DATA",FILE_TYPE_DATA);
      //1								/** 数据文件*/
  PyModule_AddIntConstant (m,"FILE_TYPE_RSA_PUBLIC",FILE_TYPE_RSA_PUBLIC);
      //2								/** RSA 公钥文件*/
  PyModule_AddIntConstant (m,"FILE_TYPE_RSA_PRIVATE",FILE_TYPE_RSA_PRIVATE);
      //3								/** RSA 私钥文件*/
  PyModule_AddIntConstant (m,"FILE_TYPE_INTERNAL_EXE",FILE_TYPE_INTERNAL_EXE);
      //4								/** 可执行文件（不可读写）*/
  
  NormalError = PyErr_NewException("pyet199.NormalError", NULL, NULL);
  Py_INCREF(NormalError);
  PyModule_AddObject(m, "NormalError", NormalError);
  
  InvalidParameter = PyErr_NewException("pyet199.InvalidParameter", NULL, NULL);
  Py_INCREF(InvalidParameter);
  PyModule_AddObject(m, "InvalidParameter", InvalidParameter);
}
