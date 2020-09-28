syntax on
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
set scrolloff=10
set encoding=utf-8
" for Coc 
set hidden
set updatetime=100
set shortmess+=c
set signcolumn=number


map W :w<CR>
map Q :q<CR>
map qc :q!<CR>
map qs :wq<CR>
map te :terminal<CR>
nnoremap s <NOP>


inoremap jj <Esc>
vnoremap jj <Esc> 




"all plugs
call plug#begin()

Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'jiangmiao/auto-pairs'
Plug 'rakr/vim-one'
Plug 'dracula/vim', { 'as': 'dracula'  }

call plug#end()

"theme
"colorscheme one
"set background=dark
colorscheme dracula 

"COC-config
inoremap <silent><expr> <TAB>
	\ pumvisible() ? "\<C-n>" :
	\ <SID>check_back_space() ? "\<TAB>" :
	\ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

if exists('*complete_info')
  inoremap <expr> <cr> complete_info()["selected"] != "-1" ? "\<C-y>" : "\<C-g>u\<CR>"
else
  inoremap <expr> <cr> pumvisible() ? "\<C-y>" : "\<C-g>u\<CR>"
endif

" coc-snippets
let g:coc_snippet_next = '<tab>'
