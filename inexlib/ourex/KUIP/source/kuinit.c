/*
 * $Id: kuinit.c,v 1.14 2009/11/18 14:09:04 barrand Exp $
 *
 * $Log: kuinit.c,v $
 * Revision 1.14  2009/11/18 14:09:04  barrand
 * *** empty log message ***
 *
 * Revision 1.13  2009/06/23 08:04:08  barrand
 * *** empty log message ***
 *
 * Revision 1.12  2009/06/22 16:36:14  barrand
 * *** empty log message ***
 *
 * Revision 1.11  2006/10/16 12:27:53  barrand
 * *** empty log message ***
 *
 * Revision 1.10  2006/10/16 12:20:37  barrand
 * *** empty log message ***
 *
 * Revision 1.9  2005/09/22 07:30:59  barrand
 * *** empty log message ***
 *
 * Revision 1.8  2005/09/22 07:28:48  barrand
 * *** empty log message ***
 *
 * Revision 1.7  2005/09/22 07:27:26  barrand
 * *** empty log message ***
 *
 * Revision 1.6  2005/09/22 07:26:10  barrand
 * *** empty log message ***
 *
 * Revision 1.5  2004/09/04 09:39:26  barrand
 * *** empty log message ***
 *
 * Revision 1.4  2003/11/04 09:54:38  barrand
 * *** empty log message ***
 *
 * Revision 1.3  2003/11/04 07:57:28  barrand
 * *** empty log message ***
 *
 * Revision 1.2  1999/11/08 11:28:37  barrand
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/05/25  13:09:15  barrand
 * KUIP
 *
 * Revision 1.2  1996/10/14 09:01:23  cremel
 * Change place of initialization for do_exit and do_quit (from kuwhat to
 * kuinit) in order to allow "exit" in pawlogon.kumac.
 *
 * Revision 1.1.1.1  1996/03/08 15:32:58  mclareni
 * Kuip
 *
 */
/*CMZ :  2.07/06 14/06/95  15.24.50  by  Gunter Folger*/
/*-- Author :    Alfred Nathaniel   25/08/92*/

/*GB #define EXTERN*/                  /* allocate storage for globals */
#include "kuip/kuip.h"
#include "kuip/kfor.h"
#include "kuip/kcom.h"
#include "kuip/ksig.h"
#include "kuip/kbrow.h"
#include "kuip/kmenu.h"
#include "kuip/kflag.h"
#include "kuip/klink.h"
#include "kuip/khash.h"
#include "kuip/kalias.h"

/*GB : Darwin : have to initialize structs*/
char cdf_def[80] = {0};
char cdf_value[80] = {0};
KcFlags kc_flags = {0};
KcValue kc_value = {0};
KcWindow kc_window = {0};
KcBreak kc_break = {0};
KcAlias kc_alias = {0};
KcJmpAddr kjmpaddr = {
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0
};

#ifdef vms
static int vaxc_rtl_initialized;
#endif

