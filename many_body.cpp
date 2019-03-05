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
#define radius 0.5
#define timestep 100//720000
#define R_array true
#define F_array false

using namespace std;

float euclidean(double x1, double y1, double z1, double x2, double y2, double z2){

	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

void find_force(double **F,double **R)    
{
     ll i,j;
     double x,y,z,dis;
     for(i=0;i<number_bodies;i++)
     {
     	x=y=z=0.0;
     	for(j=0;j<number_bodies; j++)
        {
        	dis = euclidean(R[j][0],R[j][1],R[j][2],R[i][0],R[i][1],R[i][2]);
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

void velocity_update(double **F,double **V)
{
    int i;
    for(i=0;i<number_bodies;i++)
    {
    	V[i][0] = V[i][0]+(F[i][0]*delta_t)/(2.0*mass);
    	V[i][1] = V[i][1]+(F[i][1]*delta_t)/(2.0*mass);
    	V[i][2] = V[i][2]+(F[i][2]*delta_t)/(2.0*mass);
    }
}

void collision_and_position_update(double **R,double **V)
{
    //////Inter-Collision Check////////////

	//////position update//////////
    int i;
    for(i=0;i<number_bodies;i++)
    {
    	R[i][0] += V[i][0]*delta_t;
    	R[i][1] += V[i][1]*delta_t;
    	R[i][2] += V[i][2]*delta_t;

    	/////Boundary Collision Check/////
    	if( R[i][0] >= breadth || R[i][0] <= 0)
    	{	
    		R[i][0] = (R[i][0] <= 0)?(-R[i][0]) : (2*breadth - R[i][0]);
    		V[i][0] = -V[i][0];
    	}
    	if( R[i][1] >= length || R[i][1] <= 0)
    	{	
    		R[i][1] = (R[i][1] <= 0)?(-R[i][1]) : (2*length - R[i][1]);
    		V[i][1] = -V[i][1];
    	}
    	if( R[i][2] >= height || R[i][2] <= 0)
    	{	
    		R[i][2] = (R[i][2] <= 0)?(-R[i][2]) : (2*height - R[i][2]);
    		V[i][2] = -V[i][2];
    	}
    	/////////////////////////////////

    }
}

void generate_bin_file(double **R,int num)
{
    fstream file;
    string s="trajectory"+to_string(num)+".bin";
	file.open(s,ios::out|ios::binary);

	for(ll i=0;i<number_bodies;i++)
	{
		// file.write((char*)&R[i][0],sizeof(R[i][0]));
		// file.write((char*)&R[i][1],sizeof(R[i][1]));
		// file.write((char*)&R[i][2],sizeof(R[i][2]));
        file<<R[i][0]<<" ";
        file<<R[i][1]<<" ";
        file<<R[i][2]<<" ";
        file <<"\n";
	}
	file.close();
}

void read_txt_file(double **R){

    fstream init("trajectory.txt", std::ios_base::in);
    if(init.is_open())
    {
        int c = 0;
        while (c<1000)
        {   
            init >> R[c][0] >> R[c][1] >> R[c][2];
            c++;   
        }

        return;
    }
    else
    {   cout<<"Unable to open file!"<<endl;
        exit (EXIT_FAILURE);
    }
    init.close();
}
void print_2d(bool f,bool r,double **R,double **F)
{
    if(f){
        for (int i = 0; i < 1000; ++i)
        {
            cout<< R[i][0]<< " " <<R[i][1]<< " " <<R[i][2]<< " "<<endl; 
        }
    }
    if(r)
    {
        for (int i = 0; i < 1000; ++i)
        {
            cout<< F[i][0]<< " " <<F[i][1]<< " " <<F[i][2]<< " "<<endl; 
        }
    }
    return;
}
int main()
{
	int i;
    ////////////////initialise Position matrix////////////////
    double **R;
    R = (double**)malloc(sizeof(double*)*1000);
    for(i = 0; i <1000; i++)
        R[i] = (double*)malloc(sizeof(double)*3);
    read_txt_file(R);
    ///////////////////////////////////////////////////////

    ///////////////Initialise Force matrix////////////////
    double **F;
    F = (double**)malloc(sizeof(double*)*1000);
    for( i = 0; i <1000; i++)
        F[i] = (double*)malloc(sizeof(double)*3);
    ///////////////////////////////////////////////////////

    ///Initialise Velocity matrix assuming intial rest conditions///
    double **V;
    V = (double**)calloc(1000, sizeof(double*));
    for( i = 0; i <1000; i++)
        V[i] = (double*)calloc(3, sizeof(double));
    ///////////////////////////////////////////////////////


	for( i = 0 ;i <timestep ; i++){
		
		find_force(F,R);
		
		velocity_update(F,V);
		
		collision_and_position_update(R,V);
		
		velocity_update(F,V);
		
		if(i%100 == 0)
        {
            generate_bin_file(R,i);
        }
	}
    
    if(R_array || F_array){ //set R_array to view initial matrix
        print_2d(R_array,F_array,R,F);
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







