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

class BoostEffectLibrary : public EffectLibrary {
 private:
  float booster_value = 0.5;
  bool activated = true;

 public:
  BoostEffectLibrary() = default;
  ~BoostEffectLibrary() = default;
  uint32_t process(uint32_t in) override;
  bool setInput(const std::string &, int) override;
  void reset() override {
    booster_value = 0.5;
    activated = true;
  }
};

}  // namespace cppedal::effects

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary();
}