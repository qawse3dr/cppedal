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
  cppedal-lcd-1602
  SHARED
  lcd_1602.cpp
)

target_link_libraries(
  cppedal-lcd-1602
  PUBLIC
  lcd
  wiringPiDev
  wiringPi
)