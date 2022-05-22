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

#include <stdint.h>

#include <cppedal/effect/effect_library.hpp>
#include <cppedal/lcd/lcd.hpp>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cppedal::framer {

class EffectContainer {
  std::string name_;
  std::vector<std::shared_ptr<cppedal::effects::EffectLibrary>> effects_ = {};

 public:
  std::vector<std::pair<std::string, std::string>> inputs_ = {};
  static cppedal::lcd::LCD* lcd_;

 public:
  EffectContainer(
      const std::string& name,
      std::vector<std::shared_ptr<cppedal::effects::EffectLibrary>>&& effects,
      std::vector<std::pair<std::string, std::string>>&& input);
  ~EffectContainer() = default;

  /**
   * @brief Processes the signal through all signals
   *
   * @param in input signal
   * @return uint32_t output signal
   */
  uint32_t process(uint32_t in);

  void setInput(const std::string& key, int value) {
    if (lcd_) {
      lcd_->print(key + " " + std::to_string(value), 1, 0);
    }
    for (const auto& effect : effects_) {
      effect->setInput(key, value);
    }
  }

  std::function<void(int64_t)> getCallback(const std::string& key) {
    return std::bind(&EffectContainer::setInput, this, key,
                     std::placeholders::_1);
  }

  const std::string& getName() { return name_; }
};

}  // namespace cppedal::framer