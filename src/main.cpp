#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "Image.h"
#include "filters/GaussianBlur.h"
#include "filters/GrayScale.h"
#include "filters/KuwaharaFilter.h"

enum flags { e = 1, o, f, h, i, p };

enum filters_enum {
  gaussian_blur = 0,
  grayscale,
  kuwahara,
};

// TODO: Change to array or std::array of strings (add overload utils.h)
const std::vector<std::string> valid_output_ext = {
    ".png",
    ".jpg",
    ".jpeg",
};

// TODO: Change to array or std::array of strings (add overload to utils.h)
const std::vector<std::string> valid_filters = {
    "gaussian_blur",
    "grayscale",
    "kuwahara",
};

int main(int argc, char* argv[]) {
  std::cout << "Welcome to Imagerio!\n";

#ifdef DEBUG_PRINT
  std::cout << "Here are CLI args to this tool:\n";
  for (int i = 0; i < argc; i++) {
    std::cout << "\t" << argv[i] << "\n";
  }
#endif  // !DEBUG_PRINT

  if (argc == 1) {
    // TODO: Change order of help things, add mandatory -i
    std::cout << "No input file provided to program...\n"
              << "usage: imagerio  [<option>] [<input>] ... [<option>] "
                 "[<input>] \n\n"
              << "The following options are available :\n\n"
              << "\t-o or -output     name of the outfile image, this name "
                 "must include the correct file extension \n"
              << "\t-f=<valid_filter> or -filter=<valid_filter>     name "
                 "of the filter that will be applied to the image  \n"
              << "\tsupported modes:\n"
              << "\t\t gaussian_blur - blur image with gaussian blur\n"
              << "\t\t grayscale     - make image gray\n"
              << "\t\t kuwahara      - kuwahara filter\n"
              << "\t-h or -help       list of commmands \n"
              << "\t-i or -image      image file name and path example: "
                 "./folder/image.png or "
                 "C:\\Users\\WindowsUser\\Pictures\\image.png \n"
              << "\t-p or -parllel    set the program to use "
                 "multi-threading \n\n";

    return -1;
  }

  std::string outputfile = "";
  std::string inputfile = "";
  filters_enum filter = filters_enum::gaussian_blur;
  bool earlyexit = false;
  bool parallel_impl = false;

  for (int x = 1; x < argc;) {
    if (earlyexit) {
      break;
    }

    int flag =
        starts_with("-o", argv[x]) * flags::o +
        (starts_with(argv[x], "-f") || starts_with(argv[x], "-filter")) *
            flags::f +
        (starts_with("-h", argv[x]) || starts_with("-help", argv[x])) *
            flags::h +
        (starts_with("-i", argv[x]) || starts_with("-image", argv[x])) *
            flags::i +
        (starts_with("-p", argv[x]) || starts_with("-parallel", argv[x])) *
            flags::p;

    if (flag == 0) {
      std::cerr << "Invaild Input enter -h or -help if you need help\n";
      earlyexit = true;

      break;
    }

    switch (flag) {
    case flags::o:
      if (is_valid_path(argv[x + 1]) &&
          is_valid_extension(argv[x + 1], valid_output_ext)) {
        outputfile = argv[x + 1];
        x += 2;
      } else {
        std::cerr << "Output path is invalid or extension is not "
                     "one of possible ones!\n";
        earlyexit = true;
      }
      break;
    case flags::f:
      if (is_valid_extension(argv[x], valid_filters)) {
        const std::string filter_name_unparsed = argv[x];
        size_t eq_sign_pos = filter_name_unparsed.find_first_of('=');

        const std::string filter_name = filter_name_unparsed.substr(
            eq_sign_pos + 1, filter_name_unparsed.size() - eq_sign_pos);

        auto iter =
            std::find(valid_filters.begin(), valid_filters.end(), filter_name);
        const size_t idx = std::distance(valid_filters.begin(), iter);
        filter = filters_enum(idx);
      } else {
        std::cerr << "Invalid filter name! Using Gaussian Blur as "
                     "default\n";
      }

      x += 1;
      break;
    case flags::i:
      if (std::fstream(argv[x + 1]).good() &&
          is_valid_extension(argv[x + 1], valid_output_ext)) {
        inputfile.append(argv[x + 1]);
        x += 2;
      } else {
        std::cerr << "File is invalid!\n";
        earlyexit = true;
      }

      break;
    case flags::p:
      if (!parallel_impl) {
        std::cout << "Using parallel impl\n";
        parallel_impl = true;
      }

      x += 1;

      break;
    case flags::h:
      // TODO: Change order of help things, add mandatory -i
      std::cout << "usage: imagerio  [<option>] [<input>] ... [<option>] "
                   "[<input>] \n\n"
                << "The following options are available :\n\n"
                << "\t-o or -output     name of the outfile image, this "
                   "name must include the correct file extension \n"
                << "\t-f or -filter     name of the filter that will be "
                   "applied to the image  \n"
                << "\t-h or -help       list of commmands \n"
                << "\t-i or -image      image file name and path example: "
                   "./folder/image.png or "
                   "C:\\Users\\WindowsUser\\Pictures\\image.png \n"
                << "\t-p or -parllel    set the program to use "
                   "multi-threading \n\n";
      earlyexit = true;

      break;
    default:
      std::cerr << "Wrong argument - check -help for help!\n";
      earlyexit = true;

      break;
    }
  }

  if (earlyexit) {
    return -1;
  }

  // TODO: Need to check inputfile string and exit on non-existent file or empty
  // string
  imgr::Image og_img(inputfile);

#ifdef DEBUG_PRINT
  og_img.print_stats();
  std::chrono::time_point start = std::chrono::high_resolution_clock::now();

  std::cout << "Filter: " << filter << "\n";
#endif  // !DEBUG_PRINT

  // make decision based on filter
  switch (filter) {
  case filters_enum::gaussian_blur:
    parallel_impl ? imgr::GaussianBlur::apply_gaussian_blur_parallel(og_img)
                  : imgr::GaussianBlur::apply_gaussian_blur(og_img);
    break;
  case filters_enum::grayscale:
    parallel_impl ? imgr::GrayScale::grayscaleImageParallel(og_img)
                  : imgr::GrayScale::grayscaleImage(og_img);
    break;
  case filters_enum::kuwahara:
    imgr::KuwaharaFilter::apply_kuwara_filter(og_img);
    break;
  default: std::cerr << "Unhandeled filter!!!! \n"; break;
  }

#ifdef DEBUG_PRINT
  std::chrono::time_point end = std::chrono::high_resolution_clock::now();
  std::cout << "Time for image processing: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << "\n";
#endif  // !DEBUG_PRINT

  og_img.write(outputfile);
}
