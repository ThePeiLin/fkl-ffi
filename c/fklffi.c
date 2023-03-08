#include<fakeLisp/utils.h>
#include<fakeLisp/fklni.h>
#include<string.h>
#include"fklffitype.h"
#include"fklffimem.h"
#include"fklffidll.h"

#define PREDICATE(condtion,err_infor) {\
	FKL_NI_BEGIN(exe);\
	FklVMvalue* val=fklNiGetArg(&ap,stack);\
	if(fklNiResBp(&ap,stack))\
		FKL_RAISE_BUILTIN_ERROR_CSTR(err_infor,FKL_ERR_TOOFEWARG,exe);\
	if(!val)\
		FKL_RAISE_BUILTIN_ERROR_CSTR(err_infor,FKL_ERR_TOOFEWARG,exe);\
	if(condtion)\
		fklNiReturn(FKL_VM_TRUE,&ap,stack);\
	else\
		fklNiReturn(FKL_VM_NIL,&ap,stack);\
	fklNiEnd(&ap,stack);\
}

void fkl_ffi_mem_p(FKL_DL_PROC_ARGL) PREDICATE(fklFfiIsMem(val),"ffi.mem?")

#undef PREDICATE

void fkl_ffi_null_p(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* val=fklNiGetArg(&ap,stack);
	if(!val)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.null?",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.null?",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(val))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.null?",FKL_ERR_TOOFEWARG,exe);
	if(fklFfiIsNull(FKL_GET_UD_DATA(FklFfiMem,val->u.ud)))
		fklNiReturn(FKL_VM_TRUE,&ap,stack);
	else
		fklNiReturn(FKL_VM_NIL,&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_new(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* typedeclare=fklNiGetArg(&ap,stack);
	FklVMvalue* atomic=fklNiGetArg(&ap,stack);
	if(!typedeclare)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.create",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.create",FKL_ERR_TOOMANYARG,exe);
	if((!FKL_IS_SYM(typedeclare)
				&&!FKL_IS_PAIR(typedeclare))
			||(atomic
				&&!FKL_IS_SYM(atomic)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.create",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklTypeId_t id=fklFfiGenTypeId(typedeclare,publicData);
	if(!id)
		FKL_FFI_RAISE_ERROR("ffi.create",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
	FklVMudata* mem=NULL;
	if(id==FKL_FFI_TYPE_FILE_P||id==FKL_FFI_TYPE_STRING)
		mem=fklFfiCreateMemRefUd(id,NULL,rel,pd);
	else
	{
		size_t size=fklFfiGetTypeSize(fklFfiLockAndGetTypeUnion(id,publicData));
		mem=fklFfiCreateMemUd(id,size,atomic,rel,pd);
	}
	if(!mem)
		FKL_FFI_RAISE_ERROR("ffi.create",FKL_FFI_ERR_INVALID_MEM_MODE,exe,publicData);
	fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA,mem,exe),&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_delete(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	if(!mem)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.delete",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.delete",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(mem))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.delete",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FKL_DECL_UD_DATA(m,FklFfiMem,mem->u.ud);
	free(m->mem);
	m->mem=NULL;
	fklNiReturn(FKL_VM_NIL,&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_sizeof(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* typedeclare=fklNiGetArg(&ap,stack);
	if(!typedeclare)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.sizeof",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.sizeof",FKL_ERR_TOOMANYARG,exe);
	if(!FKL_IS_SYM(typedeclare)&&!FKL_IS_PAIR(typedeclare)&&!fklFfiIsMem(typedeclare))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.sizeof",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(fklFfiIsMem(typedeclare))
	{
		FKL_DECL_UD_DATA(m,FklFfiMem,typedeclare->u.ud);
		fklNiReturn(fklMakeVMint(fklFfiGetTypeSize(fklFfiLockAndGetTypeUnion(m->type,publicData)),exe),&ap,stack);
	}
	else
	{
		FklTypeId_t id=fklFfiGenTypeId(typedeclare,publicData);
		if(!id)
			FKL_FFI_RAISE_ERROR("ffi.sizeof",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
		fklNiReturn(fklMakeVMint(fklFfiGetTypeSize(fklFfiLockAndGetTypeUnion(id,publicData)),exe),&ap,stack);
	}
	fklNiEnd(&ap,stack);
}

void fkl_ffi_alignof(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* typedeclare=fklNiGetArg(&ap,stack);
	if(!typedeclare)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.alignof",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.alignof",FKL_ERR_TOOMANYARG,exe);
	if(!FKL_IS_SYM(typedeclare)&&!FKL_IS_PAIR(typedeclare)&&!fklFfiIsMem(typedeclare))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.alignof",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(fklFfiIsMem(typedeclare))
	{
		FKL_DECL_UD_DATA(m,FklFfiMem,typedeclare->u.ud);
		fklNiReturn(fklMakeVMint(fklFfiGetTypeSize(fklFfiLockAndGetTypeUnion(m->type,publicData)),exe),&ap,stack);
	}
	else
	{
		FklTypeId_t id=fklFfiGenTypeId(typedeclare,publicData);
		if(!id)
			FKL_FFI_RAISE_ERROR("ffi.alignof",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
		fklNiReturn(fklMakeVMint(fklFfiGetTypeAlign(fklFfiLockAndGetTypeUnion(id,publicData)),exe),&ap,stack);
	}
	fklNiEnd(&ap,stack);
}

void fkl_ffi_typedef(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* typedeclare=fklNiGetArg(&ap,stack);
	FklVMvalue* typename=fklNiGetArg(&ap,stack);
	if(!typedeclare||!typename)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.typedef",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.typedef",FKL_ERR_TOOMANYARG,exe);
	if(!FKL_IS_SYM(typename)||(!FKL_IS_PAIR(typedeclare)&&!FKL_IS_SYM(typedeclare)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.typedef",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FklSid_t typenameId=FKL_GET_SYM(typename);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(fklFfiIsNativeTypeName(typenameId,publicData))
		FKL_FFI_RAISE_ERROR("ffi.typedef",FKL_FFI_ERR_INVALID_TYPENAME,exe,publicData);
	if(!fklFfiTypedef(typedeclare,typenameId,publicData))
		FKL_FFI_RAISE_ERROR("ffi.typedef",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
	fklNiReturn(typename,&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_load(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* vpath=fklNiGetArg(&ap,stack);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.load",FKL_ERR_TOOMANYARG,exe);
	if(!vpath)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.load",FKL_ERR_TOOFEWARG,exe);
	if(!FKL_IS_STR(vpath))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.load",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	char* path=fklCharBufToCstr(vpath->u.str->str,vpath->u.str->size);
	FklDllHandle handle=fklLoadDll(path);
	if(!handle)
		FKL_RAISE_BUILTIN_INVALIDSYMBOL_ERROR_CSTR("ffi.load",path,1,FKL_ERR_LOADDLLFAILD,exe);
	free(path);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	fklFfiAddSharedObj(handle,publicData);
	fklNiReturn(vpath,&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_ref(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	FklVMvalue* selector=fklNiGetArg(&ap,stack);
	FklVMvalue* index=fklNiGetArg(&ap,stack);
	if(!mem)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.ref",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.ref",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(mem)||(selector&&!FKL_IS_SYM(selector)&&selector!=FKL_VM_NIL)||(index&&!fklIsInt(index)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.ref",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FklVMudata* ref=fklFfiCreateMemRefUdWithSI(FKL_GET_UD_DATA(FklFfiMem,mem->u.ud),selector,index,rel,pd);
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(!ref)
		FKL_FFI_RAISE_ERROR("ffi.ref",FKL_FFI_ERR_INVALID_SELECTOR,exe,publicData);
	fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA
				,ref
				,exe)
			,&ap
			,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_clear(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	if(!mem)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.clear!",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.clear!",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(mem))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.clear!",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	FKL_DECL_UD_DATA(ptr,FklFfiMem,mem->u.ud);
	if(ptr->type==FKL_FFI_TYPE_STRING)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.clear!",FKL_ERR_INCORRECT_TYPE_VALUE,exe);

	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);

	FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(ptr->type,publicData);
	memset(ptr->mem,0,fklFfiGetTypeSize(tu));
	fklNiReturn(mem
			,&ap
			,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_cast_ref(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* type=fklNiGetArg(&ap,stack);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	if(!mem||!type)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.cast-ref",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.cast-ref",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(mem)||(!FKL_IS_PAIR(type)&&!FKL_IS_SYM(type)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.cast-ref",FKL_ERR_INCORRECT_TYPE_VALUE,exe);

	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);

	FklTypeId_t id=fklFfiGenTypeId(type,publicData);
	if(!id)
		FKL_FFI_RAISE_ERROR("ffi.cast-ref",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
	FklVMudata* ref=fklFfiCreateMemRefUd(id,((FklFfiMem*)mem->u.ud->data)->mem,rel,pd);
	fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA
				,ref
				,exe)
			,&ap
			,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_set(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* ref=fklNiGetArg(&ap,stack);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	if(!mem||!ref)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.set",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.set",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(ref)
			||(!fklFfiIsMem(mem)&&!fklFfiIsCastableVMvalueType(mem)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.set",FKL_ERR_INCORRECT_TYPE_VALUE,exe);

	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);

	if(fklFfiSetMem(ref->u.ud,mem,exe->symbolTable))
		FKL_FFI_RAISE_ERROR("ffi.set",FKL_FFI_ERR_INVALID_ASSIGN,exe,publicData);
	fklNiReturn(mem,&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_mem(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* val=fklNiGetArg(&ap,stack);
	if(!val)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.mem",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.mem",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsCastableVMvalueType(val))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.mem",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA,fklFfiCastVMvalueIntoMem(val,rel,pd,exe->symbolTable),exe),&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_val(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* mem=fklNiGetArg(&ap,stack);
	if(!mem)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.val",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.val",FKL_ERR_TOOMANYARG,exe);
	if(!fklFfiIsMem(mem)||!fklFfiIsValuableMem(FKL_GET_UD_DATA(FklFfiMem,mem->u.ud)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.val",FKL_ERR_INCORRECT_TYPE_VALUE,exe);
	fklNiReturn(fklFfiCreateVMvalue(FKL_GET_UD_DATA(FklFfiMem,mem->u.ud),exe),&ap,stack);
	fklNiEnd(&ap,stack);
}

void fkl_ffi_proc(FKL_DL_PROC_ARGL)
{
	FKL_NI_BEGIN(exe);
	FklVMvalue* val=fklNiGetArg(&ap,stack);
	FklVMvalue* typedeclare=fklNiGetArg(&ap,stack);
	if(!val||!typedeclare)
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.proc",FKL_ERR_TOOFEWARG,exe);
	if(fklNiResBp(&ap,stack))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.proc",FKL_ERR_TOOMANYARG,exe);
	if((!FKL_IS_SYM(typedeclare)
				&&!FKL_IS_PAIR(typedeclare))
			||(val
				&&!FKL_IS_STR(val)))
		FKL_RAISE_BUILTIN_ERROR_CSTR("ffi.proc",FKL_ERR_INCORRECT_TYPE_VALUE,exe);

	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);

	FklTypeId_t id=fklFfiGenTypeId(typedeclare,publicData);
	if(!id)
		FKL_FFI_RAISE_ERROR("ffi.proc",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
	else
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(id,publicData);
		if(!fklFfiIsFunctionType(tu)||!fklFfiIsValidFunctionType(tu,publicData))
			FKL_FFI_RAISE_ERROR("ffi.proc",FKL_FFI_ERR_INVALID_TYPEDECLARE,exe,publicData);
	}
	char* cStr=fklCharBufToCstr(val->u.str->str,val->u.str->size);
	FklVMudata* func=fklFfiCreateProcUd(id,cStr,rel,pd,exe->symbolTable);
	if(!func)
		FKL_RAISE_BUILTIN_INVALIDSYMBOL_ERROR_CSTR("ffi.proc",cStr,1,FKL_ERR_INVALIDSYMBOL,exe);
	free(cStr);
	fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA,func,exe),&ap,stack);
	fklNiEnd(&ap,stack);
}

static void fkl_ffi_pd_finalizer(FklVMudata* ud)
{
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,ud);
	fklFfiDestroyGlobDefTypeTable(pd);
	fklFfiDestroyGlobTypeList(pd);
	fklFfiDestroyAllSharedObj(pd);
}

static FklVMudMethodTable pdtable=
{
	.__append=NULL,
	.__atomic=NULL,
	.__cmp=NULL,
	.__copy=NULL,
	.__equal=NULL,
	.__prin1=NULL,
	.__princ=NULL,
	.__finalizer=fkl_ffi_pd_finalizer,
};

void _fklInit(FklVMdll* rel,FklVM* exe)
{
	FklVMgc* gc=exe->gc;
	FklVMudata* ud=fklCreateVMudata(0,&pdtable,FKL_VM_NIL,sizeof(FklFfiPublicData));
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,ud);
	fklFfiMemInit(pd,exe->symbolTable);
	fklFfiInitGlobNativeTypes(pd,exe->symbolTable);
	fklFfiInitTypedefSymbol(pd,exe->symbolTable);
	fklInitErrorTypes(pd,exe->symbolTable);
	fklFfiInitSharedObj(pd);
	FklVMvalue* udv=fklCreateVMvalueToStack(FKL_TYPE_USERDATA,ud,exe);
	fklSetRef(&rel->pd,udv,gc);
}

struct SymFunc
{
	const char* sym;
	FklVMdllFunc f;
};

static const struct SymFunc
exports[]=
{
	{"ffi-mem?",     fkl_ffi_mem_p,    },
	{"ffi-null?",    fkl_ffi_null_p,   },
	{"ffi-new",      fkl_ffi_new,      },
	{"ffi-delete",   fkl_ffi_delete,   },
	{"ffi-sizeof",   fkl_ffi_sizeof,   },
	{"ffi-alignof",  fkl_ffi_alignof,  },
	{"ffi-typedef",  fkl_ffi_typedef,  },
	{"ffi-load",     fkl_ffi_load,     },
	{"ffi-ref",      fkl_ffi_ref,      },
	{"ffi-set!",     fkl_ffi_set,      },
	{"ffi-cast-ref", fkl_ffi_cast_ref, },
	{"ffi-mem",      fkl_ffi_mem,      },
	{"ffi-val",      fkl_ffi_val,      },
	{"ffi-proc",     fkl_ffi_proc,     },
	{"ffi-clear!",   fkl_ffi_clear,    },
};

static const size_t EXPORT_NUM=sizeof(exports)/sizeof(struct SymFunc);

void _fklExportSymbolInit(size_t* pnum,FklSid_t** psyms,FklSymbolTable* table)
{
	*pnum=EXPORT_NUM;
	FklSid_t* symbols=(FklSid_t*)malloc(sizeof(FklSid_t)*EXPORT_NUM);
	FKL_ASSERT(symbols);
	for(size_t i=0;i<EXPORT_NUM;i++)
		symbols[i]=fklAddSymbolCstr(exports[i].sym,table)->id;
	*psyms=symbols;
}

FklVMvalue** _fklImportInit(FklVM* exe,FklVMvalue* dll,uint32_t* pcount)
{
	FklSymbolTable* table=exe->symbolTable;
	*pcount=EXPORT_NUM;
	FklVMvalue** loc=(FklVMvalue**)malloc(sizeof(FklVMvalue*)*EXPORT_NUM);
	FKL_ASSERT(loc||!EXPORT_NUM);
	for(size_t i=0;i<EXPORT_NUM;i++)
	{
		FklSid_t id=fklAddSymbolCstr(exports[i].sym,table)->id;
		FklVMdllFunc func=exports[i].f;
		FklVMdlproc* proc=fklCreateVMdlproc(func,dll,dll->u.dll->pd);
		proc->sid=id;
		FklVMvalue* dlproc=fklCreateVMvalueToStack(FKL_TYPE_DLPROC,proc,exe);
		loc[i]=dlproc;
	}
	return loc;
}
#undef EXPORT_NUM
