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

#include <memory>

#include "cppedal/effect/effect_library.hpp"

namespace cppedal::effects {

class Library : public EffectLibrary {
 private:
  static constexpr uint32_t max_delay_ = 800000;
  static constexpr uint32_t min_delay_ = 0;

  uint32_t buffer_[max_delay_];
  uint32_t counter_ = 0;
  uint32_t delay_depth_ = 100000;

 public:
  Library() = default;
  ~Library() = default;
  uint32_t process(uint32_t in) override;

  bool setInput(const std::string& key, int val) override {
    if (key == "delay_depth") {
      delay_depth_ = 800000 / (val + 1);
      std::cout << delay_depth_ << std::endl;
    }
    return false;
  }
  void reset() override { delay_depth_ = 100000; }
};

}  // namespace cppedal::effects

extern "C" {
// Makes the clean effect
// All effects must have this
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary();
}