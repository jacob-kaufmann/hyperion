{
  description = "hyperion";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "aarch64-darwin"
        "x86_64-darwin"
        "aarch64-linux"
        "x86_64-linux"
      ];

      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f nixpkgs.legacyPackages.${system});
    in
    {
      devShells = forAllSystems (
        pkgs:
        let
          llvm = pkgs.llvmPackages;
        in
        {
          default = pkgs.mkShellNoCC {
            packages = [
              llvm.clang-unwrapped
              llvm.lld
              llvm.llvm
              pkgs.just
            ];
          };
        }
      );
    };
}
