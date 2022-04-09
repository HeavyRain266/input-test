# Simple keyboard input test for Linux

## Building
Dependencies:
 - meson
 - ninja/samurai
 - mold linker (only if `CC=clang`)

Generate needed ninja files.
```
$ meson build
```
Now build with ninja/samurai.
```
$ ninja -C build
```

## Running
Now you are ready to test input methods.
```
$ ./build/input-test
```

When program is running, click any button on mouse/keyboard.  If device is connected, program should output character from keyboard buttons, only letters and numbers are displayed correctly at the moment.
