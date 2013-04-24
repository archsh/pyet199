#include <Python.h>
#include <datetime.h>
#include <structmember.h>
#include "ET199_32.h"
#define _STR(x) #x
#define STR(x)  _STR(x)
#define DEBUG 1
//#define NODDY_TEST 1
/***************************************************************************************************************************/
static PyObject *InvalidParameter;
static PyObject *NormalError;

#define INVALID_PARAMS(msg,ret)\
  do{\
    PyErr_SetString(InvalidParameter,msg); \
    return ret;\
  }while(0)

#define NORMAL_ERROR(dwRet,ret)\
  do{\
    char msgbuf[128]={0}; \
    if(0==ETFormatErrorMessage(dwRet,msgbuf,128)){ \
      PyErr_SetString(NormalError,msgbuf); \
      return ret;\
    }else{\
      PyErr_SetString(NormalError,"Unknow"); \
      return ret;\
    }\
  }while(0)

#define DWRET_VALIDATE(dwRet,ret)\
  do{\
    if(dwRet!=0){\
      NORMAL_ERROR(dwRet,ret);\
    }\
 }while(0)

typedef struct {
    PyObject_HEAD
    ET_CONTEXT context;
}ETContextObject;

static void ETContext_dealloc(ETContextObject* self);

static PyObject *ETContext_new();

PyDoc_STRVAR(ETContext_init__doc__,"");
static int ETContext_init(ETContextObject *self, PyObject *args, PyObject *kwds);

PyDoc_STRVAR(ETContext_open__doc__,"open((dwOpenInfoSize,dwShareMode)) -> Open the key for accessing.");
static PyObject *ETContext_open(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_close__doc__,"close() -> Close the key if it is opened.");
static PyObject *ETContext_close(ETContextObject* self);

PyDoc_STRVAR(ETContext_ctrl_led__doc__,"ctrl_led(on/off[,wink]) -> Control the LED of key: True=on, False=off, num=wink.");
static PyObject *ETContext_ctrl_led(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_ctrl_get__doc__,"ctrl_get(code[,fileId]) -> Get information from key. Id,Customer Num, ATR etc...");
static PyObject *ETContext_ctrl_get(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_ctrl_set__doc__,"ctr_set(code[,data]) -> Set information to key.");
static PyObject *ETContext_ctrl_set(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_ctrl_reset__doc__,"ctrl_reset() -> Reset connected key.");
static PyObject *ETContext_ctrl_reset(ETContextObject* self);

PyDoc_STRVAR(ETContext_create_dir__doc__,"create_dir(dirId,dirSize[,dirFlags,dirInfo]) -> Create dir.");
static PyObject *ETContext_create_dir(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_change_dir__doc__,"change_dir(dirId) -> Change current working dir.");
static PyObject *ETContext_change_dir(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_erase_dir__doc__,"erase_dir(dirId) -> Erase a dir.");
static PyObject *ETContext_erase_dir(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_verify_pin__doc__,"verify_pin(pin,pintype) -> Verify pin.");
static PyObject *ETContext_verify_pin(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_change_pin__doc__,"change_pin(oldPin,newPin,pinType[,tryCount=0xff]) -> Change pin.");
static PyObject *ETContext_change_pin(ETContextObject *self, PyObject *args);

PyDoc_STRVAR(ETContext_create_file__doc__,"create_file(fileId,fileSize,fileType) -> Create file.");
static PyObject *ETContext_create_file(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_write_file__doc__,"write_file(fileId,offset,data[,fileSize,flags,fileType]) -> Write data to a file.");
static PyObject *ETContext_write_file(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_execute__doc__,"execute(fileId,inputData) -> Execute a program in key.");
static PyObject *ETContext_execute(ETContextObject* self, PyObject *args);

PyDoc_STRVAR(ETContext_gen_rsa_key__doc__,"gen_rsa_key(keySize,e,pubFileId,priFileId) -> Generate a key pair.");
static PyObject *ETContext_gen_rsa_key(ETContextObject* self, PyObject *args);

//static PyMemberDef ETContext_members[] = 
static PyMemberDef ETContext_members[] = {
    {NULL}  //Sentinel
};

