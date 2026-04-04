/**************************************************************************
 *									  *
 *		 Copyright (C) 1994, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

/*
 * Due to compiler bug, it won't insert a sqrt.s opcode in the stream so we
 * make a function and call it for now. When compiler fixed, also remove the
 * seperate Makefile rules
 */

#include "sys/asm.h"
#include "sys/regdef.h"

LEAF(sqrtf)
    sqrt.s fv0, fa0
    j ra
END(sqrtf)
