#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#define number_bodies 1000
#define mass 1
#define length 100
#define breadth 200
#define height 400
#define delta_t 0.01
#define timestep 7//720000


void find_force(ll **F)
{
     
}

void velocity_update(ll **F,ll **V)
{

}

void collision_and_position_update(ll **R,ll **V)
{

}

void generate_bin_file(ll **R)
{

}
int main()
{
	int i;
	for(int i = 0 ;i <timestep ; i++){
		find_force();
		velocity_update();
		collision_and_position_update();
		velocity_update();
		if(i%100 == 0)
			generate_bin_file();
	}
	return 0;
}







