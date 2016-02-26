set colorcolumn=80
highlight ColorColumn ctermbg=darkgray

syntax enable
set nofoldenable
set encoding=utf-8
set number

filetype indent plugin on
filetype plugin on

" Set filetype to C++ and subtype doxygen to get doxygen syntax highlighting
augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.cpp set filetype=cpp.doxygen
augroup END

" Switch between files in buffer
nnoremap <C-Tab> :bn<CR>
nnoremap <C-S-Tab> :bp<CR>

" Change default fontsize to fit MacBook Pro 13'
set guifont=Monaco:h11
  
" Don't select first Omni-completion option
set completeopt=longest,menuone
"set completeopt=menuone,longest,preview

set backspace=indent,eol,start      " allow backspacing over everything in insert mode
set autoindent                      " always set autoindenting on
set copyindent                      " copy the previous indentation on autoindenting
set tabstop=4
set shiftwidth=4                    " number of spaces to use for autoindenting
set shiftround                      " use multiple of shiftwidth when indenting with '<' and '>'
set incsearch                       " show search matches as you type
set expandtab
set softtabstop=4

" Always set the current file directory as the local current directory
autocmd BufEnter * silent! lcd %:p:h

" Enable folding
nnoremap <silent> <Space> @=(foldlevel('.')?'za':"\<Space>")<CR>
vnoremap <Space> zf

set history=1000         " remember more commands and search history
set undolevels=1000      " use many levels of undo

" Tabs in command line mode behave like bash
set wildmode=longest,list,full
set wildmenu

" Highlight the entire word when searching for it
set hlsearch

"====[ Make tabs, trailing whitespace, and non-breaking spaces visible ]======
exec "set listchars=tab:\uBB\uBB,trail:\uB7,nbsp:~"
set list

" Move line by line even when the line is wrapped
map j gj
map k gk

" Persistent undo
set undofile                " Save undo's after file closes
set undodir=$HOME/.vim/undo " where to save undo histories
set undolevels=1000         " How many undos
set undoreload=10000        " number of lines to save for undo

" YouCompleteMe
let g:ycm_global_ycm_extra_conf = "~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py"
nnoremap <leader>g :YcmCompleter GoToDefinitionElseDeclaration<CR>

" To search in directories to open file which name is under the cursor
let &path.="/usr/local/include/AL,/usr/include/AL,"

