set number
set tabstop=4
set softtabstop=4
set shiftwidth=4
set smartindent
set showcmd
set wrap
set wildmenu
set incsearch
set hlsearch


map W :w<CR>
map s <nop>
map Q :q<CR>
map qc :q!<CR>
inoremap jj <ESC>

map tt :NERDTreeToggle<CR>

call plug#begin('~/.vim/plugged')

Plug 'w0rp/ale'
Plug 'jiangmiao/auto-pairs'
Plug 'scrooloose/nerdtree'
Plug 'ervandew/supertab'

call plug#end()
