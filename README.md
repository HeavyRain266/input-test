# Simple input device test

## Building
First install meson and ninja/samurai.

Generate needed ninja files.
```
$ meson build
```
Now build with ninja/samurai.
```
samu -C build
```

## Running
Now you are ready to test input methods.
```
$ ./build/input-test
```

When program is running, try moving cursor around or click any button on mouse/keyboard.  If devices are connected, program should output cursor position and character from keyboard buttons, only letters and numbers are displayed correctly at the moment
