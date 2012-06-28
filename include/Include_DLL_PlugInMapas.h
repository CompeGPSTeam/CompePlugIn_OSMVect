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

//*************************************************************
//*       Include_DLL_PlugInMapas.h
//*
//*  This file contains the clases and functions that a
//* PlugIn for CompeGPS should have.
//*
//*------------------------------------------------------------
//*   Version 1.0    Created on 2003-6-28
//*                 It is only compatible with CompeGPS ver. 5.22.h
//*   Version 2.0    Created on 2003-7-3: Compatible with CompeGPS 5.3 to 5.61
//*   Version 3.0    Created on 2004-7-19
//*   Version 4.0    Created on 2004-8-30
//*   Version 5.0    Created on 2012-2-7  Using 'TMonitorProcessPlugIn2'
//*************************************************************

// * EL MODULO FUE DESTRUIDO POR un CONTROL-D del BORLAND el dia 1-6-2010 (xtarafa)
// * Recuerdo que el CNTROL-D del Borland jode todos los comentarios, alineaciones, etc.
// * ITV: Yo ya veo mal, pero si me lo desalineas asi, aun me cuesta mas!!.




#if !defined INCLUDE_DLL_PLUGINMAPAS_H
#define		 INCLUDE_DLL_PLUGINMAPAS_H

#define VERSION_PLUGIN_COMPE 5

#if defined ITV_USA_LINUX
	#define EXPORTW  // Revisar como hacerlo en Lixux.
	#if !defined __int64
		#define __int64 long long
	#endif
	#if !defined NULL
		#define NULL 0
	#endif
#endif

#if !defined EXPORTW

#define EXPORTW __declspec(dllexport)

#endif

class TDatosZonaPlugIn;
class TStringPlugIn;
class TDibujoVectorial;
class TInternetPlugInData;
class TDatosDibPlugIn;
class TRegistroPlugInData;

#if defined ITV_USA_WINDOWS
#define MSCDECLC _cdecl
#else
#define MSCDECLC
#endif
#if defined _WIN64
	#define HANDLEPLUGIN __int64 
#else
	#define HANDLEPLUGIN int 
#endif

/*   2012-2-7 ITV eliminado del todo. mantis9118
// *************************************************************
// *
// * 2004-6-19 created
// * 2011-5-1 ITV renombro TMonitorProcessPlugIn por TMonitorProcessPlugIn_KK
// *				 ya que debemos dejarla de usar!. (mantis9118)
// *************************************************************
class TMonitorProcessPlugIn__KK {
	public:
		virtual void MSCDECLC Start(int numsteps) = 0;
		virtual void MSCDECLC Incre(int incsteps = 1) = 0;
		virtual void MSCDECLC SetInfo(const char *str) = 0;
		virtual bool MSCDECLC Stop() = 0;
		void *dataK; // Private data. Dont use.
};
*/

class TMonitorProcessPlugIn2;
typedef bool(*TPtrFuncMonitor)(TMonitorProcessPlugIn2 * athis, int functype,
	HANDLEPLUGIN param);

//*************************************************************
//*
//* 2007-10-6 created
//*************************************************************
class TMonitorProcessPlugIn2 {
	public:
		TMonitorProcessPlugIn2() {
			maskid         = 0xABCD1234; // codigo identificador de esta clase nueva.
			size 		   = sizeof(this);
			data 		   = NULL;  // Private data. Dont use.
			PtrFuncMonitor = NULL;
		};

		// Functions
		void Start(int numsteps) {
			if (PtrFuncMonitor)
				PtrFuncMonitor(this, 1, numsteps);
		};

		void Incre(int incsteps = 1) {
			if (PtrFuncMonitor)
				PtrFuncMonitor(this, 2, incsteps);
		};

		void SetInfo(const char* str) {
			if (PtrFuncMonitor)
				PtrFuncMonitor(this, 3, (HANDLEPLUGIN)str);
		};

		bool Stop() {
			if (!PtrFuncMonitor)
				return false;
			return PtrFuncMonitor(this, 4, 0);
		};

		// Data
		int maskid;   // Ha de ser el primer elemento, los plugins asi saben q clase es.
		int size;
		void* data; // Private data. Dont use.
		TPtrFuncMonitor PtrFuncMonitor;
};

typedef void(*Tydef_CallbackCreateDIB)(TDatosZonaPlugIn & datos);
typedef void(*Tydef_SetString)(TStringPlugIn & str1, const char*str2);
typedef void* (*TypedefMalloc)(int len);
typedef bool(*Tydef_DownloadHTTP)(void*Pdata, TInternetPlugInData & data);
typedef bool(*Tydef_GetMapKey)(void*Pdata, TRegistroPlugInData & data);
typedef bool(*Tydef_DeleteMapKey)(void*Pdata, TRegistroPlugInData & data);

#if defined ITV_USA_SYMBIAN
typedef int(*FARPROCSYMB)();

class TPlugInLibSymbian {
public:
	FARPROCSYMB Version;
	FARPROCSYMB VersionPriv;
	FARPROCSYMB VersionStr;
	FARPROCSYMB Init;
	FARPROCSYMB GetTipoArchivos;
	FARPROCSYMB Abre_Raster;
	FARPROCSYMB Abre_Zona;
	FARPROCSYMB Cierra_Raster;
	FARPROCSYMB Lee_Calibracion;
	FARPROCSYMB SetTempFolder;
};
#endif

