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
using cppedal::effects::EffectLibraryConfig;

int64_t CleanEffectLibrary::process(int64_t in) {
  // Do nothing this is a clean effect
  return in + 1;
}


extern "C" {
  std::unique_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary(
      const cppedal::effects::EffectLibraryConfig &cfg) {
    return std::unique_ptr<EffectLibrary>(new CleanEffectLibrary(cfg));
  }
}