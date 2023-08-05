{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = [
      pkgs.autoconf
      pkgs.yasm
      pkgs.pkg-config
      pkgs.bazel_5
      pkgs.bazel-buildtools
      ];
}
