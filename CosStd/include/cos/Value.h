#ifndef COS_VALUE_H
#define COS_VALUE_H

/*
 o---------------------------------------------------------------------o
 |
 | COS Value
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
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: Value.h,v 1.12 2010/05/23 08:22:55 ldeniau Exp $
 |
*/

#include <cos/Object.h>

/* NOTE-USER: Values subclasses

<- Value
   <- Number
      <- Integral
         <- Int
            <- Char
            <- Short
         <- Long
         <- BigInt     (gmp mpz_t)
      <- Floating
         <- Float
         <- Complex
         <- Rational   (gmp mpq_t)
         <- BigFloat   (gmp mpf_t)

   <- Slice
   <- Range
   <- XRange

   <- DateTime
*/

defclass(Value)
endclass

#endif // COS_VALUE_H

