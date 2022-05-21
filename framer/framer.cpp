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
  // void* handler = dlopen("lib/effects/libcppedal-effect-clean.so", RTLD_NOW);
  // if (!handler) {
  //   fprintf(stderr, "%s\n", dlerror());
  //   exit(EXIT_FAILURE);
  // }

  // makeEffectLibraryFtn effectFtn =
  //     (makeEffectLibraryFtn)dlsym(handler, CPPEDAL_MAKE_EFFECT_LIBRARY_NAME);

  // cppedal::effects::EffectLibraryConfig cfg;
  // cfg.name = "clean";
  // cfg.path = "path";

  // {
  //   auto effectLibrary = effectFtn(cfg);
  //   std::cout << effectLibrary->process(200) << std::endl;
  //   std::cout << effectLibrary->getName() << " " << effectLibrary->getPath()
  //             << std::endl;
  // }
  // dlclose(handler);
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

  //
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

  std::cout << "info" << info.name << " " << info.path << std::endl;

  return true;
}

bool Framer::start() { return true; }

void Framer::stop() {}