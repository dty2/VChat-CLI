#include "transmission.h"

int main() {
  try {
    vchat::Transmission* transmission = vchat::Transmission::getInstance();
    transmission->run();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << "\n";
  }
  return 0;
}
