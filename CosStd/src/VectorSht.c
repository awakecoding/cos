/*
 o---------------------------------------------------------------------o
 |
 | COS ShtVector
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied wstranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: VectorSht.c,v 1.2 2010/06/13 20:24:46 ldeniau Exp $
 |
*/

#define SHTVECTOR_ONLY

#define T     ShtVector
#define TM   mShtVector
#define TP  pmShtVector
#define TN    ShtVectorN
#define TF    ShtVectorFix
#define TD    ShtVectorDyn
// #define TL    ShtVectorLzy
#define TV    ShtVectorView
#define TW    ShtVectorSubView
#define TS    "short vector"
#define TE    Short

#define TF1   IntFunction1
#define TF2   IntFunction2
#define TF3   IntFunction3
#define TF4   IntFunction4
#define TF5   IntFunction5

#define PF1   I32FCT1
#define PF2   I32FCT2
#define PF3   I32FCT3
#define PF4   I32FCT4
#define PF5   I32FCT5

#define VS    Vector.ValueSequence.Sequence

#define VAL       I16
#define VALOBJ(v) aShort(v)

#define aTView(v,s)    aShtVectorView(v,s)
#define aTRef(v,s)     aShtVectorRef(v,s)

#define T_alloc(s)        ShtVector_alloc(s)
#define TV_init(vw,v,s,b) ShtVectorView_init(vw,v,s,b)

#define TOVALPTR      gshtPtr
#define TOVAL(v)      gsht(v)
#define VALUE(v)      v->Int.value
#define EQUAL(v1,v2)  (v1 == v2)

#include <cos/ShtVector.h>
#include <cos/Function.h>

makclass(ShtVector, IntegralVector);

// vector templates
#include "./tmpl/Vector_utl.h"

#include "./tmpl/Vector.c"

#include "./tmpl/Vector_dyn.c"
// #include "./tmpl/Vector_lzy.c"
#include "./tmpl/Vector_vw.c"

#include "./tmpl/Vector_acc.c"
#include "./tmpl/Vector_alg.c"
#include "./tmpl/Vector_fun.c"
// #include "./tmpl/Vector_fct.c"
// #include "./tmpl/Vector_vfn.c"

