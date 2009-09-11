#!/usr/bin/env octave
0; % this is to get it to be interpreted as a script and not a function file

function val = read_generator( filename, int_type )
    fid = fopen(filename,'rb');
    [val count] = fread(fid, Inf, int_type);
    fclose(fid);
end

function img = plot_generator( filename, int_type )
    val = read_generator( filename, int_type );
    img = reshape (val, 2400,2500); img /= max(img(:));
    imshow (img);
end

function show_min_max( filename, int_type )
    val = read_generator( filename, int_type );
    printf('results for "%s", values read in as "%s":\n', filename, int_type);
    printf('----------BEGIN-RESULTS------------------\n');
    [max_value max_value_location] = max(val(1:length(val)))
    [min_value min_value_location] = min(val(1:length(val)))
    locations_less_than_zero = find( val(1:length(val)) < 0 )'
    printf('------------END-RESULTS------------------\n');
end

show_min_max( 'test_crappy.dat', 'int32' );
show_min_max( 'test_crappy-intlv.dat', 'int32' );
show_min_max( 'test_kiss.dat', 'uint32' );
show_min_max( 'test_kiss-intlv.dat', 'uint32' );

if ( cellidx (argv(), '--no-plots') == 0)
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
