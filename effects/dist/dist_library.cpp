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

// Logic adapted from
// https://www.electrosmash.com/forum/pedal-pi/202-how-to-start-programming-pedal-pi?lang=en
uint32_t DistEffectLibrary::process(uint32_t in) {
  // Clip the signal to make it distorted
  // get samples for dist for the first 1000 samples
  if (count_ < 1000) {
    sum_ += in;
    count_++;
    mean_ = sum_ / count_;
  }
  if (in > mean_ + distortion_value) in = mean_ + distortion_value;
  if (in < mean_ - distortion_value) in = mean_ - distortion_value;
  return in;
}

bool DistEffectLibrary::setInput(const std::string& key, int value) {
  // Implement "dist"
  if (key == "dist_level") {
    distortion_value = 150 - value * 4;
  }

  return true;
}

extern "C" {
std::shared_ptr<EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new DistEffectLibrary);
}
}