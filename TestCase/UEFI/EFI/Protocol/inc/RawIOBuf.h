#ifndef _RAWIOBUF_H
#define _RAWIOBUF_H

#include "../../../cdf/ostd/inc/type.h"

/******************************************************************************
* NAME		:	sRawIOBufferTag
*-------------------------------------------------------------------------- 
* ABSTRACT	:	Raw I/O buffer structure header.
*
* PURPOSE	:	Refer to raw I/O in structured form
*-------------------------------------------------------------------------- 
* PARAMETERS:
*     
*	UINT8	u8IOBufferSize      -   the out string size (with the header size).
*	UINT8	*pu8IOBufferStart    -   the start pointer of the out string.
*	UINT8	*pu8IOBufferEnd      -   the end pointer of the out string.
*
*
******************************************************************************/    
typedef struct sRawIOBufferTag
{
	UINT8	u8IOBufferSize;
	UINT8	*pu8IOBufferStart;
	UINT8	*pu8IOBufferEnd;
}sIOBufferHdr;

#endif /* _RAWIOBUF_H */