#if !defined ITV_USA_SYMBIAN
#if defined ITV_USA_EVC
// Windows CE and others:  Pack to 4.
#pragma pack(push, 4)
#else
// PC Version: By historical reasons, we align here to 1
#pragma pack(push, 1)
#endif
#endif

//*************************************************************
//*
//* 2007-3-1 created.
//*************************************************************
class TInternetPlugInData {
	public:
		TInternetPlugInData() {
			size = sizeof(TInternetPlugInData);
			rx = NULL;
			filename[0] = '\0';
			contenttype[0] = '\0';
			str_error[0] = '\0';
		};

		int size;
		// Input Data data
		int mode; // =0: downloads a string. =1 downloads to a file
		const char* url;
		TMonitorProcessPlugIn2 *monitor;   //2012-2-7, antes _KK, ahora '2'
		// Output Data
		const char* filepathname; // (path to put the downloaded file)
		char *rx; // String will be copied here in case of mode=0. Delete it at exit!.
		int rxlen; // String will be copied here in case of mode=0
		char filename   [100]; // In case of file download, here will be writen the correct name of the file.
		char contenttype[100]; // In case of file download, here will be writen the content type
		char str_error  [100]; // In case of error, the description of the error will be written here
};

//*************************************************************
//*
//* 2007-3-1 created.
//*************************************************************
class TInternetPlugIn {
public:
	TInternetPlugIn() {
		Pdata = NULL;
		size = sizeof(TInternetPlugIn);
	};

	bool DownloadHTTP(TInternetPlugInData &data) {
		return CallBack_DownloadHTTP(Pdata, data);
	};

	int size;
	// Private Part:
	Tydef_DownloadHTTP CallBack_DownloadHTTP;
	void* Pdata;
};

// *************************************************************
// *
// *************************************************************
class TRegistroPlugInData {
	public:
		TRegistroPlugInData() {
			size         = sizeof(TRegistroPlugInData);
			mapid        = 0;
			key[0]       = '\0';
			str_error[0] = '\0';
			nom_mapa[0]  = '\0';
			monitor      = NULL;
		};
		int size;
		// Input Data data
		int mapid;
		TMonitorProcessPlugIn2 *monitor;
		// Output Data
		char key[100]; // Map Key
		char str_error[200]; // In case of error, the description of the error will be written here
		char nom_mapa[200];
};

// *************************************************************
// *
// *************************************************************
class TRegistroPlugIn {
public:

	TRegistroPlugIn() {
		size = sizeof(TRegistroPlugIn);
		Pdata = NULL;
	};

	bool GetMapKey(TRegistroPlugInData &data) {
		return CallBack_GetMapKey(Pdata, data);
	};

	bool DeleteMapKey(TRegistroPlugInData &data) {
		return CallBack_DeleteMapKey(Pdata, data);
	};

	int size;
	// Private Part:
	Tydef_GetMapKey CallBack_GetMapKey;
	Tydef_DeleteMapKey CallBack_DeleteMapKey;
	void* Pdata;
};

// *************************************************************
// *
// *************************************************************
class TStringPlugIn {
public:
	// Plug-In use:
	void Set(const char *str) {
		CallbackSetString(*this, str);
	};

	// Private Part
	Tydef_SetString CallbackSetString;
	void *data; // (DString class)
};

typedef int(*TFuncExecCommand)(TDatosDibPlugIn * obj, int idcommand,
	const char*params);

// *************************************************************
// *         class TDatosDibPlugIn
// * Class used to open raster maps.
// * 2007-9-22 [#1] added OnExecCommand
// *************************************************************
class TDatosDibPlugIn {
public:
	TDatosDibPlugIn() {
		size = sizeof(*this);
		handle = 0;
		nplugin = -1;
		AdvancedData = NULL;
		trace = NULL;
		OnExecCommand = NULL;
		params_open = NULL;
	};

	int size; // Size of this structure.
	// Only used by the PlugIns:
	HANDLEPLUGIN handle;
	// Use by both
	int px, py;
	bool read_all_at_once;
	// Only used by CompeGPS
	int nplugin;
	// Used by both:
	int bpp;
	TStringPlugIn* AdvancedData;
	int* trace; // For debug. do not use.
	TInternetPlugIn* inet; // Can use it to acces to internet.
	void* OnExecCommandData;
	TFuncExecCommand OnExecCommand; // Funcion called by the plugin in any time or thead, and CompeGPS will execute it, can be NULL.
	const char* params_open; // Extra params sent by CompeGPS to the plugin, like '/nobits' wich means: we want to open the map by tiles!
	TRegistroPlugIn* registro; // To be used with private maps
};

//*************************************************************
//*             class TDatosZonaPlugIn
//* Class used to read one section of a raster map.
//* 2005-10-31: Added this: line_size (for symbian compatibility)
//* 2007-9-22:  Added handledc
//* 2012-2-7   [#1]  ITV   aded 'monitor2'
//**************************************************************
class TDatosZonaPlugIn {
	public:
		TDatosZonaPlugIn() {
			size 				= sizeof(*this);
			bits 				= 0;
			veccolors 		  	= 0;
			CallbackCreateDIB 	= 0;
			percentage 			= -1;
			colornum 			= 0;
			path_dib[0] 		= '\0';
			handledc 			= 0;
			orientacion 		= ' ';
			//!!FUERA!!monitor2			= NULL;
		};

		int size; // Size of this structure.
		// Datos usados por compe
		void *pdib;
		Tydef_CallbackCreateDIB CallbackCreateDIB;

