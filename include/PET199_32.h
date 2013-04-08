#pragma once

#include "ET199_32.h"

#ifdef __cplusplus
extern"C"{
#endif

#define ET_E_LOAD_FILE_FAILED	0xF0001001

DWORD WINAPI PETWriteFile (
		IN			ET_CONTEXT	*pETCtx,
		IN			LPCSTR		lpszFileID,
		IN			LPCSTR		lpszPCFilePath,
		IN	OUT		DWORD		*pdwFileSize,
		IN			DWORD		dwFlags,
		IN			BYTE		bFileType,
		IN	OUT		DWORD		*pdwBytesWritten
		);

#ifdef __cplusplus
	}
#endif