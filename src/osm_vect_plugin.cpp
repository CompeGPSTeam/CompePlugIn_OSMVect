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

#include "stdafx.h"

#include "stdio.h"
#include "layers_parser.h"
#include "osm_vect.h"
#include "osm_parser.h"
#include "point.h"
#include "way.h"
#include "layer.h"
#include "quicksort.h"
#include "Include_DLL_PlugInMapas.h"

// Searches the node that contains coordinates of that node on the points Vector. It returns a point object.
 bool searchNode(Vector<PointXML> &vp, Int v_node, TVectorPointPlugIn &point){

	int left=0;
	int right=vp.size();

	while (left <= right) {
		int middle = (left + right) / 2;
		if (vp[middle].getPid() == v_node.GetId()){
			point.x=vp[middle].getPlon();
			point.y=vp[middle].getPlat();
			point.z=0.0;
			return true;
		}
		else if (vp[middle].getPid() > v_node.GetId())
			right = middle - 1;
		else
			left = middle + 1;

      }
	  return false;

}


 bool nodeExists(Vector<PointXML> &vp, Int v_node){

	int left=0;
	int right=vp.size();

	while (left <= right) {
		int middle = (left + right) / 2;
		if (vp[middle].getPid() == v_node.GetId()){
			return true;
		}
		else if (vp[middle].getPid() > v_node.GetId())
			right = middle - 1;
		else
			left = middle + 1;

      }
	  return false;

}


// Searches the layer to apply to a line on the Vector vlayer. This function returns a line object.
TLineAttributesPlugIn searchLayer(Vector<LayerXML> &vlayer, char* layerId){
	
	int lsize=vlayer.size();
	int i=0;
	char *lteste;
	lteste=layerId;
	TLineAttributesPlugIn line_s;
	if (layerId){
		while (i<lsize) {
			if (strcmp(vlayer[i].GetLayerId(),layerId) == 0) {
				line_s=vlayer[i].line;
				return line_s;
			}
			else
				i++;
		}
	}
	line_s=vlayer[lsize-1].line;
    return line_s;
}

//*************************************************************
//
//************************************************************
int EXPORTW CompePlugIn_Version(){
	return VERSION_PLUGIN_COMPE;
}

//*************************************************************************
//*
//*************************************************************************
int EXPORTW CompePlugIn_VersionPriv(){
	return 1;
}
//*************************************************************
//*
//*************************************************************
bool EXPORTW CompePlugIn_Init (int hWnd){
    return true;
}
//*************************************************************
//*
//*************************************************************
EXPORTW const char* CompePlugIn_GetTipoArchivos (){
	return "6|Open Street Maps|osm|"
		   "2|Open Street Maps|osm|";
}
//*************************************************************
//*

//*************************************************************
EXPORTW const char * CompePlugIn_VersionStr (){
	return "CompeGPS OSM Plug-In, version 1.00";
}
//*************************************************************************
//* Open File
//*************************************************************************
bool EXPORTW CompePlugIn_Open_Vector_Map (
			const char*			map_name,
			TVectorMapPlugIn&	vectormap ,
			TStringPlugIn& 		_err ){
	
	OSMPluginData *pluginData = new OSMPluginData;
	
	pluginData->rFile = fopen (map_name,"r"); 
	if (!pluginData->rFile)	{
		fclose (pluginData->rFile);
		delete pluginData;
		return false;
	};
	vectormap.handle = (HANDLEPLUGIN) pluginData; //passes handle the address of rFile				
	return true;
}

