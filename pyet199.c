#include <Python.h>
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
    DWORD dwIndex;
    DWORD dwVersion;
    DWORD dwCustomer;
    DWORD hLock;
    BYTE  bID[MAX_ID_LEN];
    BYTE  bAtr[MAX_ATR_LEN];
    DWORD dwAtrLen;
    //ET_CONTEXT etContext;
}ETContextObject;

static void
ETContext_dealloc(ETContextObject* self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
ETContext_new(PyTypeObject *type)
{
    ETContextObject *self;
    self = (ETContextObject *)type->tp_alloc(type, 0);
    //memset(&self->etContext,0,sizeof(ET_CONTEXT));
    return (PyObject *)self;
}

static int
ETContext_init(ETContextObject *self, PyObject *args, PyObject *kwds)
{
    char *pzId=NULL,*pzAtr=NULL;
    int lId=0,lAtr=0;
    static char *kwlist[] = {"dwIndex","dwVersion","hLock","dwCustomer","bAtr","bID",NULL};
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|iiiis#s#", kwlist, 
                                      &self->dwIndex,
                                      &self->dwVersion,
                                      &self->hLock,
                                      &self->dwCustomer,
                                      &pzAtr,&lAtr,
                                      &pzId,&lId))
        return -1; 
    if(pzId!=NULL && lId!=MAX_ID_LEN){
      INVALID_PARAMS("Length of ID must be 8!",-1);
    }
    memcpy(self->bID,pzId,MAX_ID_LEN);
    if(pzAtr!=NULL && lAtr>MAX_ATR_LEN){
      INVALID_PARAMS("Length of Atr must not be longer than 16!",-1);
    }
    memcpy(self->bAtr,pzAtr,lAtr);
    return 0;
}


static PyObject *
ETContext_get(ETContextObject* self, PyObject *args)
{
    static PyObject *format = NULL;
    PyObject *result;
/*
    if (format == NULL) {
        format = PyString_FromString("%s %s");
        if (format == NULL)
            return NULL;
    }

    if (self->first == NULL) {
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }

    if (self->last == NULL) {
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }

    args = Py_BuildValue("OO", self->first, self->last);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
*/    
    return result;
}

//static PyMemberDef ETContext_members[] = 
static PyMemberDef ETContext_members[] = {
    {"Index", T_INT, offsetof(ET_CONTEXT,dwIndex), READONLY,
        "dwIndex"},
    {"Version", T_INT, offsetof(ET_CONTEXT,dwVersion), READONLY,
        "dwVersion"},
    {"Lock", T_INT, offsetof(ET_CONTEXT,hLock), READONLY,
        "hLock"},
    {"Customer", T_INT, offsetof(ET_CONTEXT,dwCustomer), READONLY,
        "dwCustomer"},
    {"Id", T_STRING, offsetof(ET_CONTEXT,bID), READONLY,
        "bId"},
    {"ATR", T_STRING, offsetof(ET_CONTEXT,bAtr), READONLY,
        "bAtr"},
    {NULL}  //Sentinel
};

static PyMethodDef ETContext_methods[] = {
    {"get", (PyCFunction)ETContext_get, METH_VARARGS,
     "get variable from instance."
    },
    {NULL}  /* Sentinel */
};


static PyTypeObject ETContextType = {
#if 0
    PyObject_HEAD_INIT(NULL)
    "pyet199.ETContext",             /*tp_name*/
    sizeof(ETContextObject),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)ETContext_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "ETContext objects",           /* tp_doc */
    0,                   /* tp_traverse */
    0,                   /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    ETContext_methods,             /* tp_methods */
    ETContext_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)ETContext_init,      /* tp_init */
    0,                         /* tp_alloc */
    ETContext_new,                 /* tp_new */
#else
    PyObject_HEAD_INIT(NULL)
    "pyet199.ETContext",//const char *tp_name; /* For printing, in format "<module>.<name>" */
    sizeof(ETContextObject),0,//Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */
    /* Methods to implement standard operations */
    (destructor)ETContext_dealloc,//destructor tp_dealloc;
    0,//printfunc tp_print;
    0,//getattrfunc tp_getattr;
    0,//setattrfunc tp_setattr;
    0,//cmpfunc tp_compare;
    0,//reprfunc tp_repr;

    /* Method suites for standard classes */

    0,//PyNumberMethods *tp_as_number;
    0,//PySequenceMethods *tp_as_sequence;
    0,//PyMappingMethods *tp_as_mapping;

    /* More standard operations (here for binary compatibility) */

    0,//hashfunc tp_hash;
    0,//ternaryfunc tp_call;
    0,//reprfunc tp_str;
    0,//getattrofunc tp_getattro;
    0,//setattrofunc tp_setattro;

    /* Functions to access object as input/output buffer */
    0,//PyBufferProcs *tp_as_buffer;

    /* Flags to define presence of optional/expanded features */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,//long tp_flags;

    "ETContext objects",//const char *tp_doc; /* Documentation string */

    /* Assigned meaning in release 2.0 */
    /* call function for all accessible objects */
    0,//traverseproc tp_traverse;

    /* delete references to contained objects */
    0,//inquiry tp_clear;

    /* Assigned meaning in release 2.1 */
    /* rich comparisons */
    0,//richcmpfunc tp_richcompare;

    /* weak reference enabler */
    0,//Py_ssize_t tp_weaklistoffset;

    /* Added in release 2.2 */
    /* Iterators */
    0,//getiterfunc tp_iter;
    0,//iternextfunc tp_iternext;

    /* Attribute descriptor and subclassing stuff */
    ETContext_methods,//struct PyMethodDef *tp_methods;
    ETContext_members,//struct PyMemberDef *tp_members;
    NULL,//struct PyGetSetDef *tp_getset;
    NULL,//struct _typeobject *tp_base;
    NULL,//PyObject *tp_dict;
    NULL,//descrgetfunc tp_descr_get;
    NULL,//descrsetfunc tp_descr_set;
    NULL,//Py_ssize_t tp_dictoffset;
    (initproc)ETContext_init,//initproc tp_init;
    NULL,//allocfunc tp_alloc;
    ETContext_new,//newfunc tp_new;
    NULL,//freefunc tp_free; /* Low-level free-memory routine */
    NULL,//inquiry tp_is_gc; /* For PyObject_IS_GC */
    NULL,//PyObject *tp_bases;
    NULL,//PyObject *tp_mro; /* method resolution order */
    NULL,//PyObject *tp_cache;
    NULL,//PyObject *tp_subclasses;
    NULL,//PyObject *tp_weaklist;
    NULL,//destructor tp_del;

    /* Type attribute cache version tag. Added in version 2.6 */
    NULL,//unsigned int tp_version_tag;

