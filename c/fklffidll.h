#ifndef FKL_FFI_DLL_H
#define FKL_FFI_DLL_H
#include"fklffitype.h"

#ifdef __cplusplus
extern "C" {
#endif

#include<ffi.h>
typedef struct FklFfiProc
{
	FklTypeId_t type;
	FklSid_t sid;
	FklVMvalue* pd;
	void* func;
	ffi_cif cif;
	ffi_type** atypes;
}FklFfiProc;

void fklFfiInitProc(FklFfiProc*,FklTypeId_t,void*,FklSid_t,FklVMvalue*);
void fklFfiInitSharedObj(FklFfiPublicData*);
int fklFfiIsProc(FklVMvalue*);
void fklFfiDestroyAllSharedObj(FklFfiPublicData* pd);
void fklFfiAddSharedObj(FklFfidllHandle handle,FklFfiPublicData* pd);
FklVMudata* fklFfiCreateProcUd(FklTypeId_t id
		,const char*
		,FklVMvalue*
		,FklVMvalue* pd
		,FklSymbolTable* table);
int fklFfiIsValidFunctionType(FklDefTypeUnion type,FklFfiPublicData* pd);

#ifdef __cplusplus
}
#endif
#endif
