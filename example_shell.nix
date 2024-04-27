{ pkgs ? import <nixpkgs> {} }:
(pkgs.buildFHSUserEnv {
  name = "pipzone";
  targetPkgs = pkgs: (with pkgs; [
    git
    nodejs_21
    corepack_21
    python3
    python3Packages.pip
    python3Packages.virtualenv
    ffmpeg
  ]);
  runScript = "bash";
}).env