		// For the Plug-IN USE
		void CreateDIB() {
			CallbackCreateDIB(*this);
		};
		/*!!! FUERA!!!!
		TMonitorProcessPlugIn2* GetMonitor2(){
			if (size>=565) return monitor2;
			else return NULL;
		}; */

		// Datos compartidos
		int				px;
		int				py;
		int				colorbits; // Should be 24 always
		int				colornum; // Should be zero. (In case of 16 bits, this int can be 555 or 565)
		unsigned int*	veccolors; // Pallete. Should be NULL
		char*			bits;
		float			percentage; // Used only by versions 5.62 or upper
		int				line_size; // New: 2005-10-31 (Used by version 6.1.b or upper)
		char			path_dib[512]; // Path to the temporal file with the map. (Used by version 6.2.e or upper)
		int				handledc; // if =1, it will return here a HDC for the bitmap, so we can paint on it directly.
		char			orientacion; // Orientacion de la celda (Blom Telefot): N, S, E, W
		//!!FUERA!!! TMonitorProcessPlugIn2*  monitor2;
};

//*************************************************************
//*
//* 2006-3-27 proto
//*************************************************************
class TDatosSaveDibPlugIn {
	public:
		TDatosSaveDibPlugIn() {
			size     = sizeof(*this);
			str_imp2 = NULL;
		};

		int size; // Size of this structure
		// Internal used only by CompeGPS
		int nplugin;
		// Input values
		int w, h;
		int bpp_in;
		int num_colors;
		unsigned int *palete;
		// in/out Values
		// Block size: The number of lines must be proportional to it!.
		int xbl;
		int ybl;
		int bpp_out;
		int dir_y; // +1 y goes upwards.(zero at bottom)  -1  y goes dowm. (zero at top)
		int cal_ok;
		float compresion;
		const char *str_imp2; // Calibration to fit into the image.
		// Internal plug-in values (ignored by CompeGPS)
		HANDLEPLUGIN handle;

};

//*************************************************************
//*
//* 2006-3-27 proto
//*************************************************************
class TSaveLinesDibPlugIn {
	public:
		int size; // Size of this structure
		int w, h;
		int bpp_in;
		int num_colors;
		int *palete;
		const char *bits;
};

//*************************************************************
//*
//*************************************************************
class TDemPlugIn {
	public:
		TStringPlugIn *file_HDR;
		TStringPlugIn *file_PRJ;
		char* alts;
		void CreateAlts(int len) {
			alts = (char*)CallbackMalloc(len);
		};
		TypedefMalloc CallbackMalloc;
};

//*************************************************************************
//*
//* 2004-7-3  creado
//*************************************************************************
class TGeoRectPlugInData {
	public:
		TGeoRectPlugInData() {
			size = sizeof(*this);
		};

		int size; // Size of this structure.
		double lon1, lat1;
		double lon2, lat2;
		double lonc, latc; // Center of the map.
		double scale; // Scale, in m/pixel
		int ntemp; // Autonumeric number. (Use it for the map name if you want)
};

/*
//#pragma pack(pop)
#if defined ITV_USA_PDA
#pragma pack(push, 4)
#endif
 */

#define PS_COMPEP_SOLID			0
#define PS_COMPEP_DASH			1
#define PS_COMPEP_DOT  			2
#define PS_COMPEP_DASHDOT       3
#define PS_COMPEP_DASHDOTDOT    4
#define PS_COMPEP_NULL          5
#define PS_COMPEP_DOUBLE        6
#define PS_COMPEP_DOUBLE_FAT    7
#define PS_COMPEP_DOUBLE_ALTER  8
#define PS_COMPEP_RAIL          9
#define PS_COMPEP_RAIL_DOUBLE  10

// *************************************************************
// *
// * 2006-3-9 created
// *************************************************************
class TFontAttributesPlugIn {
public:
	TFontAttributesPlugIn() {
		this_size = sizeof(TFontAttributesPlugIn);
		fontname = 0;
		fontsize = 0;
		rotation = 0;
		justification = 0;
	};

	int this_size;
	const char *fontname;
	float fontsize;
	float rotation;
	int justification;
};

// *************************************************************
// *
// * 2006-3-9 created
// * 2006-9-26 added 'zoom_level'
// *************************************************************
class TLineAttributesPlugIn {
public:
	TLineAttributesPlugIn() {
		size = sizeof(TLineAttributesPlugIn);
		comment = NULL;
		iconid = NULL;
		border_color = 0xFFFFFFFF;
		inner_color = 0xFFFFFFFF;
		type = 0;
		layerid = 0;
		width = 1;
		FontAttr = NULL;
		databaseid = 0;
		zoom_level_min = 0;
		zoom_level_max = 0;
		transparency = -1;
		line_type = -1;
		fill_type = -1;
		points_count = -1;
		width_m = -1;
		visible = 1;
		param_type = 0;
		street_direction = 0;
		zoom_level = -1;
		altitude1 = -9999;
		altitude2 = -9999;
		NodeID_start = -1;
		NodeID_end = -1;
		Bound = 0; // jms.13.02.09.	Default Values for Nodx Entry
		isRouteParam = false;
		One_way = 0;
		Toll = 0;
		Denied_emergency = 0;
		Denied_delivery = 0; // jms.13.02.09.	Default Values for RouteParam Entry
		Denied_car = 0;
		Denied_bus = 0;
		Denied_taxi = 0;
		Denied_pedestrian = 0;
		Denied_bicycle = 0;
		Denied_truck = 0;
		LeftAreaId = -1;
		RightAreaId = -1;
		LeftStartAdress = -1;
		LeftEndAdress = -1;
		RightStartAdress = -1;
		RightEndAdress = -1;
		zipCode[0] = '\0';
		tunel = false;
		rondabout = false;
		ramp = false;
		unpaved = false;
		parentLayerId = -1; 
		Speed = 0; 
		Road_class = 0;
		draw_texts = false;
	}

