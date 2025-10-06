#include "argparse/argparse.hpp"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
  argparse::ArgumentParser program("dataextractor");

  program.add_argument("-s", "--source")
      .required()
      .help("Path to extracted data directory");
  program.add_argument("-o", "--output")
      .required()
      .help("Path to output directory");
}