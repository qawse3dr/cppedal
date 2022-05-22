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

class FuzzEffectLibrary : public EffectLibrary {
 private:
  uint32_t fuzz_value = 150;
  uint64_t sum_ = 0;
  uint64_t count_ = 0;
  uint64_t mean_ = 0;

 public:
  FuzzEffectLibrary() = default;
  ~FuzzEffectLibrary() = default;
  uint32_t process(uint32_t in) override;
  bool setInput(const std::string &, int) override;
  void reset() override {
    sum_ = 0;
    count_ = 0;
    mean_ = 0;
    fuzz_value = 150;
  }
};

}  // namespace cppedal::effects

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary();
}