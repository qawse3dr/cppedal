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

#include "library.hpp"

using cppedal::effects::EffectLibrary;
using cppedal::effects::Library;

uint32_t Library::process(uint32_t in) {
  buffer_[counter_] = in;
  counter_++;
  if (counter_ >= delay_depth_) counter_ = 0;
  in = (buffer_[counter_] + in) >> 1;
  return in;
}

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new Library);
}
}