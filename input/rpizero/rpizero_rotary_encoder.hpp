/*
 * (C) Copyright 2023 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once
#include <cppedal/input/rotary_encoder.hpp>
#include <limits>

namespace cppedal::input {

class RpiZeroRotaryEncoder : public RotaryEncoder {
 private:
  int last_gpio_;
  int clk_pin_value_;
  int data_pin_value_;
  
  static void _callback(int gpio, int level, uint32_t tick, void *user);
  void callback(int gpio, int level, uint32_t tickS);
  
 public:
  RpiZeroRotaryEncoder(uint8_t clk_pin, uint8_t data_pin, bool pull_up);
  ~RpiZeroRotaryEncoder();
};
}  // namespace cppedal::input