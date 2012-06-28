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
// Class layer
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#include "layer.h"
#include "vector.h"


LayerXML::LayerXML(){
	layId=NULL;
}

LayerXML::~LayerXML(){
	delete layId;
}

void LayerXML::SetLayerId(char* id){
	delete layId;
	layId=new char[strlen(id)+1];
	strcpy(layId,id);
}

char* LayerXML::GetLayerId(){
	return layId;
}

void LayerXML::operator = (const LayerXML& other){
	delete layId;
	if (other.layId){
		layId=new char[strlen(other.layId)+1];
		strcpy(layId,other.layId);
	}
	this->line = other.line;

}