void Kuinit(nwords)
     INTEGER *nwords;
{
#ifdef APOLLO
  status_$t istat;
#endif
  char *host_editor;
  char *host_psviewer;
  char *host_pager;
  char *host_shell;
  char *recall_style;
  char *line;
/*KEEP,VIDQQ,T=PASS.*/
static char vidqq[] = 
"@(#)KUIP     2.07/08  24/01/96  17.26.56  C: 24/01/96  17.27.58";
/*KEND.*/

#ifdef vms
  if( !vaxc_rtl_initialized ) {
    vaxc_rtl_initialized = 1;
    VAXC$CRTL_INIT();           /* init runtime library */
  }
#endif

#ifdef BROKEN_F77_IO
  /*
   * If the Fortran I/O RTL is not based on the C stream functions
   * we have to switch stdout to line buffered mode because otherwise
   * Fortran and C output would occur out of sequence.
   */
  if( !isatty( fileno(stdin) ) )
    setlinebuf( stdin );
  if( !isatty( fileno(stdout) ) )
    setlinebuf( stdout );
#endif

  /* check assumptions about type definitions */
#ifndef APOLLO                  /* avoid compiler warning */
  if( sizeof(int) < 4 )
    kprintf( " *** KUIP won't work: sizeof(int) < 4\n" );
  if( sizeof(INTEGER) != sizeof(REAL) )
    kprintf( " *** KUIP won't work: sizeof(INTEGER) != sizeof(REAL)\n" );
#endif

  Kiinit(nwords);               /* initialize Fortran part */

  Kuidf1();
  Kuidf2();
  Macdef();

  kc_alias.arg_table = khash_create( ALIAS_TABLE_SIZE );
  kc_alias.cmd_table = khash_create( ALIAS_TABLE_SIZE );
  kc_alias.global_value = khash_create( ALIAS_TABLE_SIZE );
  kc_alias.global_text  = khash_create( ALIAS_TABLE_SIZE );
  kc_alias.var_table = kc_alias.global_value;
  khash_insert( kc_alias.global_value, "@", "0", 0 );
  khash_insert( kc_alias.global_text, "@", "macro return value", 0 );

  strcpy( kc_flags.appl_file, "KUIPAPPL.TMP" );
  strcpy( kc_flags.help_file, "KUIPHELP.TMP" );
  strcpy( kc_flags.uhlp_file, "KUIPUHLP.TMP" );
#ifdef IBMVM
  strcat( kc_flags.appl_file, ".A" );
  strcat( kc_flags.help_file, ".A" );
  strcat( kc_flags.uhlp_file, ".A" );
#endif

#ifdef WINNT
  if( getenv( "HOME" ) == NULL) /* We should create HOME variable */ {
    /* GB */
    if(getenv("HOMEDRIVE")==NULL) {
      kprintf( " Kuinit : HOMEDRIVE not set. Set it to C:\n");
      putenv( strdup( "HOMEDRIVE=C:"));
    }
    if(getenv("HOMEPATH")==NULL) {
      /* kprintf( " Kuinit : HOMEPATH not set. Set it to \\ \n"); */
      putenv( strdup( "HOMEPATH=\\"));
    } /* GB : end */
    putenv( str3dup( "HOME=", getenv( "HOMEDRIVE" ), getenv( "HOMEPATH" ) ) );
  }
#endif

#if defined(UNIX) && !defined(MSDOS)
  sprintf( kc_flags.appl_file, "%s/kuipappl%d.tmp",
          getenv( "HOME" ), (int)getpid() );
  sprintf( kc_flags.help_file, "%s/kuiphelp%d.tmp",
          getenv( "HOME" ), (int)getpid() );
  sprintf( kc_flags.uhlp_file, "%s/kuipuhlp%d.tmp",
          getenv( "HOME" ), (int)getpid() );
#endif
#ifdef vms
  sprintf( kc_flags.appl_file, "%sKUIPAPPL%d.TMP;0",
          getenv( "HOME" ), getpid() );
  sprintf( kc_flags.help_file, "%sKUIPHELP%d.TMP;0",
          getenv( "HOME" ), getpid() );
  sprintf( kc_flags.uhlp_file, "%sKUIPUHLP%d.TMP;0",
          getenv( "HOME" ), getpid() );
#endif

#if defined(UNIX) || defined(vms)
  /* save initial working directory */
  if(getcwd( kc_flags.init_wdir, (sizeof kc_flags.init_wdir) )) {}/*G.Barrand*/
# ifdef UNIX
#  ifdef MSDOS
  strcat( kc_flags.init_wdir, "\\" );
#  else
  strcat( kc_flags.init_wdir, "/" );
#  endif
# endif
#endif

  kc_flags.real_time = time( NULL );
  kc_flags.user_time = clock();
  Kutim0();

#ifdef APOLLO
  pad_$isa_dm_pad( stream_$stdout, &istat );
  kc_window.is_a_pad = (istat.all == status_$ok);
#else
  kc_window.is_a_pad = 0;
#endif

#ifdef IBM370
  kc_window.is_a_tty = 0;
#else
  kc_window.is_a_tty = (isatty( fileno(stdin) ) && isatty( fileno(stdout) ));
#endif

  ku_exel( "/KUIP/SET_SHOW/BREAK !" );
  ku_exel( "/KUIP/SET_SHOW/COLUMNS -1" );
  ku_exel( "/KUIP/SET_SHOW/DOLLAR 'ON'" );

  host_editor = getenv( "KUIPEDITOR" );
  if( host_editor == NULL )
    host_editor = getenv( "EDITOR" );
  if( host_editor == NULL ) {
#ifdef UNIX
# ifdef MSDOS
#  ifdef WINNT
    host_editor = "notepad";
#  else
    host_editor = "edit";
#  endif
# else
    host_editor = "vi";
# endif
# ifdef APOLLO
    if( kc_window.is_a_pad )
      host_editor = "DM";
# endif
#define _done_
#endif
#ifdef vms
    host_editor = "EDT";
#define _done_
#endif
#ifdef IBMMVS
    host_editor = "ISPF_editor";
#define _done_
#endif
#ifdef IBMVM
    host_editor = "XEDIT";
#define _done_
#endif
#ifdef _done_
#undef _done_
#else
    host_editor = "EDIT";
#endif
  }
  line = str3dup( "/KUIP/SET_SHOW/HOST_EDITOR '", host_editor, "'" );
  ku_exel( line );
  free( line );
  ku_exel( "/KUIP/SET_SHOW/HOST_EDITOR 'WINDOW'" );
  ku_exel( "/KUIP/SET_SHOW/HOST_EDITOR 'PAD'" );

  host_psviewer = getenv( "KUIPPSVIEWER" );
  if( host_psviewer == NULL )
    host_psviewer = getenv( "PSVIEWER" );
  if( host_psviewer == NULL ) {
#ifdef __APPLE__
    host_psviewer = "open"; /*GB*/
#elif defined(WIN32)
    host_psviewer = "gsview32.exe"; /*GB*/
#elif defined(vms)
    host_psviewer = "VIEW/FORM=PS/INTERFACE=DECWINDOWS";
#else /*UNIX*/
    host_psviewer = "ghostview";
#endif
  }
  line = str3dup( "/KUIP/SET_SHOW/HOST_PSVIEWER '", host_psviewer, "'" );
  ku_exel( line );
  free( line );

  host_pager = getenv( "KUIPPAGER" );
  if( host_pager == NULL )
    host_pager = getenv( "PAGER" );
  if( host_pager == NULL ) {
#ifdef vms
    host_pager = "TYPE/PAGE";
#else
#ifdef WIN32 /*GB*/
    host_pager = "TYPE";
#else
    host_pager = kc_value.set_host_editor;
#endif
#endif
  }
  line = str3dup( "/KUIP/SET_SHOW/HOST_PAGER '", host_pager, "'" );
  ku_exel( line );
  free( line );

#ifdef UNIX
# ifdef MSDOS
  ku_exel( "/KUIP/SET_SHOW/HOST_PRINTER 'print $*'" );
# else
  ku_exel( "/KUIP/SET_SHOW/HOST_PRINTER 'lp $*'" );
# endif
#else
  ku_exel( "/KUIP/SET_SHOW/HOST_PRINTER 'XPRINT $*'" );
#endif

  host_shell = getenv( "SHELL" );
  if( host_shell == NULL ) {
    host_shell = "sh";
#ifdef MSDOS
    host_shell = "COMMAND.COM";
#endif
#ifdef WINNT
    host_shell = "start";
#endif
#ifdef IBMVM
    host_shell = "CMS";
#endif
#ifdef IBMMVS
    host_shell = "TSO";
#endif
#ifdef NEWLIB
    host_shell = "NEWLIB";
#endif
#ifdef vms
    host_shell = "DCL";
#endif
  }
  line = str3dup( "/KUIP/SET_SHOW/HOST_SHELL '", host_shell, "'" );
  ku_exel( line );
  free( line );

  ku_exel( "/KUIP/SET_SHOW/PROMPT 'Kuip []'" );

  recall_style = "NONE";
  if( kc_window.is_a_tty && !kc_window.is_a_pad ) {
#ifdef vms
    int setbuff[3];
    short chan;
    struct dsc$descriptor_s ttname;
    var_descriptor( ttname, "TT:" );
    sys$assign( &ttname, &chan, 0, 0 );
    sys$qiow( 0, chan, IO$_SENSEMODE, 0, 0, 0,
             setbuff, (sizeof setbuff), 0, 0, 0, 0 );
    sys$dassgn( chan );

    if( setbuff[2] & TT2$M_INSERT )
      recall_style = "DCL";
    else
      recall_style = "DCLO";
#else
    recall_style = "KSH";
#endif
  }
  line = str3dup( "/KUIP/SET_SHOW/RECALL_STYLE '", recall_style, "'" );
  ku_exel( line );
  free( line );

  ku_exel( "/KUIP/SET_SHOW/RECORDING !" );
  ku_exel( "/KUIP/SET_SHOW/ROOT /" );
  ku_exel( "/KUIP/SET_SHOW/STYLE 'C'" );
  ku_exel( "/KUIP/SET_SHOW/TIMING 'OFF'" );

#ifdef NEWLIB
  ku_exel( "/KUIP/HELP -NOEDIT" );
#else
  ku_exel( "/KUIP/HELP -EDIT" );
#endif

  ku_exel( "/MACRO/DEFAULTS . 'COMMAND'" );

#ifdef NEWLIB
  ku_exel( "/KUIP/ALIAS/CREATE NEWLIB SHELL C" );
#endif

  kc_window.palette_flag = 0;

  kc_flags.do_exit = 0;
  kc_flags.do_quit = 0;
}


