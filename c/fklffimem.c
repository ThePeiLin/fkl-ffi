#include"fklffimem.h"
#include"fklffidll.h"
#include<fakeLisp/utils.h>
#include<string.h>
#include<fakeLisp/fklni.h>
#include<float.h>
#ifdef _WIN32
#include<BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

static void fkl_ffi_mem_atomic_finalizer(FklVMudata* p)
{
	FKL_DECL_UD_DATA(m,FklFfiMem,p);
	free(m->mem);
}

#define PRINT_MEM_REF(FMT,TYPE,MEM,FP) fprintf(FP,FMT,*(TYPE*)(MEM))
#define ARGL FILE* fp,const void* mem
static void printShortMem (ARGL){PRINT_MEM_REF("%hd",short,mem,fp);}
static void printIntMem   (ARGL){PRINT_MEM_REF("%d",int,mem,fp);}
static void printUShortMem(ARGL){PRINT_MEM_REF("%hu",unsigned short,mem,fp);}
static void printUIntMem  (ARGL){PRINT_MEM_REF("%u",unsigned int,mem,fp);}
static void printLongMem  (ARGL){PRINT_MEM_REF("%ld",long,mem,fp);}
static void printULongMem (ARGL){PRINT_MEM_REF("%lu",unsigned long,mem,fp);}
static void printLLongMem (ARGL){PRINT_MEM_REF("%lld",long long,mem,fp);}
static void printULLongMem(ARGL){PRINT_MEM_REF("%llu",unsigned long long,mem,fp);}
static void printPtrdiff_t(ARGL){PRINT_MEM_REF("%ld",ptrdiff_t,mem,fp);}
static void printSize_t   (ARGL){PRINT_MEM_REF("%zu",size_t,mem,fp);};
static void printSsize_t  (ARGL){PRINT_MEM_REF("%zd",ssize_t,mem,fp);}
static void printChar     (ARGL){PRINT_MEM_REF("%c",char,mem,fp);}
static void printWchar_t  (ARGL){PRINT_MEM_REF("%C",wchar_t,mem,fp);}
static void printFloat    (ARGL){PRINT_MEM_REF("%f",float,mem,fp);}
static void printDouble   (ARGL){PRINT_MEM_REF("%lf",double,mem,fp);}
static void printInt8_t   (ARGL){PRINT_MEM_REF("%d",int8_t,mem,fp);}
static void printUint8_t  (ARGL){PRINT_MEM_REF("%u",uint8_t,mem,fp);}
static void printInt16_t  (ARGL){PRINT_MEM_REF("%d",int16_t,mem,fp);}
static void printUint16_t (ARGL){PRINT_MEM_REF("%u",uint16_t,mem,fp);}
static void printInt32_t  (ARGL){PRINT_MEM_REF("%d",int32_t,mem,fp);}
static void printUint32_t (ARGL){PRINT_MEM_REF("%u",uint32_t,mem,fp);}
static void printInt64_t  (ARGL){PRINT_MEM_REF("%ld",int64_t,mem,fp);}
static void printUint64_t (ARGL){PRINT_MEM_REF("%lu",uint64_t,mem,fp);}
static void printIptr     (ARGL){PRINT_MEM_REF("%ld",intptr_t,mem,fp);}
static void printUptr     (ARGL){PRINT_MEM_REF("%lu",uintptr_t,mem,fp);}
static void printVptr     (ARGL){PRINT_MEM_REF("%p",void*,mem,fp);}
#undef ARGL
#undef PRINT_MEM_REF

