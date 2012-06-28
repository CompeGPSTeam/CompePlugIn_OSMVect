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
// Line h.
//
//----------------------------------------------------------------------------
#if !defined LINE_H
#define LINE_H
#include "vector.h"

// class of integers to be used by the class WayXML to store points of each polyline.
class Int {
	public:
		Int();
		Int(int id);
		void SetId(int id);
		int GetId();
	private:
		int i;
};

// class of polylines and poligons
class WayXML{
	public:
		WayXML();
		WayXML(int wayId);
		~WayXML();
		void AddPointId(int pointId);
		void SetId(int way_id);
		void SetLayerId(char* highway_id);
		void SetLineName(char* line_nm);
		void SetWayDirection(int way_dir);
		void SetRoundAbout(int rnd_about);
		void SetTunnel(int tun);
		void SetMtbScale(int mtb_scale);
		void SetMtbUphill(int mtb_uphill);
		void SetSacScale(int sac_scale);
		void SetMaxSpeed(int max_speed);
		void SetToll(int has_toll);
		int GetId();
		char* GetLayerId();
		char* GetLineName();
		int GetRoundAbout();
		int GetTunnel();
		int GetWayDirection();
		int GetMtbScale();
		int GetMtbUphill();
		int GetSacScale();
		int GetMaxSpeed();
		int GetToll();
		Vector<Int> pointsId;
		void operator = (const WayXML& other);
	private:
		int lid; // way id
		char* layerId;
		char* lineName;
		int rndAbout;
		int tunnel;
		int wayDir; // road traffic direction
		int mtbScale; // MTB Downhill track dificulty
		int mtbUphill; // MTB Uphill track dificulty
		int sacScale; // type of walking track
		int speed; // max speed
		int toll;
};

#endif //LINE_H