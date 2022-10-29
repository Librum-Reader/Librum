// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
#ifndef _xdvi_h
#define _xdvi_h

/*
 *  Written by Eric C. Cooper, CMU
 */

#define ROUNDUP(x, y) (((x) + (y)-1) / (y))

extern unsigned long num(FILE*, int);
extern long snum(FILE*, int);
extern struct WindowRec mane, currwin;

#define one(fp) ((unsigned char)getc(fp))
#define sone(fp) ((long)one(fp))
#define two(fp) num(fp, 2)
#define stwo(fp) snum(fp, 2)
#define four(fp) num(fp, 4)
#define sfour(fp) snum(fp, 4)

#endif /* _xdvi_h */
