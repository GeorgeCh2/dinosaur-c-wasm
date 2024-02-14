all: game_wasm game_macos

game_wasm: game.c
	clang -O3 --target=wasm32 -I./raylib-5.0_webassembly/include --no-standard-libraries -Wl,--no-entry -Wl,--allow-undefined -Wl,--export=game_init -Wl,--export=game_over -Wl,--export=game_frame -o game.wasm game.c -DPLATFORM_WEB

game_macos: game.c
	clang -I./raylib-5.0_macos/include -o game game.c ./raylib-5.0_macos/lib/libraylib.a -lm -framework CoreFoundation -framework CoreGraphics -framework IOKit -framework AppKit -framework Carbon