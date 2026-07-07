qemu-system-i386 \
    -drive file=build/klaudOSv0.0.5.img,format=raw,if=floppy \
    -boot a \
    -display sdl
