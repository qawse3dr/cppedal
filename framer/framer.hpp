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
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// Includes for components
#include <cppedal/effect/effect_library.hpp>
#include <cppedal/ingestor/ingestor.hpp>
#include <cppedal/input/input.hpp>
#include <cppedal/input/push_button.hpp>
#include <cppedal/input/rotary_encoder.hpp>
#include <cppedal/lcd/lcd.hpp>
#include <cppedal/pwm_output/pwm_output.hpp>

#include "effect_container.hpp"
#include "framer_config.hpp"

namespace cppedal::framer {

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
  std::unique_ptr<cppedal::lcd::LCD> lcd_ = {};

  std::map<std::string, std::shared_ptr<cppedal::effects::EffectLibrary>>
      effect_library_map_ = {};

  std::map<std::string, std::unique_ptr<cppedal::input::Input>> input_map_ = {};
  std::vector<std::unique_ptr<EffectContainer>> effects_ = {};
  std::vector<std::unique_ptr<EffectContainer>>::iterator cur_effect_ = {};
  std::mutex effect_mutex_ = {};

  FramerConfig cfg_ = {};
  std::string dumpCfg() const;

  // Parse methods
  bool parseConfig(const std::string cfg_path);
  bool parseLibraryInfo(const std::string& name, const nlohmann::json& it,
                        FramerConfig::LibraryInfo& info);
  bool parseEffectLibs(const nlohmann::json& j);
  bool parseEffectInfo(const nlohmann::json& j);
  bool parseInputInfo(const nlohmann::json& j);

  // Load methods
  bool loadIngst();
  bool loadOutput();  //< PWM output
  bool loadInput();
  bool loadLCD();
  bool loadEffectLib(FramerConfig::LibraryInfo& effect);

  // Sets up all of the effects mapping them to actual libs and inputs
  bool setupEffects();
  bool setupInputs();

  bool setupInputCallbacks();

  std::thread work_thread_;
  bool running_ = false;
  void workLoop();

  // callbacks
  void prevEvent(int64_t pressed) {
    if (pressed == 0 && cur_effect_ != effects_.begin()) {
      std::lock_guard<std::mutex> lk(effect_mutex_);
      cur_effect_--;
      setupInputCallbacks();
    }
  }
  void nextEvent(int64_t pressed) {
    if (pressed == 0 && cur_effect_ != effects_.end() - 1) {
      std::lock_guard<std::mutex> lk(effect_mutex_);
      cur_effect_++;
      setupInputCallbacks();
    }
  }

  // Ftns to create Inputs
  makePushButtonFtn makePushButton = nullptr;
  makeRotaryEncoderFtn makeRotaryEncoder = nullptr;

 public:
  explicit Framer(const std::string& cfg_path);
  ~Framer();

  bool start();

  void stop();
};

}  // namespace cppedal::framer