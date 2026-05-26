# PGM Image Processor

A command-line image processing toolkit written in C for working with Portable Graymap (`.pgm`) images.

The project supports reading, writing, comparing, converting, reducing, tiling, and assembling ASCII (`P2`) and binary (`P5`) PGM files. It is structured as a small systems-programming project with reusable PGM parsing utilities and separate command-line programs for each operation.

## Features

- Read and validate ASCII (`P2`) and binary (`P5`) PGM files
- Echo/copy PGM images
- Compare two PGM images pixel-by-pixel
- Convert ASCII PGM to binary PGM
- Convert binary PGM to ASCII PGM
- Reduce image dimensions by an integer factor
- Split an image into tiled sub-images
- Assemble multiple PGM tiles into a larger image

## Repository Structure

```text
.
├── include/
│   └── pgm.h
├── src/
│   ├── pgm.c
│   ├── pgmEcho.c
│   ├── pgmComp.c
│   ├── pgma2b.c
│   ├── pgmb2a.c
│   ├── pgmReduce.c
│   ├── pgmTile.c
│   └── pgmAssemble.c
├── tests/
│   ├── fixtures/
│   └── run_tests.sh
├── docs/
│   ├── modules.md
│   └── test-plan.txt
├── Makefile
└── README.md
```

## Build

You need `gcc` and `make`.

```bash
make
```

This creates executables inside `bin/`:

```text
bin/pgmEcho
bin/pgmComp
bin/pgma2b
bin/pgmb2a
bin/pgmReduce
bin/pgmTile
bin/pgmAssemble
```

Clean generated files:

```bash
make clean
```

## Usage

### Echo/copy an image

```bash
bin/pgmEcho input.pgm output.pgm
```

### Compare two images

```bash
bin/pgmComp image1.pgm image2.pgm
```

Output:

```text
IDENTICAL
```

or:

```text
DIFFERENT
```

### Convert ASCII PGM to binary PGM

```bash
bin/pgma2b input_ascii.pgm output_binary.pgm
```

### Convert binary PGM to ASCII PGM

```bash
bin/pgmb2a input_binary.pgm output_ascii.pgm
```

### Reduce an image

```bash
bin/pgmReduce input.pgm 2 output.pgm
```

### Tile an image

The output template must contain `<row>` and `<column>` placeholders.

```bash
bin/pgmTile input.pgm 2 "tile_<row>_<column>.pgm"
```

### Assemble tiles

```bash
bin/pgmAssemble output.pgm width height row column input.pgm [row column input.pgm ...]
```

Example:

```bash
bin/pgmAssemble assembled.pgm 4 4 0 0 tile_0_0.pgm 0 1 tile_0_1.pgm 1 0 tile_1_0.pgm 1 1 tile_1_1.pgm
```

## Testing

Run the basic test script:

```bash
make test
```

The tests build the project, run the tools on small fixture images, and verify core behaviour such as echoing, comparison, conversion, reduction, and tiling.

## Implementation Notes

The shared PGM functionality is implemented in `src/pgm.c` and exposed through `include/pgm.h`. Each command-line program focuses on one operation and reuses the same parsing, validation, allocation, and writing logic.

Supported PGM constraints:

- Magic numbers: `P2` and `P5`
- Maximum gray value: `255`
- Minimum image dimension: `1`
- Maximum image dimension: `65536`

## Author

Murat Kurmaz

## License

No license has been specified yet.
