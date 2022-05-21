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

#include <memory>

namespace cppedal::pwm_output {

class PwmOutput {
 public:
  PwmOutput() = default;
  virtual ~PwmOutput() = default;

  virtual void output(uint32_t) = 0;
};

}  // namespace cppedal::pwm_output

extern "C" {
typedef std::unique_ptr<cppedal::pwm_output::PwmOutput> (*makePwmOutputFtn)();
}
#define CPPEDAL_MAKE_PWM_OUTPUT_NAME "makePwmOutput"