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

#include "dist_library.hpp"

using cppedal::effects::DistEffectLibrary;
using cppedal::effects::EffectLibrary;

uint32_t DistEffectLibrary::process(uint32_t in) {
  // Clip the signal to make it distorted
  // TODO change to grab an avg isntead of 2047
  if (in > 1024 + distortion_value) in = 1024 + distortion_value;
  if (in < 1024 - distortion_value) in = 1024 - distortion_value;
  return in;
}

bool DistEffectLibrary::setInput(const std::string& key, int value) {
  // Implement "dist"
  if (key == "dist_level") {
    distortion_value = 150 - value * 5;
    std::cout << distortion_value << std::endl;
  }

  return true;
}

extern "C" {
std::shared_ptr<EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new DistEffectLibrary);
}
}