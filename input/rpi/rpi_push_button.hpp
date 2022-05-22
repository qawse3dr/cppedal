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
#include <cppedal/input/push_button.hpp>
#include <limits>
#include <thread>

namespace cppedal::input {

class RpiPushButton : public PushButton {
 private:
  // Sadly wiring pi doesn't have good
  // Interupts (uses style function)
  // so instead start up a thread that will just
  // wait on an interupt
  // start this thread on creation
  // and cancel it on destrution
  // Rip interupts polling time!!
  std::thread thread_;

  void workLoop();

 public:
  RpiPushButton(uint8_t pin, bool pull_up);
  ~RpiPushButton();
};
}  // namespace cppedal::input