#define ARGL const FklFfiMem* mem
#define GET_INTEGER_MEM(TYPE) return *(TYPE*)(mem->mem);
static int64_t __get_integer_from_short    (ARGL){GET_INTEGER_MEM(short)}
static int64_t __get_integer_from_int      (ARGL){GET_INTEGER_MEM(int)}
static int64_t __get_integer_from_ushort   (ARGL){GET_INTEGER_MEM(unsigned short)}
static int64_t __get_integer_from_unsigned (ARGL){GET_INTEGER_MEM(unsigned)}
static int64_t __get_integer_from_long     (ARGL){GET_INTEGER_MEM(long)}
static int64_t __get_integer_from_ulong    (ARGL){GET_INTEGER_MEM(unsigned long)}
static int64_t __get_integer_from_longlong (ARGL){GET_INTEGER_MEM(long long)}
static int64_t __get_integer_from_ulonglong(ARGL){GET_INTEGER_MEM(unsigned long)}
static int64_t __get_integer_from_ptrdiff_t(ARGL){GET_INTEGER_MEM(ptrdiff_t)}
static int64_t __get_integer_from_size_t   (ARGL){GET_INTEGER_MEM(size_t)}
static int64_t __get_integer_from_ssize_t  (ARGL){GET_INTEGER_MEM(ssize_t)}
static int64_t __get_integer_from_char     (ARGL){GET_INTEGER_MEM(char)}
static int64_t __get_integer_from_wchar_t  (ARGL){GET_INTEGER_MEM(wchar_t)}
static int64_t __get_integer_from_float    (ARGL){GET_INTEGER_MEM(float)}
static int64_t __get_integer_from_double   (ARGL){GET_INTEGER_MEM(double)}
static int64_t __get_integer_from_int8_t   (ARGL){GET_INTEGER_MEM(int8_t)}
static int64_t __get_integer_from_uint8_t  (ARGL){GET_INTEGER_MEM(uint8_t)}
static int64_t __get_integer_from_int16_t  (ARGL){GET_INTEGER_MEM(int16_t)}
static int64_t __get_integer_from_uint16_t (ARGL){GET_INTEGER_MEM(uint16_t)}
static int64_t __get_integer_from_int32    (ARGL){GET_INTEGER_MEM(int32_t)}
static int64_t __get_integer_from_uint32_t (ARGL){GET_INTEGER_MEM(uint32_t)}
static int64_t __get_integer_from_int64_t  (ARGL){GET_INTEGER_MEM(int64_t)}
static int64_t __get_integer_from_uint64_t (ARGL){GET_INTEGER_MEM(uint64_t)}
static int64_t __get_integer_from_iptr     (ARGL){GET_INTEGER_MEM(intptr_t)}
static int64_t __get_integer_from_uptr     (ARGL){GET_INTEGER_MEM(uintptr_t)}
#undef GET_INTEGER_MEM
#define GET_DOUBLE_MEM(TYPE) return *(TYPE*)(mem->mem);
static double __get_double_from_short    (ARGL){GET_DOUBLE_MEM(short)}
static double __get_double_from_int      (ARGL){GET_DOUBLE_MEM(int)}
static double __get_double_from_uShort   (ARGL){GET_DOUBLE_MEM(unsigned short)}
static double __get_double_from_unsigned (ARGL){GET_DOUBLE_MEM(unsigned)}
static double __get_double_from_long     (ARGL){GET_DOUBLE_MEM(long)}
static double __get_double_from_ulong    (ARGL){GET_DOUBLE_MEM(unsigned long)}
static double __get_double_from_longlong (ARGL){GET_DOUBLE_MEM(long long)}
static double __get_double_from_ulonglong(ARGL){GET_DOUBLE_MEM(unsigned long)}
static double __get_double_from_ptrdiff_t(ARGL){GET_DOUBLE_MEM(ptrdiff_t)}
static double __get_double_from_size_t   (ARGL){GET_DOUBLE_MEM(size_t)}
static double __get_double_from_ssize_t  (ARGL){GET_DOUBLE_MEM(ssize_t)}
static double __get_double_from_char     (ARGL){GET_DOUBLE_MEM(char)}
static double __get_double_from_wchar_t  (ARGL){GET_DOUBLE_MEM(wchar_t)}
static double __get_double_from_float    (ARGL){GET_DOUBLE_MEM(float)}
static double __get_double_from_double   (ARGL){GET_DOUBLE_MEM(double)}
static double __get_double_from_int8_t   (ARGL){GET_DOUBLE_MEM(int8_t)}
static double __get_double_from_uint8_t  (ARGL){GET_DOUBLE_MEM(uint8_t)}
static double __get_double_from_int16_t  (ARGL){GET_DOUBLE_MEM(int16_t)}
static double __get_double_from_uint16_t (ARGL){GET_DOUBLE_MEM(uint16_t)}
static double __get_double_from_int32    (ARGL){GET_DOUBLE_MEM(int32_t)}
static double __get_double_from_uint32_t (ARGL){GET_DOUBLE_MEM(uint32_t)}
static double __get_double_from_int64_t  (ARGL){GET_DOUBLE_MEM(int64_t)}
static double __get_double_from_uint64_t (ARGL){GET_DOUBLE_MEM(uint64_t)}
static double __get_double_from_iptr     (ARGL){GET_DOUBLE_MEM(intptr_t)}
static double __get_double_from_uptr     (ARGL){GET_DOUBLE_MEM(uintptr_t)}
#undef GET_DOUBLE_MEM
static int64_t (*__ffiGetIntegerFuncList[])(ARGL)=
{
	NULL                        ,
	__get_integer_from_short    ,
	__get_integer_from_int      ,
	__get_integer_from_ushort   ,
	__get_integer_from_unsigned ,
	__get_integer_from_long     ,
	__get_integer_from_ulong    ,
	__get_integer_from_longlong ,
	__get_integer_from_ulonglong,
	__get_integer_from_ptrdiff_t,
	__get_integer_from_size_t   ,
	__get_integer_from_ssize_t  ,
	__get_integer_from_char     ,
	__get_integer_from_wchar_t  ,
	__get_integer_from_float    ,
	__get_integer_from_double   ,
	__get_integer_from_int8_t   ,
	__get_integer_from_uint8_t  ,
	__get_integer_from_int16_t  ,
	__get_integer_from_uint16_t ,
	__get_integer_from_int32    ,
	__get_integer_from_uint32_t ,
	__get_integer_from_int64_t  ,
	__get_integer_from_uint64_t ,
	__get_integer_from_iptr     ,
	__get_integer_from_uptr     ,
};
static double (*__ffiGetDoubleFuncList[])(ARGL)=
{
	NULL                  ,
	__get_double_from_short    ,
	__get_double_from_int      ,
	__get_double_from_uShort   ,
	__get_double_from_unsigned ,
	__get_double_from_long     ,
	__get_double_from_ulong    ,
	__get_double_from_longlong ,
	__get_double_from_ulonglong,
	__get_double_from_ptrdiff_t,
	__get_double_from_size_t   ,
	__get_double_from_ssize_t  ,
	__get_double_from_char     ,
	__get_double_from_wchar_t  ,
	__get_double_from_float    ,
	__get_double_from_double   ,
	__get_double_from_int8_t   ,
	__get_double_from_uint8_t  ,
	__get_double_from_int16_t  ,
	__get_double_from_uint16_t ,
	__get_double_from_int32    ,
	__get_double_from_uint32_t ,
	__get_double_from_int64_t  ,
	__get_double_from_uint64_t ,
	__get_double_from_iptr     ,
	__get_double_from_uptr     ,
};
#undef ARGL

