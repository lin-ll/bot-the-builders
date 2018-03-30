// The central file for controlling the robot

float destX;
float destY;

int controls_finished = 0;




while(True) {
	// check buttons


	// do controls thing
	controls_finished = controls.update();
	if(controls_finished){
		int *up_wall, *down_wall, *left_wall, *right_wall;
		contols.find_walls(up_wall, down_wall, left_wall, right_wall);
		dir = maze.dfs(up_wall, down_wall, left_wall, right_wall);
		controls.move(dir);
	}


	



	//// do Kalman thing
	// get all sensor values
	// call Kalman with sensor values
}



