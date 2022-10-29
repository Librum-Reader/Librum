/*
SPDX-FileCopyrightText: 2008, 2009, 2010 , 2011 jerome DOT laurens AT
u-bourgogne DOT fr SPDX-License-Identifier: X11

This file is part of the SyncTeX package.

Latest Revision: Tue Jun 14 08:23:30 UTC 2011

Version: 1.18

See synctex_parser_readme.txt for more details
*/

/* This local header file is for TEXLIVE, use your own header to fit your system
 */
#include <w2c/c-auto.h> /* for inline && HAVE_xxx */
/*	No inlining for synctex tool in texlive. */
#define SYNCTEX_INLINE
