#include "log.h"

int main(int argc, char* argv[]) {
  using namespace Pascal;
//   Logger::set_log_level(LogLevel::DEBUG);
  LOG(DEBUG) << "debug\n";
  LOG(INFO) << "info\n";
  LOG(WARNING) << "warning\n";
  LOG(ERROR) << "error\n";
}