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
#pragma once

#include <json.hpp>
#include <map>
#include <memory>
#include <vector>

// Includes for components
#include <cppedal/effect/effect_library.hpp>
#include <cppedal/ingestor/ingestor.hpp>
#include <cppedal/input/input.hpp>
#include <cppedal/pwm_output/pwm_output.hpp>

namespace cppedal::framer {

struct FramerConfig {
  struct LibraryInfo {
    std::string name;
    std::string path;
  };

  struct InputInfo {
    std::string name;
    cppedal::input::InputTypes type;
    const nlohmann::json input_data;
  };

  // Libraries
  LibraryInfo ingestor;
  LibraryInfo pwm_output;
  LibraryInfo lcd;
  LibraryInfo input_library;

  std::vector<LibraryInfo> effects_libraries;

  // Inputs
  std::vector<InputInfo> input_info;

  std::string prev_effect_button;
  std::string next_effect_button;
};

/**
 * @brief This class will hold the whole project together
 * Have
 * - ingestor
 * - pwm-output
 * - effects
 *
 * and run the whole applications
 *
 */
class Framer {
 private:
  // IO of sound
  std::unique_ptr<cppedal::ingestor::Ingestor> ingst_ = {};
  std::unique_ptr<cppedal::pwm_output::PwmOutput> output_ = {};
  std::map<std::string, std::unique_ptr<cppedal::effects::EffectLibrary>>
      effect_map_ = {};

  FramerConfig cfg_ = {};
  std::string dumpCfg() const;

  bool parseConfig(const std::string cfg_path);
  bool parseLibraryInfo(const std::string& name, const nlohmann::json& it,
                        FramerConfig::LibraryInfo& info);

 public:
  explicit Framer(const std::string& cfg_path);
  ~Framer() = default;

  bool start();

  void stop();
};

}  // namespace cppedal::framer