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

#include "bit_library.hpp"

using cppedal::effects::BitEffectLibrary;
using cppedal::effects::EffectLibrary;

uint32_t BitEffectLibrary::process(uint32_t in) { return in << bit; }

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new BitEffectLibrary);
}
}