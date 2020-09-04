{ }:

let
  pkgs = import <nixpkgs> { };
in
  pkgs.stdenv.mkDerivation {
    name = "ProjectA-1.0.0";
    src = ./.;
   buildInputs = [ pkgs.glfw3 pkgs.libGL pkgs.xorg.libX11 ];
}
