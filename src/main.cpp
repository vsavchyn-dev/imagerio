#include <omp.h>
#include <tbb/tbb.h>
#include <iostream>
#include <string>
#include "Image.h"

using namespace imgr;

enum flags
{
    e, o, f, h, i, p
};

int main(int argc, char* argv[]) {
    std::cout << "Welcome to Imagerio!\n";

    std::cout << "Here are CLI args to this tool:\n";
    for (int i = 0; i < argc; i++) {
        std::cout << "\t" << argv[i] << "\n";
    }

    // TODO: create a validation of CLI args to choose between different
    std::string outputfile = "./";
    std::string imagefile = "";
    std::string filter = "";

    for (int x = 1; x < argc; x++) {
        int flag = starts_with("-o", argv[x]) * o +
            (starts_with("-f", argv[x]) || starts_with("-filter", argv[x])) * f +
            (starts_with("-h", argv[x]) || starts_with("-help", argv[x])) * h +
            (starts_with("-i", argv[x]) || starts_with("-image", argv[x])) * i;
        if (flag == 0 && x == 1) {
            std::cout << "Invaild Input enter -h or -help if you need help\n";
            break;
        }
        switch (flag)
        {
        case o: outputfile = outputfile + argv[x + 1]; break;
        case f: filter = filter + argv[x + 1]; break;
        case h: std::cout 
            << "usage: imagerio  [<option>] [<input>] ... [<option>] [<input>] \n\n"
            << "The following options are available :\n\n"
            << " -o or -output     name of the outfile image, this name must include the correct file extension \n"
            << " -f or -filter     name of the filter that will be applied to the image  \n"
            << " -h or -help       list of commmands \n"
            << " -i or -image      image file name and path example: ./folder/image.png or C:\\Users\\WindowsUser\\Pictures\\image.png \n"
            << " -p or -parllel    set the program to use multi-threading \n\n";
            break;
        case i: imagefile = imagefile + argv[x + 1]; break;
        default:
            break;
        }
    }

    // TODO: create a file reader for Images (stb_image.h library or smth)
    Image img("./exmp.jpg");
    img.print_stats();
    img.write();
    img.write(outputfile);
    // TODO: create switch (or just default pass) to one of the filters in filter directory

    // TODO: create a file writer for images (default file out for now).
    return 0;
}
