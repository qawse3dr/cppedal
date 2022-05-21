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

#include "clean_library.hpp"

using cppedal::effects::CleanEffectLibrary;
using cppedal::effects::EffectLibrary;

uint32_t CleanEffectLibrary::process(uint32_t in) {
  // Do nothing this is a clean effect
  return in;
}

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new CleanEffectLibrary);
}
}