/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once
#include <cppedal/input/push_button.hpp>
#include <limits>

namespace cppedal::input {

class RpiZeroPushButton : public PushButton {
 private:
 	void RpiZeroCallback(int gpio, int level, uint32_t tick);
 	
 	static void _RpiZeroCallback(int gpio, int level, uint32_t tick, void* user);
	
 public:
  RpiZeroPushButton(uint8_t pin, bool pull_up);
  ~RpiZeroPushButton();
};
}  // namespace cppedal::input