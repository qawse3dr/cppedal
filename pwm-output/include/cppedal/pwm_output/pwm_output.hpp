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

namespace cppedal::pwm_output {

class PwmOutput {
 public:
  PwmOutput();
  virtual ~PwmOutput() = 0;
  
  virtual void output(int64_t) = 0;
};

} // namespace cppedal::pwm_output