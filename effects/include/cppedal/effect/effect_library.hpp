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
  
struct EffectLibraryConfig {
  std::string name;
  std::string path;
};

class EffectLibrary {
 private:
  std::string name_;
  std::string path_;

 public:
  explicit EffectLibrary(const EffectLibraryConfig& cfg);
   virtual ~EffectLibrary() = 0;

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

  inline const std::string& getName() const { return name_; }
  inline const std::string& getPath() const { return path_; }

};

} // namespace cppedal::effects