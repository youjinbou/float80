#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/misc.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/intext.h>
#include <math.h>
#include <stdio.h>

#if ( defined(__i386__) || defined(__x86_64__) )
#else
#  error "this library expects a x86 compatible platform"
#endif

#define FLOAT80_SIZE 10  /* 10 bytes */

typedef long double float80;

typedef struct {
  struct custom_operations *ops;
  float80 v;  
} float80_s;

#define Float80_val(x) *((float80 *)Data_custom_val(x))

inline int comp(const float80 l, const float80 r){
  return l == r ? 0: (l < r ? -1: 1); 
}

static int float80_compare(value l, value r){
  const float80 rlf = Float80_val(l);
  const float80 rrf = Float80_val(r);
  const int llf = comp(rlf,rrf);
  return llf;
}


CAMLprim value ml_float80_compare(value l, value r){
  value res = Val_int(float80_compare(l,r));
  return res;
}

static intnat float80_hash(value v){
  union {
    struct {
      uint32 u1;
      uint32 u2;
      unsigned short u3;
    };
    float80 v;
  } ld;
  int u3;      
  ld.v = Float80_val(v);
  u3 = ld.u3 | (ld.u3 << 16);
  return (ld.u1 ^ ld.u2) ^ u3;
}

static void float80_serialize(value v, uintnat *s32, uintnat *s64){
  int i;
  char *ld = (char *)&(Float80_val(v));
  for (i = 0; i < FLOAT80_SIZE; i++)
    caml_serialize_int_1(ld[i]);
  return;
}

static uintnat float80_deserialize(void *dst){
  int i;
  char *ld = (char *)dst;
  for (i = 0; i < FLOAT80_SIZE; i++)
    ld[i] = caml_deserialize_uint_1();
  return FLOAT80_SIZE;
}

CAMLexport struct custom_operations float80_ops = {
  "float80", custom_finalize_default, float80_compare, float80_hash,
  float80_serialize, float80_deserialize, custom_compare_ext_default
};

CAMLprim value ml_float80_copy(long double ld){
  value res = caml_alloc_custom(&float80_ops, FLOAT80_SIZE, 0, 1);  
  Float80_val(res) = ld;
  return res;
}

CAMLprim value ml_float80_of_float(value f){
  float80 ld = (float80) Double_val(f);
  value res = ml_float80_copy(ld);
  return res;
}

CAMLprim value ml_float80_to_float(value f){
  float80 ld = (float80) Float80_val(f);
  value v = caml_copy_double(ld);
  return v;
}

CAMLprim value ml_float80_of_int(value v){
  float80 ld = (float80) Int_val(v);
  value res = ml_float80_copy(ld);
  return res;
}

CAMLprim value ml_float80_to_int(value f){
  float80 ld = (float80) Float80_val(f);
  return Int_val((int)ld);
}

CAMLprim value ml_float80_of_bits(value s){
  const char *bits = String_val(s);
  return ml_float80_copy(*(float80 *)bits);
}

CAMLprim value ml_float80_to_bits(value f){
  char *bits = (char *)&(Float80_val(f));
  value res  = caml_alloc_string(FLOAT80_SIZE);
  char *s    = String_val(res);
  int i;
  for (i=0; i < FLOAT80_SIZE; i++)
    s[i] = bits[i];
  return res;
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
  value res = ml_float80_copy(r); 
  return res;
}

#define FLOAT80_UN_OP(OPNAME,OP)                         \
  CAMLprim value ml_float80_##OPNAME(value l){           \
    float80 v    = Float80_val(l);                       \
    float80 fres = OP v;                                 \
    value res = ml_float80_copy(fres);			 \
    return res;                                          \
  }

#define FLOAT80_BIN_OP(OPNAME,OP)			\
  CAMLprim value ml_float80_##OPNAME(value l, value r){ \
    float80 rlf = Float80_val(l);                       \
    float80 rrf = Float80_val(r);                       \
    float80 llf = rlf OP rrf;                           \
    value res = ml_float80_copy(llf);			\
    return res;						\
  }

#define FLOAT80_UN_FUN(FUNNAME,FUN)			\
  CAMLprim value ml_float80_##FUNNAME(value l){         \
    float80 rf = Float80_val(l);                        \
    float80 lf = FUN(rf);				\
    value res = ml_float80_copy(lf);			\
    return res;						\
  }


#define FLOAT80_BIN_FUN(FUNNAME,FUN)                     \
  CAMLprim value ml_float80_##FUNNAME(value l, value r){ \
    float80 rlf = Float80_val(l);                        \
    float80 rrf = Float80_val(r);                        \
    float80 llf = FUN(rlf,rrf);                          \
    value res = ml_float80_copy(llf);			 \
    return res;                                          \
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
  Field(r,0) = ml_float80_copy(rf);
  Field(r,1) = Val_int(ri);
  return r;
}

CAMLprim value ml_float80_ldexp(value vf, value vi){
  float80 f = Float80_val(vf);
  int     i = Int_val(vi);
  float80 rf = ldexpl(f, i);
  value   r = ml_float80_copy(rf);
  return r;
}

CAMLprim value ml_float80_modf(value v){
  float80 f = Float80_val(v);
  value   r = caml_alloc_tuple(2);
  float80 rf2;
  float80 rf1 = modfl(f, &rf2);
  Field(r,0) = ml_float80_copy(rf1);
  Field(r,1) = ml_float80_copy(rf2);
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
  value res = ml_float80_copy(f);		\
  return res;                                   \
  }                                             

UNIT_FUN(infinity,(HUGE_VALL));
UNIT_FUN(neg_infinity,(-HUGE_VALL));
UNIT_FUN(nan,(NAN));
UNIT_FUN(max_val,HUGE_VALL);
UNIT_FUN(min_val,MIN_VALL);
*/
