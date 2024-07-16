/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "boost/asio/io_context.hpp"
#include "service.h"
#include "server.h"
#include "absl/flags/parse.h" // parse the command line
#include "absl/flags/flag.h" // parse the command line
#include <filesystem>

ABSL_FLAG(std::string, port,     "2500",      "Default: 2500");
ABSL_FLAG(std::string, log_file, "",          "Default: running directory");

void start_glog(std::string log_file) {
  FLAGS_alsologtostderr = false;
  FLAGS_log_dir = log_file;
  FLAGS_max_log_size = 100 * 1024; // 100MB
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging("VChat-Server");
}
void stop_glog() { google::ShutdownGoogleLogging(); }

int main(int argc, char **argv) {
  // parse command line
  absl::ParseCommandLine(argc, argv);
  std::string port     = absl::GetFlag(FLAGS_port),
              logfile  = absl::GetFlag(FLAGS_log_file);
  if (logfile.empty()) logfile = std::filesystem::current_path();
  start_glog(logfile);
  LOG(INFO) << "VChat-Server start...";
  try {
    Store::getinstance();
    Service::getinstance();
    Server server(port);
    server.run();
    Store::free();
  } catch (const std::exception& e) {
  }
  stop_glog();
  return 0;
}
