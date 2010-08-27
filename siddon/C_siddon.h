#define SOLAR_RADIUS 695000. /* in km */
#define ARCSECOND_TO_RADIANS 0.00000484813681 /* = pi/(60*60*180) */
#define EPSILON 0.00001
#define ROIP_PIXEL_BASE 256
#define INF 1000000. /* en rayons solaires*/
#define DEGRE_TO_RADIAN 0.017453
#define N_MAX 1024/*max size of the cube in number of voxels*/

/* define the detector parameters */
typedef struct
{
  /* steps in radians*/
  float p1;
  float p2;
  /* size in pixels*/
  int n1;
  int n2;
  /* position of the sun center in pixels (can be a fraction of pixel */
  float s1;
  float s2;
}detector;

/* define the position of the detector */
typedef struct
{
  /* spherical coordinates*/
  float lon; /* longitude in radians */
  float lat; /* latitude in radians */
  float rol; /* roll angle in radians (angle between the image vertical and the Sun north pole taken positive clockwise )*/
  float d; /* distance in meters */
  /* cartesian coordinates in carrington */
  float M[3];
}orbit;

/* define the parameters of the Region of interest of the object */
typedef struct
{
  /*size of the object cube in solar radii */
  float d[3];
  /* steps in solar radius : 1 sr = 6.95E8 m */
  float p[3];
  /* size in pixels */
  int n[3];
  /* offset in solar radius : position of the firt pixel to center of rotation */
  /*bounds of the volume in solar radius*/
  float min[3];
  float max[3];
}RoiO;

void init_C_siddon();
static PyObject *call_siddon_sun(PyObject *self, PyObject *args);
static PyObject *call_siddon(PyObject *self, PyObject *args);

int  not_doublematrix(PyArrayObject *mat);
double **pymatrix_to_Carrayptrs(PyArrayObject *arrayin);
double **ptrvector(long n);
void free_Carrayptrs(double **v);

double min3(double,double,double);
double max3(double,double,double);
int signe(double);
void Compare(double*,double*,double,double);

int rotation_matrix(orbit, double [3][3]);
int define_unit_vector(double, double, double[3]);
int apply_rotation(double[3][3] , double[3], double[3]);
double distance_to_center(orbit, double *, double);

int SiddonSun(
	   PyArrayObject*, /* 2D array containing the projection*/
	   int, /* current time index */
	   PyArrayObject*, /* 3D array containing the object */
	   orbit, /* orbit structure containing detector position parameters */
	   RoiO, /* volume structure containing object discretization parameters*/
	   detector, /* detector structure containing detector discretization parameters */
	   int /* projection or backprojection flag : 1 if backprojection */
	   );/* output 1 if no error */

int Siddon(
	   PyArrayObject*, /* 2D array containing the projection*/
	   int, /* current time index */
	   PyArrayObject*, /* 3D array containing the object */
	   orbit, /* orbit structure containing detector position parameters */
	   RoiO, /* volume structure containing object discretization parameters*/
	   detector, /* detector structure containing detector discretization parameters */
	   int /* projection or backprojection flag : 1 if backprojection */
	   );/* output 1 if no error */
