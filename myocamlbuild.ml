open Ocamlbuild_plugin;;
open Command;;

dispatch begin function
| After_rules -> 
  begin


    flag ["ocaml";"byte";"link";"use_float80"]
      (S [A "-dllib"; A "-lfloat80_stubs"]);

    flag ["ocaml"; "native"; "link";"use_float80"]
      (S [A "-cclib"; A "-lfloat80_stubs"]);
      

    ocaml_lib ~extern:true ~dir:"+oUnit" "oUnit";
    ocaml_lib ~extern:true ~dir:"src" "float80";

  end
| _ -> ()
end;;