//*************************************************************************
//* Read File
//* 2012-02-20 [#1] DNP mantis13374: Pasamos el archivo de config de capas del OSM por parametro
//*************************************************************************
bool EXPORTW CompePlugIn_Read_Vector_Map3 (
			TVectorMapPlugIn&			vectormap ,
			TReadVectorMapPlugIn&		readvectormap ,
			TStringPlugIn& 				_err,
			TMonitorProcessPlugIn2	&Monitor){
	
	// open xml layers file
	LayerXMLData *xmlData = new LayerXMLData;

	char *xmlFile = NULL;
	if (readvectormap.strparams) { // [#1]
		const char *layer_param = "layers_config=";
		const char *xmlini = strstr(readvectormap.strparams,layer_param);
		const char *xmlfin = xmlini?strstr(xmlini,"@@"):NULL;

		if (xmlini && xmlfin) {
			xmlini += strlen(layer_param);
			xmlFile = new char[xmlfin - xmlini + 10];
			memcpy (xmlFile, xmlini, xmlfin - xmlini);
			xmlFile[xmlfin - xmlini] = 0;
		};
	};

	if (!xmlFile) {
		// Tratamiento por defecto
		xmlFile = new char[strlen(vectormap.path_exe) + 100];

		#if defined UNIX
			char path_char = '/';
		#else
			char path_char = '\\';
		#endif
		sprintf(xmlFile,"%s%cplugins%cOSM%cLayersOSMPlugin.xml", vectormap.path_exe, path_char, path_char, path_char);
	};

	xmlData->nFile = fopen (xmlFile,"r");
	delete[] xmlFile;
	if (!xmlData->nFile)	{
		delete xmlData;
		_err.Set ("File LayersOSMPlugin.xml Not Found");
		return false;
	}
	Vector<LayerXML> vlayer(256, 64); // vector vlayer will store layers name and the object of type TLineAttributesPlugIn
	parserLayersXML pl(&vlayer); // instatiates pl with the address of vlayer
	pl.parseLayersXML(xmlData->nFile); //parses xml layer file

	fclose(xmlData->nFile);
	delete xmlData;
	
	//New pass the pl.layersConfig to Compe as params. TODO:CJRAMS Mantis 12950

	OSMPluginData* pluginData = (OSMPluginData *)vectormap.handle; 
	Vector<PointXML> vp(0x100000, 0x100000); // vector vp stores points
	Vector<WayXML> vline(0x10000, 0x10000); // vector vline stores lines id and points coordinates
	
	// parser OSM Map
	parser p(&readvectormap,&vp,&vline, Monitor); //instatiates p with the address of readverctormap, vp and vline
	p.parse(pluginData->rFile);
	
	// sort vector points
	quicksort(vp,0,vp.size()-1);
	
	TVectorPointPlugIn point;
	TLineAttributesPlugIn line;

	// creates layers
	for(int i=0; i<vlayer.size(); i++){
		line=searchLayer(vlayer,vlayer[i].GetLayerId());
		readvectormap.NewLayer (line);
	}

	int cntPoints = 0; // counts number of points per line

	// draws lines with it's respective layer
	Monitor.SetInfo("Creating Layers");
	Monitor.Start (vline.size());
	for(int i=0;i<vline.size();i++){ 
		Monitor.Incre(1);
		line=searchLayer(vlayer,vline[i].GetLayerId()); // searches for layer
		line.databaseid = vline[i].GetId(); // line id
		line.One_way = vline[i].GetWayDirection(); // way direction
		line.NodeID_start=vline[i].pointsId[0].GetId();
		line.NodeID_end=vline[i].pointsId[vline[i].pointsId.size()-1].GetId();

		// draws polygons
		if((line.type==1 || line.type==7) && line.visible!=-1){
			int lineNum = readvectormap.NewLine (line);
			cntPoints = 0; 
			
			// verifies if all the points from a line exists on vector points
			for(int j=0;j<vline[i].pointsId.size();j++){
					if(nodeExists(vp, vline[i].pointsId[j]))
						cntPoints++;
				}
			
			if(cntPoints>1){
				// searches for nodes of each line
				for(int j=0;j<vline[i].pointsId.size();j++){
					if (searchNode(vp, vline[i].pointsId[j], point)) { 
						if(j!=vline[i].pointsId.size()-1)
							readvectormap.NewPoint (lineNum, point, false);
						else
							readvectormap.NewPoint (lineNum, point, true);
					}
				}
			}
		}
	}
	
	// draw polylines
	char layerName[4096];
	Monitor.SetInfo("Creating map");
	Monitor.Start (vline.size());
	for(int i=0;i<vline.size();i++){
		Monitor.Incre(1);
		if (vline[i].GetLayerId() && strcmp(vline[i].GetLayerId(),"shop_supermarket")==0 )
			int a=1234;
		// attributes layer to roundabout
		if(vline[i].GetLayerId() && (strcmp(vline[i].GetLayerId(),"highway_motorway")==0 || strcmp(vline[i].GetLayerId(),"highway_trunk")==0 || strcmp(vline[i].GetLayerId(),"highway_primary")==0 || 
			strcmp(vline[i].GetLayerId(),"highway_secondary")==0 || strcmp(vline[i].GetLayerId(),"highway_tertiary")==0 || strcmp(vline[i].GetLayerId(),"highway_unclassified")==0 || 
			strcmp(vline[i].GetLayerId(),"highway_road")==0 || strcmp(vline[i].GetLayerId(),"highway_track")==0 || strcmp(vline[i].GetLayerId(),"highway_service")==0 ||
			strcmp(vline[i].GetLayerId(),"highway_residential")==0) && vline[i].GetRoundAbout()==1){
				sprintf(layerName,"round_%s",vline[i].GetLayerId());
				line=searchLayer(vlayer,layerName); // attributes roundabout layer to line
			}
		// attributes layer to tunnel
		else
		if(vline[i].GetLayerId() && (strcmp(vline[i].GetLayerId(),"highway_motorway")==0 || strcmp(vline[i].GetLayerId(),"highway_trunk")==0 || strcmp(vline[i].GetLayerId(),"highway_primary")==0 || 
			strcmp(vline[i].GetLayerId(),"highway_secondary")==0 || strcmp(vline[i].GetLayerId(),"highway_tertiary")==0 || strcmp(vline[i].GetLayerId(),"highway_unclassified")==0 || 
			strcmp(vline[i].GetLayerId(),"highway_road")==0 || strcmp(vline[i].GetLayerId(),"highway_track")==0 || strcmp(vline[i].GetLayerId(),"highway_service")==0 ||
			strcmp(vline[i].GetLayerId(),"highway_residential")==0 || strcmp(vline[i].GetLayerId(),"highway_path")==0 || strcmp(vline[i].GetLayerId(),"highway_drain")==0
			|| strcmp(vline[i].GetLayerId(),"highway_pedestrian")==0 || strcmp(vline[i].GetLayerId(),"highway_footway")==0) && vline[i].GetTunnel()==1){
				sprintf(layerName,"tunnel_%s",vline[i].GetLayerId());
				line=searchLayer(vlayer,layerName); // attributes tunnel layer to line
			}
		else
			line=searchLayer(vlayer,vline[i].GetLayerId()); // attributes layer to line
		
		line.databaseid = vline[i].GetId(); 
		line.One_way = vline[i].GetWayDirection();
		if(vline[i].GetMaxSpeed()>0)
			line.Speed = vline[i].GetMaxSpeed();
		line.Toll = vline[i].GetToll();

		if(vline[i].GetLineName() != NULL)
			line.comment = vline[i].GetLineName();
		else
			line.comment = NULL;
		
		//draws <way>
		if(line.type==0 && line.visible!=-1){
			int lineNum = readvectormap.NewLine (line);	

			// drawing roudabouts. It converts one line roundabout in two lines.
			// this condition verifies if it is a roundabout and also if the first and last point are equal)
			if(vline[i].GetRoundAbout()==1 && (line.NodeID_start == line.NodeID_end)){
				// this loop will draw until the second last point
				for(int j=0;j<vline[i].pointsId.size()-1;j++){ 
					if (searchNode(vp, vline[i].pointsId[j], point)) { 
						if(j!=vline[i].pointsId.size()-2)
							readvectormap.NewPoint (lineNum, point, false);
						else
							readvectormap.NewPoint (lineNum, point, true);
					}
				}
				//this loop will draw from the second last to last point
				int lineNum = readvectormap.NewLine (line);
				for(int j=vline[i].pointsId.size()-2;j<vline[i].pointsId.size();j++){ 
					if (searchNode(vp, vline[i].pointsId[j], point)) { 
						if(j!=vline[i].pointsId.size()-1)
							readvectormap.NewPoint (lineNum, point, false);
						else
							readvectormap.NewPoint (lineNum, point, true);
					}
				}
 			}
			else{
			// draws normal way
				//verifies if the point from a line is in list of points.
				cntPoints = 0; 
				for(int j=0;j<vline[i].pointsId.size();j++){
					if(nodeExists(vp, vline[i].pointsId[j]))
						cntPoints++;
				}

				if(cntPoints>1){				
					for(int j=0;j<vline[i].pointsId.size();j++){
						if (searchNode(vp, vline[i].pointsId[j], point)) { 
							if(j!=vline[i].pointsId.size()-1)
								readvectormap.NewPoint (lineNum, point, false);
							else
								readvectormap.NewPoint (lineNum, point, true);
						}
					}

					char lName[32];
					// draws mtb:scale
					if(vline[i].GetMtbScale()!=-1 && vline[i].GetMtbUphill()==-1){
						sprintf(lName,"highway_mtb:scale_%d",vline[i].GetMtbScale());
						line=searchLayer(vlayer,lName);
						int lineNum = readvectormap.NewLine (line);
						for(int j=0;j<vline[i].pointsId.size();j++){
							if (searchNode(vp, vline[i].pointsId[j], point)) { 
								if(j!=vline[i].pointsId.size()-1)
									readvectormap.NewPoint (lineNum, point, false);
								else
									readvectormap.NewPoint (lineNum, point, true);
							}
						}
					}
					// draws mtb:scale:uphill
					if(vline[i].GetMtbUphill()!=-1){
						sprintf(layerName,"highway_mtb:scale:uphill_%d",vline[i].GetMtbUphill());
						line=searchLayer(vlayer,layerName);
						int lineNum = readvectormap.NewLine (line);

						if(vline[i].GetMtbScale()<0){
							for(int j=0;j<vline[i].pointsId.size();j++){
								if (searchNode(vp, vline[i].pointsId[j], point)) { 
									if(j!=vline[i].pointsId.size()-1)
										readvectormap.NewPoint (lineNum, point, false);
									else
										readvectormap.NewPoint (lineNum, point, true);
								}
							}
						}else{
							for(int j=vline[i].pointsId.size()-1; j>-1; j--){
								if (searchNode(vp, vline[i].pointsId[j], point)) {
									if(j!=0)
										readvectormap.NewPoint (lineNum, point, false);
									else
										readvectormap.NewPoint (lineNum, point, true);
								}
							}
						}
					}
					// draws sac_scale
					if(vline[i].GetSacScale()!=-1 && vline[i].GetMtbScale()==-1 && vline[i].GetMtbUphill()==-1){
						sprintf(layerName,"highway_sac_scale_%d",vline[i].GetSacScale());
						line=searchLayer(vlayer,layerName);
						int lineNum = readvectormap.NewLine (line);
						for(int j=0;j<vline[i].pointsId.size();j++){
							if (searchNode(vp, vline[i].pointsId[j], point)) { 
								if(j!=vline[i].pointsId.size()-1)
									readvectormap.NewPoint (lineNum, point, false);
								else
									readvectormap.NewPoint (lineNum, point, true);
							}
						}
					}
				}
			}
		}
	}


	// draws waypoints
	Monitor.SetInfo("Creating map points");
	Monitor.Start (vline.size());
	for(int i=0;i<vp.size()-1;i++){
		Monitor.Incre(1);
		if(vp[i].getType()!=NULL){
			line=searchLayer(vlayer,vp[i].getType());
			if((line.type==3 || line.type==4) && line.visible!=-1){
				line.comment=vp[i].getName();
				int lineNum = readvectormap.NewLine (line);
				point.x=vp[i].getPlon();
				point.y=vp[i].getPlat();
				point.z=0.0;
				readvectormap.NewPoint (lineNum, point, true);
			}
		}
	}
	Monitor.Start (1);
	Monitor.SetInfo("Finishing");
	return true;
};

