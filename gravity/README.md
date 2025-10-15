# Gravity Strike

Gravity Strike is a classic arcade-style shooter game originally developed by Jens Hassler. This repository contains the complete source code to build and run the game on modern Linux systems.

## Prerequisites

To compile Gravity Strike, you need the following libraries installed:

- Allegro 4.x (graphics and game library)
- AllegTTF 2.0 (TrueType font support)
- Allegro 5 audio codecs (MOD music playback)
- Lua 5 (scripting support)
- tolua 5 (Lua binding generator)
- Libnet (networking functions)

### Installing Dependencies

On Ubuntu/Debian systems:
```bash
sudo apt-get install build-essential liballegro4-dev liballegro-ttf-dev \
    liblua5.3-dev libtolua++5.1-dev liballegro5-dev liballegro-acodec5-dev \
    libnet1-dev
```

On Fedora/RHEL systems:
```bash
sudo dnf install gcc gcc-c++ allegro-devel allegro-ttf-devel \
    lua-devel tolua++-devel allegro5-devel libnet-devel
```

On Gentoo systems:
```bash
# Install allegro with legacy support
emerge -av media-libs/allegro media-libs/allegro-ttf \
    dev-lang/lua dev-lua/tolua++ \
    media-libs/allegro dev-libs/libnet
```

Note: Gravity Strike requires Allegro 4.x, but many modern systems only have Allegro 5.x installed. 
If your system only has Allegro 5, you may need to install Allegro 4 compatibility libraries or 
compile Allegro 4 from source.

## Building

There are multiple ways to build the game:

### Method 1: Using the top-level Makefile (Recommended)

```bash
make all
```

### Method 2: Using autotools (For advanced users)

First, initialize the autotools build system:

```bash
./autogen.sh
./configure
make
```

### Method 3: Using the legacy Makefile

```bash
cd src
make
```

## Running

After building, run the game with:

```bash
./gslnx
```

For best performance when running as a normal user, you can set the proper permissions:

```bash
chmod 04755 gslnx
```

## Installing System-Wide

To install the game system-wide:

```bash
sudo make install
```

This will install the executable to `/usr/local/bin` and the data files to `/usr/local/share/gravity`.

To uninstall:

```bash
sudo make uninstall
```

## Controls

- Arrow keys: Move your ship
- Ctrl: Fire primary weapon
- Alt: Fire secondary weapon
- Shift: Accelerate
- F1: Help
- F2: Sound settings
- F3: Graphics settings
- F4: Gameplay settings
- ESC: Pause/Menu

## History

This game was originally released as open-source by Jens Hassler in 2002. It was designed to work under DOS (DJGPP), Windows (Visual C++), and Linux (gcc/g++).

The highscore functionality is disabled in this version to prevent cheating on online score tables.

## Authors

- Original author: Jens Hassler
- Modern build system: Created for compatibility with modern Linux systems