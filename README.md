## Dinosaur T-Rex Game
if give the following error when build wasm:  
`error: unable to create target: 'No available targets are compatible with triple "wasm32"'
`  

you need to install `llvm` from Homebrew. Xcode's clang doesn't have support for WebAssembly.  
`brew install llvm`  

If you need to have llvm first in your PATH, run:  
  `echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.zshrc`  
then source:  
`source ~/.zshrc`