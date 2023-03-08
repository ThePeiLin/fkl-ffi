#include"fklffidll.h"
#include"fklffimem.h"
#include<fakeLisp/utils.h>
#include<fakeLisp/fklni.h>

void fklFfiAddSharedObj(FklFfidllHandle handle,FklFfiPublicData* pd)
{
	FklFfiSharedObjNode* node=(FklFfiSharedObjNode*)malloc(sizeof(FklFfiSharedObjNode));
	FKL_ASSERT(node);
	node->dll=handle;
	node->next=pd->sharedObjs;
	pd->sharedObjs=node;
}

void fklFfiInitSharedObj(FklFfiPublicData* pd)
{
	pd->sharedObjs=NULL;
}

void fklFfiDestroyAllSharedObj(FklFfiPublicData* pd)
{
	FklFfiSharedObjNode* head=pd->sharedObjs;
	pd->sharedObjs=NULL;
	while(head)
	{
		FklFfiSharedObjNode* prev=head;
		head=head->next;
#ifdef _WIN32
		DestroyLibrary(prev->dll);
#else
		dlclose(prev->dll);
#endif
		free(prev);
	}
}

static void _ffi_proc_atomic_finalizer(FklVMudata* p)
{
	FKL_DECL_UD_DATA(t,FklFfiProc,p);
	free(t->atypes);
}

static void _ffi_proc_print(const FklVMudata* ud,FILE* fp,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(f,FklFfiProc,ud);
	fprintf(fp,"#<ffi-proc ");
	fklPrintString(fklGetSymbolWithId(f->sid,table)->symbol,fp);
	fputc('>',fp);
}

static ffi_type* NativeFFITypeList[]=
{
	&ffi_type_void,
	&ffi_type_sshort,
	&ffi_type_sint,
	&ffi_type_ushort,
	&ffi_type_uint,
	&ffi_type_slong,
	&ffi_type_ulong,
	&ffi_type_slong,//long long
	&ffi_type_ulong,//unsigned long long
	&ffi_type_slong,//ptrdiff_t
	&ffi_type_ulong,//size_t
	&ffi_type_slong,//ssize_t
	&ffi_type_schar,//char
	&ffi_type_sint32,//wchar_t
	&ffi_type_float,
	&ffi_type_double,
	&ffi_type_sint8,
	&ffi_type_uint8,
	&ffi_type_sint16,
	&ffi_type_uint16,
	&ffi_type_sint32,
	&ffi_type_uint32,
	&ffi_type_sint64,
	&ffi_type_uint64,
	&ffi_type_slong,//intptr_t
	&ffi_type_ulong,//uintptr_t
	&ffi_type_pointer,//void*
};

void fklFfiPrepFFIcif(ffi_cif* cif,int argc,ffi_type** atypes,ffi_type* rtype)
{
#ifdef NDEBUG
	ffi_prep_cif(cif,FFI_DEFAULT_ABI,argc,rtype,atypes);
#else
	ffi_status r=ffi_prep_cif(cif,FFI_DEFAULT_ABI,argc,rtype,atypes);
	FKL_ASSERT(r==FFI_OK);
#endif
}

ffi_type* fklFfiGetFfiType(FklTypeId_t type)
{
	if(type>FKL_FFI_TYPE_VPTR)
		type=FKL_FFI_TYPE_VPTR;
	return NativeFFITypeList[type];
}

typedef enum
{
	FFIPROC_READY=0,
	FFIPROC_DONE,
}FfiprocFrameState;

typedef struct
{
	FklVMvalue* proc;
	FfiprocFrameState state;
}FfiprocFrameContext;

static void ffiproc_frame_print_backtrace(void* data[6],FILE* fp,FklSymbolTable* table)
{
	FfiprocFrameContext* c=(FfiprocFrameContext*)data;
	FKL_DECL_UD_DATA(ffiproc,FklFfiProc,c->proc->u.ud);
	if(ffiproc->sid)
	{
		fprintf(fp,"at ffi-proc:");
		fklPrintString(fklGetSymbolWithId(ffiproc->sid,table)->symbol
				,stderr);
		fputc('\n',fp);
	}
	else
		fputs("at <ffi-proc>\n",fp);
}

static void ffiproc_frame_atomic(void* data[6],FklVMgc* gc)
{
	FfiprocFrameContext* c=(FfiprocFrameContext*)data;
	fklGC_toGrey(c->proc,gc);
}

static void ffiproc_frame_finalizer(void* data[6])
{
	return;
}

static void ffiproc_frame_copy(FklCallObjData d,const FklCallObjData s,FklVM* exe)
{
	FfiprocFrameContext* sc=(FfiprocFrameContext*)s;
	FfiprocFrameContext* dc=(FfiprocFrameContext*)d;
	FklVMgc* gc=exe->gc;
	dc->state=sc->state;
	fklSetRef(&dc->proc,sc->proc,gc);
}

