//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <omp.h>

#define ll long long int

/////System Configuration//////////
#define number_bodies 1000
#define mass 1
#define length 100
#define breadth 200
#define height 400
#define delta_t 0.01
#define radius 0.5
#define timestep 720000
////////////////////////////////////

#define print_pose false
#define print_force false
#define print_distance false
#define DOUBLE_MAX 999999.99
#define COLLISION_CHECK
#define OPENMP

using namespace std;

float euclidean(double x1, double y1, double z1, double x2, double y2, double z2){

	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

void print_matrix(double **X)
{
        for (int i = 0; i < 1000; ++i)
        {
            cout<< X[i][0]<< " " <<X[i][1]<< " " <<X[i][2]<< " "<<endl; 
        }
    return;
}

void find_force(double **F,double **R)    
{
     ll i,j;
     double x,y,z,dis;

     #pragma omp parallel for private(j,dis) schedule(guided) reduction(+:x,y,z) 
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

    #pragma omp parallel for private(i) schedule(guided)
    for(i=0;i<number_bodies;i++)
    {
    	V[i][0] = V[i][0]+(F[i][0]*delta_t)/(2.0*mass);
    	V[i][1] = V[i][1]+(F[i][1]*delta_t)/(2.0*mass);
    	V[i][2] = V[i][2]+(F[i][2]*delta_t)/(2.0*mass);
    }
}

void initialise_distance(double **D)
{
	int i = 0 , j = 0;

    #pragma omp parallel for collapse(2)
	for( i = 0 ; i < 1000; i++)
		for ( j = 0 ; j < 1000 ; j++)
			D[i][j] = DOUBLE_MAX;
	return;
}

void compute_distance(double **D, double **R , double **V)
{
	int i = 0 , j = 0;
	float tolerance = 2*radius;

	#ifndef OPENMP
	{ 
       	for( i = 0 ; i < 1000; i++)
        {
			for ( j = i+1 ; j < 1000 ; j++)
			{
				////////Updating Position if distance is less than tolerance////
				if(euclidean(R[j][0],R[j][1],R[j][2],R[i][0],R[i][1],R[i][2]) <= tolerance)
				{
				    swap(V[i][0],V[j][0]);
                    swap(V[i][1],V[j][1]);
                    swap(V[i][2],V[j][2]);
                    swap(R[i][0],R[j][0]);
                    swap(R[i][1],R[j][1]);
                    swap(R[i][2],R[j][2]);
					
				}
				///////////////////////////////////////////////////

			}
        }
	}
	#endif

	#ifdef OPENMP
	{  
        #pragma omp parallel for private(i,j) shared(R,V) schedule(dynamic)
		for( i = 0 ; i < 1000; i++)
		{
			for ( j = i+1 ; j < 1000 ; j++)
			{
				//if( i!=j) //check if it will work upon parallezing 
				//D[i][j] = euclidean(R[j][0],R[j][1],R[j][2],R[i][0],R[i][1],R[i][2]);

				////////Updating Position and Velocity if distance is less than tolerance////
                if(euclidean(R[j][0],R[j][1],R[j][2],R[i][0],R[i][1],R[i][2]) <= tolerance)
				{
                    
                    swap(V[i][0],V[j][0]);
                    swap(V[i][1],V[j][1]);
                    swap(V[i][2],V[j][2]);
                    swap(R[i][0],R[j][0]);
                    swap(R[i][1],R[j][1]);
                    swap(R[i][2],R[j][2]);
                    
                    
				}
				///////////////////////////////////////////////////////////////
			}
		}
	}
	#endif

	return;
}

void collision_and_position_update(double **R,double **V, double **D)
{

	//////position update//////////
    int i;

    //#pragma omp parallel for private(i)
    for(i=0;i<number_bodies;i++)
    {
    	R[i][0] += V[i][0]*delta_t;
    	R[i][1] += V[i][1]*delta_t;
    	R[i][2] += V[i][2]*delta_t;

    	/////Boundary Collision Check/////
       // #pragma omp critical
      //  {
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
       // }
    	/////////////////////////////////
    }
    /////////Collision Check between bodies//////////
    	#ifdef COLLISION_CHECK
    	{  
        
            compute_distance(D,R,V);

			if(print_distance) print_matrix(D);
      
    	}
    	#endif
    /////////////////////////////////////////////////
}

void generate_bin_file(double **R,int num)
{
    fstream file;
    string s = "trajectory"+to_string(num)+".bin";
	file.open(s,ios::out|ios::binary);
	int i;
	for(i=0 ; i < number_bodies ; i++)
	{
		// file.write((char*)&R[i][0],sizeof(R[i][0]));
		// file.write((char*)&R[i][1],sizeof(R[i][1]));
		// file.write((char*)&R[i][2],sizeof(R[i][2]));
        file << R[i][0] << " ";
        file << R[i][1] << " ";
        file << R[i][2] <<  " ";
        file <<"\n";
	}
	file.close();
}

//Reads the initial coordinates of 1000 balls//
void read_txt_file(double **R){

    fstream init("trajectory.txt", std::ios_base::in);

    if(init.is_open())
    {
        int idx = 0;
        while (idx < 1000)
        {   
            init >> R[idx][0] >> R[idx][1] >> R[idx][2];
            idx++;   
        }
        init.close();
        return;
    }
    else
    {   cout << "Unable to open file!" << endl;
        exit (EXIT_FAILURE);
    }
}

int main()
{
	int i;
    double **R,**F,**V,**D;
    omp_set_num_threads(8);
    fstream out;
    string filename = "sim_log.txt";
    out.open(filename,ios::out|ios::binary);
	///Divide each initialissation into sections in openmp///////
    #pragma omp parallel sections num_threads(4) private(i)
    {
        #pragma omp section
        {
         ////////////////initialise Position matrix////////////////
            
            R = (double**)malloc(sizeof(double*)*1000);
            for(i = 0; i <1000; i++)
                R[i] = (double*)malloc(sizeof(double)*3);
            read_txt_file(R);
        /////////////////////////////////////////////////////////
        }

        #pragma omp section
        {
            ///////////////Initialise Force matrix////////////////
            F = (double**)malloc(sizeof(double*)*1000);
            for( i = 0; i <1000; i++)
                F[i] = (double*)malloc(sizeof(double)*3);
            ///////////////////////////////////////////////////////
        }
        #pragma omp section
        {
            ///Initialise Velocity matrix assuming intial rest conditions///
            V = (double**)calloc(1000, sizeof(double*));
            for( i = 0; i <1000; i++)
                V[i] = (double*)calloc(3, sizeof(double));
            ///////////////////////////////////////////////////////
        }

        #pragma omp section
        {
            ////////////////Initialise Distance matrix//////////////
            D = (double**)malloc(sizeof(double*)*1000);
            for(i = 0; i <1000; i++)
                D[i] = (double*)malloc(sizeof(double)*1000);
            initialise_distance(D);
            ///////////////////////////////////////////////////////
        }
    }
    double wt;
    double wtime=omp_get_wtime();
	for( i = 0 ;i <timestep ; i++){ //has to be sequential
		
        if(i%100 == 0) wt = omp_get_wtime();
		find_force(F,R);
		
		velocity_update(F,V);
		
		collision_and_position_update(R,V,D);
		
		velocity_update(F,V);
		
		if(i%100 == 0)
        {
            generate_bin_file(R,i); //cannot be parallized
            out << "Iteration "<<i <<" took "<< (omp_get_wtime()-wt) << "s \n";
        }
	}
    out << "Total Iteration Time : " << (omp_get_wtime()-wtime) << "\n";
    out.close();
    cout<<"Done!"<<endl;
    if(print_pose) //set print_pose to view initial matrix
        print_matrix(R);
    else if (print_force)
    	print_matrix(F);

	return 0;
}