/*
 * receive the starting address of /PAWC/
 * the address might not be fixed at link time (dynamic commons on IBM)
 */
/*GB void Kipawc( pawc )
     struct COMMON_PAWC *pawc;
{
  Pawc = pawc;
}*/

void F77_ENTRY_IC(Fmemcpy,cstr,fstr)
/* { */
  memcpy( (char*)cstr, fstr, len_fstr );
}


static int    main_argc = 0;
static char **main_argv = NULL;

/*
 * set the program arguments from a C main(argc,argv)
 */
void k_setar( size_t argc,
             char **argv )
{
  main_argc = argc;
  main_argv = argv;
}


/*
 * return the program arguments as a string
 */
char *k_getar()
{
  static char *args = NULL;

  if( args == NULL ) {

#ifdef vms
    /* KGETAR/KUARGS can be called before KUINIT */
    if( !vaxc_rtl_initialized ) {
      vaxc_rtl_initialized = 1;
      VAXC$CRTL_INIT();         /* init runtime library */
    }
#endif

    if( main_argv != NULL ) {
      int i;
      args = strdup( "" );
      for( i = 1; i < main_argc; i++ ) {
        args = mstrcat( args, main_argv[i] );
        if( i+1 < main_argc )
          args = mstrcat( args, " " );
      }
    }
    else {

#ifdef UNIX

      INTEGER argc = Kiargc();
      /*GB char argv[256]; */
      /*GB INTEGER i; */

      args = strdup( "" );
      /*GB for( i = 1; i <= argc; i++ ) {
        char *p;

        F77_CALL_xC(Getarg,&i,argv,(sizeof argv));
        p = fstrdup( argv, (sizeof argv) );
        args = mstrcat( args, p );
        free( p );
        if( i < argc )
          args = mstrcat( args, " " );
      } GB*/
#endif

#ifdef vms
      struct dsc$descriptor_s argv_dsc;
      char argv[256];
      short largv;
      int force_prompt = 0;

      fix_descriptor( argv_dsc, argv, (sizeof argv) );
      lib$get_foreign( &argv_dsc, NULL, &largv, &force_prompt );
      args = strupper( fstrdup( argv, largv ) );
#endif

#ifdef IBM370
      char argv[100];
      INTEGER largv = (sizeof argv);
      fstrset( argv, largv, "" );
      F77_CALL_xC(Goparm,&largv,argv,largv);
      args = strupper( fstrdup( argv, largv ) );
#endif

    }
  }
  return args;
}


