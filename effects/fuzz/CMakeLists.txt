#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#

add_library(
  cppedal-effect-fuzz
  SHARED
  fuzz_library.cpp
)


target_link_libraries(
  cppedal-effect-fuzz
  PUBLIC
  cppedal-effects
)
