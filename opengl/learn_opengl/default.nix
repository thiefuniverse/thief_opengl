{ }:

let
  pkgs = import <nixpkgs> { };
in
  pkgs.stdenv.mkDerivation {
    name = "learnopengl";
    src = ./.;
    buildInputs = [ pkgs.glfw3 pkgs.libGL pkgs.xorg.libX11 ];
}
