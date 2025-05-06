{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  name = "badapple";
  nativeBuildInputs = with pkgs; [
    cmake
    opencv
    sfml_2
  ];
}
