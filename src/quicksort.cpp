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

#include "stdafx.h"


//----------------------------------------------------------------------------
//
// Quicksort h.
//
//----------------------------------------------------------------------------
#include "quicksort.h"

int divide(Vector<PointXML> &vp, int beg, int end, int pivot){
	
	int left = beg;
	int right = end;
	PointXML temp;
	int pVal = vp[pivot].getPid();

	// While left and right index don't find each other 
	while(left<right){
		while(vp[right].getPid()>pVal){
			right--;
		}
		while(vp[left].getPid()<pVal){
			left++;
		}

		// change of members
		if(left < right){
			temp = vp[left];
			vp[left] = vp[right];
			vp[right] = temp;
	    }
	  }
	 
	  //New position of pivot
	  return right;
}

void quicksort(Vector<PointXML> &vp, int beg, int end){
	  if(beg < end){
		int pivot = (beg + end) >> 1;
	    divide(vp, beg , end, pivot );
	    quicksort( vp, beg, pivot - 1 );//ordeno la lista de los menores
	    quicksort( vp, pivot + 1, end );//ordeno la lista de los mayores
	  }
}
