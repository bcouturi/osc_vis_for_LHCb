
/*
 * Progress - A simple progress meter widget, incorporating display of
 *		percentage complete, and estimated time to completion.
 *
 * Copyright (c) 1997 Richard H. Porter
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * Author: Dick Porter <dick@cymru.net>
 *
 */

#ifndef _PROGRESS_P_H_
#define _PROGRESS_P_H_

/* this */
#include <OnX/Xt/Progress.h>

#include <Xm/PrimitiveP.h>   

#include <time.h>

typedef struct {
	int make_compiler_happy;	/* keep compiler happy */
} XrhpProgressClassPart;

typedef struct _XrhpProgressClassRec {
	CoreClassPart core_class;
	XmPrimitiveClassPart primitive_class;
	XrhpProgressClassPart progress_class;
} XrhpProgressClassRec;

extern XrhpProgressClassRec xrhpProgressClassRec;

typedef struct {
    /* resources */
	int max_value;
	int value;
	Boolean show_percent;
	Boolean show_time;
	XFontStruct *font;

    /* private state */
	time_t start_time;
	GC line_gc;
	GC blank_gc;
	GC text_gc;
} XrhpProgressPart;

typedef struct _XrhpProgressRec {
	CorePart core;
	XmPrimitivePart primitive;
	XrhpProgressPart progress;
} XrhpProgressRec;

#endif /* _PROGRESS_P_H_ */
