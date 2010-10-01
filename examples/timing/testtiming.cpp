#include <xylose/Vector.h>
#include <xylose/timing/Timing.h>
#include <xylose/timing/Printer.h>
#include <xylose/timing/element/Exponential.h>

#include <fstream>

//#include <cfloat>


namespace {

  namespace timing = xylose::timing;

  using xylose::Vector;
  using xylose::V3;

  struct ScaledScalarField {
    double bg;
    xylose::timing::Timing timing;

    ScaledScalarField( const double & bg = 0.0 ) : bg(bg) { }

    double operator() ( const Vector<double,3u> & r ) {
      return timing.getVal() * bg;
    }
  };

  struct ScaledVectorField {
    Vector<double,3u> bg;
    xylose::timing::Timing timing;

    ScaledVectorField( const Vector<double,3u> & bg = 0.0 ) : bg(bg) { }

    void operator() ( Vector<double,3u> & a, const Vector<double,3u> & r) {
      a = timing.getVal() * bg;
    }
  };


  struct ScaledGravity : ScaledVectorField {
    ScaledGravity() : ScaledVectorField( V3(0.,0., 9.81) ) { }

    void accel( Vector<double,3u> & a, const Vector<double,3u> & r) {
      return ScaledVectorField::operator() (a,  r);
    }
  };


  /** The millisecond */
  static const double ms = 0.001;

}


int main() {
    double t_max = 15.*ms;
    double dt    = 0.1*ms;

    ScaledGravity gravity;
    ScaledScalarField sfield;
    ScaledVectorField vfield;

    timing::TimingsVector gtimings;
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 0.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 3.0, 1.0, 0.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gravity.timing.timings = gtimings;

    sfield.bg = 1.0;
    timing::TimingsVector stimings;
    stimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 0.0));
    stimings.push_back(new timing::element::Exponential(4.*ms, 1.0, 0.0, 5.0));
    stimings.push_back(new timing::element::Exponential(1.*ms, 1.0, 0.0, 8.0));
    stimings.push_back(new timing::element::Exponential(5.*ms, 3.0, 3.0, 0.0));
    stimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    sfield.timing.timings = stimings;

    vfield.bg = V3(1.0,-1.0,-.5);
    timing::TimingsVector vtimings;
    vtimings.push_back(new timing::element::Exponential(4.*ms, 1.0, 0.0, 0.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 3.0));
    vtimings.push_back(new timing::element::Exponential(5.*ms, 1.0, 0.0, 7.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 3.0, 7.0, 0.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 1.0));
    vfield.timing.timings = vtimings;


    std::ofstream gout("gravity-force.dat");
    std::ofstream sout("scalar-field.dat");
    std::ofstream vout("vector-field.dat");
    Vector<double,3> r(0.0);

    for (double t = 0.0; t <= t_max ; t+=dt) {
        gravity.timing.set_time(t);
        sfield.timing.set_time(t);
        vfield.timing.set_time(t);
        Vector<double,3> a;
        gravity.accel(a,r);

        gout << t << '\t'
            << a << '\n'
            << std::flush;

        sout << t << '\t'
            << sfield(r) << '\n'
            << std::flush;

        vfield(a,r);
        vout << t << '\t'
            << a << '\n'
            << std::flush;
    }

    timing::Printer tp;
    tp.timers.push_back(&vfield.timing);
    tp.timers.push_back(&sfield.timing);
    tp.timers.push_back(&gravity.timing);
    tp.print("timing.dat", 0.0, dt, t_max);
}

