#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/misc.h>
#include <caml/memory.h>
//#include <caml/custom.h>
#include <math.h>
#include <stdio.h>

#define FLOAT80_SIZE 10  /* 10 bytes */

typedef long double float80;

#define Float80_val(x) *((float80 *)String_val(x))

void float80_copy_str(char *r, const char *l){
  int i;
  for (i=0;i<FLOAT80_SIZE;i++)
    r[i] = l[i];
}

void store_float80_val(value v,float80 f){
  float80_copy_str(String_val(v), (const char *)&f);
}

CAMLprim value ml_float80_copy(value r, value l){
  float80_copy_str(String_val(r),String_val(l));
  return Val_unit;
}

static value alloc_float80(){
  return caml_alloc_string(FLOAT80_SIZE);
}

static value float80_copy(float80 v){
  const char *l = (const char *)&v;
  value res = alloc_float80();
  char *r = String_val(res);
  float80_copy_str(r,l);
  return res;
}

CAMLprim value ml_float80_of_float(value f){
  float80 ld = (float80) Double_val(f);
  value res = float80_copy(ld);
  return res;
}

CAMLprim value ml_float80_to_float(value f){
  float80 ld = (float80) Float80_val(f);
  value v = caml_copy_double(ld);
  return v;
}

CAMLprim value ml_float80_of_int(value v){
  float80 ld = (float80) Int_val(v);
  value res = float80_copy(ld);
  return res;
}

CAMLprim value ml_float80_to_int(value f){
  float80 ld = (float80) Float80_val(f);
  return Int_val((int)ld);
}


#ifdef WIN32
#define HAS_BROKEN_PRINTF
#endif

/* original code taken from ocaml ver. 4.00.1
   license GPL 
*/

CAMLprim value ml_float80_format(value fmt, value arg){
#define MAX_DIGITS 350
  /* Max number of decimal digits in a "natural" (not artificially padded)
     representation of a float. Can be quite big for %f format.
     Max exponent for IEEE format is 308 decimal digits.
     Rounded up for good measure. */
  char format_buffer[MAX_DIGITS + 20];
  int prec, i;
  char * p;
  char * dest;
  value res;
  float80 d = Float80_val(arg);

#ifdef HAS_BROKEN_PRINTF
  if (isfinite(d)) {
#endif
    prec = MAX_DIGITS;
    for (p = String_val(fmt); *p != 0; p++) {
      if (*p >= '0' && *p <= '9') {
        i = atoi(p) + MAX_DIGITS;
        if (i > prec) prec = i;
        break;
      }
    }
    for( ; *p != 0; p++) {
      if (*p == '.') {
        i = atoi(p+1) + MAX_DIGITS;
        if (i > prec) prec = i;
        break;
      }
    }
    if (prec < sizeof(format_buffer)) {
      dest = format_buffer;
    } else {
      dest = caml_stat_alloc(prec);
    }
    sprintf(dest, String_val(fmt), d);
    res = caml_copy_string(dest);
    if (dest != format_buffer) {
      caml_stat_free(dest);
    }
#ifdef HAS_BROKEN_PRINTF
  } else {
    if (isnan(d)) {
        res = caml_copy_string("nan");
    } else {
      if (d > 0) {
        res = caml_copy_string("inf");
      } else {
        res = caml_copy_string("-inf");
      }
    }
  }
#endif
  return res;
}


CAMLprim value ml_float80_of_string(value v){
  char *str = String_val(v);
  char *end = str + caml_string_length(v);
  float80 r = strtold(str, &end);
  value res = alloc_float80(); 
  store_float80_val(res, r);
  return res;
}

#define FLOAT80_UN_OP(OPNAME,OP)                         \
  CAMLprim value ml_float80_##OPNAME(value l){           \
    float80 v    = Float80_val(l);                       \
    float80 fres = OP v;                                 \
    value res = float80_copy(fres);                      \
    return res;                                          \
  }

#define FLOAT80_BIN_OP(OPNAME,OP)			\
  CAMLprim value ml_float80_##OPNAME(value l, value r){ \
    float80 rlf = Float80_val(l);                       \
    float80 rrf = Float80_val(r);                       \
    float80 llf = rlf OP rrf;                           \
    value res = float80_copy(llf);			\
    return res;						\
  }

#define FLOAT80_UN_FUN(FUNNAME,FUN)			\
  CAMLprim value ml_float80_##FUNNAME(value l){         \
    float80 rf = Float80_val(l);                        \
    float80 lf = FUN(rf);				\
    value res = float80_copy(lf);                       \
    return res;						\
  }