static void (*NativeTypePrinterList[])(FILE*,const void*)=
{
	printShortMem ,
	printIntMem   ,
	printUShortMem,
	printUIntMem  ,
	printLongMem  ,
	printULongMem ,
	printLLongMem ,
	printULLongMem,
	printPtrdiff_t,
	printSize_t   ,
	printSsize_t  ,
	printChar     ,
	printWchar_t  ,
	printFloat    ,
	printDouble   ,
	printInt8_t   ,
	printUint8_t  ,
	printInt16_t  ,
	printUint16_t ,
	printInt32_t  ,
	printUint32_t ,
	printInt64_t  ,
	printUint64_t ,
	printIptr     ,
	printUptr     ,
	printVptr     ,
};

static void __ffi_mem_print(const void* mem
		,FklTypeId_t type
		,FklFfiPublicData* pd
		,FILE* fp
		,FklSymbolTable* table)
{
	if(fklFfiIsNativeTypeId(type))
		NativeTypePrinterList[type-1](fp,mem);
	else
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(type,pd);
		if(fklFfiIsPtrTypeId(type,tu)||fklFfiIsFunctionType(tu))
			fprintf(fp,"%p",*((void**)(mem)));
		else if(type==FKL_FFI_TYPE_FILE_P)
			fprintf(fp,"%p",*((void**)(mem)));
		else if(type==FKL_FFI_TYPE_STRING)
			fprintf(fp,"%s",(char*)mem);
		else if(fklFfiIsStructType(tu))
		{
			FklDefStructType* st=FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(type,pd).all);
			fputc('{',fp);
			size_t offset=0;
			for(FklHashTableNodeList* list=st->layout->list;list;list=list->next)
			{
				FklFfiMemberHashItem* item=list->node->item;
				FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(item->type,pd);
				size_t align=fklFfiGetTypeAlign(tu);
				offset+=(offset%align)?align-offset%align:0;
				fputc('.',fp);
				fklPrintString(fklGetSymbolWithId(item->key,table)->symbol,fp);
				fputc('=',fp);
				__ffi_mem_print(mem+offset,item->type,pd,fp,table);
				size_t memberSize=fklFfiGetTypeSize(tu);
				offset+=memberSize;
				fputc(',',fp);
			}
			fputc('}',fp);
		}
		else if(fklFfiIsUnionType(tu))
		{
			FklDefUnionType* ut=FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(type,pd).all);
			fputc('{',fp);
			for(FklHashTableNodeList* list=ut->layout->list;list;list=list->next)
			{
				FklFfiMemberHashItem* item=list->node->item;
				fputc('.',fp);
				fklPrintString(fklGetSymbolWithId(item->key,table)->symbol,fp);
				fputc('=',fp);
				__ffi_mem_print(mem,item->type,pd,fp,table);
				fputc(';',fp);
			}
			fputc('}',fp);
		}
		else if(fklFfiIsArrayType(tu))
		{
			FklDefArrayType* at=FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(type,pd).all);
			fputc('[',fp);
			for(uint32_t i=0;i<at->num;i++)
			{
				__ffi_mem_print(mem+i*fklFfiGetTypeSize(tu),at->etype,pd,fp,table);
				fputc(',',fp);
			}
			fputc(']',fp);
		}
	}
}

static void fkl_ffi_mem_print(const FklVMudata* p,FILE* fp,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(m,FklFfiMem,p);
	FklVMvalue* pdv=m->pd;
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,pdv->u.ud);
	__ffi_mem_print(m->mem,m->type,pd,fp,table);
}

static int fkl_ffi_mem_equal(const FklVMudata* a,const FklVMudata* b)
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
	else
	{
		FKL_DECL_UD_DATA(m0,FklFfiMem,a);
		FKL_DECL_UD_DATA(m1,FklFfiMem,b);
		FKL_DECL_UD_DATA(pd,FklFfiPublicData,m0->pd->u.ud);
		if(m0->type==m1->type)
		{
			if((m0->type==FKL_FFI_TYPE_STRING||m0->type==FKL_FFI_TYPE_FILE_P)&&m0->mem==m1->mem)
				return 1;
			else if(!memcmp(m0->mem,m1->mem,fklFfiGetTypeSize(fklFfiLockAndGetTypeUnion(m0->type,pd))))
				return 1;
		}
	}
	return 0;
}

