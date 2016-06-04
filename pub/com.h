#ifndef _IVM_PUB_COM_H_
#define _IVM_PUB_COM_H_

#define IVM_PRIVATE static
#define IVM_PUBLIC extern

#define IVM_INLINE inline __attribute__((always_inline))

#ifdef __cplusplus
	#define IVM_COM_HEADER extern "C" {
	#define IVM_COM_END }
#else
	#define IVM_COM_HEADER
	#define IVM_COM_END
#endif

#endif
