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
// OSM parser cpp.
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#include <stdio.h>
#include "osm_parser.h"
#include "point.h"
#include "way.h"
#include "vector.h"
#include "expat.h"


parser::parser(TReadVectorMapPlugIn *readvectormap, Vector<PointXML> *vp, Vector<WayXML> *vline, TMonitorProcessPlugIn2	&monitor){
	this->readvectormap = readvectormap;
	this->vp = vp;
	this->vline = vline;
	this->cline=0;
	this->point = false;
	this->line =true;
	this->flag_node =false;
	this->flag_way =false;
	this->flag_nd =false;
	this->actWay = NULL;
	this->actPoint = NULL;
	this->Monitor = &monitor;
}

parser::~parser(){
	delete [] m_buf;
}

void parser::start_element(void *data, const char *el, const char **attr){
	
	parser& self = *(parser*)data;
	if(strcmp(el, "node") == 0)	{
		self.parse_point(attr); // parses a point
		self.flag_node=true; // flag_node is true until it findes </node>
    } else if(strcmp(el, "way") == 0){
		self.line_id=(atoi(attr[1])); // line number 
		self.flag_way=true; // flag_way is true until it findes </way>
	} else if(strcmp(el, "nd") == 0){
		self.node_id=atoi(attr[1]); // node number
		self.parse_line(attr, self.line_id, self.node_id); // 
	} else  if(strcmp(el, "tag") == 0){
		if(self.flag_node) {
			self.parse_tagNode(attr);
		} else if(self.flag_way){
			self.parse_tagWay(attr, self.flag_node);
		}
	} else if(strcmp(el, "relation") == 0){
		self.parse_relation();
	} else {
		return;
	}

} 

void parser::end_element(void *data, const char *el){

	parser& self = *(parser*)data;
    if(strcmp(el, "node") == 0) {
		self.flag_node=false;
    } else if(strcmp(el, "way") == 0) {
		self.flag_way=false;
    }

}

void parser::parse_point(const char** attr){

	int i;
	float lat = 0.0;
	float lon = 0.0;
	PointXML points;

	for(int iat=0;attr[iat];iat++){
		if(strcmp(attr[iat], "id") == 0){
			i=atoi(attr[iat+1]);
			points.setPid(i);
		} else if(strcmp(attr[iat], "lat") == 0){
			lat=(float)atof(attr[iat+1]);
			points.setPlat(lat);
		} else if(strcmp(attr[iat], "lon") == 0){
			lon=(float)atof(attr[iat+1]);
			points.setPlon(lon);
		} 
	}
	
	actPoint=vp->push_back(points);
}

void parser::parse_line(const char** attr, int line_id, int node_id){

	if(cline==line_id && actWay){
		actWay->AddPointId(node_id);
	}	
	else{
		WayXML way;
		way.SetId(line_id);	
		actWay = vline->push_back(way);
		actWay->AddPointId(node_id);
		cline=line_id;

	}
}

void parser::parse_tagNode(const char** attr){
	
	for(int iat=0;attr[iat];iat++){
		if(strcmp(attr[iat], "ref") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setName((char*)attr[iat+2]);
		} else if (strcmp(attr[iat], "name") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setName((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "amenity") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "highway") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "tourism") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "atm") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "place") == 0 && attr[iat+1]  && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		} else if(strcmp(attr[iat], "natural") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
			actPoint->setType((char*)attr[iat+2]);
		}
	}
}

void parser::parse_tagWay(const char** attr, bool flag_nd){

	if(cline!=line_id){
			WayXML way;
			way.SetId(line_id);	
			actWay = vline->push_back(way);
			cline=line_id;
	}
	if(!flag_node){
		char layerId[4096];
		for(int iat=0;attr[iat];iat++){
			if(strcmp(attr[iat], "aerialway") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"aerialway_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "aeroway") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"aeroway_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "amenity") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"amenity_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "barrier") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"barrier_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "boundary") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"boundary_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "building") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"building_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "craft") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"craft_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "geological") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"geological_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "highway") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"highway_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "mtb:scale") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetMtbScale(atoi(attr[iat+2]));
			} else if(strcmp(attr[iat], "mtb:scale:uphill") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetMtbUphill(atoi(attr[iat+2]));
			} else if(strcmp(attr[iat], "sac_scale") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetSacScale(atoi(attr[iat+2]));
			} else if(strcmp(attr[iat], "historic") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"historic_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "landuse") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"landuse_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "leisure") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"leisure_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "man_made") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"man_made_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "military") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"military_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "natural") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"natural_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "office") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"office_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "place") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"place_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "power") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"power_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "public_transport") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"public_transport_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "railway") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"railway_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "route") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"route_%s",attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "shop") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"shop_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "sport") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"sport_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "tourism") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"tourism_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "waterway") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				sprintf(layerId,"waterway_%s",(char*)attr[iat+2]);
				actWay->SetLayerId(layerId);
			} else if(strcmp(attr[iat], "ref") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetLineName((char*)attr[iat+2]);
			} else if(strcmp(attr[iat], "name") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetLineName((char*)attr[iat+2]);
			} else if(strcmp(attr[iat], "oneway") == 0 && strcmp(attr[iat+2], "yes") == 0){
				actWay->SetWayDirection(1);
			} else if(strcmp(attr[iat], "oneway") == 0 && strcmp(attr[iat+2], "no") == 0){
				actWay->SetWayDirection(0);
			} else if(strcmp(attr[iat], "oneway") == 0 && strcmp(attr[iat+2], "-1") == 0){
				actWay->SetWayDirection(-1);
			} else if(strcmp(attr[iat], "junction") == 0 && strcmp(attr[iat+2], "roundabout") == 0){
				actWay->SetRoundAbout(1);
				actWay->SetWayDirection(1);
			} else if(strcmp(attr[iat], "tunnel") == 0 && strcmp(attr[iat+2], "yes") == 0){
				actWay->SetTunnel(1);
			} else if(strcmp(attr[iat], "toll") == 0 && strcmp(attr[iat+2], "yes") == 0){
				actWay->SetToll(1);
			} else if(strcmp(attr[iat], "maxspeed") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
				actWay->SetMaxSpeed(atoi(attr[iat+2])/3.6);
			} else if(strcmp(attr[iat], "piste:type") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
	//			actWay->SetPisteType(attr[iat+2]);
			} else if(strcmp(attr[iat], "piste:difficulty") == 0 && attr[iat+1] && strcmp(attr[iat+1], "v") == 0){
	//			actWay->SetPisteDifficulty(attr[iat+2]);
			}
		}
	}
}

void parser::parse_relation(){

}

void parser::parse(FILE *file){

	XML_Parser p = XML_ParserCreate(NULL);
	XML_SetUserData(p, this);
	XML_SetElementHandler(p, start_element, end_element);
//	XML_SetCharacterDataHandler(p, content);

	bool done = false;

	fseek (file , 0 , SEEK_END);
	int fileSize = ftell (file);
	rewind (file);
	if (Monitor){
		Monitor->SetInfo("Reading map");
		Monitor->Start (fileSize);
	}
	int buf_size = 2<<20;
	m_buf = new char[buf_size];
	do
	{
		int len = (int)fread(m_buf, 1, buf_size, file);
		if (Monitor){
			Monitor->Incre(len);
		}
		done = (len < buf_size);
		if(!XML_Parse(p, m_buf, len, done)){
			delete[] m_buf;
			m_buf = NULL;
			XML_ParserFree(p);
			throw "Error de Parsing file";
		}
	}
	while(!done);
	delete[] m_buf;
	m_buf = NULL;
	XML_ParserFree(p);
}