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
#include <cppedal/input/rotary_encoder.hpp>
#include <limits>
#include <thread>

namespace cppedal::input {

class RpiRotaryEncoder : public RotaryEncoder {
 private:
  std::thread thread_;

  void workLoop();

 public:
  RpiRotaryEncoder(uint8_t clk_pin, uint8_t data_pin, bool pull_up);
  ~RpiRotaryEncoder();
};
}  // namespace cppedal::input