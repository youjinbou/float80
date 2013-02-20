type t = string

external format : string -> t -> string = "ml_float80_format"
external of_string : string -> t = "ml_float80_of_string"

let to_string f = 
  format "%.12LG" f

external compare : t -> t -> int = "ml_float80_compare"

external add : t -> t -> t = "ml_float80_add"
external sub : t -> t -> t = "ml_float80_sub"
external mul : t -> t -> t = "ml_float80_mul"
external div : t -> t -> t = "ml_float80_div"

external of_float : float -> t = "ml_float80_of_float"
external to_float : t -> float = "ml_float80_to_float"

external neg : t -> t = "ml_float80_neg"
external pos : t -> t = "%identity" 
  (* positive unary *)

external pow  : t -> t -> t = "ml_float80_pow" 
external sqrt : t -> t = "ml_float80_sqrt"
external exp  : t -> t = "ml_float80_exp"

external log : t -> t = "ml_float80_log"
external log10 : t -> t = "ml_float80_log10"
external expm1 : t -> t = "ml_float80_expm1"
external log1p : t -> t = "ml_float80_log1p"

external cos  : t -> t = "ml_float80_cos"
external sin  : t -> t = "ml_float80_sin"
external tan  : t -> t = "ml_float80_tan"
external acos : t -> t = "ml_float80_acos"
external asin : t -> t = "ml_float80_asin"
external atan : t -> t = "ml_float80_atan"

external atan2 : t -> t -> t = "ml_float80_atan2"
external hypot : t -> t -> t = "ml_float80_hypot"

external cosh : t -> t = "ml_float80_cosh"
external sinh : t -> t = "ml_float80_sinh"
external tanh : t -> t = "ml_float80_tanh"
external acosh : t -> t = "ml_float80_acosh"
external asinh : t -> t = "ml_float80_asinh"
external atanh : t -> t = "ml_float80_atanh"

external ceil : t -> t = "ml_float80_ceil"

external floor : t -> t = "ml_float80_floor"

external abs : t -> t = "ml_float80_abs"

external copysign : t -> t -> t
                  = "ml_float80_copysign"
(** [copysign x y] returns a t whose absolute value is that of [x]
  and whose sign is that of [y].  If [x] is [nan], returns [nan].
  If [y] is [nan], returns either [x] or [-. x], but it is not
  specified which.
  @since 4.00.0  *)

external rem : t -> t -> t = "ml_float80_rem"
(** [mod_t a b] returns the remainder of [a] with respect to
   [b].  The returned value is [a -. n *. b], where [n]
   is the quotient [a /. b] rounded towards zero to an integer. *)

external frexp : t -> t * int = "ml_float80_frexp"

external ldexp : t -> int -> t = "ml_float80_ldexp"

external modf : t -> t * t = "ml_float80_modf"

external of_int : int -> t = "ml_float80_of_int"
external to_int : t -> int = "ml_float80_to_int"

external of_bits : string -> t = "%identity"
external to_bits : t -> string = "%identity"

let min_float = of_bits "\x00\x00\x00\x00\x00\x00\x00\x80\x01\x00"
let max_float = of_bits "\xff\xff\xff\xff\xff\xff\xff\xff\xfe\x7f"
let infinity = of_bits "\x00\x00\x00\x00\x00\x00\x00\x80\xff\x7f"
let neg_infinity = of_bits "\x00\x00\x00\x00\x00\x00\x00\x80\xff\xff"
let epsilon_float = of_bits "\x00\x00\x00\x00\x00\x00\x00\x80\xc0\x3f"
let nan = of_bits "\x00\x00\x00\x00\x00\x00\x00\xc0\xff\x7f"

external classify : t -> fpclass = "ml_float80_classify"