	int size; // Size of this class.
	const char* iconid; // The Plugin should new it and delete it.
	int databaseid;
	unsigned int border_color;
	unsigned int inner_color;
	int type; // ==0: polilinea, =1: poligono, =2 poligono de triangulos
	// =3: un solo punto
	// & 0x80 = es 2D.
	int layerid;
	const char *comment; // The Plugin should new it and delete it.
	float width;
	TFontAttributesPlugIn* FontAttr;
	int points_count; // Not required, but CompeGPS goes faster if it is available.
	float zoom_level_min; // In meter/pixels
	float zoom_level_max; // In meter/pixels
	float transparency;
	int line_type;
	int fill_type;
	int zoom_level;
	float width_m;
	int visible;
	int param_type; // =0 'param' not used.
	// =2 'param' is the PK. (Kilometric point)
	// =   to be decided........
	int street_direction; // 0= boths directions. 1=only one direction
	float altitude1, altitude2;
	// jms.11.02.09.	Entry Nodx= , it has the ID of the related nodes, in the start and end
	int NodeID_start; // ID del nodo inicial de la polyline
	int NodeID_end; // ID del nodo final de la polyline
	int Bound; // 0  normal NodeID definition.
	// 1  bound point NodeID definition – this point will be used as entry/exit point of the map – connection to other separate IMG files

	// jms.11.02.09.	Entry RouteParam= , it has the params of the route --> polyline, ...
	int Speed; // velocidad en km/h
	bool isRouteParam; // Indica si la entrada RouteParam estaba en la seccion
	int Road_class; // The lowest importance is 0, the highest is 4
	int One_way; // 1  = one way road, where the permitted direction is always from the beginning of the road to the end, considering the digitalisation direction
	// -1 = one-way road, opposite to the digitalisation direction
	// 0  = two-way road
	int Toll; // Defines that it is a toll road
	int Denied_emergency; // 1 = no emergency vehicles allowed on the road
	int Denied_delivery; // 1 = no delivery vehicles allowed on the road
	int Denied_car; // 1 = no cars allowed on the road
	int Denied_bus; // 1 = no buses allowed on the road.
	int Denied_taxi; // 1 = no taxis allowed on the road
	int Denied_pedestrian; // 1 = no pedestrians allowed on the road
	int Denied_bicycle; // 1 = no bicycles allowed on the road
	int Denied_truck; // 1 = no trucks allowed on the road
	__int64 LeftAreaId; // Left area Id
	__int64 RightAreaId; // Right area Id
	int LeftStartAdress; // Left Adress Start
	int LeftEndAdress; // Left Adress End
	int RightStartAdress; // Right Adress Start
	int RightEndAdress; // Right Adress End
	char zipCode[11]; // Zip code
	bool tunel; //Defines a tunel vector
	bool rondabout; //Defined a roundabout
	bool ramp; //Defines is vector is entrance/exit ramp (highways or mayor roads)
	bool unpaved; //Defines an unpaved road
	int parentLayerId; //Only used for new layer, to define the parent layer id
	bool draw_texts; 
};

// *************************************************************
// *
// * 2009-02-12 created JMS
// *************************************************************
class TLineRestrictionsPlugIn {
public:
	TLineRestrictionsPlugIn() {
		size = sizeof(TLineRestrictionsPlugIn);
		NodeID_start = -1;
		NodeID_middle = -1;
		NodeID_end = -1;
	};

	int size; // Size of this class.
	int NodeID_start; // Nodo donde se inicia la restriccion
	int NodeID_middle; // Nodo de la interseccion entre las carreteras que comprenden la restriccion
	int NodeID_end; // Nodo donde acaba la restriccion
};

// *************************************************************
// *
// * 2006-3-9 created
// *************************************************************
class TVectorPointPlugIn {
public:
	TVectorPointPlugIn() {
		size = sizeof(TVectorPointPlugIn);
		param = -1; /* auxData=-1; */ };

	int size;
	double x, y, z;
	double param;
	int auxData;
};

class TReadVectorMapPlugIn;
typedef int(*Tydef_CallbackNewLine)(TReadVectorMapPlugIn & datos,
	TLineAttributesPlugIn & line_attr);
typedef int(*Tydef_CallbackNewPoint)(TReadVectorMapPlugIn & datos, int lineid,
	TVectorPointPlugIn & point, bool last_point);
typedef int(*Tydef_CallbackNewIsland)(TReadVectorMapPlugIn & datos, int lineid);
typedef int(*Tydef_CallbackNewRestriction)(TReadVectorMapPlugIn & datos,
	TLineRestrictionsPlugIn & restriction); // jms.23.03.09. Añadimos funcion Callback para las restricciones

