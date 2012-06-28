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
// Class Point
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#include "point.h"
#include "vector.h"


PointXML::PointXML(){
	pid		= 0;
	plat	= 0.0;
	plon	= 0.0;
	pname	= NULL;
	ptype	= NULL;
}

PointXML::~PointXML(){
	delete pname;
	delete ptype;
}

void PointXML::setPid(int id){
	pid = id;
}

void PointXML::setPlat(float lat){
	plat = lat;
}

void PointXML::setPlon(float lon){
	plon = lon;
}

void PointXML::setName(char* name){
	delete pname;
	pname=new char[strlen(name)+1];
	strcpy(pname,name);
}

void PointXML::setType(char* type){
	delete ptype;
	ptype=new char[strlen(type)+1];
	strcpy(ptype,type);
}

int PointXML::getPid(){
	return pid;
}

float PointXML::getPlon(){
	return plon;
}

float PointXML::getPlat(){
	return plat;
}

char* PointXML::getName(){
	return pname;
}

char* PointXML::getType(){
	return ptype;
}

void PointXML::operator = (const PointXML& other){
	pid		= other.pid;
	plat	= other.plat;
	plon	= other.plon;

	delete pname;
	if (other.pname){
		pname=new char[strlen(other.pname)+1];
		strcpy(pname,other.pname);
	}

	delete ptype;
	if (other.ptype) {
		ptype = new char[strlen(other.ptype)+1];
		strcpy(ptype,other.ptype);
	}

}
