# imagerio

_**Image processing with parallel computing capabilities!**_

A C++ command-line tool for applying sophisticated image filters with support for parallel processing. Built with modern C++ and optimized for performance using OpenMP.

## Key Features

- **Different Filters**
  - Gaussian Blur with adaptive kernel sizing
  - Grayscale conversion
  - Kuwahara filter for edge-preserving smoothing \*_(parallel version in-progress)_
  - Others are in-progress!
- **Parallel Processing Support** via OpenMP for improved performance on multi-core systems
- **Flexible Input/Output** handling with support for common image formats (PNG, JPG, JPEG)
- **Command-Line Interface** designed for easy integration into image processing pipelines

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- OpenMP 4.5+ for parallel processing capabilities
- CMake 3.10 or higher
- Git

## Dependencies

- [stb_image](https://github.com/nothings/stb) - Single-file public domain library for image reading/writing
- OpenMP - Parallel Programming framework

## Installation

1. Clone the repository:

   ```sh
   git clone https://github.com/vsavchyn-dev/imagerio.git
   cd imagerio
   ```

2. Create a build directory and navigate into it:

   ```sh
   mkdir build && cd build
   ```

3. Run CMake to configure the project:

   ```sh
   cmake ..
   ```

4. Compile the project:

   ```sh
   make
   ```

## Usage

Basic Command Structure:

```sh
./imagerio [options] [input]
```

Replace `[options]` with the required parameters for your application. Here are the available options:

- `-o` or `-output`: Name of the output file, must include the correct file extension (e.g., `.png`, `.jpg`, `.jpeg`).
- `-f=<valid_filter>` or `-filter=<valid_filter>`: Name of the filter to be applied to the image. Supported filters are:
  - `gaussian_blur`: Applies Gaussian blur to the image.
  - `grayscale`: Converts the image to grayscale.
  - `kuwahara`: Applies Kuwahara filter to the image.
- `-h` or `-help`: Displays the list of available commands.
- `-i` or `-image`: Specifies the image file name and path (e.g., `./folder/image.png` or `C:\Users\WindowsUser\Pictures\image.png`).
- `-p` or `-parallel`: Enables multi-threading.

## Example Commands

Apply Gaussian blur with parallel processing

```sh
./imagerio -i input.jpg -o output.png -f=gaussian_blur -p
```

Convert to grayscale

```sh
./imagerio -i color.png -o gray.png -f=grayscale
```

Apply Kuwahara filter

```sh
./imagerio -i photo.jpg -o filtered.jpg -f=kuwahara
```

## Roadmap

- [x] ~~Basic CLI implementation~~
- [x] ~~Gaussian blur and Grayscale filters~~
- [x] ~~CMake for easier build~~
- [x] ~~Parallel implementation of Gaussian blur and Grayscale filters using OpenMP~~
- [x] ~~Default Kuwahara filter implementation~~
- [ ] Parallel implementation of Kuwahara filter
- [ ] Additional image filters?
- [ ] Performance optimizations (for default and OMP versions of algos)
- [ ] Chaining filters feature

## Contributors

- [Vladyslav Savchyn (vsavchyn-dev)](https://github.com/vsavchyn-dev)
- [Omer Ibrahim](https://github.com/OmerAhmedIbrahim)

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
