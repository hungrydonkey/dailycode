set number
set tabstop=4
set autoindent
set softtabstop=4
set shiftwidth=4
set showcmd
set wrap
set wildmenu
set incsearch
set hlsearch
set scrolloff=10
map W :w<CR>
map Q :q<CR>
map qc :q!<CR>
map qs :wq<CR>
nnoremap s <NOP>


map tt :NERDTreeToggle<CR>
inoremap jj <Esc>`^


let g:indentLine_char = '┊'
let g:ale_lint_on_text_changed = 'always'



call plug#begin()

Plug 'w0rp/ale'
Plug 'vim-airline/vim-airline'
Plug 'jiangmiao/auto-pairs'
Plug 'scrooloose/nerdtree'
Plug 'yggdroot/indentline'


"NCM2
Plug 'ncm2/ncm2'
Plug 'roxma/nvim-yarp'


autocmd BufEnter * call ncm2#enable_for_buffer()
set completeopt=noinsert,menuone,noselect

"ncm2 config

inoremap <expr> <CR> (pumvisible() ? "\<c-y>\<cr>" : "\<CR>")
inoremap <expr> <Tab> pumvisible() ? "\<C-n>" : "\<Tab>"
inoremap <expr> <S-Tab> pumvisible() ? "\<C-p>" : "\<S-Tab>"
inoremap <silent> <expr> <CR> ncm2_neosnippet#expand_or("\<CR>", 'n')

imap <expr><TAB>  pumvisible() ? "\<C-n>" : neosnippet#expandable_or_jumpable() ? "\<Plug>(neosnippet_expand_or_jump)" : "\<TAB>"

if has('conceal')
  set conceallevel=2 concealcursor=niv
endif

"pyclang
let g:ncm2_pyclang#library_path         =  '/usr/lib/libclang.so.8'
"highlight
let g:ncm2#match_highlight = 'bold'
let g:neosnippet#enable_snipmate_compatibility = 1
let g:neosnippet#snippets_directory='~/.config/nvim/plugged/vim-snippets/snippets'
let g:neosnippet#disable_runtime_snippets = {
		\   '_' : 1,
		\ }
"let g:ncm2_jedi#python_version = 3



Plug 'ncm2/ncm2-bufword'
Plug 'ncm2/ncm2-path'
"Plug 'ncm2/ncm2-ultisnips'
"Plug 'SirVer/ultisnips'
Plug 'Shougo/neosnippet.vim'
Plug 'ncm2/ncm2-neosnippet'
Plug 'ncm2/ncm2-pyclang'
Plug 'ncm2/ncm2-jedi'
Plug 'ncm2/ncm2-match-highlight'

call plug#end()