void F77_ENTRY_C(Kgetar,chline)
/* { */
  fstrset( chline, len_chline, k_getar() );
}


/*
 * parse the argument string
 */
void F77_ENTRY_C4I(Kuargs,prgnam,logonf,batchf,logfil,ierror)
/* { */
  char *prog = fstrdup( prgnam, len_prgnam );
  char *args = str3dup( " ", k_getar(), " " );
  char *bval = NULL;
  char *lval = NULL;
  int nval = 0;
  char *bopt;
  char *lopt;
  char *nopt;
  char *p;

  *ierror = 0;
  fstrset( batchf, len_batchf, "" );
  fstrset( logfil, len_logfil, "" );

#ifdef ARG_STYLE_UNIX
  bopt = "-b";
  lopt = "-l";
  nopt = "-n";

  if( (p = strstr( args, " -b " )) != NULL ) {
    p += 4;
    while( *p == ' ' )
      p++;
    bval = strdup( p );
    if( (p = strchr( bval, ' ' )) != NULL )
      *p = '\0';
  }

  if( (p = strstr( args, " -l " )) != NULL ) {
    p += 4;
    while( *p == ' ' )
      p++;
    lval = strdup( p );
    if( (p = strchr( lval, ' ' )) != NULL )
      *p = '\0';
  }

  if( strstr( args, " -n " ) != NULL )
    nval = 1;
#endif

#ifdef ARG_STYLE_VMS
  bopt = "/BATCH";
  lopt = "/LOGON";
  nopt = "/NOLOG";
  strupper( args );

  if( (p = strstr( args, "/BATCH" )) != NULL ) {
    p += 7;
    if( p[-1] != '=' )
      p = input_line( "Batch command file:", '\0' );
    if( p != NULL ) {
      bval = strdup( p );
      if( (p = strchr( bval, ' ' )) != NULL )
        *p = '\0';
      if( (p = strchr( bval, '/' )) != NULL )
        *p = '\0';
    }
  }

  if( (p = strstr( args, "/LOGON" )) != NULL ) {
    p += 7;
    if( p[-1] != '=' )
      p = input_line( "Logon command file:", '\0' );
    if( p != NULL ) {
      lval = strdup( p );
      if( (p = strchr( lval, ' ' )) != NULL )
        *p = '\0';
      if( (p = strchr( lval, '/' )) != NULL )
        *p = '\0';
    }
  }

  if( strstr( args, "/NOLOG" ) != NULL )
    nval = 1;
