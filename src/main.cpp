#include <omp.h>
#include <tbb/tbb.h>
#include <iostream>
#include "Image.h"

using namespace imgr;

int main(int argc, char* argv[]) {
    std::cout << "Welcome to Imagerio!\n";

    std::cout << "Here are CLI args to this tool:\n";
    for (int i = 0; i < argc; i++) {
        std::cout << "\t" << argv[i] << "\n";
    }

    // TODO: create a validation of CLI args to choose between different

    // TODO: create a file reader for Images (stb_image.h library or smth)
    Image img("./exmp.jpg");
    img.print_stats();
    img.write();
    img.write("./exmp-copy2.jpg");
    // TODO: create switch (or just default pass) to one of the filters in filter directory

    // TODO: create a file writer for images (default file out for now).
    return 0;
}
