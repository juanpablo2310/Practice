#include <vector>
#include <string>
#include <iomanip> 
#include <iostream> 
#include <cmath> 
////////////////////////////////////////////////////////////////
#include <TGraph.h>
#include <TGraph2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TMatrix.h>
#include <TMath.h>
using namespace std;
int main() {
int nx = 140, ny = 140;
int nt = 50;
double Re = 1., VW = 0.3, SW = VW;
double xmax = 20., ymax = 10.;

double xObs1 = 60, xObs2 = 70;
double yObs1 = 60, yObs2 = 70;

TMatrix V(nx+1,ny+1);
TMatrix S(nx+1,ny+1);

for( int i=1; i< nx ; ++i) {
	for(int j=0; j<ny ; ++j) {
		double y = (double) j*(ymax/ny);
		S[i][j] = 0.;
		V[i][j] = y;
	}
}

std::vector<TMatrix> vec;
std::vector<TMatrix> vec1;
vec1.push_back( S );
vec.push_back( V );

for(int t=1; t <= nt; ++t) {
	//Metodo de relajación para el flujo
	for(int i=1; i< nx; ++i) {
		double Tem = 1. + 2.*V[i][ny-1] + V[i+1][ny] + V[i-1][ny];
		V[i][ny] = SW*(Tem/4.) + (1. - SW)*V[i][ny];
	}
	for(int j=1; j< ny; ++j) {
		double Tem = 2.*V[2][j]+V[1][j+1]+V[1][j-1]-S[1][j];
		V[1][j] = SW*(Tem/4.) + (1. - SW)*V[1][j];
	}
	for(int j=1; j<ny; ++j) {
		double Tem = 2.*V[nx-1][j]+V[nx][j-1]+V[nx][j+1]-S[nx][j];
		V[nx][j] = SW*(Tem/4.) + (1. - SW)*V[nx][j];
	}
	for(int i=0; i<nx + 1; ++i) {
		V[i][1] = 0.;
	}
	for(int i=1; i< nx; ++i) {
		for(int j=1; j< ny; ++j) {
			double Tem = V[i][j-1] + V[i][j+1] + V[i+1][j] - S[i][j];
			V[i][j] = SW*(Tem/4.) + (1. - SW)*V[i][j];
		}
	}
	//obstaculo
	for(int i=xObs1; i<=xObs2; ++i){
		for(int j=yObs1; j<=yObs2; ++j) {
			V[i][j] = 0.;
		}
	}
	
	V[0][ny] = (V[1][ny] + V[1][ny-1])/2.;
	V[nx][ny] = (V[nx-1][ny] + V[nx][ny-1])/2.;
	
	//Metodo de Relajacion para la vorticidad
	
	double Re4 = Re/4.;
	for(int j=1;  j<ny-2; ++j) {
		double Tem = 2.*S[nx-1][j] + S[nx][j+1] + S[nx][j-1];
		S[nx][j] = VW*(Tem/4.) + (1. - VW)*S[nx][j];
	}
	
	for(int i=1; i <nx; ++i) {
		for(int j=1; j< ny; ++j) {
			double Tem = S[i][j+1]+S[i][j-1]+S[i+1][j]+S[i-1][j];
			double Tem2 = Re4*(V[i][j+1] - V[i][j-1])*(S[i+1][j]-S[i-1][j]);
			double Tem3 = Re4*(V[i+1][j] - V[i-1][j])*(S[i][j+1]-S[i][j-1]);
			S[i][j] = VW*((Tem-Tem2+Tem3)/4.)+(1. - VW)*S[i][j];
		}
	}
	
	for(int i=0; i<nx+1; ++i) {
		S[i][0]= 0.;
		S[i][ny]=0.;
	}
	for(int j=1; j< ny; ++j) {
		S[0][j] = 0.;
	}
	
	//Vorticidad en el obstaculo
	
	for(int j=yObs1; j<=yObs2; ++j) {
		S[xObs1][j] = 2.*(V[xObs1-1][j]-V[xObs1][j]);
		S[xObs2][j] = 2.*(V[xObs2+1][j]-V[xObs2][j]);
	}
	for(int i=xObs1+1; i<=xObs2-1; ++i) {
		S[i][yObs2] = 2.*(V[i][yObs2+1]-V[i][yObs2]);
	}
	for(int i=xObs1+1; i<=xObs1-1; ++i) {
		for(int j=yObs1; j<=yObs2-1; ++j) {
			S[i][j] = 0.;
		}
	}
	
	vec.push_back( V );
	vec1.push_back( S );
	//~ std::cout<<"iteracion :"<<t<<std::endl;
	cout<<S[80][80]<<endl;

}

TMatrix Vtmp(nx+1,ny+1);

for(int n=0; n< (int)vec.size(); ++n) {
	TGraph2D g((nx+1)*(ny+1));
	int point = 0;
	Vtmp = vec[ n ];
	for( int i=0; i< nx +1; ++i) {
		for (int j=0; j< ny + 1; ++j) {
			double x = 0. + (double) i*(xmax/nx);
			double y = 0. + (double) j*(ymax/ny);
			g.SetPoint(point, x, y, Vtmp[i][j]);
			++point;
		}
	}
	g.SetTitle( " Simulacion de un Fluido Incompresible " );
    g.GetXaxis() ->SetTitle("v");
    g.GetYaxis() ->SetTitle("u");
    
    TCanvas c( " ", " ", 1000, 1000 );
    c.cd();
    g.Draw( "CONT1" );    
    c.Print( "Flujo.gif+15" );
    std::cout << "Plotting t " << n << std::endl;
}

return 0;
} 
