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

#include <limits>
#include <memory>

#include "input.hpp"

namespace cppedal::input {

class RotaryEncoder : public Input {
 protected:
  uint8_t clk_pin_;
  uint8_t data_pin_;
  bool pull_up_;

  int64_t value_ = {0};

  int64_t max_ = std::numeric_limits<int64_t>::max();
  int64_t min_ = std::numeric_limits<int64_t>::min();

 public:
  RotaryEncoder(uint8_t clk_pin, uint8_t data_pin, bool pull_up)
      : Input(InputTypes::kRotaryEncoder),
        clk_pin_(clk_pin),
        data_pin_(data_pin),
        pull_up_(pull_up) {}
  ~RotaryEncoder() = default;
  int64_t getValue() { return value_; }
  inline void setValue(int64_t value) {
    value_ = value;
    if (callback_) callback_(value_);
  }
  void reset() {
    value_ = 0;
    max_ = std::numeric_limits<int64_t>::max();
    min_ = std::numeric_limits<int64_t>::min();
  }
  inline void setMax(int64_t max) { max_ = max; }
  inline void setMin(int64_t min) { min_ = min; }
};

}  // namespace cppedal::input

extern "C" {
typedef std::unique_ptr<cppedal::input::Input> (*makeRotaryEncoderFtn)(
    uint8_t clk, uint8_t data, bool pull_up);
}
#define CPPEDAL_MAKE_ROTARY_ENCODER_NAME "makeRotaryEncoder"