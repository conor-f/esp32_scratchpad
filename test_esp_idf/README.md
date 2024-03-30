## About
- This is a sample to get up and running with ESP32 without needing to use
    horrendous Arduino "IDE"
- Based on [esp-idf](https://docs.espressif.com/projects/esp-idf)
- Always have to run `get_idf` to source the env for `esp-idf`
    - This makes wrapping things in a script a little more awkward...

## Usage
- `get_idf` - Add the `esp-idf` tools to your session
- `idf.py set-target esp32` - Only used during setup (I think?)
- `idf.py menuconfig` - Set details like WiFi details etc.
- `idf.py flash` - Build binaries and flash them onto the device
- `idf.py monitor` - Watch the serial interface
    - Use `Ctrl + ]` to exit
    - I _think_ it uses 115,200 baud by default

## Todo
- Would be really nice to wrap this fully in a Makefile
- Or to find a more streamlined system that doesn't require sourcing the tools