static int fkl_ffi_mem_cmp_VU(const FklVMvalue* a,FklFfiMem* b,int* isUnableToBeCmp)
{
	if((FKL_IS_STR(a)&&b->type!=FKL_FFI_TYPE_STRING)
			||(!FKL_IS_STR(a)&&b->type==FKL_FFI_TYPE_STRING)
			||!fklIsVMnumber(a)
			||!fklFfiIsNumTypeId(b->type))
	{
		*isUnableToBeCmp=1;
		return 0;
	}
	if(b->type==FKL_FFI_TYPE_STRING)
	{
		size_t len=strlen(b->mem);
		int r=memcmp(a->u.str->str,b->mem,a->u.str->size<len?a->u.str->size:len);
		if(!r)
			return (int64_t)a->u.str->size-(int64_t)len;
		return r;
	}
	else if(FKL_IS_F64(a)||b->type==FKL_FFI_TYPE_DOUBLE)
	{
		double ad=fklGetDouble(a);
		double bd=__ffiGetDoubleFuncList[b->type](b);
		if(ad-bd>DBL_EPSILON)
			return 1;
		else if(ad-bd<-DBL_EPSILON)
			return -1;
		else
			return 0;
	}
	else if(FKL_IS_FIX(a))
	{
		int64_t ai=fklGetInt(a);
		int64_t bi=__ffiGetIntegerFuncList[b->type](b);
		if(ai>bi)
			return 1;
		else if(ai<bi)
			return -1;
		else
			return 0;
	}
	else if(FKL_IS_BIG_INT(a))
	{
		if(fklIsLtI64MinBigInt(a->u.bigInt))
			return -1;
		else if(fklIsGtI64MaxBigInt(a->u.bigInt))
			return 1;
		else
		{
			int64_t ai=fklBigIntToI64(a->u.bigInt);
			int64_t bi=__ffiGetIntegerFuncList[b->type](b);
			if(ai>bi)
				return 1;
			else if(ai<bi)
				return -1;
			else
				return 0;
		}
	}
	return 0;
}

static int fkl_ffi_mem_cmp_UU(const FklFfiMem* a,const FklFfiMem* b,int* isUnableToBeCmp)
{
	if((a->type==FKL_FFI_TYPE_STRING&&b->type!=a->type)
			||(a->type!=FKL_FFI_TYPE_STRING&&b->type==FKL_FFI_TYPE_STRING)
			||!fklFfiIsNumTypeId(a->type)
			||!fklFfiIsNumTypeId(b->type))
	{
		*isUnableToBeCmp=1;
		return 0;
	}
	if(a->type==FKL_FFI_TYPE_STRING&&b->type==FKL_FFI_TYPE_STRING)
		return strcmp(a->mem,b->mem);
	else if(fklFfiIsIntegerTypeId(a->type)&&fklFfiIsIntegerTypeId(b->type))
	{
		int64_t ai=__ffiGetIntegerFuncList[a->type](a);
		int64_t bi=__ffiGetIntegerFuncList[b->type](b);
		if(ai>bi)
			return 1;
		else if(ai<bi)
			return -1;
		else
			return 0;
	}
	else
	{
		double ad=__ffiGetDoubleFuncList[a->type](a);
		double bd=__ffiGetDoubleFuncList[b->type](b);
		if(ad-bd>DBL_EPSILON)
			return 1;
		else if(ad-bd<-DBL_EPSILON)
			return -1;
		else
			return 0;
	}
	return 0;
}

static int fkl_ffi_mem_cmp(const FklVMudata* a,const FklVMvalue* b,int* isUnableToBeCmp)
{
	if(!fklFfiIsMem(b))
		return fkl_ffi_mem_cmp_VU(b,FKL_GET_UD_DATA(FklFfiMem,a),isUnableToBeCmp)*-1;
	else
		return fkl_ffi_mem_cmp_UU(FKL_GET_UD_DATA(FklFfiMem,a),FKL_GET_UD_DATA(FklFfiMem,b->u.ud),isUnableToBeCmp);
}

static FklVMudMethodTable FfiMemMethodTable=
{
	.__princ=fkl_ffi_mem_print,
	.__prin1=fkl_ffi_mem_print,
	.__finalizer=NULL,
	.__equal=fkl_ffi_mem_equal,
	.__call=NULL,
	.__cmp=fkl_ffi_mem_cmp,
	.__write=NULL,
	.__atomic=NULL,
	.__append=NULL,
	.__copy=NULL,
	.__hash=NULL,
	.__setq_hook=NULL,
};

static FklVMudMethodTable FfiAtomicMemMethodTable=
{
	.__princ=fkl_ffi_mem_print,
	.__prin1=fkl_ffi_mem_print,
	.__finalizer=fkl_ffi_mem_atomic_finalizer,
	.__equal=fkl_ffi_mem_equal,
	.__call=NULL,
	.__cmp=fkl_ffi_mem_cmp,
	.__write=NULL,
	.__atomic=NULL,
	.__append=NULL,
	.__copy=NULL,
	.__hash=NULL,
	.__setq_hook=NULL,
};

void fklFfiMemInit(FklFfiPublicData* pd,FklSymbolTable* table)
{
	pd->memUdSid=fklAddSymbolCstr("ffi-mem",table)->id;
	pd->atomicSid=fklAddSymbolCstr("atomic",table)->id;
	pd->rawSid=fklAddSymbolCstr("raw",table)->id;
	pd->refSid=fklAddSymbolCstr("&",table)->id;
	pd->deRefSid=fklAddSymbolCstr("*",table)->id;
}

inline FklVMudata* fklFfiCreateAtomicMemUd(FklTypeId_t type
		,size_t size
		,FklVMvalue* rel
		,FklVMvalue* pd)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklVMudata* ud=fklCreateVMudata(publicData->memUdSid,&FfiAtomicMemMethodTable,rel,sizeof(FklFfiMem));
	fklFfiInitMem(FKL_GET_UD_DATA(FklFfiMem,ud),type,size,pd);
	return ud;
}

