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
            echo "Project structure:"
            echo "  src/     - Source code organized by functionality"
            echo "  include/ - Header files"
            echo "  examples/- Example drawings and data files"
            echo ""
            echo "Commands:"
            echo "  make          - Build the application"
            echo "  make debug    - Build with debug info"
            echo "  make clean    - Clean build artifacts"
            echo "  make help     - Show all available targets"
            echo ""
            echo "To run: Switch to TTY (Ctrl+Alt+F2) and run 'sudo ./paint'"
          '';
        };

        packages.default = pkgs.stdenv.mkDerivation {
          pname = "framebuffer-paint";
          version = "1.0.0";
          
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
            description = "Framebuffer-based paint application with geometric drawing tools";
            platforms = platforms.linux;
            license = licenses.mit; # Change as appropriate
          };
        };
      });
}
