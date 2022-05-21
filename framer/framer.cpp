/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */

#include "framer.hpp"

#include <dlfcn.h>

#include <fstream>
#include <iostream>
#include <sstream>

using cppedal::framer::Framer;
using cppedal::framer::FramerConfig;

Framer::Framer(const std::string& cfg_path) {
  if (!parseConfig(cfg_path)) {
    std::cerr << "Failed to parse config" << std::endl;
  } else {
    std::cout << dumpCfg();
  }

  // Get ingestor lib
  ingest_handler_ = dlopen(cfg_.ingestor.path.c_str(), RTLD_NOW);
  if (!ingest_handler_) {
    std::cerr << "Failed to open ingestor " << cfg_.ingestor.name << " at "
              << cfg_.ingestor.path << " with " << dlerror();
    exit(EXIT_FAILURE);
  }

  // Create ingestor
  makeIngestorFtn ingst_ftn = reinterpret_cast<makeIngestorFtn>(
      dlsym(ingest_handler_, CPPEDAL_MAKE_INGESTOR_NAME));
  if (!ingst_ftn) {
    std::cerr << "Failed to find ingestor" << CPPEDAL_MAKE_INGESTOR_NAME
              << " in " << cfg_.ingestor.name << std::endl;
  }
  ingst_ = std::move(ingst_ftn());

  // Create PWM_OUTPUT
  output_handler_ = dlopen(cfg_.pwm_output.path.c_str(), RTLD_NOW);
  if (!output_handler_) {
    std::cerr << "Failed to open pwm_output " << cfg_.pwm_output.name << " at "
              << cfg_.pwm_output.path << " with " << dlerror();
    exit(EXIT_FAILURE);
  }

  // Create ingestor
  makePwmOutputFtn output_ftn = reinterpret_cast<makePwmOutputFtn>(
      dlsym(output_handler_, CPPEDAL_MAKE_PWM_OUTPUT_NAME));
  if (!output_ftn) {
    std::cerr << "Failed to find pwm_output " << CPPEDAL_MAKE_PWM_OUTPUT_NAME
              << " in " << cfg_.pwm_output.name << std::endl;
  }
  output_ = std::move(output_ftn());
}

Framer::~Framer() {
  // we have to delete all pointers before libraries
  ingst_.reset();
  output_.reset();

  dlclose(output_handler_);
  dlclose(ingest_handler_);
}

void Framer::workLoop() {
  uint32_t input;
  while (running_) {
    // Get input from ingestor
    input = ingst_->ingest();

    // Do effects

    // output to pwm
    output_->output(input);
  }
}

bool Framer::parseConfig(const std::string cfg_path) {
  std::ifstream fin(cfg_path);
  nlohmann::json j;

  // Open file
  if (!fin.is_open()) {
    std::cerr << "Failed to open cfg " << cfg_path << " does it exist."
              << std::endl;
    return false;
  }
  // Parse config
  try {
    std::stringstream ss;
    ss << fin.rdbuf();
    j = nlohmann::json::parse(ss.str(), nullptr, true, true);
  } catch (const std::exception& e) {
    std::cerr << "Failed to parse json config " << cfg_path << ": with error "
              << e.what() << std::endl;
    return false;
  }

  /* Retrive info from config */

  // Get library paths
  if (!parseLibraryInfo("ingestor", j, cfg_.ingestor) ||
      !parseLibraryInfo("pwm_output", j, cfg_.pwm_output) ||
      !parseLibraryInfo("lcd", j, cfg_.lcd) ||
      !parseLibraryInfo("input_library", j, cfg_.input_library))
    return false;

  // // parse effect libraries
  // parseEffectLibs(j);

  // // input info
  // parseInputInfo(j);

  // // effects
  // parseEffectInfo(j);

  // SUSSY BUTTONS
  auto it = j.find("prev-effect-button");
  if (it == j.end()) {
    std::cerr << "Failed to find prev-effect-button" << std::endl;
    return false;
  }
  cfg_.prev_effect_button = it.value().get<std::string>();

  it = j.find("next-effect-button");
  if (it == j.end()) {
    std::cerr << "Failed to find next-effect-button" << std::endl;
    return false;
  }
  cfg_.next_effect_button = it.value().get<std::string>();

  return true;
}

std::string Framer::dumpCfg() const {
  std::stringstream ss;

  ss << "CONFIG START" << std::endl
     << "ingestor:   " << cfg_.ingestor.name << " - " << cfg_.ingestor.path
     << std::endl
     << "pwm_output: " << cfg_.pwm_output.name << " - " << cfg_.pwm_output.path
     << std::endl
     << "lcd:        " << cfg_.lcd.name << " - " << cfg_.lcd.path << std::endl
     << "input_libs: " << cfg_.input_library.name << " - "
     << cfg_.input_library.path << std::endl

     << "prev_butt:  " << cfg_.prev_effect_button << std::endl
     << "next_butt:  " << cfg_.next_effect_button << std::endl
     << "CONFIG END  " << std::endl;
  return ss.str();
}

bool Framer::parseLibraryInfo(const std::string& name, const nlohmann::json& j,
                              FramerConfig::LibraryInfo& info) {
  auto it = j.find(name);
  if (it == j.end()) {
    std::cerr << "\"" << name << "\", not found" << std::endl;
  }

  if (it.value().is_object()) {
    std::cerr << "\"" << name << "\", excepted type object with name and path"
              << std::endl;
  }

  const nlohmann::json& libj = it.value();

  auto name_it = libj.find("name");
  if (name_it == libj.end()) {
    std::cerr << "\"" << name << "\", name not found" << std::endl;
  }

  auto path_it = libj.find("path");
  if (path_it == libj.end()) {
    std::cerr << "\"" << name << "\", path not found" << std::endl;
  }

  info.name = name_it.value().get<std::string>();
  info.path = path_it.value().get<std::string>();

  return true;
}

bool Framer::start() {
  if (running_) {
    std::cerr << "Framer: already running ignore start()" << std::endl;
    return false;
  }

  running_ = true;
  work_thread_ = std::thread(&Framer::workLoop, this);
  return true;
}

void Framer::stop() {
  running_ = false;
  if (work_thread_.joinable()) {
    work_thread_.join();
  }
}