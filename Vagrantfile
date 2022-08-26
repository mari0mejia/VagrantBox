# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "archlinux/archlinux"

  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. 
  config.vm.box_check_update = false

  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    vb.gui = true
    # Increase memory from default 1Gig to 2Gig
    vb.memory = 2048
    vb.customize ["modifyvm", :id, "--vram", "24"]
    vb.customize ["modifyvm", :id, "--graphicscontroller", "vmsvga"]
  end

  # Enable provisioning with a shell script. 
  config.vm.provision "shell", inline: <<-SHELL

    awk 'US_entry {print; US_entry=0}
/United States/ {print; US_entry=1}' /etc/pacman.d/mirrorlist >/tmp/mlist.txt
cat /etc/pacman.d/mirrorlist >>/tmp/mlist.txt
cp /tmp/mlist.txt /etc/pacman.d/mirrorlist
#    pacman-key --refresh-keys
#    pacman-key --populate archlinux
    pacman -Syu --noconfirm
    pacman -S --needed --noconfirm vim
    pacman -S --needed --noconfirm pacman-contrib
    pacman -S --needed --noconfirm xorg exo garcon thunar thunar-volman tumbler xfce4-appfinder xfce4-panel xfce4-session xfce4-settings xfce4-terminal xfconf xfdesktop xfwm4 xfce4-goodies
    pacman -S --needed --noconfirm base-devel adobe-source-code-pro-fonts glu freeglut xorg-xeyes
    pacman -S --needed --noconfirm valgrind
    pacman -S --needed --noconfirm gdb bc sharutils git
    pacman -S --needed --noconfirm tree
    pacman -S --needed --noconfirm man-pages
    pacman -S --needed --noconfirm zip unzip
    paccache -r
  
    rm -f /etc/localtime
    ln -s /usr/share/zoneinfo/US/Pacific /etc/localtime
    hwclock --systohc --utc
    echo "csci515" >/etc/hostname
  
    echo "set -o vi" >>/home/vagrant/.bashrc
    echo 'ulimit -c 0' >>/home/vagrant/.bashrc
    echo "export EDITOR=vim" >>/home/vagrant/.bashrc
    echo "PS1='[csci515 \\W]\$ '" >>/home/vagrant/.bashrc
    echo 'PATH=$PATH:$HOME/bin:../../bin' >>/home/vagrant/.bashrc
    if cat /proc/version | grep Microsoft
    then
      dircolors >>~/.bashrc
      sed -i 's/ow=[^:]\+:/ow=01;36;40:/' ~/.bashrc
    fi


cat >/home/vagrant/.vimrc <<"ENDOFVIMRC"
" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

" line numbers
set number

set history=50          " keep 50 lines of command line history
set ruler               " show the cursor position all the time
set showcmd             " display incomplete commands
set incsearch           " do incremental searching
set expandtab           " change tabs to spaces

" CTRL-U in insert mode deletes a lot.  Use CTRL-G u to first break undo,
" so that you can undo CTRL-U after inserting a line break.
inoremap <C-U> <C-G>u<C-U>

" In many terminal emulators the mouse works just fine, thus enable it.
if has('mouse')
  set mouse=a
endif

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
    au!


    " When editing a file, always jump to the last known cursor position.
    " Don't do it when the position is invalid or when inside an event handler
    " (happens when dropping a file on gvim).
    " Also don't do it when the mark is in the first line, that is the default
    " position when opening a file.
    autocmd BufReadPost *
          \\ if line("'\\"") > 1 && line("'\\"") <= line("$") |
          \\   exe "normal! g`\\"" |
          \\ endif

  augroup END

else

  set autoindent                " always set autoindenting on

endif " has("autocmd")

" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
" Only define it when not defined already.
if !exists(":DiffOrig")
  command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
        \\ | wincmd p | diffthis
endif

set shiftwidth=2
filetype indent on

" Additional vim directives added to
" 1. discourage use of arrow keys
" 2. ensure that tabbing is enabled when editing Makefiles
" 3. add sane indenting in C and C++ files
inoremap  <Up>     <NOP>
inoremap  <Down>   <NOP>
inoremap  <Left>   <NOP>
inoremap  <Right>  <NOP>
noremap   <Up>     <NOP>
noremap   <Down>   <NOP>
noremap   <Left>   <NOP>
noremap   <Right>  <NOP>
autocmd FileType make set noexpandtab|set autoindent
autocmd FileType c set tabstop=2|set shiftwidth=2|set expandtab|set autoindent
autocmd FileType cpp set tabstop=2|set shiftwidth=2|set expandtab|set autoindent

"for Tyson's GPL
autocmd BufNewFile,BufRead *.gpl set syntax=cpp|setlocal filetype=cpp
autocmd BufNewFile,BufRead *.y set ai

autocmd Syntax c,cpp setlocal foldmethod=syntax
autocmd Syntax c,cpp normal zR

"set these last (if no filetype then set a filetype)
autocmd BufEnter * if &filetype == "" | setlocal ft=txt | endif
autocmd FileType txt set noexpandtab|set nonumber

syntax enable
set bg=dark
"allow copy/paste across vim sessions
if has('clipboard')
  set clipboard^=unnamed
endif
ENDOFVIMRC
    mkdir /home/vagrant/bin
    chown -R vagrant:vagrant /home/vagrant

  #
  SHELL

  config.vm.provision "shell", inline: <<-ANOTHERSHELL
    cp /usr/lib/systemd/system/getty\@.service /etc/systemd/system/autologin@.service
    sed -i 's|^ExecStart.*$|ExecStart=-/sbin/agetty -a vagrant %I 38400|' /etc/systemd/system/autologin@.service
    systemctl disable getty@.service
    #manually enable it
    systemctl enable autologin@.service

    cat >>~vagrant/.bash_profile <<"ENDOOFBASHPROFILE"
    if [[ -z $DISPLAY ]] && [[ $(tty) == "/dev/tty1" ]]; then
      exec /usr/bin/startxfce4
    else
      export DISPLAY=":0"
    fi
ENDOOFBASHPROFILE

    ################################
    # Prepare vagrant Terminal prefs
    ################################
    mkdir -p ~vagrant/.config/xfce4/terminal
    cat >~vagrant/.config/xfce4/terminal/terminalrc <<"ENDOFRC"
    [Configuration]
    FontName=Adobe Source Code Pro 10
    ColorForeground=#000000
    ColorBackground=#ffffff
ENDOFRC
    ################################
    # Disable screen saver
    ################################
    pacman -Rs --noconfirm xfce4-screensaver 


    chown -R vagrant:vagrant ~vagrant/.config
    systemctl isolate graphical.target
  ANOTHERSHELL

  #this file is located at ecc-linux:/var/www/tagibson/515
  config.vm.provision "shell", path: "http://ecc-linux.csuchico.edu/~tagibson/515/gpl_executable.shar", privileged: false

end


