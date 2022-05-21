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

#include "framer.hpp"

#include <dlfcn.h>
#include <iostream>

using cppedal::framer::Framer;

Framer::Framer(const std::string& cfg_path) {
  
  void* handler = dlopen("lib/effects/libcppedal-effect-clean.so", RTLD_NOW);
  if(!handler) {
    fprintf(stderr, "%s\n", dlerror());
               exit(EXIT_FAILURE);
  }

  makeEffectLibraryFtn effectFtn = (makeEffectLibraryFtn) dlsym(handler, CPPEDAL_MAKE_EFFECT_LIBRARY_NAME);
  
  cppedal::effects::EffectLibraryConfig cfg;
  cfg.name = "clean";
  cfg.path = "path";

  {
    auto effectLibrary = effectFtn(cfg);
  std::cout << effectLibrary->process(200) << std::endl;
  std::cout << effectLibrary->getName() << " " << effectLibrary->getPath() << std::endl;
  }
  dlclose(handler);
}

bool Framer::start() { return true; }

void Framer::stop() {}