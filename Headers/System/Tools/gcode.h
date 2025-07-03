/*


*/
class gcode{
	enum class COMMANDS {
		
	}
	enum class device_types {PRINTER3D}
	enum class unit_types {MM, INCH}
	enum class plane_types {XY,XZ,YZ}
	
	uint32_t flags;
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