static PyMethodDef ETContext_methods[] = {
    {"open", (PyCFunction)ETContext_open, METH_VARARGS,ETContext_open__doc__},
    {"close", (PyCFunction)ETContext_close, METH_VARARGS,ETContext_close__doc__},
    {"ctrl_led", (PyCFunction)ETContext_ctrl_led, METH_VARARGS,ETContext_ctrl_led__doc__},
    {"ctrl_get", (PyCFunction)ETContext_ctrl_get, METH_VARARGS,ETContext_ctrl_get__doc__},
    {"ctrl_set", (PyCFunction)ETContext_ctrl_set, METH_VARARGS,ETContext_ctrl_set__doc__},
    {"ctrl_reset", (PyCFunction)ETContext_ctrl_reset, METH_VARARGS,ETContext_ctrl_reset__doc__},
    {"create_dir", (PyCFunction)ETContext_create_dir, METH_VARARGS,ETContext_create_dir__doc__},
    {"change_dir", (PyCFunction)ETContext_change_dir, METH_VARARGS,ETContext_change_dir__doc__},
    {"erase_dir", (PyCFunction)ETContext_erase_dir, METH_VARARGS,ETContext_erase_dir__doc__},
    {"verify_pin", (PyCFunction)ETContext_verify_pin, METH_VARARGS,ETContext_verify_pin__doc__},
    {"change_pin", (PyCFunction)ETContext_change_pin, METH_VARARGS,ETContext_change_pin__doc__},
    {"create_file", (PyCFunction)ETContext_create_file, METH_VARARGS,ETContext_create_file__doc__},
    {"write_file", (PyCFunction)ETContext_write_file, METH_VARARGS,ETContext_write_file__doc__},
    {"execute", (PyCFunction)ETContext_execute, METH_VARARGS,ETContext_execute__doc__},
    {"gen_rsa_key", (PyCFunction)ETContext_gen_rsa_key, METH_VARARGS,ETContext_gen_rsa_key__doc__},
    {NULL}  /* Sentinel */
};

/*
static PyGetSetDef ETContext_getsetlist[] = {
    {"closed", (getter)ETContext_get_closed, NULL, "True if the key is closed"},
    {"opened", (getter)ETContext_get_opened, NULL, "True if the key is opened"},
    {0},
};
*/

//ETContextType

PyDoc_STRVAR(ETContext__doc__, "Type for ET Context objects.");

static PyTypeObject ETContextType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "pyet199.ETContext",          /*tp_name*/
  sizeof(ETContextObject),              /*tp_basicsize*/
  0,                            /*tp_itemsize*/
  /* methods */
  (destructor)ETContext_dealloc,        /*tp_dealloc*/
  0,                            /*tp_print*/
  0,                            /*tp_getattr */
  0,                            /*tp_setattr */
  0,                            /*tp_compare*/
  0,                            /*tp_repr*/
  0,                            /*tp_as_number*/
  0,                            /*tp_as_sequence*/
  0,                            /*tp_as_mapping*/
  0,                            /*tp_hash*/
  0,                       /*tp_call*/
  0,                            /*tp_str*/
  0,                            /*tp_getattro */
  0,                            /*tp_setattro */
  0,                            /*tp_as_buffer */
  Py_TPFLAGS_DEFAULT,           /*tp_flags*/
  ETContext__doc__,                 /*tp_doc */
  0,                            /*tp_traverse */
  0,                            /*tp_clear */
  0,                            /*tp_richcompare */
  0,                            /*tp_weaklistoffset */
  PyObject_SelfIter,            /*tp_iter */
  0,    /*tp_iternext */
  ETContext_methods,                    /*tp_methods */
  ETContext_members,                 /*tp_members */
  //ETContext_getsetlist,              /*tp_getset */
};

#define PyETContext_CheckExact(op) (Py_TYPE(op) == &ETContextType)

static void
ETContext_dealloc(ETContextObject* self)
{
    self->ob_type->tp_free((PyObject*)self);
}

PyDoc_STRVAR(py_as_ETContext__doc__,"Convert an object to ETContext object.");
static PyObject * 
as_ETContext(ETContextObject *self, PyObject *args)
{
  ETContextObject *obj=NULL;
  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }
  if(NULL == obj || !PyETContext_CheckExact(obj)){
    INVALID_PARAMS("Invalid param!",NULL);
  }
  return (PyObject*)obj;
}