//*************************************************************
//*         class TVectorMapPlugIn
//* Class used to open vector maps
//* 2006-3-9 created
//* 2009-6-11 [#1]  ITV added vmap parameters.
//* 2012-2-22 [#2] DNP mantis13374: anyadir strparams, donde se podran pasar mas parametros de configuracion que necesite el plugin
//*************************************************************
class TReadVectorMapPlugIn {
public:
	TReadVectorMapPlugIn() {
		size = sizeof(TReadVectorMapPlugIn);
		file_PRJ = NULL;
		PtrNewLayer = NULL;
		search_str = NULL;
		PtrNewIsland = NULL;
		PtrNewRestriction = NULL;
		lev = 0;
		cx = 0;
		cy = 0;
	};

	int size; // Size of this structure.
	// Use by both
	TStringPlugIn *file_PRJ;

	int NewLayer(TLineAttributesPlugIn &line_attr) {
		if (PtrNewLayer)
			return PtrNewLayer(*this, line_attr);
		else
			return -1;
	};

	int NewLine(TLineAttributesPlugIn &line_attr) {
		if (PtrNewLine)
			return PtrNewLine(*this, line_attr);
		else
			return -1;
	};

	int NewIsland(int lineid) {
		if (PtrNewIsland && size >= 32) {
			return PtrNewIsland(*this, lineid);
		}
		else
			return -1;
	};

	int NewPoint(int lineid, TVectorPointPlugIn &point, bool last_point) {
		if (PtrNewPoint)
			return PtrNewPoint(*this, lineid, point, last_point);
		else
			return -1;
	};

	int NewRestriction(TLineRestrictionsPlugIn &restriction) { // jms.23.03.09. Añadimos funcion Callback para las restricciones
		if (PtrNewRestriction) {
			return PtrNewRestriction(*this, restriction);
		}
		else
			return -1;
	};

	// Only used by CompeGPS
	void *param;
	Tydef_CallbackNewLine PtrNewLayer;
	Tydef_CallbackNewLine PtrNewLine;
	Tydef_CallbackNewPoint PtrNewPoint;
	Tydef_CallbackNewRestriction PtrNewRestriction;
	// jms.23.03.09. Añadimos restricciones
	// Extra opening params
	const char* search_str;
	Tydef_CallbackNewIsland PtrNewIsland;
	int lev, cx, cy; // [#1] open parameters, only for Vector_F
		// size = 48
	const char *strparams;	// [#2] from CompeGPS to plug-in: parametros adicionales que se le pueden pasar al plugin
							//		internamente puede haber nombres de archivo, etc, y el plugin quiza no tenga modulos comunes
							//		El formato usado es:
							//		"param1=valor1@@param2=valor2@@......paramN=valorN@@"
							//		De momento solo lo usa OSM para dar el archivo de configuracion de capas:
							//		"layers_config=....nombre completo del xml con path....@@"
		// size = 52
};

// *************************************************************************
// *
// * 2004-6-19  created
// *************************************************************************
class TVectorMapPlugIn {
public:
	TVectorMapPlugIn() {
		size = sizeof(TVectorMapPlugIn);
		read = 0;
		write = 0;
		create = 0;
		random_saving = 0;
		layer_mode = 0;
	};

	int size;
	// Data used by both
	const char *path_exe; // from CompeGPS to plug-in
	int read; // from CompeGPS to plug-in
	int write; // from CompeGPS to plug-in
	int create; // from CompeGPS to plug-in
	int random_saving; // from plug-in to CompeGPS
	// Data used by CompeGPS
	int nplugin;
	// Data used by the PlugIn:
	HANDLEPLUGIN handle;
	int layer_mode; // =0 Standard.  =2 Garmin.
	int aux1; // =0 for future use.
};

// *************************************************************************
// *
// * 2007-1-22 created
// *************************************************************************
class TDataHeaderPlugIn {
public:
	TDataHeaderPlugIn() {
		int size1 = sizeof(*this);
		char *ptr = (char*)this;
		for (int f = 0; f < size1; f++) {
			ptr[f] = 0;
		}; // ZeroMemory
		size = size1;
		npoints = -1;
	};

	int size;
	const char* igc_header___;
	int npoints; // Optional: number of track points. put -1 if unknown.
	int trackcolor;
	int trackwidth;
	const char* name;
	const char* description;
	const char* pilot_name;
	const char* icon_name;
	const char* start_name;
	const char* end_name;
	const char* gps_name;
	const char* gps_serial_number;
	int trackid; // (Used to download a specific track)
	int duration; // in seconds
	int legid; // To separate diferent legs of a track
	int datatype; // =1, track, =2 waypoints =3, routes
	// Future use here.
	// ................
};

class TDataPointPlugIn {
public:
	TDataPointPlugIn() {
		int size1 = sizeof(*this);
		char *ptr = (char*)this;
		for (int f = 0; f < size1; f++) {
			ptr[f] = 0;
		}; // ZeroMemory
		size = size1;
	};

	int size;
	const char* igc_section; // (if not null, use this, and not the other data!(
	double x, y; // lon,lat.  (degrees,degrees,meters. All in wgs84)
	double alt_gps;
	double alt_baro;
	double time1; // in segs since 1970. (note: it is a double, so we do not have the 2038 limit! ;)
	char* short_name; // (only for waypoints)
	char* description;
	char* icon;
	int radius; // in meters.
	char* time2; // time in format yyyy-mm-dd-hh-mm-ss, if the plugin does not know how to calculate the segs1970
	// >>>> Hasta aqui, size32 = 68 / size64 = 88
	// 2011-07-20 DNP mantis11409: Nuevos campos (FIT)
	int    heart_rate;  // bpm
	int    cadence;		// rpm
	int    power;		// watts
	double grade;		// %
	int    resistance;	// 0 min, 254 max
	double cycle_lenght;// m
	int    temperature; // C
	// >>>> Hasta aqui, size32 = 104 / size64 = 124
	// Future use here.
	// ................
};

