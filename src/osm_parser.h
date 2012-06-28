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
// OSM parser h.
//
//----------------------------------------------------------------------------

#if !defined OSM_PARSER_H
#define OSM_PARSER_H

#include "Include_DLL_PlugInMapas.h"
#include "vector.h"

class TReadVectorMapPlugIn;
class PointXML;
class WayXML;

class parser{

public:
	parser(TReadVectorMapPlugIn *readvectormap, Vector<PointXML> *vp, Vector<WayXML> *vline, TMonitorProcessPlugIn2	&monitor);
	~parser();
	void parse(FILE *file);

private:
    // XML event handlers
	static void start_element(void* data, const char* el, const char** attr);
	static void end_element(void* data, const char* el);
	static void content(void* data, const char* s, int len);

	void parse_point(const char** attr);
	void parse_line(const char** attr, int line_id, int node_id);
	void parse_tagNode(const char** attr);
	void parse_tagWay(const char** attr, bool flag_nd);
	void parse_relation();
	TReadVectorMapPlugIn *readvectormap;
    bool point;
	bool line;
	bool flag_node;
	bool flag_way;
	bool flag_nd;
	int line_id;
	int node_id;
	int cline; //line number controler
	char* m_buf;
	Vector<PointXML> *vp;
	Vector<WayXML> *vline;
	WayXML *actWay;
	PointXML *actPoint;
	TMonitorProcessPlugIn2	*Monitor;

};

#endif //OSM_PARSER_H