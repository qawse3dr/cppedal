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
#include <atomic>
#include <memory>

#include "input.hpp"

namespace cppedal::input {

class PushButton : public Input {
 protected:
  uint8_t pin_;
  bool pull_up_;
  std::atomic_bool pressed_ = {false};

 public:
  PushButton(uint8_t pin, bool pull_up)
      : Input(InputTypes::kPushButton), pin_(pin), pull_up_(pull_up) {}
  virtual ~PushButton() = default;

  bool pressed() const { return pressed_; };
};

}  // namespace cppedal::input

extern "C" {
typedef std::unique_ptr<cppedal::input::Input> (*makePushButtonFtn)(
    uint8_t pin, bool pull_up);
}
#define CPPEDAL_MAKE_PUSH_BUTTON_NAME "makePushButton"