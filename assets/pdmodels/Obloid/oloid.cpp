// oloid.cpp
// Keenan Crane
// August 31, 2014
//
// Generates a closed manifold quad mesh of the developable "oloid"
// surface, using (roughly) the requested number of polygons.
//
// usage: oloid out.obj nPolygons
//

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int idx( int i, int n )
// reindex to avoid duplicate vertices (otherwise, mesh will have a boundary)
{
   if( i > 0 )
   {
      i--;
   }

   if( i == n*4-2 )
   {
      i--;
   }

   return 1+i;
}

int main( int argc, char** argv )
{
   if( argc != 3 )
   {
      cerr << "Generates a closed manifold quad mesh of the developable \"oloid\"" << endl;
      cerr << "surface, using (roughly) the requested number of polygons." << endl;
      cerr << endl;
      cerr << "usage: " << argv[0] << " out.obj nPolygons" << endl;
      return 1;
   }

   ofstream out( argv[1] );
   if( !out.is_open() )
   {
      cerr << "Error: could not open file " << argv[1] << " for output!" << endl;
      return 2;
   }

   int nPolygons = strtol( argv[2], NULL, 0 );
   int N = nPolygons/4 + 1;

   // output vertices
   for( int i = 0; i < N; i++ )
   {
      const double pi = 3.141592653589793;
      double s = (double) i / (double) (N-1);
      double t = (2.*pi/3.) * ( 3.*s - 2.*pow( s, 1.5 ) ); // reparameterize to get more uniform spacing

      // circular arc 1
      double ax = sin(t);
      double ay = -cos(t);
      double az = 0.;

      // circular arc 2
      double bx = 0.;
      double by = 1./(1.+cos(t));
      double bz = sqrt(1.+2.*cos(t))/(1.+cos(t));

      // construct each arc using two separate pieces (for symmetry)
                     out << "v " <<  ax << " " << ay << " " <<  az << endl;
      if( i  >  0  ) out << "v " << -ax << " " << ay << " " <<  az << endl; // omit duplicate vertex
                     out << "v " <<  bx << " " << by << " " <<  bz << endl;
      if( i != N-1 ) out << "v " <<  bx << " " << by << " " << -bz << endl; // omit duplicate vertex
   }

   // output quads, as pairwise connections between each of two pieces on two arcs
   for( int i = 0; i < N-1; i++ )
   {
      int v0, v1, v2, v3;
      int j = i+1;

      v0 = idx( j*4+0, N );
      v1 = idx( j*4+2, N );
      v2 = idx( i*4+2, N );
      v3 = idx( i*4+0, N );
      out << "f " << v0 << " " << v1 << " " << v2 << " " << v3 << endl;

      v0 = idx( i*4+0, N );
      v1 = idx( i*4+3, N );
      v2 = idx( j*4+3, N );
      v3 = idx( j*4+0, N );
      out << "f " << v0 << " " << v1 << " " << v2 << " " << v3 << endl;

      v0 = idx( i*4+1, N );
      v1 = idx( i*4+2, N );
      v2 = idx( j*4+2, N );
      v3 = idx( j*4+1, N );
      out << "f " << v0 << " " << v1 << " " << v2 << " " << v3 << endl;

      v0 = idx( j*4+1, N );
      v1 = idx( j*4+3, N );
      v2 = idx( i*4+3, N );
      v3 = idx( i*4+1, N );
      out << "f " << v0 << " " << v1 << " " << v2 << " " << v3 << endl;
   }

   return 0;
}