#ifdef COUNT_ALLOCS
    /* these must be last and never explicitly initialized */
    NULL,//Py_ssize_t tp_allocs;
    NULL,//Py_ssize_t tp_frees;
    NULL,//Py_ssize_t tp_maxalloc;
    NULL,//struct _typeobject *tp_prev;
    NULL,//struct _typeobject *tp_next;
#endif
#endif //
};
/*********************************************************************************************************************/
#ifdef NODDY_TEST
typedef struct {
    PyObject_HEAD
    PyObject *first; /* first name */
    PyObject *last;  /* last name */
    int number;
} Noddy;

static void
Noddy_dealloc(Noddy* self)
{
    Py_XDECREF(self->first);
    Py_XDECREF(self->last);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Noddy *self;

    self = (Noddy *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->first = PyString_FromString("");
        if (self->first == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }
        
        self->last = PyString_FromString("");
        if (self->last == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }

        self->number = 0;
    }

    return (PyObject *)self;
}

static int
Noddy_init(Noddy *self, PyObject *args, PyObject *kwds)
{
    PyObject *first=NULL, *last=NULL, *tmp;

    static char *kwlist[] = {"first", "last", "number", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist, 
                                      &first, &last, 
                                      &self->number))
        return -1; 

    if (first) {
        tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        Py_XDECREF(tmp);
    }

    if (last) {
        tmp = self->last;
        Py_INCREF(last);
        self->last = last;
        Py_XDECREF(tmp);
    }

    return 0;
}


static PyMemberDef Noddy_members[] = {
    {"first", T_OBJECT_EX, offsetof(Noddy, first), 0,
     "first name"},
    {"last", T_OBJECT_EX, offsetof(Noddy, last), 0,
     "last name"},
    {"number", T_INT, offsetof(Noddy, number), 0,
     "noddy number"},
    {NULL}  /* Sentinel */
};

static PyObject *
Noddy_name(Noddy* self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL) {
        format = PyString_FromString("%s %s");
        if (format == NULL)
            return NULL;
    }

    if (self->first == NULL) {
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }

    if (self->last == NULL) {
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }

    args = Py_BuildValue("OO", self->first, self->last);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
    
    return result;
}

static PyMethodDef Noddy_methods[] = {
    {"name", (PyCFunction)Noddy_name, METH_NOARGS,
     "Return the name, combining the first and last name"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject NoddyType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "noddy.Noddy",             /*tp_name*/
    sizeof(Noddy),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Noddy_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Noddy objects",           /* tp_doc */
    0,                   /* tp_traverse */
    0,                   /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
};
#endif /*NODDY_TEST*/
/*********************************************************************************************************************/

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
    NORMAL_ERROR(dwRet,NULL);
  }
  if(keyCount==0){
    return result;
  }
  pETContextList = malloc(sizeof(ET_CONTEXT)*keyCount);
  memset(pETContextList,0,sizeof(ET_CONTEXT)*keyCount);
  dwRet=ETEnum(pETContextList,&keyCount);
  if(0!=dwRet){
    free(pETContextList);
    NORMAL_ERROR(dwRet,NULL);
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
static PyObject *pETOpen(PyObject *self, PyObject *args){
  PyObject *pETCtxObj=NULL;
  ET_OPENINFO pOpenInfo={0,0};
  if (!PyArg_ParseTuple(args, "O|ii", &pETCtxObj,&pOpenInfo.dwOpenInfoSize,&pOpenInfo.dwShareMode)) {
    return NULL;
  }
  
  //Py_RETURN_NONE;
  return pETCtxObj;
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
  { "ETOpen", (PyCFunction)pETOpen, METH_VARARGS,
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
  { "ETVerifyPin", (PyCFunction)pETVerifyPin, METH_VARARGS,
      "ETVerifyPin: " },
  { "ETChangePin", (PyCFunction)pETChangePin, METH_VARARGS,
      "ETChangePin: " },
  { "ETWriteFile", (PyCFunction)pETWriteFile, METH_VARARGS,
      "ETWriteFile: " },
  { "ETExecute", (PyCFunction)pETExecute, METH_VARARGS,
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

  //ETContextType
  Py_INCREF(&ETContextType);
  PyModule_AddObject(m, "ETContext", (PyObject *)&ETContextType);

  #ifdef NODDY_TEST
  Py_INCREF(&NoddyType);
  PyModule_AddObject(m, "Noddy", (PyObject *)&NoddyType);
  #endif //NODDY_TEST

}