static int ffiproc_frame_end(void* data[6])
{
	FfiprocFrameContext* c=(FfiprocFrameContext*)data;
	return c->state==FFIPROC_DONE;
}

static void _ffi_proc_invoke(FklFfiProc* proc
		,FklVM* exe
		,FklVMvalue* rel)
{
	FKL_NI_BEGIN(exe);
	FklTypeId_t type=proc->type;
	FklVMvalue* pdv=proc->pd;
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,pdv->u.ud);
	FklDefFuncType* ft=(FklDefFuncType*)FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(type,pd).all);
	uint32_t anum=ft->anum;
	uint32_t i=0;
	FklTypeId_t rtype=ft->rtype;
	FklTypeId_t* atypes=ft->atypes;
	FklVMvalue** args=(FklVMvalue**)malloc(sizeof(FklVMvalue*)*anum);
	FKL_ASSERT(args);
	for(i=0;i<anum;i++)
	{
		FklVMvalue* v=fklNiGetArg(&ap,stack);
		if(v==NULL)
		{
			free(args);
			FKL_RAISE_BUILTIN_ERROR(fklGetSymbolWithId(proc->sid,exe->symbolTable)->symbol,FKL_ERR_TOOFEWARG,exe);
		}
		args[i]=v;
	}
	if(fklNiResBp(&ap,stack))
	{
		free(args);
		FKL_RAISE_BUILTIN_ERROR(fklGetSymbolWithId(proc->sid,exe->symbolTable)->symbol,FKL_ERR_TOOMANYARG,exe);
	}
	void** pArgs=(void**)malloc(sizeof(void*)*anum);
	FklVMudata** udataList=(FklVMudata**)malloc(sizeof(FklVMudata*)*anum);
	FKL_ASSERT(udataList);
	FKL_ASSERT(pArgs);
	for(i=0;i<anum;i++)
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(atypes[i],pd);
		FklVMudata* ud=fklFfiCreateMemUd(atypes[i],fklFfiGetTypeSize(tu),NULL,rel,pdv);
		if(fklFfiSetMemForProc(ud,args[i],exe->symbolTable))
		{
			for(uint32_t j=0;j<i;j++)
			{
				FklVMudata* tud=udataList[i];
				fklDestroyVMudata(tud);
			}
			fklDestroyVMudata(ud);
			free(args);
			free(pArgs);
			free(udataList);
			FKL_RAISE_BUILTIN_ERROR(fklGetSymbolWithId(proc->sid,exe->symbolTable)->symbol,FKL_ERR_INCORRECT_TYPE_VALUE,exe);
		}
		FKL_DECL_UD_DATA(mem,FklFfiMem,ud);
		if(mem->type==FKL_FFI_TYPE_FILE_P||mem->type==FKL_FFI_TYPE_STRING||fklFfiIsArrayType(fklFfiLockAndGetTypeUnion(mem->type,pd)))
			pArgs[i]=&mem->mem;
		else
			pArgs[i]=mem->mem;
		udataList[i]=ud;
	}
	if(rtype==0)
	{
		void* retval=NULL;
		ffi_call(&proc->cif,proc->func,&retval,pArgs);
		fklNiReturn(FKL_VM_NIL,&ap,stack);
	}
	else
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(rtype,pd);
		FklVMudata* ud=fklFfiCreateMemUd(rtype,fklFfiGetTypeSize(tu),NULL,rel,pdv);
		void* retval=((FklFfiMem*)ud->data)->mem;
		FKL_ASSERT(retval);
		ffi_call(&proc->cif,proc->func,retval,pArgs);
		fklNiReturn(fklCreateVMvalueToStack(FKL_TYPE_USERDATA,ud,exe),&ap,stack);
	}
	for(i=0;i<anum;i++)
	{
		FklVMudata* ud=udataList[i];
		fklDestroyVMudata(ud);
	}
	free(udataList);
	free(pArgs);
	free(args);
	fklNiEnd(&ap,stack);
}

static void ffiproc_frame_step(void* data[6],FklVM* exe)
{
	FfiprocFrameContext* c=(FfiprocFrameContext*)data;
	FKL_DECL_UD_DATA(ffiproc,FklFfiProc,c->proc->u.ud);
	switch(c->state)
	{
		case FFIPROC_READY:
			c->state=FFIPROC_DONE;
			_ffi_proc_invoke(ffiproc,exe,c->proc->u.ud->rel);
			break;
		case FFIPROC_DONE:
			break;
	}
}

