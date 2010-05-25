#!/usr/bin/env gnuplot

set xlabel 't (s)'
set ylabel 'x(t)'

set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nX-Axis"
plot \
    'out.dat' u 1:2  w lp    t 'RK Solution', \
    ''        u 1:9  w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;

set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nY-Axis"
plot \
    'out.dat' u 1:3  w lp    t 'RK Solution', \
    ''        u 1:10 w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;

set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nZ-Axis"
plot \
    'out.dat' u 1:4  w lp    t 'RK Solution', \
    ''        u 1:11 w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;


set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nv_x-Axis"
plot \
    'out.dat' u 1:5  w lp    t 'RK Solution', \
    ''        u 1:12 w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;

set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nv_y-Axis"
plot \
    'out.dat' u 1:6  w lp    t 'RK Solution', \
    ''        u 1:13 w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;

set title "Solution to x\"(t) = -k*x - k/10 * sin(wp * t)\nv_z-Axis"
plot \
    'out.dat' u 1:7  w lp    t 'RK Solution', \
    ''        u 1:14 w lp pt 6 ps 3 t 'Analytic Solution'

pause -1;

set title "dt_step used\nin solution to x\"(t) = -k*x - k/10 * sin(wp * t)"
plot \
    'out.dat' u 1:8  w lp    t 'RK Solution'

pause -1;

