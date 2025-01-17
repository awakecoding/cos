#ifndef COS_GEN_FLOATOP_H
#define COS_GEN_FLOATOP_H

/*
 o---------------------------------------------------------------------o
 |
 | COS generic for floats operators
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
 | $Id: floatop.h,v 1.6 2010/05/25 15:33:39 ldeniau Exp $
 |
*/

#include <cos/gen/op/GFloatOp.h>

/* NOTE-USER: operators policy
   The policy of these messages is to return the operation result in a
   new autoDelete object resulting from the promotion/coercion of _1
   and _2 which follows the rules of C99 for primitive types.
*/
defgeneric(OBJ, (G_conj  ) gconj , _1);  // return _1^*
defgeneric(OBJ, (G_arg   ) garg  , _1);  // return arg(_1)

defgeneric(OBJ, (G_exp   ) gexp  , _1);  // return exp(_1)
defgeneric(OBJ, (G_log   ) glog  , _1);  // return log(_1)
defgeneric(OBJ, (G_sqrt  ) gsqrt , _1);  // return sqrt(_1)

defgeneric(OBJ, (G_cos   ) gcos  , _1);  // return cos(_1)
defgeneric(OBJ, (G_sin   ) gsin  , _1);  // return sin(_1)
defgeneric(OBJ, (G_tan   ) gtan  , _1);  // return tan(_1)
defgeneric(OBJ, (G_acos  ) gacos , _1);  // return acos(_1)
defgeneric(OBJ, (G_asin  ) gasin , _1);  // return asin(_1)
defgeneric(OBJ, (G_atan  ) gatan , _1);  // return atan(_1)
defgeneric(OBJ, (G_cosh  ) gcosh , _1);  // return cosh(_1)
defgeneric(OBJ, (G_sinh  ) gsinh , _1);  // return sinh(_1)
defgeneric(OBJ, (G_tanh  ) gtanh , _1);  // return tanh(_1)
defgeneric(OBJ, (G_acosh ) gacosh, _1);  // return acosh(_1)
defgeneric(OBJ, (G_asinh ) gasinh, _1);  // return asinh(_1)
defgeneric(OBJ, (G_atanh ) gatanh, _1);  // return atanh(_1)

defgeneric(OBJ, (G_hypot ) ghypot , _1, _2);     // return sqrt(_1^2+_2^2)
defgeneric(OBJ, (G_sqrErr) gsqrErr, _1, _2);     // return (_1-_2)^2
defgeneric(OBJ, (G_mulAdd) gmulAdd, _1, _2, _3); // return _1*_2+_3

/* NOTE-USER: operators policy
   The policy of these messages is to perform the operation 'in place'
   in _1 without any type promotion/coercion and return _1. For Complex
   results from gabsolute and gargument are returned in the real part.
*/
defgeneric(OBJ, (G_conjugate  ) gconjugate  , _1);  // return _1 = _1^*
defgeneric(OBJ, (G_argument   ) gargument   , _1);  // return _1 = arg(_1)

defgeneric(OBJ, (G_exponential) gexponential, _1);  // return _1 = exp(_1)
defgeneric(OBJ, (G_logarithm  ) glogarithm  , _1);  // return _1 = log(_1)
defgeneric(OBJ, (G_sqroot     ) gsqroot     , _1);  // return _1 = sqrt(_1)

defgeneric(OBJ, (G_cosine     ) gcosine     , _1);  // return _1 = cos(_1)
defgeneric(OBJ, (G_sine       ) gsine       , _1);  // return _1 = sin(_1)
defgeneric(OBJ, (G_tangent    ) gtangent    , _1);  // return _1 = tan(_1)
defgeneric(OBJ, (G_acosine    ) gacosine    , _1);  // return _1 = acos(_1)
defgeneric(OBJ, (G_asine      ) gasine      , _1);  // return _1 = asin(_1)
defgeneric(OBJ, (G_atangent   ) gatangent   , _1);  // return _1 = atan(_1)
defgeneric(OBJ, (G_cosineh    ) gcosineh    , _1);  // return _1 = cosh(_1)
defgeneric(OBJ, (G_sineh      ) gsineh      , _1);  // return _1 = sinh(_1)
defgeneric(OBJ, (G_tangenth   ) gtangenth   , _1);  // return _1 = tanh(_1)
defgeneric(OBJ, (G_acosineh   ) gacosineh   , _1);  // return _1 = acosh(_1)
defgeneric(OBJ, (G_asineh     ) gasineh     , _1);  // return _1 = asinh(_1)
defgeneric(OBJ, (G_atangenth  ) gatangenth  , _1);  // return _1 = atanh(_1)

#endif // COS_GEN_FLOATOP_H
