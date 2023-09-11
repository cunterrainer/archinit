set nocompatible
filetype off

call plug#begin('~/.config/nvim/plugged')
Plug 'preservim/nerdtree'
Plug 'ctrlpvim/ctrlp.vim'
Plug 'nvim-treesitter/nvim-treesitter', {'do': ':TSUpdate'}
Plug 'nvim-treesitter/nvim-treesitter-textobjects'
Plug 'jiangmiao/auto-pairs'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'github/copilot.vim'
Plug 'tpope/vim-commentary'
Plug 'tpope/vim-surround'
Plug 'tpope/vim-repeat'
Plug 'glts/vim-magnum'
Plug 'glts/vim-radical'
Plug 'terryma/vim-smooth-scroll'
" Plug 'frazrepo/vim-rainbow' " causes lag
Plug 'ryanoasis/vim-devicons'
Plug 'preservim/tagbar'
Plug 'ludovicchabant/vim-gutentags'
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
let g:rainbow_active = 1
let g:gutentags_generate_on_new = 1
let g:gutentags_generate_on_missing = 1
let g:gutentags_generate_on_write = 1
let g:gutentags_generate_on_empty_buffer = 0
let g:gutentags_add_default_project_roots = 0
let g:gutentags_project_root = ['package.json', '.git']
let g:gutentags_cache_dir = expand('~/.cache/vim/ctags/')
let g:gutentags_ctags_extra_args = ['--tag-relative=yes', '--fields=+ailmnS']
nmap <F8> :TagbarToggle<CR>


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

"Auto completion on tab press
inoremap <silent><expr> <Tab> coc#pum#visible() ? coc#_select_confirm() : "\<C-g>u\<Tab>"
" Start NERDTree and put the cursor back in the other window.
autocmd VimEnter * NERDTree | wincmd p
" Close the tab if NERDTree is the only window remaining in it.
autocmd BufEnter * if winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif
" Exit Vim if NERDTree is the only window remaining in the only tab.
autocmd BufEnter * if tabpagenr('$') == 1 && winnr('$') == 1 && exists('b:NERDTree') && b:NERDTree.isTabTree() | quit | endif
" Open the existing NERDTree on each new tab.
autocmd BufWinEnter * if &buftype != 'quickfix' && getcmdwintype() == '' | silent NERDTreeMirror | endif

noremap <silent> <c-u> :call smooth_scroll#up(&scroll, 0, 1)<CR>
noremap <silent> <c-d> :call smooth_scroll#down(&scroll, 0, 1)<CR>
noremap <silent> <c-b> :call smooth_scroll#up(&scroll*2, 10, 4)<CR>
noremap <silent> <c-f> :call smooth_scroll#down(&scroll*2, 10, 4)<CR>
