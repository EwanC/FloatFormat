#include <iostream>
#include <unordered_map>
#include "FF.h"

namespace {
void printUsage() {
  std::cout << "Usage: ff [-f <float> | -h <half> | -d <double>]\n";
  std::cout << "Floats can be entered in hex integer, hex float, "
            << "fixed point float, or scientific float formats\n";
  std::cout << "    -h, --half   <half>     16-bit half precision\n";
  std::cout << "    -f, --float  <float>    32-bit single precision\n";
  std::cout << "    -d, --double <double>   64-bit double precision\n";
  std::cout << "    --help                  Displays this message and exit\n";
}

using ParseFunctorType = std::add_pointer<decltype(FF::parseHalf)>::type;
const std::unordered_map<std::string, ParseFunctorType> functor_map = {
    {"--half", FF::parseHalf},     {"-h", FF::parseHalf},
    {"--float", FF::parseSingle},  {"-f", FF::parseSingle},
    {"--double", FF::parseDouble}, {"-d", FF::parseDouble},
};
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printUsage();
    return -1;
  }

  for (int i = 1; i < argc; i++) {
    const std::string arg(argv[i]);

    if (arg == "--help") {
      printUsage();
      return 0;
    }

    ParseFunctorType parser;
    try {
      parser = functor_map.at(arg);
    } catch (std::out_of_range&) {
      std::cerr << "Unkown argument " << arg << "\n";
      return -1;
    }

    if (i == (argc - 1)) {
      std::cerr << "Expected another argument after " << arg << "\n";
      return -1;
    }

    parser(argv[i + 1]);
    return 0;
  }

  std::cerr << "Incorrect Usage.\n";
  printUsage();
  return -1;
}