// *************************************************************************
// *
// *   Callback Functions for track and wpt reading.
// * 2007-1-22 created
// *************************************************************************
class TDataLoaderPlugIn;
typedef bool(*Tydef_NewDataPoint)(const TDataLoaderPlugIn & trkdata,
	const TDataHeaderPlugIn & header, const TDataPointPlugIn & pdata);
/* DELETE!!!!!!!!!!!!!!!!!!
class TWptLoaderPlugIn;
typedef bool (*Tydef_NewWpt)         (TWptLoaderPlugIn  &wptsdata,TTrackPointPlugIn &pdata);
 */

// *************************************************************************
// *
// * 2007-1-22 created
// *************************************************************************
class TDataLoaderPlugIn {
public:
	TDataLoaderPlugIn() {
		size = sizeof(TDataLoaderPlugIn);
		param = 0;
		PtrNewDataPoint = NULL;
		handle = NULL;
		datatype = 0;
		params_open = NULL;
		nplugin = -1;
	};

	int size;
	int param; // Used only by CompeGPS.
	// Used by both:
	Tydef_NewDataPoint PtrNewDataPoint;
	HANDLEPLUGIN handle; // Used only by the PlugIn
	int datatype; // =1, track, =2 waypoints =3, routes
	int nplugin;
	// Functions
	void NewDataPoint(const TDataHeaderPlugIn &header, TDataPointPlugIn &pdata)
	{
		if (PtrNewDataPoint)
			PtrNewDataPoint(*this, header, pdata);
	};
	const char* params_open; // Extra params sent by CompeGPS to the plugin, like
							 //'/posX' wich means: we want to get the waypoints for specific coordinates (int lat/lon WSG84)
							 //'/posY' wich means: we want to get the waypoints for specific coordinates (int lat/lon WSG84)
							 //'/rad' wich means: we want to get the waypoints for specific coordinates (in meters) (optional, 10m by default)
};

/* OUT!!!!!!!!!
class TWptLoaderPlugIn{
public:
int size;
int param;
Tydef_NewWpt  PtrNewWpt;
// Functions
void NewWpt (TTrackPointPlugIn &pdata){
if (PtrNewWpt)
PtrNewWpt(*this,pdata);
};
};
class TWptHeaderPlugIn {
public:
int size;
};
 */

// *************************************************************************
// *
// * Error codes for CompeGPS Plug-INS
// * 2004-6-19  created
// *************************************************************************
enum {
	CompePlugIn_ERROR = 0, CompePlugIn_OK = 1, CompePlugIn_ZoomError = 2,
	// This plug-in cannot create an image of this zoom.
};

// *************************************************************************
// *
// * 2006-3-17
// *************************************************************************
class TLoginPlugIn {
public:
	TLoginPlugIn() {
		size = sizeof(TLoginPlugIn);
		reserved = 0;
	};

	int size;
	const char* login;
	const char* pcid;
	const char* keys;
	int reserved;
};

// *************************************************************************
// *
// * 2007-5-5 created
// *************************************************************************
class TJoystickPlugIn {
public:
	TJoystickPlugIn() {
		size = sizeof(TJoystickPlugIn);
		returns_type = 0;
	};

	int size; // in
	int command; // in  // (1=initialize.  2=getstate 3=disconect)
	char device_name[32]; // out
	int device_type; // out 1=normal joystick. 2=volante, 3=joystick 3D like 3Dconnexion
	int state; // out //  1=ok.   0=error.
	int nbuttons; // out number of buttons.. from 0 to 31
	int naxis; // out
	int buttons; // out 31 posible buttons.
	double axis[16]; // out
	int returns_type; // out: 0 if no axis_type or button_types are returned.
	int axis_types[16]; // out: can be returned during initialization.
	int button_types[16]; // out: can be returned during initialization.
};

// *************************************************************************
// *
// * 2007-8-4 created
// *************************************************************************
class TProtocolDataPlugIn {
public:
	TProtocolDataPlugIn() {
		size = sizeof(TProtocolDataPlugIn);
		use_compegps_serial = false;
		asks_version = false;
		downloads_tracks = false;
		downloads_routes = false;
		downloads_wpts = false;
		sends_tracks = false;
		sends_routes = false;
		sends_wpts = false;
		gets_gps_position = false;
		deletes_tracks = false;
		deletes_routes = false;
		deletes_wpts = false;
		bauds_list = NULL;
		bauds_default = 0;
		comment = NULL;
		EvtChar = 0;
	};

	int size; // in
	bool use_compegps_serial;
	bool asks_version;
	bool downloads_tracks;
	bool downloads_routes;
	bool downloads_wpts;
	bool sends_tracks;
	bool sends_routes;
	bool sends_wpts;
	bool deletes_tracks;
	bool deletes_routes;
	bool deletes_wpts;
	bool gets_gps_position;
	const char* bauds_list;
	int bauds_default;
	const char* comment; // Is displayed when the user selects this protocol. (for not friendly protocolos)
	char EvtChar; // if all messages end with this special char.
};

class TGPSPlugIn;
typedef bool(*Tydef_Transmit)(TGPSPlugIn & gps, const char*tx, int len_tx);
typedef int(*Tydef_Receive)(TGPSPlugIn & gps, int tout, char*rx, int size_rx);

