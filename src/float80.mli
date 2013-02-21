type t

val format : string -> t -> string
val of_string : string -> t
val to_string : t -> string
val compare : t -> t -> int
val add : t -> t -> t
val sub : t -> t -> t
val mul : t -> t -> t
val div : t -> t -> t
val of_float : float -> t
val to_float : t -> float
val neg : t -> t
val pos : t -> t
val pow : t -> t -> t
val sqrt : t -> t
val exp : t -> t
val log : t -> t
val log10 : t -> t
val expm1 : t -> t
val log1p : t -> t
val cos : t -> t
val sin : t -> t
val tan : t -> t
val acos : t -> t
val asin : t -> t
val atan : t -> t
val atan2 : t -> t -> t
val hypot : t -> t -> t
val cosh : t -> t
val sinh : t -> t
val tanh : t -> t
val acosh : t -> t
val asinh : t -> t
val atanh : t -> t
val ceil : t -> t
val floor : t -> t
val abs : t -> t
val copysign : t -> t -> t
val rem : t -> t -> t
val frexp : t -> t * int
val ldexp : t -> int -> t
val modf : t -> t * t
val of_int : int -> t
val to_int : t -> int
val of_bits : string -> t
val to_bits : t -> string
val min_float : t
val max_float : t
val infinity : t
val neg_infinity : t
val epsilon : t
val nan : t
val classify : t -> fpclass
