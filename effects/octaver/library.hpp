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
  static constexpr uint32_t buffer_size_ = 5000;

  uint32_t buffer_[buffer_size_];
  uint32_t read_delay_ = 0;
  uint32_t write_delay_ = 0;
  int divider = 0;

  uint32_t octave = 1;

 public:
  Library() = default;
  ~Library() = default;
  uint32_t process(uint32_t in) override;

  bool setInput(const std::string& key, int val) override {
    if (key == "octaver" && val == 1) {
      octave++;
      if (octave == 3) {
        octave = 0;
      }
    }
    return false;
  }
  void reset() override { octave = 1; }
};

}  // namespace cppedal::effects

extern "C" {
// Makes the clean effect
// All effects must have this
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary();
}