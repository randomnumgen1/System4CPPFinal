/*


*/
class gcode{
	private:
		void Addgcode(std::string code,bool PRINTER3D){
			
		}
	public:
	enum class COMMANDS {
		
	}
	enum class device_types : uint32_t {PRINTER3D}
	enum class unit_types {MM, INCH}
	enum class plane_types {XY,XZ,YZ}
	
	uint32_t flags;
	gcode(){
		Addgcode("G00","Rapid Move");
		Addgcode("G01","Linear Move");
		Addgcode("G02","Arc Move (clockwise)");
		Addgcode("G03","Arc Move (counterclockwise)");
		Addgcode("G04","Dwell");
		Addgcode("G05","Cubic Spline");
		Addgcode("G5.1 ","Quadratic Spline");
		
		Addgcode("G09",);
		Addgcode("G10",);
		Addgcode("G15",);
		Addgcode("G16",);
		Addgcode("G17",);
		Addgcode("G18",);
		Addgcode("G19",);
		Addgcode("G20",);
		Addgcode("G21",);
		Addgcode("G27",);
		Addgcode("G28",);
		Addgcode("G29",);
		Addgcode("G30",);
		Addgcode("G32",);
		Addgcode("G40",);
		Addgcode("G41",);
		Addgcode("G42",);
		Addgcode("G43",);
		Addgcode("G44",);
		Addgcode("G49",);
		Addgcode("G50",);
		Addgcode("G51",);
		Addgcode("G52",);
		Addgcode("G53",);
		Addgcode("G54",);
		Addgcode("G55",);
		Addgcode("G56",);
		Addgcode("G57",);
		Addgcode("G58",);
		Addgcode("G59",);
		Addgcode("G61",);
		Addgcode("G62",);
		Addgcode("G63",);
		Addgcode("G64",);
		Addgcode("G65",);
		Addgcode("G68",);
		Addgcode("G69",);
		Addgcode("G73",);
		Addgcode("G74",);
		Addgcode("G76",);
		Addgcode("G80",);
		Addgcode("G81",);
		Addgcode("G82",);
		Addgcode("G83",);
		Addgcode("G84",);
		Addgcode("G85",);
		Addgcode("G86",);
		Addgcode("G87",);
		Addgcode("G88",);
		Addgcode("G89",);
		Addgcode("G90",);
		Addgcode("G90.1",);
		Addgcode("G91",);
		Addgcode("G91.1",);
		Addgcode("G92",);
		Addgcode("G92.1",);
		Addgcode("G92.2",);
		Addgcode("G92.3",);
		Addgcode("G93",);
		Addgcode("G94",);
		Addgcode("G95",);
		Addgcode("G96",);
		Addgcode("G97",);
		Addgcode("G98",);
		Addgcode("G99",);
		
		
		Addgcode("M01",);
		Addgcode("M02",);
		Addgcode("M03",);
		Addgcode("M04",);
		Addgcode("M05",);
		Addgcode("M06",);
		Addgcode("M07",);
		Addgcode("M08",);
		Addgcode("M09",);
		Addgcode("M17",);
		Addgcode("M29",);
		Addgcode("M30",);
		Addgcode("M97",);
		Addgcode("M98",);
		Addgcode("M99",);
		
		
	}
	void SetDeviceType(device_types device_type){
		if(device_type == device_types::PRINTER3D){
			
		}else{
			
		}
	}
	void SetUnits(unit_types unit_type){
		if(unit_type == unit_types::MM){
			//G21
		}else if(unit_type == unit_types::INCH){
			//G20
		}else{
			
		}
	}
	void SetPlane(plane_types plane_type){
		if(plane_type == plane_types::XY){
			//G17
		}else if(plane_type == plane_types::XZ){
			//G18
		}else if(plane_type == plane_types::YZ){
			//G19
		}else{
			
		}
	}
	void SetPositioningMode(){
		//G90 is for absolute mode
		//G91 is for relative mode
	}
	void SetFanSpeed(){
		//M106
	}
	void Home(IntermediatePointx,IntermediatePointy,IntermediatePointz){
		//G28
	}
	
	
	
	
	
	
	
	
	
};