static const FklVMframeContextMethodTable FfiprocContextMethodTable=
{
	.atomic=ffiproc_frame_atomic,
	.finalizer=ffiproc_frame_finalizer,
	.copy=ffiproc_frame_copy,
	.print_backtrace=ffiproc_frame_print_backtrace,
	.end=ffiproc_frame_end,
	.step=ffiproc_frame_step,
};

inline static void initFfiprocFrameContext(FklCallObjData data,FklVMvalue* proc,FklVMgc* gc)
{
	FfiprocFrameContext* c=(FfiprocFrameContext*)data;
	fklSetRef(&c->proc,proc,gc);
	c->state=FFIPROC_READY;
}

static void _ffi_call_proc(FklVMvalue* ud,FklVM* exe)
{
	FklVMframe* prev=exe->frames;
	FklVMframe* f=fklCreateOtherObjVMframe(&FfiprocContextMethodTable,prev);
	initFfiprocFrameContext(f->u.o.data,ud,exe->gc);
	fklPushVMframe(f,exe);
}

int ffi_proc_equal(const FklVMudata* a,const FklVMudata* b)
{
	if(a->t->__call!=b->t->__call)
		return 0;
	if(a->t->__call)
	{
		FKL_DECL_UD_DATA(p0,FklFfiProc,a);
		FKL_DECL_UD_DATA(p1,FklFfiProc,b);
		if(p0->type==p1->type&&p0->func==p1->func)
			return 1;
	}
	return 0;
}

static FklVMudMethodTable FfiProcMethodTable=
{
	.__princ=_ffi_proc_print,
	.__prin1=_ffi_proc_print,
	.__finalizer=_ffi_proc_atomic_finalizer,
	.__equal=ffi_proc_equal,
	.__call=_ffi_call_proc,
	.__write=NULL,
	.__atomic=NULL,
	.__append=NULL,
	.__copy=NULL,
	.__hash=NULL,
	.__setq_hook=NULL,
};

int fklFfiIsProc(FklVMvalue* p)
{
	return FKL_IS_USERDATA(p)&&p->u.ud->t==&FfiProcMethodTable;
}

int fklFfiIsValidFunctionType(FklDefTypeUnion tu,FklFfiPublicData* pd)
{
	FklDefFuncType* ft=(FklDefFuncType*)FKL_GET_TYPES_PTR(tu.all);
	uint32_t anum=ft->anum;
	FklTypeId_t* atypes=ft->atypes;
	FklTypeId_t rtype=ft->rtype;
	uint32_t i=0;
	for(;i<anum;i++)
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(atypes[i],pd);
		if(!fklFfiIsArrayType(tu)&&fklFfiGetTypeSize(tu)>sizeof(void*))
			return 0;
	}
	if(rtype)
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(rtype,pd);
		if(fklFfiIsArrayType(tu)&&fklFfiGetTypeSize(tu)>sizeof(void*))
			return 0;
	}
	return 1;
}

FklVMudata* fklFfiCreateProcUd(FklTypeId_t type
		,const char* cStr
		,FklVMvalue* rel
		,FklVMvalue* pd
		,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	void* address=NULL;
	for(FklFfiSharedObjNode* head=publicData->sharedObjs;head;head=head->next)
	{
		address=fklGetAddress(cStr,head->dll);
		if(address)
			break;
	}
	if(!address)
		return NULL;
	FklVMudata* ud=fklCreateVMudata(publicData->memUdSid,&FfiProcMethodTable,rel,sizeof(FklFfiProc));
	FKL_DECL_UD_DATA(ffiproc,FklFfiProc,ud);
	fklFfiInitProc(ffiproc,type,address,fklAddSymbolCstr(cStr,table)->id,pd);
	return ud;
}

inline void fklFfiInitProc(FklFfiProc* tmp,FklTypeId_t type,void* func,FklSid_t sid,FklVMvalue* pd)
{
	tmp->type=type;
	tmp->func=func;
	tmp->sid=sid;
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklDefFuncType* ft=(FklDefFuncType*)FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(type,publicData).all);
	uint32_t anum=ft->anum;
	FklTypeId_t* atypes=ft->atypes;
	ffi_type** ffiAtypes=(ffi_type**)malloc(sizeof(ffi_type*)*anum);
	FKL_ASSERT(ffiAtypes);
	uint32_t i=0;
	for(;i<anum;i++)
		ffiAtypes[i]=fklFfiGetFfiType(atypes[i]);
	FklTypeId_t rtype=ft->rtype;
	ffi_type* ffiRtype=NULL;
	ffiRtype=fklFfiGetFfiType(rtype);
	fklFfiPrepFFIcif(&tmp->cif,anum,ffiAtypes,ffiRtype);
	tmp->atypes=ffiAtypes;
}
