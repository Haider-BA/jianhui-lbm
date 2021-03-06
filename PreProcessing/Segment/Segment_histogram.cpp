#include<iostream>
#include<cmath>
#include<cstdlib>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>

using namespace std; 
      
int main (int argc , char * argv [])
{

int nx=300;
int ny=300;
int nz=220;
int mode=2; //1=two phases, 2=three phases
char poreFileName[128]="Filename.txt";
char poreFileNameVTK[128]="segment.vtk";
char poreFileNameOut1[128]="segment_300x300x220.dat";
char poreFileNameOut2[128]="phase.dat";
//output VTK file,0=no, 1=yes

int ImageJ_sequence=1;
int local_min=5; //half of local min
int peak_check_length=10;
int area_max_radius=10;
int flat_check_histo=20;
int linear_fitting_radius=6;
//double solid_part_shift_percentage=0.3;
int VTK_OUT=1;
int DAT_writting=1;
int TXT_sequence_writting=0;
//===========VTK AND OUT ARE ALL WRITTEN IN BINARY FORMAT===============
//===========================================================





int histo[256];

int*** seg;
double his1[256];
double his2[256];
double his3[256];
double his4[256];
int his5[256];
int peak1,peak2,peak3;
int peak1s,peak2s,peak3s;
int peak1s_cor,peak2s_cor,peak3s_cor;
int val1,val2,val3;
int sum1,sum2,sum3,sum4;
double max1,max2,max3;

	FILE *ftest;
	ifstream fin;
	
	ftest = fopen(poreFileName, "r");

	if(ftest == NULL)
	{
		cout << "\n The pore geometry file (" << poreFileName <<
			") does not exist!!!!\n";
		cout << " Please check the file\n\n";

		exit(0);
	}
	fclose(ftest);

	fin.open(poreFileName);


int pore;
	int i, j, k,ci,cj,ck;
	
	
	
	seg= new int**[nx];
		
	for (i=0; i<nx;i++)
	{
	      
		seg[i] = new int*[ny];

	       for (j=0;j<ny;j++)
	       {
	               
			seg[i][j] = new int[nz];


	               for (k=0;k<nz;k++)
				{
	                       
				seg[i][j][k] = 0;
				}
	       }
	}
		
	

//for (i=0;i<256;i++)
//	{histo[i]=0;his1[i]=0.0;his2[i]=0.0;his4[i]=0.0;}



	for(k=0 ; k<nz ; k++)		
	{        
	 for (i=0;i<256;i++)
	{histo[i]=0;his1[i]=0.0;his2[i]=0.0;his4[i]=0.0;his5[i];}       
	        
	        
	for(j=0 ; j<ny ; j++)
	for(i=0 ; i<nx ; i++)				///*********


	//while (!fin.eof())                                        //**********
		{	
			//fin >> ci >> cj>> ck>>pore;
			fin >> pore;
			
			//if (pore == 0.0)	{Solid[ci-1][cj-1][ck-1] = 0;}
			//if (pore == 0.0)	{Solid[i][j][k] = 0;}
			
			//if (pore == 1.0) 	{Solid[ci-1][cj-1][ck-1] = 1;sum++;}
			//if (pore == 1.0) 	{Solid[i][j][k] = 1;sum++;}
			
			
			
			seg[i][j][k]=pore;
			
			//histo[pore]+=1;
		}
	}	
	fin.close();
	

for(k=0 ; k<nz ; k++)		
	{ 
	        for (i=0;i<256;i++)
	                {histo[i]=0;his1[i]=0.0;his2[i]=0.0;his4[i]=0.0;his5[i];}       
	                
	                for(j=0 ; j<ny ; j++)
	                for(i=0 ; i<nx ; i++)				///*********
	                {	
			
			histo[seg[i][j][k]]+=1;
			}






	
	for (i=10;i<256-10;i++)
		{
		//his1[i]=histo[i+1]-histo[i];
		his2[i]=(histo[i+1]-histo[i-1])/2;
		//his5[i]=(histo[i+1]+histo[i-1]-2*histo[i]);
		}
	
	for (i=linear_fitting_radius;i<256-linear_fitting_radius;i++)
	        {
	                sum1=0;sum2=0;
	                for (j=i-linear_fitting_radius;j<=i+linear_fitting_radius;j++)
	                        {
	                                sum1+=j;sum2+=histo[j];
	                        }
	                        sum1/=linear_fitting_radius*2+1;
	                        sum2/=linear_fitting_radius*2+1;
	                        sum3=0;sum4=0;
	                 for (j=i-linear_fitting_radius;j<=i+linear_fitting_radius;j++)
	                        {
	                                sum3+=(j-sum1)*(histo[j]-sum2);
	                                sum4+=(j-sum1)*(j-sum1);
	                        }
	                        his1[i]=sum3/sum4;
	                
	        }


	for (i=0;i<256;i++)
		{sum1=0;
		for (j=-area_max_radius;j<=0;j++)
			if ((i+j>=0) and (i+j<256) and (histo[i+j]>sum1))
				sum1=histo[i+j];
		his5[i]=sum1;
		}


	

double max_val=0.0;	
double max_val2=0.0;








	//-----------peak search mode2-----------------
	peak3s=0;
	for (i=0;i<256;i++)
	        if (histo[i]>peak3s)
	        {peak3s=histo[i];peak3s_cor=i;}
	peak3=peak3s_cor;
	
	
	for (i=0;i<256;i++)
		{
		if (his5[i]>max_val)
			max_val=his5[i];
		//if (histo[255-i]>max_val2)
		//	max_val2=histo[i];
		his3[i]=max_val;//his4[i]=max_val2;
		}
	peak2s=0;peak1s=0;
	
	//!!!!!warning ending point for peak2 search 
//=====================================================================
	for (i=peak_check_length;i<200;i++)
		{
		if ((his5[i]==histo[i]) and (histo[i]>0))
		{
		        sum1=0;
		        for (j=0;j<=peak_check_length;j++)
			if (abs(his5[i]-his5[i+j])>flat_check_histo)
			{
			sum1=1;
			//cout<<i<<"        **        "<<i+j<<"        "<<his5[i]<<"        "<<his5[i+j]<<endl;
			}
		}
		else
		        sum1=1;
		
		if (sum1==0)
			{//cout<<his5[i]<<"	@@@@@@@ "<<i<<endl;
			if ((his5[i]>peak2s))
				{
				peak2s=his5[i];peak2s_cor=i;
				}
				
					
			}
		}
		
		for (i=peak_check_length;i<peak2s_cor-60;i++)
		{
		if ((his5[i]==histo[i]) and (histo[i]>0))
		{
		        sum1=0;
		        for (j=0;j<=peak_check_length;j++)
			if (abs(his5[i]-his5[i+j])>flat_check_histo)
			{
			sum1=1;
			//cout<<i<<"        **        "<<i+j<<"        "<<his5[i]<<"        "<<his5[i+j]<<endl;
			}
		}
		else
		        sum1=1;
		
		if (sum1==0)
			{//cout<<his5[i]<<"	@@@@@@@ "<<i<<endl;
			if ((his5[i]>peak1s))
				{
				peak1s=his5[i];peak1s_cor=i;
				}
				
					
			}
		}
		
		
		
	//cout<<"asdfasdfas       "<<peak1s<<"	"<<peak2s<<"	"<<peak3s<<endl;


	//if (peak3s_cor>peak2s_cor)
	//	{sum1=peak3s_cor;peak3s_cor=peak2s_cor;peak2s_cor=sum1;}
	if (peak2s_cor<peak1s_cor)
		{sum1=peak2s_cor;peak2s_cor=peak1s_cor;peak1s_cor=sum1;}
	


    	peak1=peak1s_cor;peak2=peak2s_cor;peak3=peak3s_cor;
	
    	
    	
    	
    	if (peak2-peak1<20)
    	        {
    	                peak1=peak2;
    	                peak2=0;peak2s=0;
    	                for (i=peak1+20;i<200;i++)
    	                {
    	                        if ((his5[i]==histo[i]) and (histo[i]>0))
    	                        {
    	                                sum1=0;
    	                                for (j=0;j<=peak_check_length-3;j++)
    	                                        if (abs(his5[i]-his5[i+j])>flat_check_histo)
    	                                        {
    	                                                sum1=1;
			
    	                                        }
    	                        }
    	                        else
    	                                sum1=1;
		
    	                        if (sum1==0)
    	                        {//cout<<his5[i]<<"	@@@@@@@ "<<i<<endl;
    	                                if ((peak2s==0) and (his5[i]>histo[peak1]))
    	                                {
    	                                        peak2s=his5[i];peak2=i;
    	                                }
				
					
    	                        }
    	                }
    	                
    	                
    	        }


	for (i=local_min;i<256-local_min;i++)
		{
		sum1=7000000;
		for (j=i-local_min;j<=i+local_min;j++)
			if (histo[j]<sum1)
				sum1=histo[j];	

		his4[i]=sum1;
		}

	
		
		cout<<"========PEAKS==============="<<k<<endl;
		cout<<peak1<<"        "<<peak2<<"        "<<peak3<<endl;
		//cout<<"============================"<<endl;
		
		for (i=0;i<256;i++)
		        his2[i]=his4[i]-histo[i];
		
		val1=-1;val2=-1;val3=256;
		for (i=peak2-10;i>peak1;i--)
		        {
		                if ((his1[i]<1) and (val1<0))
		                {
		                        val1=i;
		                        for (j=i;j>i-20;j--)
		                        if (his1[j]>5)
		                                val1=-1;
		                }
		        }

	//!!!!!!!warning  val search starting point or radius		        
//====================================================================
		for (i=peak3;i>peak2;i--)
		        {
		              if ((his1[i]<6) and (val2<0) and (histo[i]<=histo[peak2]))
		                      val2=i;
		        }
		   
		
	
	cout<<"===========Segment Vals================"<<endl;
	cout<<val1<<"	"<<val2<<endl;
	cout<<"======================================="<<endl;
	cout<<endl;
	
	for(j=0 ; j<ny ; j++)
	for(i=0 ; i<nx ; i++)	
	if (seg[i][j][k]<val1)
	        seg[i][j][k]=0;
	        else
	                if (seg[i][j][k]<val2)
	                seg[i][j][k]=1;
	                else
	                       seg[i][j][k]=2;
		        
		        
	        
		        
		        
		        
		        
		        
	}	
	//=========================================
//fin.close();


	
	
	if (VTK_OUT==1)
	{
	cout<<"Start writing VTK file"<<endl;
	cout<<nx<<"         "<<ny<<"         "<<nz<<endl;

	ostringstream name;
	name<<poreFileNameVTK;
	//name<<"Clashach_z_sym_196x196x388_8.946.dat";
	ofstream out;
	out.open(name.str().c_str());
	
	
	out<<"# vtk DataFile Version 2.0"<<endl;
	out<<"J.Yang Lattice Boltzmann Simulation 3D Single Phase-Solid-Density"<<endl;
	out<<"ASCII"<<endl;
	out<<"DATASET STRUCTURED_POINTS"<<endl;
	out<<"DIMENSIONS         "<<nx<<"         "<<ny<<"         "<<nz<<endl;       ///*********
	out<<"ORIGIN 0 0 0"<<endl;
	out<<"SPACING 1 1 1"<<endl;
	out<<"POINT_DATA     "<<nx*ny*nz<<endl;				///*********
	out<<"SCALARS sample_scalars float"<<endl;
	out<<"LOOKUP_TABLE default"<<endl;
	
	
	for (k=0;k<nz;k++)
	{
	//cout<<k<<endl;
	for (j=0;j<ny;j++)
	for (i=0;i<nx;i++)
/*	
        if (seg[i][j][k]<val1)
	        out<<0<<" ";
	        else
	                if (seg[i][j][k]<val2)
	                out<<1<<" ";
	                else
	                        out<<2<<" ";
*/	  
		out<<seg[i][j][k]<<" ";
	}
	

	//out.write((char *)(&seg[0][0][0]), sizeof(int)*nx*ny*nz); 

	out.close();

	cout<<"VTK file ouput COMPLETE"<<endl;
	cout<<endl;
	}

	ostringstream name2;ofstream out2;
	//=================================================================
	if (DAT_writting==1)
	{
	cout<<"Start writing cal DAT1 file"<<endl;
	cout<<nx<<"         "<<ny<<"         "<<nz<<endl;
	
	name2<<poreFileNameOut1;
	//name<<"Clashach_z_sym_196x196x388_8.946.dat";
	
	out2.open(name2.str().c_str());
	
		for (k=0;k<nz;k++)
		{
	
	        //cout<<k<<endl;
		for (j=0;j<ny;j++)
		{
			for (i=0;i<nx;i++)
			if (seg[i][j][k]==2)
				out2<<1<<" ";
			else
				out2<<0<<" ";
			//out2<<seg[i][j][k]<<" ";
			out2<<endl;
		}
	
		//out2.write((char *)(&seg[0][0][0]), sizeof(int)*nx*ny*nz); 
		}
	out2.close();
	cout<<"DAT1 file ouput complete"<<endl;
	cout<<endl;

	cout<<"Start writing cal DAT2 file"<<endl;
	cout<<nx<<"         "<<ny<<"         "<<nz<<endl;
	name2.str("");
	name2<<poreFileNameOut2;
	
	
	out2.open(name2.str().c_str());
	
		for (k=0;k<nz;k++)
		{
	
	        //cout<<k<<endl;
		for (j=0;j<ny;j++)
		{
			for (i=0;i<nx;i++)
			{
			if (k<10)
				out2<<seg[i][j][k]*2-1<<" ";
			else
				out2<<1<<" ";
 
			
			}
			out2<<endl;
		}
	
		//out2.write((char *)(&seg[0][0][0]), sizeof(int)*nx*ny*nz); 
		}
	out2.close();
	cout<<"DAT2 file ouput complete"<<endl;
	cout<<endl;


	

	cout<<"----------------------"<<endl;
	}


	
	

	if (TXT_sequence_writting==1)
	{
	cout<<"Start writing TXT file"<<endl;
	for (k=0;k<nz;k++)
	{
	//cout<<k<<endl;
	name2.str("");
	name2<<"Segment_TXT_seqence_"<<setfill('0')<<setw(5)<<k<<".txt";
	
	out2.open(name2.str().c_str());
	

		for (j=0;j<ny;j++)
		{
			for (i=0;i<nx;i++)
			out2<<seg[i][j][k]<<" ";
			out2<<endl;
		}
		out2.close();
		}
	

	cout<<"TXT files ouput complete"<<endl;
	cout<<"----------------------"<<endl;
	cout<<endl;
	}







	ostringstream name3;
	name3<<"histo_test.dat";
	//name<<"Clashach_z_sym_196x196x388_8.946.dat";
	ofstream out3;
	out3.open(name3.str().c_str());
	
	for (i=0;i<256;i++)
		out3<<histo[i]<<endl;
	
	out3.close();

	name3.str("");
	name3<<"histo_test2.dat";
	out3.open(name3.str().c_str());
	for (i=0;i<256;i++)
		out3<<his1[i]<<endl;
	
	out3.close();


	name3.str("");
	name3<<"histo_test3.dat";
	out3.open(name3.str().c_str());
	for (i=0;i<256;i++)
		out3<<his5[i]<<endl;
	
	out3.close();

	name3.str("");
	name3<<"histo_test4.dat";
	out3.open(name3.str().c_str());
	for (i=0;i<256;i++)
		out3<<his4[i]<<endl;
	
	out3.close();
			
}


