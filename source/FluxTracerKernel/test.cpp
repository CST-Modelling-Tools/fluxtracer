#include "test.h"

#include "math/equations.h"
#include "functors/VoxelTraversal.h"
#include "functors/FluxPlane.h"
#include "extrusion/ExtrudedReceiver.h"


using namespace  cyi;



void testQuadraticEquation()
{
    QuadraticEquation eq;
    eq.a = 2.;
    eq.b = -38.;
    eq.c = 168.;
    int n = eq.solve();
    cout << n << " roots: " << endl;
    if (n > 0) cout << "x = " << eq.x1 << endl;
    if (n > 1) cout << "x = " << eq.x2 << endl;
    cout << endl;
}

void testVoxelTraversal()
{
    cout << "Creating grid" << endl;
    VoxelTraversal voxelTraversal(
        BoundingBox(vec3r(0., 0., 0.), vec3r(10., 20., 30.)),
        vec3i(10, 20, 30)
    );

    cout << "Processing ray" << endl;
    Ray ray(vec3r(0., 0., 0.), vec3r(10., 20., 30.));
    voxelTraversal.process(ray);

    cout << "Writing output" << endl;
    voxelTraversal.save("FluxTracerOutput.vtk");

    cout << endl;
}

void testFluxPlane()
{
    FluxPlane fp(
        Grid(Interval(-30., 30.), 30),
        Grid(Interval(-50., 50.), 50),
        90.
    );
    fp.save("FluxPlane.vtk");
}

void testExtrudedCylinder()
{
    QVector<double> parameters;

    int nMax = 8;
    int mMax = 3;

    for (int n = 0; n < nMax; ++n)
      for (int m = 0; m < mMax; ++m)
        parameters << 3.*pow(double(m + 1)/mMax, 2.);

    for (int n = 0; n < nMax; ++n)
        parameters << 1. << 2.;

    ExtrudedReceiver cyl;
    cyl.initMesh(nMax, mMax);
    cyl.initPoints(parameters);
    cyl.save("ExtrudedCylinder.vtk");
}


/*

#include "nlopt/nlopt.h"

double myfunc(unsigned n, const double *x, double *grad, void *my_func_data)
{
    if (grad) {
        grad[0] = 0.0;
        grad[1] = 0.5 / sqrt(x[1]);
    }
    return sqrt(x[1]);
}

typedef struct {
    double a, b;
} my_constraint_data;

double myconstraint(unsigned n, const double *x, double *grad, void *data)
{
    my_constraint_data *d = (my_constraint_data *) data;
    double a = d->a, b = d->b;
    if (grad) {
        grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
        grad[1] = -1.0;
    }
    return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
}



void testNLOPT()
{
    nlopt_opt opt;
    opt = nlopt_create(NLOPT_LD_MMA, 2);

    double lb[2] = { -HUGE_VAL, 0 };
    nlopt_set_lower_bounds(opt, lb);

    nlopt_set_min_objective(opt, myfunc, NULL);

    my_constraint_data data[2] = { {2,0}, {-1,1} };

    nlopt_add_inequality_constraint(opt, myconstraint, &data[0], 1e-8);
    nlopt_add_inequality_constraint(opt, myconstraint, &data[1], 1e-8);

    nlopt_set_xtol_rel(opt, 1e-4);

    double x[2] = { 1.234, 5.678 };
    double minf;
    if (nlopt_optimize(opt, x, &minf) < 0) {
        printf("nlopt failed!\n");
    }
    else {
        printf("found minimum at f(%g,%g) = %0.10g\n", x[0], x[1], minf);
    }
    nlopt_destroy(opt);

    cout << "sadfg" << endl;
}

*/

#include "math/2D/Matrix.h"


void findInertiaTensor()
{
    // import
    VoxelGrid grid;
    grid.open("C:/Neo/Programming/VoxelTraversal.vtk");
    vec3i dims = grid.tensor().dimensions();
    vec3r r0 = grid.box().cornerMin() + 0.5*grid.step();


    cout << "Center of mass" << endl;
    // https://en.wikipedia.org/wiki/Center_of_mass
    double M = 0.;
    double Rx = 0.;
    double Ry = 0.;
    double Rz = 0.;

    vec3i n;
    for (n.mx = 0; n.mx < dims.mx; ++n.mx)
    for (n.my = 0; n.my < dims.my; ++n.my)
    for (n.mz = 0; n.mz < dims.mz; ++n.mz) {
        real m = grid.tensor()(n); // mass
        M += m;
        vec3r r = r0 + grid.step()*n; // position

        Rx += m*r.mx;
        Ry += m*r.my;
        Rz += m*r.mz;
    }

    Rx /= M;
    Ry /= M;
    Rz /= M;
    vec3r R(Rx, Ry, Rz);
    cout << R << endl << endl;


    cout << "Inertia tensor" << endl;
    // https://en.wikipedia.org/wiki/Moment_of_inertia#Inertia_tensor
    double Ixx = 0.;
    double Iyy = 0.;
    double Izz = 0.;
    double Ixy = 0.;
    double Ixz = 0.;
    double Iyz = 0.;

    r0 -= R; // around center of mass
    for (n.mx = 0; n.mx < dims.mx; ++n.mx)
    for (n.my = 0; n.my < dims.my; ++n.my)
    for (n.mz = 0; n.mz < dims.mz; ++n.mz) {
        real m = grid.tensor()(n); // mass
        vec3r r = r0 + grid.step()*n; // position

        Ixx += m*(r.my*r.my + r.mz*r.mz);
        Iyy += m*(r.mx*r.mx + r.mz*r.mz);
        Izz += m*(r.mx*r.mx + r.my*r.my);

        Ixy -= m*r.mx*r.my;
        Ixz -= m*r.mx*r.mz;
        Iyz -= m*r.my*r.mz;
    }

    Matrix<double> ans(3, 3);
    ans.setData({
        Ixx, Ixy, Ixz,
        Ixy, Iyy, Iyz,
        Ixz, Iyz, Izz,
    });


    cout <<  "{";
    for (int r = 0; r < ans.rows(); ++r) {
        cout << "{";
        for (int c = 0; c < ans.cols(); ++c) {
            cout << ans(r, c);
            if (c < ans.cols() - 1) cout << ", ";
        }
        cout << "}";
        if (r < ans.rows() - 1) cout << ", ";
    }
    cout <<  "}";
    cout << endl << endl;
    
    //
    // eigenvalues
    // https://en.wikipedia.org/wiki/Eigenvalue_algorithm#3%C3%973_matrices
}
