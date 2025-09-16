# Keen Port Reference Assets and Formats

This folder documents small, dummy formats used by the early bring-up code and tests. They are not original Keen formats.

- ARGB32 image (`TEST.IMG`)
  - [u16 width][u16 height][u32 ARGB pixels, little-endian, width*height entries]
  - Used by `Game::Assets::LoadUserImageARGB32`.

- Paletted image (`TESTPAL.IMG`)
  - [u16 width][u16 height][256 * u32 ARGB palette][u8 indices, width*height]
  - Palette entries are ARGB32, little-endian.
  - Indices are 0..255; index 0 is typically treated as transparent in tests.
  - Used by `Game::Assets::LoadUserImagePAL8` and paletted blits.

- Paletted tileset (`TILES.PAL8`)
  - Same as paletted image; tiles assumed 8x8; code maps a linear tile index to `(sx,sy)` in the sheet.
  - Used by `Game::Resources::LoadTilesPal8` and `DrawTile`.

You can generate these files from tests or small utilities using `FS::File` + `FS::Writer`.

