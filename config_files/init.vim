set nocompatible
filetype off

let data_dir = has('nvim') ? stdpath('data') . '/site' : '~/.vim'
if empty(glob(data_dir . '/autoload/plug.vim'))
  silent execute '!curl -fLo '.data_dir.'/autoload/plug.vim --create-dirs  https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
  autocmd VimEnter * PlugInstall --sync | source $MYVIMRC
endif

call plug#begin('~/.config/nvim/plugged')
Plug 'preservim/nerdtree'
Plug 'ctrlpvim/ctrlp.vim'
Plug 'nvim-treesitter/nvim-treesitter', {'do': ':TSUpdate'} 
Plug 'jiangmiao/auto-pairs'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'github/copilot.vim'
"" Themes
Plug 'morhetz/gruvbox'
Plug 'NLKNguyen/papercolor-theme'
Plug 'dikiaap/minimalist'
Plug 'ajmwagar/vim-deus'
Plug 'glepnir/oceanic-material'
Plug 'fcpg/vim-farout'
Plug 'kyoz/purify'
Plug 'junegunn/seoul256.vim'
Plug 'rakr/vim-two-firewatch'
Plug 'jacoborus/tender.vim'
Plug 'jaredgorski/spacecamp'
call plug#end()


source $HOME/.config/nvim/themes/gruvbox.vim
"source $HOME/.config/nvim/themes/papercolor-theme.vim
"source $HOME/.config/nvim/themes/minimalist.vim
"source $HOME/.config/nvim/themes/vim-deus.vim
"source $HOME/.config/nvim/themes/oceanic-material.vim
"source $HOME/.config/nvim/themes/vim-farout.vim
"source $HOME/.config/nvim/themes/purify.vim
"source $HOME/.config/nvim/themes/seoul256.vim
"source $HOME/.config/nvim/themes/vim-two-firewatch.vim
"source $HOME/.config/nvim/themes/tender.vim
"source $HOME/.config/nvim/themes/spacecamp.vim


lua require("tree-highlight")
lua require("tree-ics")
lua require("tree-indent")


"let g:airline_theme='simple'
let g:airline_theme = 'tender'
map <silent> <C-n> :NERDTreeFocus<CR>
map <C-y> :w !xclip -sel c <CR><CR>
imap <silent><script><expr> <Alt> <Plug>(copilot-next)
imap <silent><script><expr> <Ctrl> <Plug>(copilot-previous)
set pastetoggle=<F2>



filetype plugin indent on
syntax on
"set colorcolumn=80
"set background=dark
set number
set nowrap
set smartcase
set hlsearch
set noerrorbells
set expandtab
set smartindent
set mouse=nvi
set smarttab
set autoindent
set cursorline
set nobackup
set nowritebackup
set clipboard=unnamedplus
set updatetime=300
set timeoutlen=500
set tabstop=4 softtabstop=4 "noexpandtab
set shiftwidth=4
