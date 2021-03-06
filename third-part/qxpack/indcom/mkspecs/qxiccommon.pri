# /////////////////////////////////////////////////////////////////////////////
#
# replace functions 
#
# /////////////////////////////////////////////////////////////////////////////
# =============================================================================
# replace functions used to return a file basename
# args: $$1
# =============================================================================
defineReplace( baseNameOfFile ) {
    b_name    = $$basename( $$1 )
    b_name_tr = $$section( b_name, ., -2, -2 )
    isEmpty(b_name_tr): b_name_tr = $$b_name
    return( $$b_name_tr )
}

# =============================================================================
# generate the temp remove pro file
# args: $$1 -- pro file name full path  $$2 -- tgt file list
# =============================================================================
defineTest( genTempRmvProFile ) {
    pro_file    = $$1
    source_list = $$2

    rmv_tgt_fn_path = "RMV_TGT_FP += \\"

    for ( file, source_list ) {
        fn1= $$absolute_path( $$clean_path( $$file))
        rmv_tgt_fn_path += "   $${fn1} \\"
    }

    rmv_tgt_fn_path += " "

    write_file( $$pro_file, rmv_tgt_fn_path )

    rmv_code_src = \
        "tgt_cntr = "\
        "!isEmpty( RMV_TGT_FP ) {"\
        "for ( tgt_file, RMV_TGT_FP ) { " \
        "    tgt_file = \$\$member( RMV_TGT_FP, \$\$size( tgt_cntr )) " \
        "    tgt_cntr += 1 "\
        "    exists( \$\$tgt_file ) { "\
        "       message( remove \$\$tgt_file ) "\
        "       contains( QMAKE_HOST.os, Windows ) {"\
#        "       win32 {"\
        "           rmv_tgt_file = \$\$replace( tgt_file, /, \\\\ )" \
        "           system( \"del \$\${rmv_tgt_file} /F /Q\")" \
        "       } else {" \
        "           system( \"\\rm -rf \$\${tgt_tgt_file}\" )" \
        "       }" \
        "   }"\
        "}"\
        "}"

    write_file( $$pro_file, rmv_code_src, append )

}


# =============================================================================
# generate the temp copy pro file
# args: $$1 -- pro file name   $$2 -- source lists
#       $$3 -- base dir        $$4 -- install dir
#       $$5 -- include dir
# =============================================================================
defineTest( genTempCopyProFile ) {
    pro_file      = $$4/$$1
    source_list   = $$2
    base_dir      = $$3
    install_dir   = $$4
    include_dir   = $$5

    ! exists( $$4 ) : mkpath( $$4 )  # make sure install dir existed

    cpy_src_file = "CPY_SRC_FILE += \\"
    cpy_tgt_path = "CPY_TGT_PATH += \\"
    cpy_tgt_file = "CPY_TGT_FILE += \\"

    for( file, source_list ) {
        fn1= $$absolute_path( $$clean_path( $$file))
        fn2= $$relative_path( $$fn1, $$base_dir )  # get the relative path base on basedir
        fn3= $$section( fn2, ../, -1, -1 )
        fn_file = $$section( fn3, /, -1, -1 )
        fn_path = $$section( fn3, /,  0, -2 )

        cpy_tgt_path += "    $${include_dir}/$${fn_path} \\"
        cpy_tgt_file += "    $${include_dir}/$${fn_path}/$${fn_file}  \\"
        cpy_src_file += "    $$fn1 \\"
    }
    cpy_src_file += " "
    cpy_tgt_path += " "
    cpy_tgt_file += " "

    write_file( $$pro_file, cpy_src_file )
    write_file( $$pro_file, cpy_tgt_path, append )
    write_file( $$pro_file, cpy_tgt_file, append )

    cpy_code_src = \
        "for ( tgt_path, CPY_TGT_PATH ) {" \
        "    ! exists( \$\$tgt_path ) : mkpath( \$\$tgt_path ) " \
        "}" \
        "tgt_cntr = "\
        "for ( src_file, CPY_SRC_FILE ) { " \
        "    tgt_file = \$\$member( CPY_TGT_FILE, \$\$size( tgt_cntr )) " \
        "    tgt_cntr += 1 "\
        "    exists( \$\$src_file ) { "\
        "       message( copy \$\$src_file ) "\
        "       contains( QMAKE_HOST.os, Windows ) {"\
        "           cpy_tgt_file = \$\$replace( tgt_file, /, \\\\ )" \
        "           cpy_src_file = \$\$replace( src_file, /, \\\\ )" \
      #  "           \$\${QMAKE_COPY_FILE} \$\${cpy_src_file} \$\${cpy_tgt_file} " \
        "           system( \"copy \$\${cpy_src_file} \$\${cpy_tgt_file} /y \")" \
        "       } else {" \
        "           system( \"\\cp -rf \$\${src_file} \$\${tgt_file}\" )" \
       # "           \$\${QMAKE_COPY_FILE} \$\${src_file} \$\${tgt_file} " \
        "       }" \
        "   }"\
        "}"

    write_file( $$pro_file, cpy_code_src, append )
}