//*************************************************************************
//* Calibrate map
//*************************************************************************

bool EXPORTW CompePlugIn_Lee_Calibracion(const char *map_name,
					TStringPlugIn &fichero_IMP2,TStringPlugIn &err){


	const char *imp =	"CompeGPS MAP File\r\n"
						"<Header>\r\n"
						"Version=2\r\n"
						"VerCompeGPS=6.2.n\r\n"
						"Projection=1,Lat/Long,\r\n"
						"ProjectionToView=2,Mercator\r\n"
						"Coordinates=0\r\n"
						"Datum=WGS84\r\n"
						"</Header>\r\n"
						"<Map>\r\n"
						"VectorMap=%s\r\n"
						"ScaleMpix=1\r\n"
						"BitmapWidth=-1\r\n"
						"BitmapHeight=-1\r\n"
						"</Map>\r\n"
						"<Calibration>\r\n"
						"P0=-1.00000000,-1.00000000,00M,0.00000000,0.00000000\r\n"
						"P1=-1.00000000,-1.00000000,00M,0.00000000,0.00000000\r\n"
						"</Calibration>\r\n";
	char *imp2 = new char[strlen(imp) + strlen(map_name) + 10];
	sprintf(imp2, imp, map_name);
	fichero_IMP2.Set (imp2);
	delete[] imp2;
	return true;
}


//*************************************************************************
//* Close File
//*************************************************************************
bool EXPORTW CompePlugIn_Close_Vector_Map(TVectorMapPlugIn&	vectormap,
										  TStringPlugIn& _err){
	
	OSMPluginData * pluginData = (OSMPluginData *)vectormap.handle; 
	fclose(pluginData->rFile);
	delete pluginData;
	vectormap.handle = 0;
	return true;
}