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
// Layers parser h.
//
//----------------------------------------------------------------------------

#if !defined LAYERS_PARSER_H
#define LAYERS_PARSER_H

#include "Include_DLL_PlugInMapas.h"
#include "vector.h"

class LayerXML;
class LayersConfig{
public:
	LayersConfig();
	~LayersConfig();
	char *iconsFile;
};
class parserLayersXML{

public:
	parserLayersXML(Vector<LayerXML> *vlayer);
	~parserLayersXML();
	void parseLayersXML(FILE *file);

private:
    // XML event handlers
	static void start_element(void* data, const char* el, const char** attr);
	static void end_element(void* data, const char* el);
	static void content(void* data, const char* s, int len);

	void parse_layerId(const char* el, const char** attr);
	void parse_icons(const char** attr);
	char* m_buf;
	Vector<LayerXML> *vlayer;
	LayerXML *actLayer;
	LayersConfig layersConfig;

};

#endif //LAYERS_PARSER_H