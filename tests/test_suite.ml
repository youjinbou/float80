open OUnit;;
module F = Float80;;
module P = Printf;;

let fequal a b = 
  F.compare a b = 0


let consts = [
  "infinity", F.infinity, FP_infinite;
  "neg infinity", F.neg_infinity, FP_infinite;
(*  "epsilon", F.add (F.of_int 0) F.epsilon_float, FP_subnormal; *)
  "nan", F.nan, FP_nan;
];;

let test_const v c =
  assert_equal (F.classify v) c

let test_consts = List.map (fun (l,v,c) -> l >:: fun () -> test_const v c) consts


let test_epsilon () =
  assert_equal ~cmp:fequal (F.add (F.of_int 0) F.epsilon_float) F.epsilon_float


let test_bin op f1 f2 res = 
  let f1 = F.of_string f1
  and f2 = F.of_string f2 
  and f3 = F.of_string res in
  let f4 = op f1 f2 in
  assert_equal ~cmp:fequal f3 f4


let bin_ops = [
  "add", F.add, "1", "2", "3";
  "mul", F.mul, "2", "5", "10";
  "sub", F.sub, "6", "7", "-1";
  "div", F.div, "5", "2", "2.5";
  "pow", F.pow, "4", "3", "64"
];;


let test_bins = List.map (fun (l,o,v1,v2,r) -> l >:: fun () -> test_bin o v1 v2 r) bin_ops
;;


let suite = 
  "suite">::: [
    "constants"  >::: test_consts;
    "binary ops" >::: test_bins;
    "epsilon"    >:: test_epsilon;
  ]
;;

let _ = 
  run_test_tt_main suite
;;

