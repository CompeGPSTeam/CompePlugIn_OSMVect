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

#if !defined OSM_VECT_H
#define OSM_VECT_H

class OSMPluginData{
	public:
		OSMPluginData(){};
		~OSMPluginData(){};

		//Data
		FILE* rFile;
};

class LayerXMLData{
	public:
		LayerXMLData(){};
		~LayerXMLData(){};

		//Data
		FILE* nFile;
};

#endif // OSM_VECT_H
