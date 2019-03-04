//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string>

#define ll long long int
#define number_bodies 1000
#define mass 1
#define length 100
#define breadth 200
#define height 400
#define delta_t 0.01
#define timestep 7//720000

using namespace std;

void find_force(double F[][3],double R[][3],ll dim)    
{
     ll i,j;
     double x,y,z,dis;
     for(i=0;i<dim;i++)
     {
     	x=y=z=0.0;
     	for(j=0;j<dim;j++)
        {
        	dis=sqrt((R[j][0]-R[i][0])*(R[j][0]-R[i][0]) + (R[j][1]-R[i][1])*(R[j][1]-R[i][1]) + (R[j][2]-R[i][2])*(R[j][2]-R[i][2]));
        	if(dis!=0)
        	{
        	x=x+(R[j][0]-R[i][0])/dis;
        	y=y+(R[j][1]-R[i][1])/dis;
        	z=z+(R[j][2]-R[i][2])/dis;
            }
        }
        F[i][0]=x*mass*mass;
        F[i][1]=y*mass*mass;
        F[i][2]=z*mass*mass;
     }
}

void velocity_update(double F[][3],double V[][3],ll dim)
{
    ll i;
    for(i=0;i<dim;i++)
    {
    	V[i][0]=V[i][0]+(F[i][0]*delta_t)/(2.0*mass);
    	V[i][1]=V[i][1]+(F[i][1]*delta_t)/(2.0*mass);
    	V[i][2]=V[i][2]+(F[i][2]*delta_t)/(2.0*mass);
    }
}

void collision_and_position_update(ll **R,ll **V)
{
    
}

void generate_bin_file(double R[][3],ll num,ll dim)
{
    fstream file;
    string s="trajectory"+to_string(num)+".bin";
	file.open(s,ios::out|ios::binary);

	for(ll i=0;i<dim;i++)
	{
		file.write((char*)&R[i][0],sizeof(R[i][0]));
		file.write((char*)&R[i][1],sizeof(R[i][1]));
		file.write((char*)&R[i][2],sizeof(R[i][2]));
	}
	file.close();
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

/*int main()
{
	double R[3][3]={{1,2,3},{3,2,1},{2,1,3}};
	double F[3][3]={0};

    find_force(F,R,3);
	for(int i=0;i<3;i++)
	{
		cout << F[i][0] << " " << F[i][1] << " " << F[i][2] << "\n";
	}
	generate_bin_file(R,1,3);
    return 0;
}*/







