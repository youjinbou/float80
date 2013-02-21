open OUnit;;
module F = Float80;;
module P = Printf;;

let fequal a b = 
  F.compare a b = 0


let consts = [
  "infinity", F.infinity, FP_infinite;
  "neg infinity", F.neg_infinity, FP_infinite;
  "epsilon", F.epsilon, FP_normal;
  "nan", F.nan, FP_nan;
];;

let test_const v c =
  assert_equal (F.classify v) c

let test_consts = List.map (fun (l,v,c) -> l >:: fun () -> test_const v c) consts

let test_epsilon () =
  assert_equal ~cmp:fequal (F.add (F.of_int 0) F.epsilon) F.epsilon


let test_una op f1 res =
  let f1 = F.of_string f1
  and fr = F.of_string res in
  let ft = op f1 in
  assert_equal ~cmp:fequal fr ft

let una_ops = [
  "neg",  F.neg,  "1", "-1";
  "pos",  F.pos,  "2",  "2";
  "sqrt", F.sqrt, "9",  "3";
  "cos",  F.cos,  "0",  "1";
  "sin",  F.sin,  "0",  "0";
]


let test_bin op f1 f2 res = 
  let f1 = F.of_string f1
  and f2 = F.of_string f2 
  and fr = F.of_string res in
  let ft = op f1 f2 in
  assert_equal ~cmp:fequal fr ft


let bin_ops = [
  "add", F.add, "1", "2", "3";
  "mul", F.mul, "2", "5", "10";
  "sub", F.sub, "6", "7", "-1";
  "div", F.div, "5", "2", "2.5";
  "pow", F.pow, "4", "3", "64"
];;


let test_bins = List.map (fun (l,o,v1,v2,r) -> l >:: fun () -> test_bin o v1 v2 r) bin_ops
;;

let test_unas = List.map (fun (l,o,v1,r) -> l >:: fun () -> test_una o v1 r) una_ops
;;

let suite = 
  "suite">::: [
    "constants"  >::: test_consts;
    "binary ops" >::: test_bins;
    "unary ops" >::: test_unas;
    "epsilon"    >:: test_epsilon;
  ]
;;

let _ = 
  run_test_tt_main suite
;;