inline FklVMudata* fklFfiCreateRawMemUd(FklTypeId_t type
		,size_t size
		,FklVMvalue* rel
		,FklVMvalue* pd)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklVMudata* ud=fklCreateVMudata(publicData->memUdSid,&FfiMemMethodTable,rel,sizeof(FklFfiMem));
	fklFfiInitMem(FKL_GET_UD_DATA(FklFfiMem,ud),type,size,pd);
	return ud;
}

FklVMudata* fklFfiCreateMemUd(FklTypeId_t type
		,size_t size
		,FklVMvalue* atomic
		,FklVMvalue* rel
		,FklVMvalue* pd)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(atomic==NULL||FKL_GET_SYM(atomic)==publicData->atomicSid)
		return fklFfiCreateAtomicMemUd(type,size,rel,pd);
	else if(FKL_GET_SYM(atomic)==publicData->rawSid)
		return fklFfiCreateRawMemUd(type,size,rel,pd);
	else
		return NULL;
}

FklVMudata* fklFfiCreateMemRefUd(FklTypeId_t type,void* mem,FklVMvalue* rel,FklVMvalue* pd)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklVMudata* ud=fklCreateVMudata(publicData->memUdSid,&FfiMemMethodTable,rel,sizeof(FklFfiMem));
	fklFfiInitRef(FKL_GET_UD_DATA(FklFfiMem,ud),type,mem,pd);
	return ud;
}

FklVMudata* fklFfiCreateMemRefUdWithSI(FklFfiMem* m
		,FklVMvalue* selector
		,FklVMvalue* pindex
		,FklVMvalue* rel
		,FklVMvalue* pd)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	if(selector==NULL||selector==FKL_VM_NIL)
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(m->type,publicData);
		if(pindex&&(!fklFfiIsPtrTypeId(m->type,tu)&&!fklFfiIsArrayType(tu)&&m->type!=FKL_FFI_TYPE_STRING))
			return NULL;
		if(pindex&&(fklFfiIsPtrTypeId(m->type,tu)||fklFfiIsArrayType(tu)||m->type==FKL_FFI_TYPE_STRING))
		{
			int64_t index=fklGetInt(pindex);
			if(fklFfiIsPtrTypeId(m->type,tu))
			{
				FklDefPtrType* pt=FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(m->type,publicData).all);
				void* p=*(void**)m->mem;
				FklDefTypeUnion ptu=fklFfiLockAndGetTypeUnion(pt->ptype,publicData);
				return fklFfiCreateMemRefUd(pt->ptype,p+index*fklFfiGetTypeSize(ptu),rel,pd);
			}
			else if(fklFfiIsArrayType(tu))
			{
				FklDefArrayType* at=FKL_GET_TYPES_PTR(fklFfiLockAndGetTypeUnion(m->type,publicData).all);
				FklDefTypeUnion etu=fklFfiLockAndGetTypeUnion(at->etype,publicData);
				return fklFfiCreateMemRefUd(at->etype,m->mem+index*fklFfiGetTypeSize(etu),rel,pd);
			}
			else
				return fklFfiCreateMemRefUd(FKL_FFI_TYPE_CHAR,m->mem+index*sizeof(char),rel,pd);
		}
		else
			return fklFfiCreateMemRefUd(m->type,m->mem,rel,pd);
	}
	else
	{
		FklDefTypeUnion tu=fklFfiLockAndGetTypeUnion(m->type,publicData);
		FklSid_t selectorId=FKL_GET_SYM(selector);
		if(selectorId==publicData->refSid)
		{
			FklVMudata* ud=fklFfiCreateMemUd(fklFfiCreatePtrType(m->type,publicData),sizeof(void*),NULL,rel,pd);
			FKL_DECL_UD_DATA(ptr,FklFfiMem,ud);
			*(void**)ptr->mem=m->mem;
			return ud;
		}
		else if(selectorId==publicData->deRefSid)
		{
			if(!fklFfiIsPtrTypeId(m->type,tu))
				return NULL;
			FklDefPtrType* ptrType=FKL_GET_TYPES_PTR(tu.all);
			return fklFfiCreateMemRefUd(ptrType->ptype,*(void**)m->mem,rel,pd);
		}
		else if(fklFfiIsStructType(tu)||fklFfiIsUnionType(tu))
		{
			FklHashTable* layout=NULL;
			if(fklFfiIsStructType(tu))
			{
				FklDefStructType* st=FKL_GET_TYPES_PTR(tu.all);
				layout=st->layout;
			}
			else
			{
				FklDefUnionType* ut=FKL_GET_TYPES_PTR(tu.all);
				layout=ut->layout;
			}
			FklFfiMemberHashItem* item=fklGetHashItem(&selectorId,layout);
			FklVMudata* ud=fklFfiCreateMemRefUd(item->type,m->mem+item->offset,rel,pd);
			return ud;
		}
	}
	return NULL;
}

int fklFfiIsMem(const FklVMvalue* p)
{
	return FKL_IS_USERDATA(p)
		&&(p->u.ud->t==&FfiMemMethodTable||p->u.ud->t==&FfiAtomicMemMethodTable);
}

static int __ffiGetVMvalueAsF64(FklVMvalue* val,double* d)
{
	if(fklIsInt(val))
		*d=fklGetInt(val);
	else if(FKL_IS_CHR(val))
		*d=FKL_GET_CHR(val);
	else if(FKL_IS_F64(val))
		*d=val->u.f64;
	else if(fklFfiIsMem(val)&&fklFfiIsNumTypeId(((FklFfiMem*)val->u.ud->data)->type))
	{
		FKL_DECL_UD_DATA(mem,FklFfiMem,val->u.ud);
		*d=__ffiGetDoubleFuncList[mem->type](mem);
	}
	else
		return 1;
	return 0;
}

