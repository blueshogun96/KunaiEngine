#pragma once

#include "nvdebug.h"

/* 
 * Debug output level 
 * Set to log only errors by default
 */
#ifndef KE_DBG_LEVEL
#define KE_DBG_LEVEL 3
#endif

/*
 * Debug message priorities
 */
#define KE_ERROR		1
#define KE_WARNING		2
#define KE_DBGLVL(a)	(3+a)

/*
 * Call stack 
 */
void KeCallStackPush( std::string function );
void KeCallStackPop();

#if _DEBUG
#define _pushfn     KeCallStackPush(__KE_FUNCTION__)
#define _popfn      KeCallStackPop()
#else
#define _pushfn
#define _popfn
#endif