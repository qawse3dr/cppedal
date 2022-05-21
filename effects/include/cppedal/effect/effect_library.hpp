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

#include <iostream>
#include <memory>
#include <string>

namespace cppedal::effects {

class EffectLibrary {
 public:
  explicit EffectLibrary() = default;
  virtual ~EffectLibrary() = default;

  /**
   * @brief Each effect will take in a int64_t input signal
   *        and return a int64_t signal. for example for a
   *        clean/tranparent effect would be
   *        ```
   *        inline uint32_t process(uint32_t in) { return in; }
   *        ```
   * @exception InvalidInput (if I do exceptions)
   *
   * @param in signal in
   * @return uint32_t  signal out
   */
  virtual uint32_t process(uint32_t in) = 0;

  /**
   * @brief Set the Input of an effect based on key.
   *        This is up to the effect to impl.
   *        for example a distrotion effect might have a key
   *        "dist" which will control the destorition level.
   *
   * @note This will only be called when the value is updated.
   *       and whenever effects change the value will always default
   *       to 50, and it is the responsibility of the effect to normalize
   *       this input.
   * @note The effect will be responsible for thread safety
   *
   * @param key   key defined in cppedal.json ex "dist"
   * @param value new value.
   * @return true
   * @return false
   */
  virtual inline bool setInput(const std::string &, int) {
    // By default do nothing
    std::cerr << "SetInput is not implemented for this effect ignoring"
              << std::endl;
    return false;
  }
};

}  // namespace cppedal::effects

extern "C" {
typedef std::shared_ptr<cppedal::effects::EffectLibrary> (
    *makeEffectLibraryFtn)();
}
#define CPPEDAL_MAKE_EFFECT_LIBRARY_NAME "makeEffectLibrary"