static int __ffiGetVMvalueAsI64(FklVMvalue* val,int64_t* i)
{
	if(fklIsInt(val))
		*i=fklGetInt(val);
	else if(FKL_IS_CHR(val))
		*i=FKL_GET_CHR(val);
	else if(FKL_IS_F64(val))
		*i=val->u.f64;
	else if(fklFfiIsMem(val)&&fklFfiIsNumTypeId(((FklFfiMem*)val->u.ud->data)->type))
	{
		FKL_DECL_UD_DATA(mem,FklFfiMem,val->u.ud);
		*i=__ffiGetIntegerFuncList[mem->type](mem);
	}
	else
		return 1;
	return 0;
}

#define ARGL void* mem,FklVMvalue* val
#define SET_INTEGER_MEM(TYPE) int64_t i=0.0;if(__ffiGetVMvalueAsI64(val,&i))return 1;*(TYPE*)mem=i;return 0;
#define SET_FLOAT_MEM(TYPE) double d=0.0;if(__ffiGetVMvalueAsF64(val,&d))return 1;*(TYPE*)mem=d;return 0;
static int __set_short    (ARGL){SET_INTEGER_MEM(short)}
static int __set_int      (ARGL){SET_INTEGER_MEM(int)}
static int __set_ushort   (ARGL){SET_INTEGER_MEM(unsigned short)}
static int __set_unsigned (ARGL){SET_INTEGER_MEM(unsigned)}
static int __set_long     (ARGL){SET_INTEGER_MEM(long)}
static int __set_ulong    (ARGL){SET_INTEGER_MEM(unsigned long)}
static int __set_longlong (ARGL){SET_INTEGER_MEM(long long)}
static int __set_ulonglong(ARGL){SET_INTEGER_MEM(unsigned long long)}
static int __set_ptrdiff_t(ARGL){SET_INTEGER_MEM(ptrdiff_t)}
static int __set_size_t   (ARGL){SET_INTEGER_MEM(size_t)}
static int __set_ssize_t  (ARGL){SET_INTEGER_MEM(ssize_t)}
static int __set_char     (ARGL){SET_INTEGER_MEM(char)}
static int __set_wchar_t  (ARGL){SET_INTEGER_MEM(wchar_t)}
static int __set_float    (ARGL){SET_FLOAT_MEM(float)}
static int __set_double   (ARGL){SET_FLOAT_MEM(double)}
static int __set_int8_t   (ARGL){SET_INTEGER_MEM(int8_t)}
static int __set_uint8_t  (ARGL){SET_INTEGER_MEM(uint8_t)}
static int __set_int16_t  (ARGL){SET_INTEGER_MEM(int16_t)}
static int __set_uint16_t (ARGL){SET_INTEGER_MEM(uint16_t)}
static int __set_int32_t  (ARGL){SET_INTEGER_MEM(int32_t)}
static int __set_uint32_t (ARGL){SET_INTEGER_MEM(uint32_t)}
static int __set_int64_t  (ARGL){SET_INTEGER_MEM(int64_t)}
static int __set_uint64_t (ARGL){SET_INTEGER_MEM(uint64_t)}
static int __set_iptr     (ARGL){SET_INTEGER_MEM(intptr_t)}
static int __set_uptr     (ARGL){SET_INTEGER_MEM(uintptr_t)}
#undef SET_INTEGER_MEM
#undef SET_FLOAT_MEM
#undef ARGL
static int (*__ffiMemSetList[])(void*,FklVMvalue*)=
{
	NULL           ,
	__set_short    ,
	__set_int      ,
	__set_ushort   ,
	__set_unsigned ,
	__set_long     ,
	__set_ulong    ,
	__set_longlong ,
	__set_ulonglong,
	__set_ptrdiff_t,
	__set_size_t   ,
	__set_ssize_t  ,
	__set_char     ,
	__set_wchar_t  ,
	__set_float    ,
	__set_double   ,
	__set_int8_t   ,
	__set_uint8_t  ,
	__set_int16_t  ,
	__set_uint16_t ,
	__set_int32_t  ,
	__set_uint32_t ,
	__set_int64_t  ,
	__set_uint64_t ,
	__set_iptr     ,
	__set_uptr     ,
};

