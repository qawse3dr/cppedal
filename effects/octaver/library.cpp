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
  buffer_[write_delay_] = in;
  write_delay_++;
  if (write_delay_ >= buffer_size_) write_delay_ = 0;
  in = buffer_[read_delay_];
  switch (octave) {
    case 0:
      divider++;
      if (divider >= 2) {
        read_delay_ += 1;
        divider = 0;
      }
      break;
    case 1:
      read_delay_ += 1;
      break;
    case 2:
      read_delay_ += 2;
      break;
  }
  if (read_delay_ >= buffer_size_) read_delay_ = 0;

  return in;
}

extern "C" {
std::shared_ptr<cppedal::effects::EffectLibrary> makeEffectLibrary() {
  return std::unique_ptr<EffectLibrary>(new Library);
}
}