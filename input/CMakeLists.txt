#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/input)

# Input
add_subdirectory(rpi)
add_subdirectory(rpizero)

add_library(cppedal-input INTERFACE)

target_include_directories(
  cppedal-input
  INTERFACE
  include/
)

