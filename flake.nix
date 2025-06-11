{
  description = "Framebuffer Paint Application";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            gnumake
            gdb
            # Framebuffer support
            linuxPackages.kernel
            kmod
          ];
          
          shellHook = ''
            echo "=== Framebuffer Paint Development Environment ==="
            echo "To build: make"
            echo "To run: sudo ./paint (requires framebuffer access)"
            echo ""
            echo "Make sure to run from TTY (Ctrl+Alt+F2) for framebuffer access"
            echo "Or enable framebuffer with: sudo modprobe vfb"
          '';
        };

        packages.default = pkgs.stdenv.mkDerivation {
          pname = "paint";
          version = "0.1.0";
          
          src = ./.;
          
          buildInputs = with pkgs; [ gcc gnumake ];
          
          buildPhase = ''
            make
          '';
          
          installPhase = ''
            mkdir -p $out/bin
            cp paint $out/bin/
          '';
          
          meta = with pkgs.lib; {
            description = "Framebuffer-based paint application";
            platforms = platforms.linux;
          };
        };
      });
}