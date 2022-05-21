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

  if (!loadIngst() || !loadOutput() || !loadInput() || !loadLCD()) {
    std::cerr << "Failed to load shared libs" << std::endl;
    exit(-1);
  }

  // Load Effect libs
  for (auto& e : cfg_.effects_libraries) {
    if (!loadEffectLib(e)) {
      exit(-1);
    }
  }

  // setup effect configs
}

void Framer::workLoop() {
  uint32_t input;
  auto effect = effect_library_map_["clean"];
  while (running_) {
    // Get input from ingestor
    input = ingst_->ingest();

    // Do effects
    input = effect->process(input);
    // {
    //   std::lock_guard<std::mutex> lk(effect_mutex_);
    //   input = cur_effect_->process(input);
    // }

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

  // parse effect libraries
  if (!parseEffectLibs(j)) {
    return false;
  }
  if (cfg_.effects_libraries.empty()) {
    std::cerr << "No effect are loading why are you using a pedal..."
              << std::endl;
    return false;
  }

  // // input info
  // parseInputInfo(j);

  // effects
  if (!parseEffectInfo(j)) {
    return false;
  }

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
     << "EFFECT LIBS" << std::endl;

  for (const auto& effect : cfg_.effects_libraries) {
    ss << "{" << effect.name << " - " << effect.path << "}" << std::endl;
  }
  ss << "EFFECTS" << std::endl;
  for (const auto& effect : cfg_.effects_info) {
    ss << "{ name " << effect.name << ", effects: [";
    for (auto& e : effect.effect_libraries) {
      ss << " " << e << ",";
    }
    ss << "], inputs: [";
    for (auto& i : effect.input_mapping) {
      ss << " (" << i.first << ", " << i.second << "), ";
    }
    ss << "]}" << std::endl;
  }
  ss << "prev_butt:  " << cfg_.prev_effect_button << std::endl
     << "next_butt:  " << cfg_.next_effect_button << std::endl
     << "CONFIG END  " << std::endl;
  return ss.str();
}

bool Framer::parseLibraryInfo(const std::string& name, const nlohmann::json& j,
                              FramerConfig::LibraryInfo& info) {
  auto it = j.find(name);
  if (it == j.end()) {
    std::cerr << "\"" << name << "\", not found" << std::endl;
    return false;
  }

  if (!it.value().is_object()) {
    std::cerr << "\"" << name << "\", expected type object with name and path"
              << std::endl;
    return false;
  }

  const nlohmann::json& libj = it.value();

  auto name_it = libj.find("name");
  if (name_it == libj.end()) {
    std::cerr << "\"" << name << "\", name not found" << std::endl;
    return false;
  }

  auto path_it = libj.find("path");
  if (path_it == libj.end()) {
    std::cerr << "\"" << name << "\", path not found" << std::endl;
    return false;
  }

  info.name = name_it.value().get<std::string>();
  info.path = path_it.value().get<std::string>();

  return true;
}

bool Framer::parseEffectLibs(const nlohmann::json& j) {
  auto effect_array_it = j.find("effect_libraries");
  if (effect_array_it == j.end()) {
    std::cerr << "Failed to find effect_libraries" << std::endl;
    return false;
  }

  // Make sure it is an array
  if (!effect_array_it.value().is_array()) {
    std::cerr << "effect_libraries must be an array" << std::endl;
    return false;
  }

  int i = 0;
  for (const auto& effect : effect_array_it.value()) {
    i++;
    if (!effect.is_object()) {
      std::cerr << "effect_libraries " << i << " must be an object";
      return false;
    }
    auto name_it = effect.find("name");
    if (name_it == effect.end()) {
      std::cerr << "name not found for effect_libraries: " << i << std::endl;
      return false;
    }

    auto path_it = effect.find("path");
    if (path_it == effect.end()) {
      std::cerr << "path not found for effect_libraries: " << i << std::endl;
      return false;
    }

    FramerConfig::LibraryInfo info;
    info.name = name_it.value().get<std::string>();
    info.path = path_it.value().get<std::string>();
    cfg_.effects_libraries.emplace_back(std::move(info));
  }
  return true;
}

bool Framer::parseEffectInfo(const nlohmann::json& j) {
  auto effect_array_it = j.find("effects");
  if (effect_array_it == j.end()) {
    std::cerr << "Failed to find effects" << std::endl;
    return false;
  }
  // Make sure it is an array
  if (!effect_array_it.value().is_array()) {
    std::cerr << "effests must be an array" << std::endl;
    return false;
  }

  int i = 0;

  for (const auto& effect : effect_array_it.value()) {
    i++;
    if (!effect.is_object()) {
      std::cerr << "effects " << i << " must be an object";
      return false;
    }

    FramerConfig::EffectInfo info;
    auto name_it = effect.find("name");
    if (name_it == effect.end() || !name_it.value().is_string()) {
      std::cerr << "String name not found for effect: " << i << std::endl;
    }
    info.name = name_it.value().get<std::string>();

    auto lib_it = effect.find("effects");
    if (lib_it == effect.end() || !lib_it.value().is_array()) {
      std::cerr << "Array effects not found for effect: " << i << std::endl;
    }
    for (const auto& lib : lib_it.value()) {
      info.effect_libraries.emplace_back(lib.get<std::string>());
    }

    auto input_it = effect.find("inputs");
    if (input_it == effect.end() || !input_it.value().is_array()) {
      std::cerr << "Array input not found for effect: " << i << std::endl;
    }
    for (const auto& input : input_it.value()) {
      // TODO error check
      info.input_mapping.emplace_back(input["name"].get<std::string>(),
                                      input["input"].get<std::string>());
    }

    cfg_.effects_info.emplace_back(std::move(info));
  }
  return true;
}

bool Framer::loadIngst() {
  cfg_.ingestor.handler = dlopen(cfg_.ingestor.path.c_str(), RTLD_NOW);
  if (!cfg_.ingestor.handler) {
    std::cerr << "Failed to open ingestor " << cfg_.ingestor.name << " at "
              << cfg_.ingestor.path << " with " << dlerror();
    return false;
  }

  // Create ingestor
  makeIngestorFtn ingst_ftn = reinterpret_cast<makeIngestorFtn>(
      dlsym(cfg_.ingestor.handler, CPPEDAL_MAKE_INGESTOR_NAME));
  if (!ingst_ftn) {
    std::cerr << "Failed to find ingestor" << CPPEDAL_MAKE_INGESTOR_NAME
              << " in " << cfg_.ingestor.name << std::endl;
    return false;
  }
  ingst_ = std::move(ingst_ftn());
  return true;
}

bool Framer::loadOutput() {
  cfg_.pwm_output.handler = dlopen(cfg_.pwm_output.path.c_str(), RTLD_NOW);
  if (!cfg_.pwm_output.handler) {
    std::cerr << "Failed to open pwm_output " << cfg_.pwm_output.name << " at "
              << cfg_.pwm_output.path << " with " << dlerror();
    return false;
  }

  // Create ingestor
  makePwmOutputFtn output_ftn = reinterpret_cast<makePwmOutputFtn>(
      dlsym(cfg_.pwm_output.handler, CPPEDAL_MAKE_PWM_OUTPUT_NAME));
  if (!output_ftn) {
    std::cerr << "Failed to find pwm_output " << CPPEDAL_MAKE_PWM_OUTPUT_NAME
              << " in " << cfg_.pwm_output.name << std::endl;
    return false;
  }
  output_ = std::move(output_ftn());
  return true;
}

bool Framer::loadInput() { return true; }
bool Framer::loadLCD() { return true; }

bool Framer::loadEffectLib(FramerConfig::LibraryInfo& effect) {
  if (effect_library_map_.find(effect.name) != effect_library_map_.end()) {
    std::cerr << "Effect " << effect.name
              << " defined twice choose a different name" << std::endl;
    return false;
  }

  effect.handler = dlopen(effect.path.c_str(), RTLD_NOW);
  if (!effect.handler) {
    std::cerr << "Failed to open effect library " << effect.name << " - "
              << effect.path << std::endl;
    return false;
  }
  makeEffectLibraryFtn effect_ftn = reinterpret_cast<makeEffectLibraryFtn>(
      dlsym(effect.handler, CPPEDAL_MAKE_EFFECT_LIBRARY_NAME));
  if (!effect_ftn) {
    std::cerr << "Failed to load " << effect.name << " function"
              << CPPEDAL_MAKE_EFFECT_LIBRARY_NAME << std::endl;
  }

  effect_library_map_.emplace(effect.name, effect_ftn());
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
Framer::~Framer() {
  // we have to delete all pointers before libraries
  ingst_.reset();
  output_.reset();
  effect_library_map_.clear();
  effects_.clear();

  for (const auto& effect : cfg_.effects_libraries) {
    if (effect.handler) dlclose(effect.handler);
  }

  if (cfg_.pwm_output.handler) dlclose(cfg_.pwm_output.handler);
  if (cfg_.ingestor.handler) dlclose(cfg_.ingestor.handler);
}