// *************************************************************************
// *
// * 2007-8-4 created
// *************************************************************************
class TGPSPlugIn {
public:
	TGPSPlugIn() {
		size = sizeof(TGPSPlugIn);
		handle = 0;
		nCommPort = 0;
		baudRate = 0;
		PtrTransmit = NULL;
		PtrReceive = NULL;
		EvtChar = 0;
	};

	int size; // in
	HANDLEPLUGIN handle; // used by the plugins
	HANDLEPLUGIN handle_res; // only used by CompeGPS
	int nCommPort; // =0 com1, 1=com2, 2=com3, etc...
	int baudRate;
	int EvtChar;
	Tydef_Transmit PtrTransmit;
	Tydef_Receive PtrReceive;

	bool Transmit(const char* tx, int len_tx) {
		if (PtrTransmit)
			return PtrTransmit(*this, tx, len_tx);
		return false;
	};

	int Receive(int tout, char* rx, int size_rx) {
		if (PtrReceive)
			return PtrReceive(*this, tout, rx, size_rx);
		return false;
	};
};

// *************************************************************************
// *
// * 2009-6-6 created
// *************************************************************************
class TVectorMapFastPlugIn {
public:
	TVectorMapFastPlugIn() {
		size = sizeof(TVectorMapFastPlugIn);
		handle = 0;
		xml_header = NULL;
	};

	int size; // in
	HANDLEPLUGIN handle; // used by the plugins
	int nplugin; // used only by CompeGPS
	const char* xml_header; // alocated and dealocated by the plugin.
	// !!!!!MORE!!!!!!!

};

// Callback for Bojour connections, or other messages for Main CompeGPS Window.  (private)
typedef void(*Tydef_CallbackMainCompe)(const char*dllId, void*Pthis,
	int command, void*CommandData);