int fklFfiSetMemForProc(FklVMudata* ud,FklVMvalue* val,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(ref,FklFfiMem,ud);
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,ref->pd->u.ud);
	if(fklFfiIsNumTypeId(ref->type))
		return __ffiMemSetList[ref->type](ref->mem,val);
	else
	{
		FklDefTypeUnion rtu=fklFfiLockAndGetTypeUnion(ref->type,pd);
		if(fklFfiIsPtrTypeId(ref->type,rtu))
		{
			if(val==FKL_VM_NIL)
				*(void**)ref->mem=NULL;
			else if(fklFfiIsMem(val))
			{
				FKL_DECL_UD_DATA(valmem,FklFfiMem,val->u.ud);
				FklDefTypeUnion vtu=fklFfiLockAndGetTypeUnion(valmem->type,pd);
				if(!fklFfiIsPtrTypeId(valmem->type,vtu)&&valmem->type!=FKL_FFI_TYPE_STRING&&valmem->type!=FKL_FFI_TYPE_FILE_P)
					return 1;
				if(fklFfiIsPtrTypeId(valmem->type,vtu))
					*(void**)ref->mem=*(void**)valmem->mem;
				else if(valmem->type==FKL_FFI_TYPE_STRING)
					*(void**)ref->mem=valmem->mem;
				else if(valmem->type==FKL_FFI_TYPE_FILE_P)
					*(void**)ref->mem=valmem->mem;
				else
					return 1;
			}
			else if(fklFfiIsProc(val))
			{
				FKL_DECL_UD_DATA(valproc,FklFfiProc,val->u.ud);
				*(void**)ref->mem=valproc->func;
			}
			else
				return 1;
		}
		else if(ref->type==FKL_FFI_TYPE_STRING)
		{
			if(FKL_IS_STR(val))
			{
				if(ref->mem)
					free(ref->mem);
				ref->mem=fklCharBufToCstr(val->u.str->str,val->u.str->size);
			}
			else if(FKL_IS_SYM(val))
			{
				if(ref->mem)
					free(ref->mem);
				FklString* s=fklGetSymbolWithId(FKL_GET_SYM(val),table)->symbol;
				ref->mem=fklCharBufToCstr(s->str,s->size);
			}
			else if(fklFfiIsMem(val))
			{
				FKL_DECL_UD_DATA(m,FklFfiMem,val->u.ud);
				if(m->type!=FKL_FFI_TYPE_STRING)
					return 1;
				free(ref->mem);
				ref->mem=m->mem;
				ud->t=&FfiMemMethodTable;
			}
			else
				return 1;
		}
		else if(ref->type==FKL_FFI_TYPE_FILE_P)
		{
			if(FKL_IS_FP(val))
				ref->mem=val->u.fp->fp;
			else
				return 1;
		}
		else
		{
			if(!fklFfiIsMem(val))
				return 1;
			else
			{
				FKL_DECL_UD_DATA(valmem,FklFfiMem,val->u.ud);
				if(ref->type!=valmem->type)
					return 1;
				FklDefTypeUnion rtu=fklFfiLockAndGetTypeUnion(ref->type,pd);
				if(fklFfiIsArrayType(rtu))
				{
					free(ref->mem);
					ref->mem=valmem->mem;
					ud->t=&FfiMemMethodTable;
				}
				else
					memcpy(ref->mem,valmem->mem
							,fklFfiGetTypeSize(rtu));
			}
		}
	}
	return 0;
}

int fklFfiSetMem(FklVMudata* ud,FklVMvalue* val,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(ref,FklFfiMem,ud);
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,ref->pd->u.ud);
	if(fklFfiIsNumTypeId(ref->type))
		return __ffiMemSetList[ref->type](ref->mem,val);
	else
	{
		FklDefTypeUnion rtu=fklFfiLockAndGetTypeUnion(ref->type,pd);
		if(fklFfiIsPtrTypeId(ref->type,rtu))
		{
			if(val==FKL_VM_NIL)
				*(void**)ref->mem=NULL;
			else if(fklFfiIsMem(val))
			{
				FKL_DECL_UD_DATA(valmem,FklFfiMem,val->u.ud);
				FklDefTypeUnion vtu=fklFfiLockAndGetTypeUnion(valmem->type,pd);
				if(!fklFfiIsPtrTypeId(valmem->type,vtu)&&valmem->type!=FKL_FFI_TYPE_STRING&&valmem->type!=FKL_FFI_TYPE_FILE_P)
					return 1;
				if(fklFfiIsPtrTypeId(valmem->type,vtu))
					*(void**)ref->mem=*(void**)valmem->mem;
				else if(valmem->type==FKL_FFI_TYPE_STRING)
					*(void**)ref->mem=valmem->mem;
				else if(valmem->type==FKL_FFI_TYPE_FILE_P)
					*(void**)ref->mem=valmem->mem;
				else
					return 1;
			}
			else if(fklFfiIsProc(val))
			{
				FKL_DECL_UD_DATA(valproc,FklFfiProc,val->u.ud);
				*(void**)ref->mem=valproc->func;
			}
			else
				return 1;
		}
		else if(ref->type==FKL_FFI_TYPE_STRING)
		{
			if(FKL_IS_STR(val))
			{
				if(ref->mem)
					free(ref->mem);
				ref->mem=fklCharBufToCstr(val->u.str->str,val->u.str->size);
			}
			else if(FKL_IS_SYM(val))
			{
				if(ref->mem)
					free(ref->mem);
				FklString* s=fklGetSymbolWithId(FKL_GET_SYM(val),table)->symbol;
				ref->mem=fklCharBufToCstr(s->str,s->size);
			}
			else if(fklFfiIsMem(val))
			{
				FKL_DECL_UD_DATA(m,FklFfiMem,val->u.ud);
				if(m->type!=FKL_FFI_TYPE_STRING)
					return 1;
				free(ref->mem);
				ref->mem=fklCopyCstr(m->mem);
			}
			else
				return 1;
		}
		else if(ref->type==FKL_FFI_TYPE_FILE_P)
		{
			if(FKL_IS_FP(val))
				ref->mem=val->u.fp->fp;
			else
				return 1;
		}
		else
		{
			if(!fklFfiIsMem(val))
				return 1;
			else
			{
				FKL_DECL_UD_DATA(valmem,FklFfiMem,val->u.ud);
				if(ref->type!=valmem->type)
					return 1;
				else
					memcpy(ref->mem,valmem->mem,fklFfiGetTypeSize(rtu));
			}
		}
	}
	return 0;
}

