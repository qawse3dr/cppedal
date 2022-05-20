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

#include "clean.hpp"

using cppedal::effects::CleanEffect;

int64_t  CleanEffect::process(int64_t in) {
  // Do nothing this is a clean effect
  return in;
}

extern "C" {
  cppedal::effects::Effect* makeEffect() {
    return new CleanEffect
  }
}