static PyObject *
ETContext_new()
{
    ETContextObject *self;
    self = PyObject_New(ETContextObject, &ETContextType);
    if (self == NULL)
        return NULL;
    memset(&self->context,0,sizeof(ET_CONTEXT));
    return (PyObject*)self;
}

static int
ETContext_init(ETContextObject *self, PyObject *args, PyObject *kwds)
{
    char *pzId=NULL,*pzAtr=NULL;
    int lId=0,lAtr=0;
    static char *kwlist[] = {"dwIndex","dwVersion","hLock","dwCustomer","bAtr","bID",NULL};
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iiiis#s#", kwlist, 
                                      &self->context.dwIndex,
                                      &self->context.dwVersion,
                                      &self->context.hLock,
                                      &self->context.dwCustomer,
                                      &pzAtr,&lAtr,
                                      &pzId,&lId))
        return -1; 
    if(pzId!=NULL && lId!=MAX_ID_LEN){
      INVALID_PARAMS("Length of ID must be 8!",-1);
    }
    memcpy(self->context.bID,pzId,MAX_ID_LEN);
    if(pzAtr!=NULL && lAtr>MAX_ATR_LEN){
      INVALID_PARAMS("Length of Atr must not be longer than 16!",-1);
    }
    memcpy(self->context.bAtr,pzAtr,lAtr);
    return 0;
}

static PyObject *
ETContext_open(ETContextObject* self, PyObject *args)
{
    DWORD dwRet;
    ET_OPENINFO OpenInfo;
    PyObject *pyOpenInfo=NULL;
    if (!PyArg_ParseTuple(args, "|O", &pyOpenInfo)) {
      return NULL;
    }
    if(NULL != pyOpenInfo){
      //printf("PyTuple_Check(pyOpenInfo)=%d\n",PyTuple_Check(pyOpenInfo));
      //printf("PyTuple_Size(pyOpenInfo)=%d\n",PyTuple_Size(pyOpenInfo));
      //printf("PyInt_Check(PyTuple_GetItem(pyOpenInfo,0)=%d\n",PyInt_Check(PyTuple_GetItem(pyOpenInfo,0)));
      //printf("PyInt_Check(PyTuple_GetItem(pyOpenInfo,1))=%d\n",PyInt_Check(PyTuple_GetItem(pyOpenInfo,1)));
      if(!PyTuple_Check(pyOpenInfo) 
        || 2!=PyTuple_Size(pyOpenInfo)
        || !PyInt_Check(PyTuple_GetItem(pyOpenInfo,0))
        || !PyInt_Check(PyTuple_GetItem(pyOpenInfo,1))){
        INVALID_PARAMS("Open Info must be a tuple with 2 integers.",NULL);
      }
      OpenInfo.dwOpenInfoSize=PyInt_AsUnsignedLongMask(PyTuple_GetItem(pyOpenInfo,0));
      OpenInfo.dwShareMode=PyInt_AsUnsignedLongMask(PyTuple_GetItem(pyOpenInfo,1));
      //printf("OpenInfo: %08x,%08x\n",OpenInfo.dwOpenInfoSize,OpenInfo.dwShareMode);
      dwRet= ETOpenEx(&self->context,&OpenInfo);
    }else{
      dwRet = ETOpen(&self->context);
    }
    DWRET_VALIDATE(dwRet,NULL);
    return self;
}

static PyObject *
ETContext_close(ETContextObject* self)
{
    DWORD dwRet;
    dwRet = ETClose(&self->context);
    DWRET_VALIDATE(dwRet,NULL);
    Py_RETURN_TRUE;
}

