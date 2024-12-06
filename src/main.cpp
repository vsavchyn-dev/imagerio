#include <omp.h>
#include <tbb/tbb.h>
#include <iostream>
#include <string>
#include "Image.h"
#include "filters/GaussianBlur.h"
#include "filters/GrayScale.h"

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
    bool earlyexit = false;

    // TODO: Exit early if there are no args passed (for now it's good as we're testing things only)
    for (int x = 1; x < argc; x++) {
        int flag = starts_with("-o", argv[x]) * o +
            (starts_with("-f", argv[x]) || starts_with("-filter", argv[x])) * f +
            (starts_with("-h", argv[x]) || starts_with("-help", argv[x])) * h +
            (starts_with("-i", argv[x]) || starts_with("-image", argv[x])) * i;
        if (flag == 0 && x == 1) {
            std::cout << "Invaild Input enter -h or -help if you need help\n";
            earlyexit = true;
            break;
        }
        switch (flag)
        {
        case o: outputfile.append(argv[x + 1]); break;
        case f: filter.append(argv[x + 1]); break;
        case i: imagefile.append(argv[x + 1]); break;
        case h: std::cout 
            << "usage: imagerio  [<option>] [<input>] ... [<option>] [<input>] \n\n"
            << "The following options are available :\n\n"
            << " -o or -output     name of the outfile image, this name must include the correct file extension \n"
            << " -f or -filter     name of the filter that will be applied to the image  \n"
            << " -h or -help       list of commmands \n"
            << " -i or -image      image file name and path example: ./folder/image.png or C:\\Users\\WindowsUser\\Pictures\\image.png \n"
            << " -p or -parllel    set the program to use multi-threading \n\n";
            earlyexit = true;
            break;
        default:
            break;
        }
    }
    if (earlyexit) {
        return 0;
    }

    // TODO: finish with validation in the image class for read and write. 
    imgr::Image img(imagefile);
    imgr::Image img_copy = img;
    imgr::Image img_gray = img;
    imgr::Image img_gray_para = img;

    // TODO: create switch (or just default pass) to one of the filters in filter directory
    img.print_stats();
    auto start = std::chrono::high_resolution_clock::now();
    imgr::GaussianBlur::apply_gaussian_blur(img);
    auto end = std::chrono::high_resolution_clock::now();
    img.print_stats();
    std::cout << "Time for gaussian blur: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "\n";

    start = std::chrono::high_resolution_clock::now();
    imgr::GaussianBlur::apply_gaussian_blur_parallel(img_copy);
    end = std::chrono::high_resolution_clock::now();
    img_copy.print_stats();
    std::cout << "Time for gaussian blur parallel: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "\n";

    start = std::chrono::high_resolution_clock::now();
    imgr::grayscaleImage(img_gray);
    end = std::chrono::high_resolution_clock::now();
    img_gray.print_stats();
    std::cout << "Time for Grayscale: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "\n";

    start = std::chrono::high_resolution_clock::now();
    imgr::grayscaleImageParallel(img_gray_para);
    end = std::chrono::high_resolution_clock::now();
    img_gray.print_stats();
    std::cout << "Time for Grayscale Parallel: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "\n";

    img.write("./exmp-blurred.jpg");
    img_copy.write("./exmp-blurred-parallel.jpg");
    img_gray.write("./exmp-gray.jpg");
    img_gray_para.write("./exmp-gray-parallel.jpg");

    return 0;
}
