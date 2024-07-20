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

#include "tui.h" // tui
#include "info.h" // data
#include "absl/flags/parse.h" // parse the command line
#include "absl/flags/flag.h" // parse the command line
#include "ui.h"
#include "function.h"
#include <filesystem>

ABSL_FLAG(std::string, language, "EN",        "Default: EN(English). Option: EN(English), CH(Chinese)");
ABSL_FLAG(std::string, address,  "127.0.0.1", "Default: 127.0.0.1");
ABSL_FLAG(std::string, port,     "2500",      "Default: 2500");
ABSL_FLAG(std::string, log_file, "",          "Default: running directory");

void start_log(std::string log_file) {
  FLAGS_alsologtostderr = false; //  set true will write log to screen(stderr)
  FLAGS_log_dir = log_file; // set log file directory
  FLAGS_max_log_size = 10 * 1024; // 10MB
  FLAGS_minloglevel = google::INFO; // set min log level
  google::InitGoogleLogging("VChat-Client"); // initialize
}

void stop_log() { google::ShutdownGoogleLogging(); }

std::unique_ptr<Function> function = nullptr;

int main(int argc, char **argv) {
  // parse command line
  absl::ParseCommandLine(argc, argv);
  std::string language = absl::GetFlag(FLAGS_language),
              address  = absl::GetFlag(FLAGS_address),
              port     = absl::GetFlag(FLAGS_port),
              logfile  = absl::GetFlag(FLAGS_log_file);
  if (logfile.empty()) logfile = std::filesystem::current_path();
  start_log(logfile);
  LOG(INFO) << "VChat-Client start...";
  try {
    Info::getinstance();
    function = std::make_unique<Function>(address, port);
    Tui ui(language);
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
  }
  stop_log();
  return 0;
}
