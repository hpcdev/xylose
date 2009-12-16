#!/usr/bin/env octave
0; % this is to get it to be interpreted as a script and not a function file

function val = read_generator( filename, int_type )
    fid = fopen(filename,'rb');
    [val count] = fread(fid, Inf, int_type);
    fclose(fid);
end

function real_max = get_real_max( filename, int_type )
    switch int_type
      case {'int32' 'integer*4'}
        if ( isempty( strfind( filename, 'crappy' ) ) )
            real_max = 2**31;
        else
            real_max = 2**30;
        endif
      case {'uint32'}
        real_max = 2**32;
      otherwise
        error ("invalid value");
    endswitch
end

function img = plot_generator( filename, int_type )
    val = read_generator( filename, int_type );
    if ( length(val) == 0 )
        return;
    endif
    img = reshape (val, 2400,2500); img /= get_real_max( filename, int_type );
    imshow (img);
end

function show_stats( filename, int_type, results_file, ext )
    val = read_generator( filename, int_type );
    if ( length(val) == 0 )
        return;
    endif
    printf('results for "%s", values read in as "%s":\n', filename, int_type);
    printf('----------BEGIN-RESULTS------------------\n');
    [max_value max_value_location] = max(val(1:length(val)))
    [min_value min_value_location] = min(val(1:length(val)))
    sd = std(val(1:length(val)))
    locations_less_than_zero = find( val(1:length(val)) < 0 )'

    real_max = get_real_max(filename, int_type);

    avg = mean(val);
    printf('avg: %g, avg/max (avg/realmax): %g (%g), max/real_max:  %g\n', \
            avg, avg/max_value, avg/real_max, max_value/real_max );
    printf('------------END-RESULTS------------------\n');

    if ( exist('results_file') && !isempty(results_file) )
        if ( exist('ext') && !isempty(ext) )
            results_file = strcat(results_file, ext);
        endif
        fid = fopen(results_file, 'a');
        fprintf(fid, '%g\t', \
                [ max_value, max_value_location, \
                  min_value, min_value_location, \
                  sd, \
                  length(locations_less_than_zero), \
                  avg, real_max ] );
        fprintf(fid,'\n');
        fclose(fid);
    endif
end

results_file = []
[results_arg err] = cellidx (argv(), '--results');
if ( results_arg )
    results_file = argv(){ results_arg+1 }
endif

show_stats( 'test_crappy.dat',       'int32', results_file, '.ice' );
show_stats( 'test_crappy-intlv.dat', 'int32', results_file, '.icei' );
show_stats( 'test_kiss.dat',        'uint32', results_file, '.kss' );
show_stats( 'test_kiss-intlv.dat',  'uint32', results_file, '.kssi' );

[no_plots err] = cellidx (argv(), '--no-plots');
if ( no_plots == 0 )
    figure(1);
    plot_generator( 'test_crappy.dat', 'int32' );
    figure(2);
    plot_generator( 'test_crappy-intlv.dat', 'int32' );
    figure(3);
    plot_generator( 'test_kiss.dat', 'uint32' );
    figure(4);
    plot_generator( 'test_kiss-intlv.dat', 'uint32' );
    figure(1);

    printf('press enter to exit\n');
    fflush(stdout);
    pause;
end