int fklFfiIsNull(FklFfiMem* m)
{
	FKL_DECL_UD_DATA(pd,FklFfiPublicData,m->pd->u.ud);
	return (m->type==FKL_FFI_TYPE_VPTR
			||FKL_GET_TYPES_TAG(fklFfiLockAndGetTypeUnion(m->type,pd).all))
		&&!(*(void**)m->mem);
}

int fklFfiIsCastableVMvalueType(FklVMvalue* v)
{
	return !FKL_IS_PAIR(v)
		&&!FKL_IS_CHAN(v)
		&&!FKL_IS_DLL(v)
		&&!FKL_IS_PROC(v)
		&&!FKL_IS_DLPROC(v)
		&&!FKL_IS_VECTOR(v)
		&&!FKL_IS_ERR(v)
		&&!fklFfiIsMem(v);
}

inline void fklFfiInitMem(FklFfiMem* m
		,FklTypeId_t type
		,size_t size
		,FklVMvalue* pd)
{
	void* mem=calloc(1,size);
	FKL_ASSERT(mem);
	m->pd=pd;
	m->mem=mem;
	m->type=type;
}

inline void fklFfiInitRef(FklFfiMem* m,FklTypeId_t t,void* mem,FklVMvalue* pd)
{
	m->type=t;
	m->mem=mem;
	m->pd=pd;
}

FklVMudata* fklFfiCastVMvalueIntoMem(FklVMvalue* v
		,FklVMvalue* rel
		,FklVMvalue* pd
		,FklSymbolTable* table)
{
	FKL_DECL_UD_DATA(publicData,FklFfiPublicData,pd->u.ud);
	FklVMudata* r=NULL;
	if(fklFfiIsMem(v))
	{
		if(v->u.ud->t->__call)
		{
			FKL_DECL_UD_DATA(proc,FklFfiProc,v->u.ud);
			r=fklFfiCreateMemRefUd(proc->type,proc->func,rel,pd);
		}
		else
		{
			FKL_DECL_UD_DATA(mem,FklFfiMem,v->u.ud);
			r=fklFfiCreateMemRefUd(mem->type,mem->mem,rel,pd);
		}
	}
	else
	{
		r=fklCreateVMudata(publicData->memUdSid,&FfiAtomicMemMethodTable,rel,sizeof(FklFfiMem));
		FKL_DECL_UD_DATA(m,FklFfiMem,r);
		if(FKL_IS_FIX(v))
		{
			fklFfiInitMem(m,FKL_FFI_TYPE_INT64_T,sizeof(void*),pd);
			*(int64_t*)m->mem=FKL_GET_FIX(v);
		}
		else if(FKL_IS_BIG_INT(v)&&fklIsGeLeI64BigInt(v->u.bigInt))
		{
			fklFfiInitMem(m,FKL_FFI_TYPE_INT64_T,sizeof(void*),pd);
			*(int64_t*)m->mem=fklBigIntToI64(v->u.bigInt);
		}
		else if(FKL_IS_F64(v))
		{
			fklFfiInitMem(m,FKL_FFI_TYPE_DOUBLE,sizeof(void*),pd);
			*(double*)m->mem=v->u.f64;
		}
		else if(FKL_IS_CHR(v))
		{
			fklFfiInitMem(m,FKL_FFI_TYPE_CHAR,sizeof(void*),pd);
			*(char*)m->mem=FKL_GET_CHR(v);
		}
		else if(FKL_IS_STR(v))
			fklFfiInitRef(m,FKL_FFI_TYPE_STRING,fklCharBufToCstr(v->u.str->str,v->u.str->size),pd);
		else if(FKL_IS_SYM(v))
		{
			FklString* s=fklGetSymbolWithId(FKL_GET_SYM(v),table)->symbol;
			fklFfiInitRef(m,FKL_FFI_TYPE_STRING,fklCharBufToCstr(s->str,s->size),pd);
		}
		else if(FKL_IS_FP(v))
		{
			fklFfiInitMem(m,FKL_FFI_TYPE_FILE_P,sizeof(void*),pd);
			*(FILE**)m->mem=v->u.fp->fp;
		}
		else if(v==FKL_VM_NIL)
			fklFfiInitMem(m,FKL_FFI_TYPE_VPTR,sizeof(void*),pd);
	}
	return r;
}

int fklFfiIsValuableMem(FklFfiMem* mem)
{
	if(fklFfiIsNumTypeId(mem->type)||fklFfiIsNull(mem)||mem->type==FKL_FFI_TYPE_STRING)
		return 1;
	return 0;
}

FklVMvalue* fklFfiCreateVMvalue(FklFfiMem* mem,FklVM* vm)
{
	if(fklFfiIsNull(mem))
		return FKL_VM_NIL;
	else if(mem->type==FKL_FFI_TYPE_STRING)
	{
		FklString* str=fklCreateString(strlen(mem->mem),mem->mem);
		return fklCreateVMvalueToStack(FKL_TYPE_STR,str,vm);
	}
	else
	{
		if(fklFfiIsFloatTypeId(mem->type))
		{
			double d=__ffiGetDoubleFuncList[mem->type](mem);
			return fklCreateVMvalueToStack(FKL_TYPE_F64,&d,vm);
		}
		else
			return fklMakeVMint(__ffiGetIntegerFuncList[mem->type](mem),vm);
	}
}
