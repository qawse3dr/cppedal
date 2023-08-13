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

#include <cppedal/pwm_output/pwm_output.hpp>
#include <memory>

namespace cppedal::pwm_output {

class RpiZeroSinglePwmOutput : public PwmOutput {
 public:
  RpiZeroSinglePwmOutput();
  ~RpiZeroSinglePwmOutput();

  void output(uint32_t out) override;
};

}  // namespace cppedal::pwm_output

extern "C" {
std::unique_ptr<cppedal::pwm_output::PwmOutput> makePwmOutput();
}