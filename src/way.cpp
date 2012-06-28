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
// Class Line
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#include "way.h"

Int::Int(){
	i=0;
}

Int::Int(int id){
	i=id;
}

void Int::SetId(int id){
	i=id;
}

int Int::GetId(){
	return i;
}


//Classe Way XML
WayXML::WayXML():pointsId(32, 16){
	lid = 0;
	layerId = NULL;
	lineName = NULL;
	rndAbout = 0;
	tunnel = 0;
	wayDir = 0;
	mtbScale = -1;
	mtbUphill = -1;
	sacScale = -1;
	toll = 0;
	speed = 0;
}

WayXML::WayXML(int wayId):pointsId(32, 16){
	lid = wayId;
	layerId = NULL;
	lineName = NULL;
	rndAbout = 0;
	tunnel = 0;
	wayDir = 0;
	mtbScale = -1;
	mtbUphill = -1;
	sacScale = -1;
	toll = 0;
	speed = 0;
}

void WayXML::SetId(int way_id){
	lid = way_id;
}

void WayXML::SetLayerId(char* hway_id){
	delete layerId;
	layerId=new char[strlen(hway_id)+1];
	strcpy(layerId,hway_id);
}

void WayXML::SetLineName(char* line_nm){
	delete lineName;
	lineName = new char[strlen(line_nm)+1];
	strcpy(lineName,line_nm);
}

void WayXML::SetWayDirection(int way_dir){
	wayDir = way_dir;
}

void WayXML::SetRoundAbout(int rnd_about){
	rndAbout = rnd_about;
}

void WayXML::SetTunnel(int tun){
	tunnel = tun;
}

void WayXML::SetMtbScale(int mtb_scale){
	mtbScale = mtb_scale;
}

void WayXML::SetMtbUphill(int mtb_uphill){
	mtbUphill = mtb_uphill;
}

void WayXML::SetSacScale(int sac_scale){
	sacScale = sac_scale;
}

void WayXML::SetMaxSpeed(int max_speed){
	speed = max_speed;
}

void WayXML::SetToll(int has_toll){
	toll = has_toll;
}

int WayXML::GetId(){
	return lid;
}

char* WayXML::GetLayerId(){
	return layerId;
}

char* WayXML::GetLineName(){
	return lineName;
}

int WayXML::GetWayDirection(){
	return wayDir;
}

int WayXML::GetRoundAbout(){
	return rndAbout;
}

int WayXML::GetTunnel(){
	return tunnel;
}

int WayXML::GetMtbScale(){
	return mtbScale;
}

int WayXML::GetMtbUphill(){
	return mtbUphill;
}

int WayXML::GetSacScale(){
	return sacScale;
}

int WayXML::GetMaxSpeed(){
	return speed;
}

int WayXML::GetToll(){
	return toll;
}

void WayXML::AddPointId(int pointId){
	Int i(pointId);
	pointsId.push_back(i);

}

void WayXML::operator = (const WayXML& other){
	this->lid = other.lid;
	this->pointsId = other.pointsId;

	delete layerId;
	if (other.layerId){
		layerId=new char[strlen(other.layerId)+1];
		strcpy(layerId,other.layerId);
	}

	delete lineName;
	if (other.lineName) {
		lineName = new char[strlen(other.lineName)+1];
		strcpy(lineName,other.lineName);
	}
}


WayXML::~WayXML(){
	delete layerId;
	delete lineName;
}