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
// Point h.
//
//----------------------------------------------------------------------------
#if !defined POINT_H
#define POINT_H

class PointXML{
	public:
		PointXML();
		~PointXML();
		void setPid(int);
		void setPlat(float);
		void setPlon(float);
		void setName(char*);
		void setType(char*);
		int getPid();
		float getPlat();
		float getPlon();
		char* getName();
		char* getType();
		void operator = (const PointXML& other);

	private:
		int pid;
		float plat;
		float plon;
		char* pname;
		char* ptype; //type of waypoint
};

#endif //POINT_H