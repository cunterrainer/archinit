import os

packages = [
        "coc-als", # arduino, camel, missing
        "coc-sh", # Bitbake missing
        "coc-clangd",
        "coc-cmake", # common lisp missing
        "coc-clojure", # crystalline missing
        "coc-css",
        "coc-flutter",
        "coc-dlang",
        "coc-deno", # dhall, dockerfile, docker compose, elixir, elvish, erlang, elm, flow, fortran, fsharp, gleam missing
        "coc-go", # godot, graphql, groovy, haskell missing
        "coc-haxe",
        "coc-html",
        "coc-java",
        "coc-tsserver",
        "coc-json",
        "coc-julia", # kotlin, latex missing
        "coc-texlab", # lua missing
        "coc-markdownlint", # nim, nix, ocaml, missing
        "coc-perl", # php, purescript missing
        "coc-pyright",
        "coc-r-lsp", # robot framework, racket missing
        "coc-rome",
        "coc-solargraph",
        "coc-rust-analyzer", # sql, scala missing
        "coc-solidity", # system verilog, systemd, terraform, typst, V, Vala missing
        "coc-vetur", # yang missing
        "coc-zig" # vim/erb missing
        ]

cmd = "nvim -c \"TSInstall all\" -c \"CocInstall "

args = 1
for p in packages:
    args += 1
    cmd += p + " "
    if args % 10 == 0 or args == len(packages)+1: # nvim doesn't allow more than 10 arguments
        cmd += "\""
        print(cmd)
        os.system(cmd)
        cmd = "nvim -c \"CocInstall "
