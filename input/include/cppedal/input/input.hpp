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

#include <stdint.h>

#include <functional>

namespace cppedal::input {

enum class InputTypes { kPushButton, kRotaryEncoder };
class Input {
 protected:
  std::function<void(int64_t)> callback_ = nullptr;

 private:
  InputTypes input_type_;

 public:
  Input(InputTypes input_type) : input_type_(input_type){};
  virtual ~Input() = default;

  void setCallback(std::function<void(int64_t)> callback) {
    callback_ = callback;
  }

  inline InputTypes getInputType() { return input_type_; }
};

}  // namespace cppedal::input