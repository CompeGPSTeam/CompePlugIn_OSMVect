/*
   Copyright CompeGPS Team SL(www.compegps.com)

   Licensed under the GNU Lesser General Public License (LGPL)
   You may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.gnu.org/licenses/lgpl.html

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

//----------------------------------------------------------------------------
//
// Quicksort h.
//
//----------------------------------------------------------------------------
#if !defined QUICKSORT_H
#define QUICKSORT_H

#include "vector.h"
#include "point.h"


int divide(Vector<PointXML>&,int beg, int end, int pivot);
void quicksort(Vector<PointXML>&,int beg, int end);

#endif