#define FLOAT80_BIN_FUN(FUNNAME,FUN)                     \
  CAMLprim value ml_float80_##FUNNAME(value l, value r){ \
    float80 rlf = Float80_val(l);                        \
    float80 rrf = Float80_val(r);                        \
    float80 llf = FUN(rlf,rrf);                          \
    value res = float80_copy(llf);                       \
    return res;                                          \
  }

inline int comp(const float80 l, const float80 r){
  return l == r ? 0: (l < r ? -1: 1); 
}


CAMLprim value ml_float80_compare(value l, value r){
  const float80 rlf = Float80_val(l);
  const float80 rrf = Float80_val(r);
  const int llf = comp(rlf,rrf);
  value res = Val_int(llf);
  return res;
}


FLOAT80_BIN_OP(add,+);
FLOAT80_BIN_OP(sub,-);
FLOAT80_BIN_OP(mul,*);
FLOAT80_BIN_OP(div,/);


FLOAT80_UN_OP(neg,-);
//FLOAT80_UN_OP(pos,+);

FLOAT80_BIN_FUN(pow,powl);
FLOAT80_UN_FUN(sqrt,sqrtl);
FLOAT80_UN_FUN(exp,expl);

FLOAT80_UN_FUN(log,logl);
FLOAT80_UN_FUN(log10,log10l);
FLOAT80_UN_FUN(expm1,expm1l);
FLOAT80_UN_FUN(log1p,log1pl);

FLOAT80_UN_FUN(cos,cosl);
FLOAT80_UN_FUN(sin,sinl);
FLOAT80_UN_FUN(tan,tanl);
FLOAT80_UN_FUN(acos,acosl);
FLOAT80_UN_FUN(asin,asinl);
FLOAT80_UN_FUN(atan,atanl);

FLOAT80_BIN_FUN(atan2,atan2l);
FLOAT80_BIN_FUN(hypot,hypotl);

FLOAT80_UN_FUN(cosh,coshl);
FLOAT80_UN_FUN(sinh,sinhl);
FLOAT80_UN_FUN(tanh,tanhl);
FLOAT80_UN_FUN(acosh,acoshl);
FLOAT80_UN_FUN(asinh,asinhl);
FLOAT80_UN_FUN(atanh,atanhl);

FLOAT80_UN_FUN(ceil,ceill);
FLOAT80_UN_FUN(floor,floorl);

float80 absl(float80 v){
  if (v < 0.0)
    return -v;
  return v;
}

FLOAT80_UN_FUN(abs,absl);
FLOAT80_BIN_FUN(copysign,copysignl);

FLOAT80_BIN_FUN(rem,remainderl);

CAMLprim value ml_float80_frexp(value v){
  float80 f = Float80_val(v);
  value r = caml_alloc_tuple(2);
  int ri;
  float80 rf = frexpl(f, &ri);
  Field(r,0) = alloc_float80();
  Field(r,1) = Val_int(ri);
  store_float80_val(Field(r,0), rf);
  return r;
}

CAMLprim value ml_float80_ldexp(value vf, value vi){
  float80 f = Float80_val(vf);
  int     i = Int_val(vi);
  float80 rf = ldexpl(f, i);
  value   r = float80_copy(rf);
  return r;
}

CAMLprim value ml_float80_modf(value v){
  float80 f = Float80_val(v);
  value   r = caml_alloc_tuple(2);
  float80 rf2;
  float80 rf1 = modfl(f, &rf2);
  Field(r,0) = alloc_float80();
  Field(r,1) = alloc_float80();
  store_float80_val(Field(r,0), rf1);
  store_float80_val(Field(r,1), rf2);
  return r;
}

enum {
  ml_FP_NORMAL = 0,
  ml_FP_SUBNORMAL,
  ml_FP_ZERO,
  ml_FP_INFINITE,
  ml_FP_NAN,
};

CAMLprim value ml_float80_classify(value v){
  float80 f = Float80_val(v);
  switch (fpclassify(f)){
  case FP_NORMAL    : return Val_int(ml_FP_NORMAL);
  case FP_SUBNORMAL : return Val_int(ml_FP_SUBNORMAL);
  case FP_ZERO      : return Val_int(ml_FP_ZERO);
  case FP_INFINITE  : return Val_int(ml_FP_INFINITE);
  case FP_NAN       : 
  default: return Val_int(ml_FP_NAN);
  }
}
/*

#define UNIT_FUN(NAME,VAL)                      \
CAMLprim value ml_float80_##NAME(value unit){   \
  float80 f = VAL;                              \
  value res = float80_copy(f);                  \
  return res;                                   \
  }                                             

UNIT_FUN(infinity,(HUGE_VALL));
UNIT_FUN(neg_infinity,(-HUGE_VALL));
UNIT_FUN(nan,(NAN));
UNIT_FUN(max_val,HUGE_VALL);
UNIT_FUN(min_val,MIN_VALL);
*/
