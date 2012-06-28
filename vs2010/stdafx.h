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

#if !defined STDAFX_H
#define STDAFX_H

#if !XML_BUILDING_EXPAT
#include <windows.h>


#if defined _DEBUG 
	//#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
	//#define malloc(__nNumberOfBytesToMalloc) \
	//   _malloc_dbg(__nNumberOfBytesToMalloc, _NORMAL_BLOCK, __FILE__, __LINE__);
#endif 
#endif

#define COMPILED_FROM_DSP

#endif // STDAFX_H