static PyObject *
ETContext_ctrl_led(ETContextObject* self, PyObject *args)
{
    DWORD onoff=0,wink=0,dwRet;
    if (!PyArg_ParseTuple(args, "i|i", &onoff,&wink)) {
      return NULL;
    }
    if(wink>0){
        dwRet = ETControl(&self->context,ET_LED_WINK,&wink,sizeof(wink),NULL,NULL,NULL);
        DWRET_VALIDATE(dwRet,NULL);
    }else{
        if(onoff){
            dwRet = ETControl(&self->context,ET_LED_UP,NULL,0,NULL,0,NULL);
            DWRET_VALIDATE(dwRet,NULL);
        }else{
            dwRet = ETControl(&self->context,ET_LED_DOWN,NULL,0,NULL,0,NULL);
            DWRET_VALIDATE(dwRet,NULL);
        }
    }
    
    Py_RETURN_TRUE;
}


static PyObject *
ETContext_ctrl_get(ETContextObject* self, PyObject *args)
{
    DWORD ctrlcode=0,dwRet;
    BYTE  bFileId[5]={0};
    WORD  wFileId=0;
    DWORD fileIdLen=0,i;
    BYTE  outBuffer[256]={0},*pTmp;
    DWORD bytesReturned=0;
    PET_MANUFACTURE_DATE pManuDate=NULL;
    PEFINFO pFileInfo=NULL;
    PyObject *pyRet=NULL;
    if (!PyArg_ParseTuple(args, "i|H", &ctrlcode,&wFileId)) {
      return NULL;
    }

    switch(ctrlcode){
        case ET_GET_DEVICE_TYPE:
            dwRet = ETControl(&self->context,ET_GET_DEVICE_TYPE,NULL,0,outBuffer,1,&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0] as ?
            return Py_BuildValue("i",outBuffer[0]);
            break;
        case ET_GET_SERIAL_NUMBER:
            dwRet = ETControl(&self->context,ET_GET_SERIAL_NUMBER,NULL,0,outBuffer,9,&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-7] as String
            return PyByteArray_FromStringAndSize(outBuffer,8);
            break;
        case ET_GET_DEVICE_USABLE_SPACE:
            dwRet = ETControl(&self->context,ET_GET_DEVICE_USABLE_SPACE,NULL,0,outBuffer,sizeof(DWORD),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-3] as DWORD
            return Py_BuildValue("i",*(DWORD*)outBuffer);
            break;
        case ET_GET_DEVICE_ATR:
            dwRet = ETControl(&self->context,ET_GET_DEVICE_ATR,NULL,0,outBuffer,17,&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-15] as String
            return PyByteArray_FromStringAndSize(outBuffer,16);
            break;
        case ET_GET_CUSTOMER_NAME:
            dwRet = ETControl(&self->context,ET_GET_CUSTOMER_NAME,NULL,0,outBuffer,sizeof(DWORD),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-3] as DWORD
            return Py_BuildValue("I",*(DWORD*)outBuffer);
            break;
        case ET_GET_MANUFACTURE_DATE:
            pManuDate=(PET_MANUFACTURE_DATE)outBuffer;
            dwRet = ETControl(&self->context,ET_GET_MANUFACTURE_DATE,NULL,0,outBuffer,sizeof(ET_MANUFACTURE_DATE),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return pManuDate as DateTime
            #if 1
            return Py_BuildValue("(iiiiii)",2000+pManuDate->byYear,
                                              pManuDate->byMonth,
                                              pManuDate->byDay,
                                              pManuDate->byHour,
                                              pManuDate->byMinute,
                                              pManuDate->bySecond);
            #else //PyDateTime_FromDateAndTime cause a segmentation fault... WHY??
            printf("1---GOGOGOGOGO...\n");
            pyRet = PyDateTime_FromDateAndTime(2000+pManuDate->byYear,
                                              pManuDate->byMonth,
                                              pManuDate->byDay,
                                              pManuDate->byHour,
                                              pManuDate->byMinute,
                                              pManuDate->bySecond,
                                              0);
            printf("2---GOGOGOGOGO...\n");
            Py_INCREF(pyRet);
            printf("3---GOGOGOGOGO...\n");
            return pyRet;
            #endif
            break;
        case ET_GET_DF_AVAILABLE_SPACE:
            dwRet = ETControl(&self->context,ET_GET_DF_AVAILABLE_SPACE,NULL,0,outBuffer,sizeof(DWORD),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-1] as WORD
            return Py_BuildValue("I",*(DWORD*)outBuffer);
            break;
        case ET_GET_EF_INFO:
            pFileInfo=(PEFINFO)outBuffer;
            if(bFileId==0){
                INVALID_PARAMS("An valid File Id should provide!",NULL);
            }
            sprintf(bFileId,"%04x",wFileId);
            dwRet = ETControl(&self->context,ET_GET_EF_INFO,bFileId,4,outBuffer,sizeof(EFINFO),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return pFileInfo as Dict
            return Py_BuildValue("{sHsbsI}",
                                    "wFileID",pFileInfo->wFileID,
                                    "bFileType",pFileInfo->bFileType,
                                    "wFileSize",pFileInfo->wFileSize);
            break;
        case ET_GET_COS_VERSION:
            dwRet = ETControl(&self->context,ET_GET_COS_VERSION,NULL,0,outBuffer,sizeof(WORD),&bytesReturned);
            DWRET_VALIDATE(dwRet,NULL);
            //Return outBuffer[0-1] as WORD
            return Py_BuildValue("H",*(WORD*)outBuffer);
            break;
        default:
            INVALID_PARAMS("param must between ET_GET_DEVICE_TYPE - ET_GET_COS_VERSION!",NULL);
            break;
    }
    
    Py_RETURN_NONE;
}


static PyObject *
ETContext_ctrl_set(ETContextObject* self, PyObject *args)
{
    DWORD ctrlcode=0,dwRet,dwOut,len;
    BYTE  buffer[24]={0};
    PyObject *pyParam=NULL;
    if (!PyArg_ParseTuple(args, "i|O", &ctrlcode,&pyParam)) {
      return NULL;
    }
    switch(ctrlcode){
      case ET_SET_DEVICE_ATR:
        if(NULL == pyParam || !PyByteArray_Check(pyParam)){
          INVALID_PARAMS("ATR should provide must be a bytearray!",NULL);
        }
        len = PyByteArray_Size(pyParam);
        if(len>16 || len<1){
          INVALID_PARAMS("ATR length must between 1~16!",NULL);
        }
        dwRet = ETControl(&self->context,ET_SET_DEVICE_ATR,PyByteArray_AsString(pyParam),len,NULL,0,&dwOut);
        DWRET_VALIDATE(dwRet,NULL);
        break;
      case ET_SET_DEVICE_TYPE:
        if(NULL == pyParam || !PyInt_Check(pyParam)){
          INVALID_PARAMS("Param should be an integer.",NULL);
        }
        dwRet = PyInt_AsUnsignedLongMask(pyParam);
        if(dwRet!=ET_DEVICE_TYPE_PKI && dwRet!=ET_DEVICE_TYPE_DONGLE && dwRet!=ET_DEVICE_TYPE_EMPTY){
          INVALID_PARAMS("Device type should be 1,2 or 4!",NULL);
        }
        buffer[0] = dwRet;
        dwRet = ETControl(&self->context,ET_SET_DEVICE_ATR,buffer,1,NULL,0,&dwOut);
        DWRET_VALIDATE(dwRet,NULL);
        break;
      case ET_SET_SHELL_KEY:
        if(NULL == pyParam || !PyByteArray_Check(pyParam)){
          INVALID_PARAMS("Shell key should provide must be a bytearray!",NULL);
        }
        len = PyByteArray_Size(pyParam);
        if(len>8 || len<1){
          INVALID_PARAMS("ATR length must between 1~8!",NULL);
        }
        dwRet = ETControl(&self->context,ET_SET_SHELL_KEY,PyByteArray_AsString(pyParam),len,NULL,0,&dwOut);
        DWRET_VALIDATE(dwRet,NULL);
        break;
      case ET_SET_CUSTOMER_NAME:
        if(NULL == pyParam || !PyByteArray_Check(pyParam)){
          INVALID_PARAMS("Customer name should provide must be a bytearray!",NULL);
        }
        len = PyByteArray_Size(pyParam);
        if(len>250 || len<1){
          INVALID_PARAMS("ATR length must between 1~250!",NULL);
        }
        dwRet = ETControl(&self->context,ET_SET_CUSTOMER_NAME,PyByteArray_AsString(pyParam),len,NULL,0,&dwOut);
        DWRET_VALIDATE(dwRet,NULL);
        break;
      default:
        INVALID_PARAMS("param must between ET_SET_DEVICE_ATR - ET_SET_CUSTOMER_NAME!",NULL);
        break;
    }
    Py_RETURN_TRUE;
}


static PyObject *
ETContext_ctrl_reset(ETContextObject* self)
{
  DWORD dwRet,dwOut;
  dwRet = ETControl(&self->context,ET_RESET_DEVICE,NULL,0,NULL,0,&dwOut);
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_create_dir(ETContextObject* self, PyObject *args)
{
  BYTE pszDirId[5]={0};
  WORD dirId=0;
  DWORD dwRet,dwDirSize,dwFlags=ET_CREATE_ROOT_DIR;
  ET_CREATEDIRINFO createInfo={0};
  PyObject *pyDirInfo=NULL,*pyAtr=NULL;
  if(!PyArg_ParseTuple(args, "HII|O", &dirId,&dwDirSize,&dwFlags,&pyDirInfo)) {
    return NULL;
  }
  if(0!=dirId){
    sprintf(pszDirId,"%04x",dirId);
  }else{
    dwDirSize=0;
    dwFlags=ET_CREATE_ROOT_DIR;
  }
  if(NULL != pyDirInfo){
    if(!PyTuple_Check(pyDirInfo) 
        || 2!=PyTuple_Size(pyDirInfo)
        || !PyInt_Check(PyTuple_GetItem(pyDirInfo,0))
        || !PyByteArray_Check(PyTuple_GetItem(pyDirInfo,1))){
      INVALID_PARAMS("DirInfo should be a tuple with one integer and one bytearray!",NULL);
    }
    pyAtr = PyTuple_GetItem(pyDirInfo,1);
    dwRet = PyByteArray_Size(pyAtr);
    if(dwRet>16 || dwRet<1){
      INVALID_PARAMS("ATR length must between 1~16!",NULL);
    }
    createInfo.dwCreateDirInfoSize = PyInt_AsUnsignedLongMask(PyTuple_GetItem(pyDirInfo,0));
    memcpy(createInfo.szAtr,PyByteArray_AsString(pyAtr),dwRet);
    dwRet = ETCreateDirEx(&self->context,pszDirId,dwDirSize,dwFlags,&createInfo);
    DWRET_VALIDATE(dwRet,NULL);
  }else{
    dwRet = ETCreateDir(&self->context,pszDirId,dwDirSize,dwFlags);
    DWRET_VALIDATE(dwRet,NULL);
  }
  
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_change_dir(ETContextObject* self, PyObject *args)
{
  BYTE  pszPath[5]={0};
  WORD  wPath;
  DWORD dwRet;
  if(!PyArg_ParseTuple(args, "H", &wPath)) {
    return NULL;
  }
  if(wPath==0){
    INVALID_PARAMS("Not a valid path!",NULL);
  }
  sprintf(pszPath,"%04x",wPath);
  dwRet = ETChangeDir(&self->context,pszPath);
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_erase_dir(ETContextObject* self, PyObject *args)
{
  BYTE  pszPath[32]={0};
  WORD  wPathLen;
  DWORD dwRet;
  if(!PyArg_ParseTuple(args, "s#", pszPath,&wPathLen)) {
    return NULL;
  }
  if(wPathLen!=0){
    dwRet = ETChangeDir(&self->context,pszPath);
    DWRET_VALIDATE(dwRet,NULL);
  }
  //sprintf(pszPath,"%04x",wPath);
  dwRet = ETEraseDir(&self->context,NULL);
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_change_pin(ETContextObject *self, PyObject *args)
{
  BYTE  *pbOldPin=NULL;
  BYTE  *pbNewPin=NULL;
  BYTE  byPinTryCount=0xFF;
  DWORD dwRet,dwOldPinLen,dwNewPinLen,dwPinType;
  if(!PyArg_ParseTuple(args, "s#s#I|B", &pbOldPin,&dwOldPinLen,
                                       &pbNewPin,&dwNewPinLen,
                                       &dwPinType,&dwRet)) {
    return NULL;
  }
  byPinTryCount = dwRet;
  dwRet = ETChangePin(&self->context,pbOldPin,dwOldPinLen,pbNewPin,dwNewPinLen,dwPinType,byPinTryCount);
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_verify_pin(ETContextObject* self, PyObject *args)
{
  BYTE *pbPin=NULL;
  DWORD dwPinLen,dwPinType,dwRet;
  if(!PyArg_ParseTuple(args, "s#I", &pbPin,&dwPinLen,&dwPinType)) {
    return NULL;
  }
  dwRet = ETVerifyPin(&self->context,pbPin,dwPinLen,dwPinType);
  /*
  if(ET_S_SUCCESS == dwRet){
    Py_RETURN_TRUE;
  }else if(dwRet&0xF0000C00){
    Py_RETURN_FALSE;
  }else{
    DWRET_VALIDATE(dwRet,NULL);
  }
  */
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_create_file(ETContextObject* self, PyObject *args)
{
  BYTE *lpszFileID[5]={0};
  WORD fileId;
  DWORD dwRet,dwFileSize;
  BYTE  bFileType;
  if(!PyArg_ParseTuple(args, "HII", &fileId,&dwFileSize,&dwRet)) {
    return NULL;
  }
  if(fileId==0){
    INVALID_PARAMS("Invalid File Id!",NULL);
  }
  sprintf(lpszFileID,"%04x",fileId);
  bFileType=dwRet;
  dwRet = ETCreateFile(&self->context,lpszFileID,dwFileSize,bFileType);
  DWRET_VALIDATE(dwRet,NULL);
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_write_file(ETContextObject* self, PyObject *args)
{
  BYTE lpszFileID[5]={0},*pBuffer=NULL,bFileType;
  WORD fileId;
  DWORD dwRet,dwOffset,dwBufferSize,dwFileSize=0,dwFlags,dwBytesWritten;
  if(!PyArg_ParseTuple(args, "HIs#|III", &fileId,&dwOffset,&pBuffer,&dwBufferSize,&dwFileSize,&dwFlags,&dwRet)) {
    return NULL;
  }
  if(fileId==0){
    INVALID_PARAMS("Invalid File Id!",NULL);
  }
  sprintf(lpszFileID,"%04x",fileId);
  if(dwFileSize>0){
    bFileType = dwRet;
    dwRet = ETWriteFileEx(&self->context,lpszFileID,dwOffset,pBuffer,dwBufferSize,dwFileSize,&dwBytesWritten,dwFlags,bFileType);
    DWRET_VALIDATE(dwRet,NULL);
    return Py_BuildValue("i",dwBytesWritten);
  }else{
    dwRet = ETWriteFile(&self->context,lpszFileID,dwOffset,pBuffer,dwBufferSize);
    DWRET_VALIDATE(dwRet,NULL);
  }
  Py_RETURN_TRUE;
}

static PyObject *
ETContext_execute(ETContextObject* self, PyObject *args)
{
  BYTE lpszFileID[5]={0},*pInBuffer=NULL,bFileType,OutBuffer[256];
  DWORD dwRet,dwInbufferSize,dwBytesReturned;
  WORD fileId;
  if(!PyArg_ParseTuple(args, "Hz#", &fileId,&pInBuffer,&dwInbufferSize)) {
    return NULL;
  }
  if(fileId==0){
    INVALID_PARAMS("Invalid File Id!",NULL);
  }
  sprintf(lpszFileID,"%04x",fileId);
  dwRet = ETExecute(&self->context,lpszFileID,pInBuffer,dwInbufferSize,OutBuffer,256,&dwBytesReturned);
  DWRET_VALIDATE(dwRet,NULL);
  return PyByteArray_FromStringAndSize(OutBuffer,dwBytesReturned);
}

static PyObject *
ETContext_gen_rsa_key(ETContextObject* self, PyObject *args)
{
  PyObject *pyRet=NULL;
  WORD wKeySize;
  DWORD dwRet,dwE,dwPubKeyDataSize=2120,dwPriKeyDataSize=2120;
  BYTE lpszPubFileID[5]={0},lpszPriFileID[5]={0},pbPubKeyData[2120],pbPriKeyData[2120];
  WORD pubFileId,priFileId;
  if(!PyArg_ParseTuple(args,"HIHH", 
                            &wKeySize,
                            &dwE,
                            &pubFileId,
                            &priFileId)) {
    return NULL;
  }
  if(0==pubFileId || 0==priFileId){
    INVALID_PARAMS("Invalid File Id!",NULL);
  }
  sprintf(lpszPubFileID,"%04x",pubFileId);
  sprintf(lpszPriFileID,"%04x",priFileId);
  dwRet = ETGenRsaKey(&self->context,
                      wKeySize,
                      dwE,
                      lpszPubFileID,
                      lpszPriFileID,
                      pbPubKeyData,
                      &dwPubKeyDataSize,
                      pbPriKeyData,
                      &dwPriKeyDataSize);
  DWRET_VALIDATE(dwRet,NULL);
  #if 1
  return  Py_BuildValue("(s#s#)",pbPubKeyData,dwPubKeyDataSize,pbPriKeyData,dwPriKeyDataSize);
  #else
  pyRet = PyTuple_New(2);
  PyTuple_SetItem(pyRet,0,PyByteArray_FromStringAndSize(pbPubKeyData,dwPubKeyDataSize));
  PyTuple_SetItem(pyRet,1,PyByteArray_FromStringAndSize(pbPriKeyData,dwPriKeyDataSize));
  Py_INCREF(pyRet);
  return pyRet;
  #endif
}

/*********************************************************************************************************************/

PyDoc_STRVAR(pyETEnum__doc__,"Enumerate() -> Enumerate connected ET199 keys. Return a list of ETContext objects.");
/**
 * Enumerate connected ET199 devices.  -> @ref: ETEnum
 * Args: No.
 * Return: A list of ETContext objects. NONE if no ET199 was connected or failed.
 */
static PyObject *pyETEnum(PyObject *self){
  /** Get connected keys*/
  DWORD keyCount=0,dwRet=0;
  int i=0;
  ET_CONTEXT *pETContextList = NULL;
  PyObject *result = PyList_New(0);
  dwRet = ETEnum(NULL,&keyCount);
  if(ET_E_INSUFFICIENT_BUFFER!=dwRet && !dwRet){
    DWRET_VALIDATE(dwRet,NULL);
  }
  if(keyCount==0){
    return result;
  }
  pETContextList = malloc(sizeof(ET_CONTEXT)*keyCount);
  memset(pETContextList,0,sizeof(ET_CONTEXT)*keyCount);
  dwRet=ETEnum(pETContextList,&keyCount);
  if(0!=dwRet){
    free(pETContextList);
    DWRET_VALIDATE(dwRet,NULL);
  }
  for(i=0;i<keyCount;i++){
    ETContextObject *etcontext = (ETContextObject*)ETContext_new();
    ET_CONTEXT *pETC = &pETContextList[i];
    /*
    DWORD dwOut=0;
    
    dwRet = ETOpen(pETC);
    dwRet = ETControl(pETC,ET_GET_SERIAL_NUMBER, NULL, 0, pETC->bID, 8, &dwOut);
    dwRet = ETControl(pETC,ET_GET_DEVICE_ATR,NULL,0,pETC->bAtr,16,&pETC->dwAtrLen);
    dwRet = ETControl(pETC,ET_GET_CUSTOMER_NAME,NULL,0,&pETC->dwCustomer,4,&dwOut);
    */
    memcpy(&etcontext->context,pETC,sizeof(ET_CONTEXT));
    PyList_Append(result, etcontext);
    /*
    dwRet = ETClose(pETC);
    */
  }
  //Py_XDECREF(pETContextList);
  free(pETContextList);
  return result;
}

/***************************************************************************************************************************/
static PyMethodDef methods[] = {
  { "Enumerate", (PyCFunction)pyETEnum, METH_NOARGS,pyETEnum__doc__},
  { "as_ETContext",as_ETContext,METH_VARARGS,py_as_ETContext__doc__},
  { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
initpyet199(void)
{
  PyObject *m,*d;
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

  /* Add some symbolic constants to the module */
   /* Export C API */
  Py_TYPE(&ETContextType)=&PyType_Type;
  if (PyType_Ready(&ETContextType) < 0) return;
  d = PyModule_GetDict(m);
  PyDict_SetItemString(d,"ETContext",  (PyObject*)&ETContextType);

}
