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

#include <cppedal/effect/effect_library.hpp>
#include <memory>
#include <vector>

namespace cppedal::framer {

class EffectContainer {
  std::vector<std::shared_ptr<cppedal::effects::EffectLibrary>> effects_ = {};

 public:
  EffectContainer() = default;
  ~EffectContainer() = default;

  /**
   * @brief Processes the signal through all signals
   *
   * @param in input signal
   * @return uint32_t output signal
   */
  uint32_t process(uint32_t in);
};

}  // namespace cppedal::framer