extern "C" {
	// General Plug-In Functions
	int EXPORTW CompePlugIn_Version();
	int EXPORTW CompePlugIn_GetCodepage();
	EXPORTW const char* CompePlugIn_VersionStr();
	bool EXPORTW CompePlugIn_Init(int hWnd);
	void EXPORTW CompePlugIn_Close();
	int EXPORTW CompePlugIn_VersionPriv();
	bool EXPORTW CompePlugIn_InitLogin(const char*login, const char*pcid);
	bool EXPORTW CompePlugIn_InitLogin2(TLoginPlugIn & params,
		TStringPlugIn & err);
	int EXPORTW CompePlugIn_GetLicenseId();
	void EXPORTW CompePlugIn_SetLicense(const char*license);
	EXPORTW const char* CompePlugIn_GetTipoArchivos();
	void EXPORTW CompePlugIn_Configuration();
	// Map Functions
	bool EXPORTW CompePlugIn_Lee_Calibracion(const char*nom_mapa,
		TStringPlugIn & fichero_IMP2, TStringPlugIn & err);
	// Bitmap Functions
	bool EXPORTW CompePlugIn_Abre_Raster(const char*nom_mapa,
		TDatosDibPlugIn & datos, TStringPlugIn & err);
	int EXPORTW CompePlugIn_Abre_Zona(TDatosDibPlugIn & datos, int left,
		int right, int top, int bottom, int& sizeX, int& sizeY,
		TDatosZonaPlugIn& dib, TStringPlugIn& err,
		TMonitorProcessPlugIn2 & monitor);
	void EXPORTW CompePlugIn_Cierra_Raster(TDatosDibPlugIn & datos);
	// !!DELETE!! void EXPORTW CompePlugIn_ConfiguracionMapa (const char *nom_map_bmp);
	bool EXPORTW CompePlugIn_GetLocalizacion(TDatosDibPlugIn & _datos,
		double lon, double lat, int bmpx, int bmpy, char*buff, int len,
		TStringPlugIn & err);
	void EXPORTW CompePlugIn_SetTempFolder(const char*nom_folder);
	bool EXPORTW CompePlugIn_GetCoordsLocalizacion(TDatosDibPlugIn & _datos,
		const char*place_name, double&x, double&y, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_CalculaRuta( // !!!!!!!FUNCTION CANCELED!!!!!!!!!!!!!!!!!!!!!!!!!
		TDatosDibPlugIn & _datos, double x1, double y1, double x2, double y2,
		TStringPlugIn & ruta, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GetPlaceCoordinates(TDatosDibPlugIn * amap,
		const char*place_name, TStringPlugIn & wptout,
		TStringPlugIn & err); // Nuevo: 2006-1-23
	// Saving Raster Functions
	bool EXPORTW CompePlugIn_Save_Raster_Create(const char*nom_mapa,
		TDatosSaveDibPlugIn & dib, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Save_Raster_AddLines(TDatosSaveDibPlugIn & dib,
		TSaveLinesDibPlugIn & SomeLines, TMonitorProcessPlugIn2 & monitor,
		TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Save_Raster_Close(TDatosSaveDibPlugIn & dib,
		bool cancelar, TMonitorProcessPlugIn2 & monitor,
		TStringPlugIn & err);
	// Elevation maps
	bool EXPORTW CompePlugIn_Lee_DEM(const char*nom_dem, TDemPlugIn & dem,
		TStringPlugIn & string_error, TMonitorProcessPlugIn2 * monitor,
		bool lee_alturas);
	// Import Vector Maps
	bool EXPORTW CompePlugIn_Open_Vector_Map(const char*map_name,
		TVectorMapPlugIn & vectormap, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Read_Vector_Map3(TVectorMapPlugIn & vectormap,
		TReadVectorMapPlugIn & readvectormap, TStringPlugIn & err,
		TMonitorProcessPlugIn2 & monitor);

	bool EXPORTW CompePlugIn_Add_Layer(TVectorMapPlugIn & vectormap,
		TLineAttributesPlugIn & Line, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Add_Poliline_Vector_Map
		(TVectorMapPlugIn & vectormap, TLineAttributesPlugIn & Line,
		int npoints, TVectorPointPlugIn * points, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Delete_Poliline_Vector_Map
		(TVectorMapPlugIn & vectormap, int dababaseid, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Close_Vector_Map(TVectorMapPlugIn & vectormap,
		TStringPlugIn & _err);
	// Data Reading: Tracks / Waypoints / Routes / Profiles
	bool EXPORTW CompePlugIn_Read_Data(const char*trackname,
		TDataLoaderPlugIn & trkloader, TStringPlugIn & err);
	// Tracks
	bool EXPORTW CompePlugIn_Read_Track(const char*trackname,
		TStringPlugIn & igcout, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Read_Track2(const char*trackname,
		TDataLoaderPlugIn & trkloader, TStringPlugIn & err);
	// Tracks, Saving
	bool EXPORTW CompePlugIn_Save_Track_Open(const char*trackname,
		TDataHeaderPlugIn & header, TDataLoaderPlugIn & trkloader,
		TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Save_Track_NewPoint(TDataLoaderPlugIn & trkloader,
		TDataPointPlugIn & point);
	bool EXPORTW CompePlugIn_Save_Track_Close(TDataLoaderPlugIn & trkloader);
	// Waypoints
	bool EXPORTW CompePlugIn_Read_Wpts(const char*wptsname,
		TStringPlugIn & wptout, TStringPlugIn & err); // nuevo: 2006-1-15
	bool EXPORTW CompePlugIn_Read_Wpts2(const char*wptsname,
		TDataLoaderPlugIn & wptsdata, TStringPlugIn & err); // nuevo: 2007-1-24
	// Waypoints, Saving
	bool EXPORTW CompePlugIn_Save_Wpts_Open(const char*wptsname,
		TDataLoaderPlugIn & trkloader, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_Save_Wpts_NewPoint(TDataLoaderPlugIn & trkloader,
		TDataPointPlugIn & point);
	bool EXPORTW CompePlugIn_Save_Wpts_Close(TDataLoaderPlugIn & trkloader);

	// Joystick or any display controller
	bool EXPORTW CompePlugIn_GetJoystickState(TJoystickPlugIn & data,
		TStringPlugIn & err);

	// GPS Connection
	bool EXPORTW CompePlugIn_GPS_GetProtocolData(const char*protid,
		TProtocolDataPlugIn & data);
	bool EXPORTW CompePlugIn_GPS_Open(const char*protid, TGPSPlugIn & gps,
		TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_Close(TGPSPlugIn & gps);
	bool EXPORTW CompePlugIn_GPS_GetVersion(TGPSPlugIn & gps,
		TStringPlugIn & version, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_GetTrackList(TGPSPlugIn & gps,
		TDataLoaderPlugIn & trkloader, TMonitorProcessPlugIn2 & monitor,
		TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_Download_Data(TGPSPlugIn & gps, int ntrack,
		TDataLoaderPlugIn & trkloader, TMonitorProcessPlugIn2 & monitor,
		TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_Save_Data_Open(TGPSPlugIn & gps,
		const char*dataname, TDataLoaderPlugIn & loader, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_Save_Data_NewPoint(TGPSPlugIn & gps,
		TDataLoaderPlugIn & loader, TDataHeaderPlugIn & header,
		TDataPointPlugIn & point, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_GPS_Save_Data_Close(TGPSPlugIn & gps,
		TDataLoaderPlugIn & loader);

	bool EXPORTW CompePlugIn_GPS_Delete_Data(TGPSPlugIn & gps, int nelem,
		TDataLoaderPlugIn & loader, TStringPlugIn & err);
	int EXPORTW CompePlugIn_GPS_GetRealTimeData(TGPSPlugIn & gps,
		TStringPlugIn & nmea, TDataLoaderPlugIn & loader,
		// (alternative system to nmea)
		TStringPlugIn & err);

	// Map Download Plug-INS.
	bool EXPORTW CompePlugIn_Download_Map(const char*params,
		TGeoRectPlugInData & rect, const char*folder,
		TStringPlugIn & mapasdowloaded, TStringPlugIn & err,
		TMonitorProcessPlugIn2 & monitor, TStringPlugIn & outparams);
	// Tiled Fast Vector maps
	bool EXPORTW CompePlugIn_Open_F_Vector_Map(const char*map_name,
		TVectorMapFastPlugIn & vectormap, TStringPlugIn & err);
	bool EXPORTW CompePlugIn_ReadCell_F_Vector_Map
		(TVectorMapFastPlugIn & vectormap,
		TReadVectorMapPlugIn & readvectormapCell, TStringPlugIn & err);
	void EXPORTW CompePlugIn_Close_F_Vector_Map
		(TVectorMapFastPlugIn & vectormap);
	// MAin Callback
	bool EXPORTW CompePlugIn_Set_CallbackMainCompe
		(Tydef_CallbackMainCompe _CallbackMainCompe,
		void*Pthis, TStringPlugIn & s_err);
}

#if !defined ITV_USA_SYMBIAN
#pragma pack(pop)
#endif

#endif //INCLUDE_DLL_PLUGINMAPAS_H