# =============================================================================
# generate the temp copy data pro file
# args: $$1 -- pro file name   $$2 -- source lists
#       $$3 -- base dir        $$4 -- install dir
#       $$5 -- dest dir
# =============================================================================
defineTest( genTempCopyDataProFile ) {
    pro_file      = $$4/$$1
    source_list   = $$2
    base_dir      = $$3
    install_dir   = $$4
    dest_dir      = $$5

    ! exists( $$4 ) : mkpath( $$4 )  # make sure install dir existed

    cpy_src_file = "CPY_SRC_FILE += \\"
    cpy_tgt_path = "CPY_TGT_PATH += \\"
    cpy_tgt_file = "CPY_TGT_FILE += \\"

    for( file, source_list ) {
        fn1= $$absolute_path( $$clean_path( $$file))
        fn2= $$relative_path( $$fn1, $$base_dir )  # get the relative path base on basedir
        fn3= $$section( fn2, ../, -1, -1 )
        fn_file = $$section( fn3, /, -1, -1 )
        fn_path = $$section( fn3, /,  0, -2 )

        cpy_tgt_path += "    $${dest_dir} \\"
        cpy_tgt_file += "    $${dest_dir}/$${fn_file}  \\"
        cpy_src_file += "    $$fn1 \\"
    }
    cpy_src_file += " "
    cpy_tgt_path += " "
    cpy_tgt_file += " "

    write_file( $$pro_file, cpy_src_file )
    write_file( $$pro_file, cpy_tgt_path, append )
    write_file( $$pro_file, cpy_tgt_file, append )

    cpy_code_src = \
        "for ( tgt_path, CPY_TGT_PATH ) {" \
        "    ! exists( \$\$tgt_path ) : mkpath( \$\$tgt_path ) " \
        "}" \
        "tgt_cntr = "\
        "for ( src_file, CPY_SRC_FILE ) { " \
        "    tgt_file = \$\$member( CPY_TGT_FILE, \$\$size( tgt_cntr )) " \
        "    tgt_cntr += 1 "\
        "    exists( \$\$src_file ) { "\
        "       message( copy \$\$src_file ) "\
        "       contains( QMAKE_HOST.os, Windows ) {"\
        "           cpy_tgt_file = \$\$replace( tgt_file, /, \\\\ )" \
        "           cpy_src_file = \$\$replace( src_file, /, \\\\ )" \
      #  "           \$\${QMAKE_COPY_FILE} \$\${cpy_src_file} \$\${cpy_tgt_file} " \
        "           system( \"copy \$\${cpy_src_file} \$\${cpy_tgt_file} /y \")" \
        "       } else {" \
        "           system( \"\\cp -rf \$\${src_file} \$\${tgt_file}\" )" \
       # "           \$\${QMAKE_COPY_FILE} \$\${src_file} \$\${tgt_file} " \
        "       }" \
        "   }"\
        "}"

    write_file( $$pro_file, cpy_code_src, append )
}

# =============================================================================
# generate the temp copy files pro file
# args: $$1 -- pro file name   $$2 -- source lists
#       $$3 -- dest. list      $$4 -- install dir
# =============================================================================
defineTest( genTempCopyFileProFile ) {
    pro_file    = $$4/$$1
    src_list    = $$2
    dest_list   = $$3
    install_dir = $$4

    # -------------------------------------------------------------------------
    # check the list count
    # -------------------------------------------------------------------------
    ! exists( $$4 ) : mkpath( $$4 )  # make sure install dir existed
    src_list_cntr  = $$size( src_list )
    dest_list_cntr = $$size( dest_list)

    !equals( src_list_cntr, $$dest_list_cntr ) {
        error( "genTempCopyFileProFile found source list number not equal to dest list" );
    }

    # -------------------------------------------------------------------------
    # source files
    # -------------------------------------------------------------------------
    cpy_src_file = "CPY_SRC_FILE += \\"
    for( src_file, src_list ) {
        src_fn1 = $$absolute_path( $$clean_path( $$src_file))
        cpy_src_file += " $${src_fn1} \\"
    }
    cpy_src_file += " "

    # -------------------------------------------------------------------------
    # dest. files
    # -------------------------------------------------------------------------
    cpy_tgt_path = "CPY_TGT_PATH += \\"
    cpy_tgt_file = "CPY_TGT_FILE += \\"
    for( dst_file, dest_list ) {
        dst_fn1  = $$absolute_path( $$clean_path( $$dst_file))
        dst_path = $$section( dst_fn1, /, 0, -2 )
        cpy_tgt_path += " $${dst_path} \\"
        cpy_tgt_file += " $${dst_fn1}  \\"
    }
    cpy_tgt_path += " "
    cpy_tgt_file += " "

    # -------------------------------------------------------------------------
    # write the src and tgt variables
    # -------------------------------------------------------------------------
    write_file( $$pro_file, cpy_src_file )
    write_file( $$pro_file, cpy_tgt_path, append )
    write_file( $$pro_file, cpy_tgt_file, append )

    cpy_code_src = \
        "for ( tgt_path, CPY_TGT_PATH ) {" \
        "    ! exists( \$\$tgt_path ) : mkpath( \$\$tgt_path ) " \
        "}" \
        "tgt_cntr = 0"\
        "for ( src_file, CPY_SRC_FILE ) { " \
        "    tgt_file = \$\$member( CPY_TGT_FILE, \$\$tgt_cntr ) " \
        "    tgt_cntr = \$\$num_add( \$\$tgt_cntr, 1 )"\
        "    exists( \$\$src_file ) { "\
        "       message( copy \$\$src_file ) "\
        "       contains( QMAKE_HOST.os, Windows ) {"\
        "           cpy_tgt_file = \$\$replace( tgt_file, /, \\\\ )" \
        "           cpy_src_file = \$\$replace( src_file, /, \\\\ )" \
      #  "           \$\${QMAKE_COPY_FILE} \$\${cpy_src_file} \$\${cpy_tgt_file} " \
        "           system( \"copy \$\${cpy_src_file} \$\${cpy_tgt_file} /y \")" \
        "       } else {" \
        "           system( \"\\cp -rf \$\${src_file} \$\${tgt_file}\" )" \
       # "           \$\${QMAKE_COPY_FILE} \$\${src_file} \$\${tgt_file} " \
        "       }" \
        "   }"\
        "}"

    write_file( $$pro_file, cpy_code_src, append )
}
