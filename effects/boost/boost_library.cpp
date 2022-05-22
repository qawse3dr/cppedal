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

// LOGIC Grabbed from
// https://www.electrosmash.com/forum/pedal-pi/202-how-to-start-programming-pedal-pi?lang=en

#include "boost_library.hpp"

using cppedal::effects::BoostEffectLibrary;
using cppedal::effects::EffectLibrary;

uint32_t BoostEffectLibrary::process(uint32_t in) {
  if (!activated) {
    return in;
  }
  return static_cast<uint32_t>(static_cast<float>(in) * booster_value);
}

bool BoostEffectLibrary::setInput(const std::string& key, int value) {
  if (key == "boost_level") {
    booster_value = (value + 20.0) / 40.0;
  } else if (key == "boost_on") {
    activated = static_cast<bool>(value);
  }

  return true;
}

extern "C" {
std::shared_ptr<EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new BoostEffectLibrary);
}
}