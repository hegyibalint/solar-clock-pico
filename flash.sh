#!/usr/bin/env fish

# Switch to the directory of the script
cd (status dirname)
ninja -C build && JLinkExe -device RP2040_M0_0 -if SWD -speed 4000 -autoconnect 1 -CommanderScript flash.jlink