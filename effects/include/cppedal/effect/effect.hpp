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

#include <json.hpp>

namespace cppedal::effects {
  
struct EffectConfig {
  std::string name;
  std::string path;
};

class Effect {
  public:
    explicit Effect(const EffectConfig& cfg);
    virtual ~Effect() = 0;

    /**
     * @brief Each effect will take in a int64_t input signal
     *        and return a int64_t signal. for example for a
     *        clean/tranparent effect would be
     *        ```
     *        inline int64_t process(int64_t in) { return in; }
     *        ```
     * @exception InvalidInput (if I do exceptions)
     * 
     * @param in signal in 
     * @return int64_t  signal out
     */
    virtual int64_t process(int64_t in) = 0;
};

} // namespace cppedal::effects