#endif

#ifdef ARG_STYLE_CMS
  bopt = "(BATch";
  lopt = "(LOGon";
  nopt = "(NOLog";
  if( (p = strchr( args, '(' )) == NULL ) {
    args[0] = '\0';
  }
  else {
    strcpy( args, p );
    strupper( args );
    args[0] = ' ';
  }

  if( (p = strstr( args, " BAT" )) != NULL ) {
    p += 4;
    if( p[0] == 'C' ) {
      p++;
      if( p[0] == 'H' )
        p++;
    }
    if( p[0] == '=' )
      p++;
    else
      p = "";
    bval = strdup( p );
    if( (p = strchr( bval, ' ' )) != NULL )
      *p = '\0';
  }

  if( (p = strstr( args, " LOG" )) != NULL ) {
    p += 4;
    if( p[0] == 'O' ) {
      p++;
      if( p[0] == 'N' )
        p++;
    }
    if( p[0] == '=' )
      p++;
    else
      p = "";
    lval = strdup( p );
    if( (p = strchr( lval, ' ' )) != NULL )
      *p = '\0';
  }

  if( (p = strstr( args, " NOL" )) != NULL ) {
    p += 4;
    if( p[0] == 'O' ) {
      p++;
      if( p[0] == 'G' )
        p++;
    }
    if( p[0] == ' ' || p[0] == '\0' )
      nval = 1;
  }
#endif

  if( lval != NULL ) {
    if( bval != NULL ) {
      kprintf( " %s: Options %s and %s are mutually exclusive\n",
             prog, bopt, lopt );
      *ierror = 1;
    }
    if( nval != 0 ) {
      kprintf( " %s: Options %s and %s are mutually exclusive\n",
             prog, nopt, lopt );
      *ierror = 1;
    }
  }

  if( *ierror == 0 && bval != NULL ) {
    if( bval[0] == '\0' ) {
      kprintf( " %s: Batch command file expected\n", prog );
      *ierror = 1;
    }
    else {
      FILE *stream;
      p = ku_home( bval, "kumac" );
      free( bval );
      bval = p;
      if( (stream = fopen( bval, "r" )) == NULL ) {
        kprintf( " %s: Batch command file %s not found\n", prog, bval );
        *ierror = 1;
      }
      else {
        fclose( stream );
        fstrset( batchf, len_batchf, bval );
        p = strrchr( bval, '.' );
        *p = '\0';
        bval = mstrcat( bval, ".log" );
#ifdef IBMVM
        bval = mstrcat( bval, ".a" );
#endif
        fstrset( logfil, len_logfil, bval );
      }
    }
  }

  if( *ierror == 0 && lval != NULL ) {
    if( lval[0] == '\0' ) {
      kprintf( " %s: Logon command file expected\n", prog );
      *ierror = 1;
    }
    else {
      FILE *stream;
      p = ku_home( lval, "kumac" );
      free( lval );
      lval = p;
      if( (stream = fopen( lval, "r" )) == NULL ) {
        kprintf( " %s: Logon command file %s not found\n", prog, lval );
        *ierror = 1;
      }
      else {
        fclose( stream );
        fstrset( logonf, len_logonf, lval );
      }
    }
  }

  if( nval != 0 ) {
    /* no logon file wanted */
    fstrset( logonf, len_logonf, "" );
  }

  free( prog );
  free( args );
  if( bval != NULL )
    free( bval );
  if( lval != NULL )
    free( lval );
}


/*
 * expand "~" and "$var"
 * add default file extension
 * the pointer returned is allocated
 */
char *ku_home( const char *fname,
              const char *ftype ) /* default file extension */
{
  char *path = fexpand( fname, ftype );

#ifdef HPUX
  char *p;
/*
 * There is a serious performance penalty when accessing an NFS file
 * with a path which involves a symbolic link on the server side.
 * Therefore we resolve all links and remove them from the path.
 */
  p = fsymlink( path );
  free( path );
  path = p;
#endif

  return path;
}


/*
 * expand home directory
 */
void F77_ENTRY_CI(Kuhome,chpath,lpath)
/* { */
  char *path = fstrdup( chpath, len_chpath );
  char *new_path = ku_home( path, NULL );
  free( path );
  *lpath = fstrset( chpath, len_chpath, new_path );
  free( new_path );
}


