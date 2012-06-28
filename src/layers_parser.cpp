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
// Layers parser cpp.
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#include <stdio.h>
#include "layers_parser.h"
#include "layer.h"
#include "vector.h"
#include "expat.h"
#include "osm_vect.h"


parserLayersXML::parserLayersXML(Vector<LayerXML> *vlayer){

	this->vlayer = vlayer;
	actLayer = NULL;

}

parserLayersXML::~parserLayersXML(){
	if (vlayer) {
		int size = vlayer->size();
		#pragma omp parallel for
		for (int i = 0; i < size; i++){
			LayerXML &layer = (*vlayer)[i];
			delete layer.line.iconid;
			delete layer.line.comment;
		}
	}
	
	delete [] m_buf;
}

void parserLayersXML::start_element(void *data, const char *el, const char **attr){
	
	parserLayersXML& self = *(parserLayersXML*)data;
	
	if(strcmp(el, "layer") == 0)
		self.parse_layerId(el, attr);

	if(strcmp(el, "icons") == 0)
		self.parse_icons(attr);
} 

void parserLayersXML::end_element(void *data, const char *el){
	if(strcmp(el, "layer") == 0)
		return;
	if(strcmp(el, "icons") == 0)
		return;
}
void parserLayersXML::parse_icons(const char** attr){
	for(int iat=0;attr[iat];iat++){
		if(strcmp(attr[iat], "file") == 0 ){
			layersConfig.iconsFile = new char[strlen(attr[iat+1])+1];
			strcpy(layersConfig.iconsFile, attr[iat+1]);
		}
	}
}

void parserLayersXML::parse_layerId(const char *el, const char** attr){
	LayerXML layer;
	layer.line.border_color = layer.line.inner_color = 0;
	for(int iat=0;attr[iat];iat++){
		if(strcmp(attr[iat], "id") == 0 ){
			layer.SetLayerId((char*)attr[iat+1]);
		}else if (strcmp(attr[iat], "name") == 0 ){
			char* val = new char[strlen(attr[iat+1])+1];
			strcpy(val, attr[iat+1]);
			layer.line.comment = val;
		} else if (strcmp(attr[iat], "icon") == 0 ){
			char* val = new char[strlen(attr[iat+1])+1];
			strcpy(val, attr[iat+1]);
			layer.line.iconid = val;
		} else if (strcmp(attr[iat], "layerid") == 0 ){
			layer.line.layerid = atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "type") == 0 ){
			layer.line.type = atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "visible") == 0 ){
			layer.line.visible = atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "draw_texts") == 0 ){
			layer.line.draw_texts = strcmp(attr[iat+1], "true") == 0;
		} else if (strcmp(attr[iat], "color_r") == 0 ){
			layer.line.border_color += atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "color_g") == 0 ){
			layer.line.border_color += (atoi(attr[iat+1]) << 8);
		} else if (strcmp(attr[iat], "color_b") == 0 ){
			layer.line.border_color += (atoi(attr[iat+1]) << 16);
		} else if (strcmp(attr[iat], "fill_color_r") == 0 ){
			layer.line.inner_color += atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "fill_color_g") == 0 ){
			layer.line.inner_color += (atoi(attr[iat+1]) << 8);
		} else if (strcmp(attr[iat], "fill_color_b") == 0 ){
			layer.line.inner_color += (atoi(attr[iat+1]) << 16);
		} else if (strcmp(attr[iat], "near_zoom") == 0 ){
			layer.line.zoom_level_max = (float)atof(attr[iat+1]);
		} else if (strcmp(attr[iat], "far_zoom") == 0 ){
			layer.line.zoom_level_min = (float)atof(attr[iat+1]);
		} else if (strcmp(attr[iat], "pixel_thickness") == 0 ){
			layer.line.width = (float)atof(attr[iat+1]);
		} else if (strcmp(attr[iat], "real_thickness") == 0 ){
			layer.line.width_m = (float)atof(attr[iat+1]);
		} else if (strcmp(attr[iat], "line_type") == 0 ){
			layer.line.line_type = atoi(attr[iat+1]);
		} else if (strcmp(attr[iat], "transparency") == 0 ){
			layer.line.transparency = (float)atof(attr[iat+1]);
		} else if (strcmp(attr[iat], "usable_routing") == 0 ){
			layer.line.Road_class = atoi(attr[iat+1]);
		} else if(strcmp(attr[iat], "max_speed") == 0 ){
			layer.line.Speed = (int)(atoi(attr[iat+1])/3.6);
		} else if(strcmp(attr[iat], "height") == 0 ){
			//Not used
		} else if(strcmp(attr[iat], "parent_layer_id") == 0 ){
			layer.line.parentLayerId =  atoi(attr[iat+1]);
		} else if(strcmp(attr[iat], "route_param") == 0 ){
			layer.line.isRouteParam = strcmp(attr[iat+1], "true") == 0;
		} else if(strcmp(attr[iat], "tunnel") == 0 ){
			layer.line.tunel = strcmp(attr[iat+1], "true") == 0;
		} else if(strcmp(attr[iat], "roundabout") == 0 ){
			layer.line.rondabout = strcmp(attr[iat+1], "true") == 0;
		} else if(strcmp(attr[iat], "route_param") == 0 ){
			layer.line.isRouteParam = strcmp(attr[iat+1], "true") == 0;
		} else if(strcmp(attr[iat], "ramp") == 0 ){
			layer.line.ramp = strcmp(attr[iat+1], "true") == 0;
		} else if(strcmp(attr[iat], "unpaved") == 0 ){
			layer.line.unpaved = strcmp(attr[iat+1], "true") == 0;
		}
	}
	vlayer->push_back(layer);
}
	
void parserLayersXML::content(void *data, const char *el, int len){

}

void parserLayersXML::parseLayersXML(FILE *file){

	XML_Parser p = XML_ParserCreate(NULL);
	XML_SetUserData(p, this);
	XML_SetElementHandler(p, start_element, end_element);
//	XML_SetCharacterDataHandler(p, content);

	long buf_size;
	bool done = false;

	buf_size = 200<<10;
	rewind (file);
	m_buf = new char[buf_size];
	do{
		int len = (int)fread(m_buf, 1, buf_size, file);
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

LayersConfig::LayersConfig(){
	iconsFile = NULL;
}
LayersConfig::~LayersConfig(){
	delete[] iconsFile;
	iconsFile = NULL;
}
