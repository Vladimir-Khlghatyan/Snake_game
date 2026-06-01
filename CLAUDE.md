# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Platform requirement

This is a POSIX terminal program. It depends on `<termios.h>`, `<unistd.h>`, and `<fcntl.h>`, so it **only builds and runs on Linux/macOS (or WSL)** — it will not compile natively on Windows. There is no compiler-portable abstraction layer.

## Commands

- `make` — build the `SnakeGame` executable (objects are emitted next to sources in `srcs/`).
- `make clean` — remove `.o` files.
- `make fclean` — remove `.o` files and the `SnakeGame` binary.
- `make re` — full rebuild (`fclean` + build).
- `./SnakeGame` — run the game. Arrow keys steer; ESC quits.

Compiler is `c++` with `-std=c++2a -I./includes`. There is no test suite, linter, or CI.

## Architecture

The entire game is one class, `Snake`, declared in [includes/Snake.hpp](includes/Snake.hpp) and implemented in [srcs/Snake.cpp](srcs/Snake.cpp). [srcs/main.cpp](srcs/main.cpp) just constructs `Snake(20, 40)` (height, width) and calls `play()` then `gameOver()`.

The game loop lives entirely in `Snake::play()`: read non-blocking stdin → sleep → update direction → `updateBoard()` → clear and reprint. The terminal is put into raw, non-blocking, no-echo mode in `initNecessaryLibraries()` and restored in the destructor (and in `gameOver()`).

Key state and the invariants that tie it together:

- **`_board`** (`vector<vector<string>>`) is the source of truth for rendering. Cells hold a space, the `SNAKE` glyph, or the `FROG` glyph (ANSI-colored strings, not chars).
- **`_order`** (`queue<pii>`) records snake body cells in insertion order. Each move pushes the new head; when no frog is eaten, the front (tail) is popped and blanked. This is how the body advances.
- **`_freeIndexes`** (`set<pii>`) is the pool of empty cells used to place a new frog. **This set must be kept in lockstep with the board**: erase a cell when it becomes occupied (new head, new frog) and insert it when it is vacated (tail moves off). `getRandomPoint()` picks from and erases out of this set. If this set drifts out of sync, frogs spawn on top of the snake — a recurring class of bug here.
- **`_usedIndexes`** is currently written but never read (effectively dead).

Other conventions worth knowing before editing:

- **Directions** are ints: `1` right, `2` down, `3` left, `4` up. `updateDirection()` maps raw key codes (`65`–`68`, the final byte of arrow-key escape sequences `ESC [ A/B/C/D`) to these and blocks 180° reversals.
- **Movement speed is direction-dependent.** Because the board is twice as wide as tall, vertical moves sleep twice as long (`_speed * ((_direction % 2) ? 1 : 2)`) so motion looks uniform. `_speed` decreases by 10ms per frog eaten, floored at 50ms.
- **Wall hits use a grace mechanic**, not instant death: `_currentChance` (reset from `_chance`) absorbs a couple of wall collisions before game over; body collisions are always fatal.
- **Game-over exits hard.** `updateBoard()` calls `gameOver()` then `exit(0)` directly on a fatal collision, bypassing normal unwinding. `gameOverBoard()` overwrites `_board` with ASCII-art and the final score.
- Rendering reprints the whole board every frame via `clearTerminal()` (cursor-up + erase-line escapes) followed by `printBoard()`.

## Repo note

`*.o` is gitignored but the built `SnakeGame` binary is tracked in the repo (the `.gitignore` entry is `Game`, not the actual binary name). Avoid committing rebuilt artifacts.
