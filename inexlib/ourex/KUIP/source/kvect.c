/*
 * $Id: kvect.c,v 1.5 2009/06/30 08:49:14 barrand Exp $
 *
 * $Log: kvect.c,v $
 * Revision 1.5  2009/06/30 08:49:14  barrand
 * *** empty log message ***
 *
 * Revision 1.4  2004/12/23 14:53:48  barrand
 * *** empty log message ***
 *
 * Revision 1.3  2004/09/04 09:40:29  barrand
 * *** empty log message ***
 *
 * Revision 1.2  1999/11/08 11:27:49  barrand
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/05/25  13:09:15  barrand
 * KUIP
 *
 * Revision 1.1.1.1  1996/03/08 15:32:59  mclareni
 * Kuip
 *
 */
/*CMZ :  2.06/03 12/01/95  18.15.01  by  Gunter Folger*/
/*-- Author :    Alfred Nathaniel   24/08/92*/
/* kvect.c: vector handling */

#include "kuip/kuip.h"
#include "kuip/kfor.h"
#include "kuip/kcom.h"
#include "kuip/kmenu.h"
#include "kuip/klink.h"
#include "kuip/kflag.h"
#include "kuip/khash.h"

/*GB static int   repeat_value( const char*, const char*, const char*,
                          INTEGER*, REAL*); */

/*
 * action routine for /VECTOR/CREATE
 */
/*GB int kxvcreate()
{
  char *vspec = ku_getc();
  char *vname = strdup( vspec );
  int  lvname;
  char *vtype = ku_getc();
  int  lvtype = strlen( vtype );
  char *value;
  int   length[4];
  INTEGER LENGTH[3];
  INTEGER LLOW;
  INTEGER LHIGH;

  length[0] = 0;
  length[1] = 0;
  length[2] = 0;
  if( strchr( vspec, '(' ) == NULL ) {
    length[0] = 1;
  }
  else {
    int ntok = sscanf( vspec, "%[^(](%d,%d,%d,%d)",
                      vname, &length[0], &length[1], &length[2], &length[3]);
    if( ntok < 2  || ntok > 4) {
      ku_alfa();
      kprintf( " *** VECTOR/CREATE %s: invalid dimensions\n", vspec );
      free( vname );
      return -1;
    }
  }

  lvname = strlen( vname );
  F77_CALL_Cxx(Kuvect,vname,lvname,&LLOW,&LHIGH);
  if( LLOW != 0 ) {
    int vlen = LHIGH - LLOW + 1;
    F77_CALL_C(Kuvdel,vname,lvname);
    ku_alfa();
    kprintf( " *** VECTOR/CREATE %s: existing vector %s(%d) replaced\n",
           vspec, vname, vlen );
  }

  LENGTH[0] = length[0];
  LENGTH[1] = length[1];
  LENGTH[2] = length[2];
  F77_CALL_CCx3(Kuvcre,vname,lvname,vtype,lvtype,LENGTH,&LLOW,&LHIGH);
  free( vname );
  if( LLOW == 0 ) {
    ku_alfa();
   kprintf( " *** VECTOR/CREATE %s: cannot create\n", vspec );
    return -1;
  }

  ku_getc();
  while( (value = ku_getl()) != NULL ) {
    INTEGER ival;
    REAL rval;
    int repeat;
    int i;

    if( vtype[0] == 'I' )
      repeat = repeat_value( "VECTOR/CREATE", vspec, value, &ival, NULL );
    else
      repeat = repeat_value( "VECTOR/CREATE", vspec, value, NULL, &rval );

    if( repeat < 1 )
      return -1;

    if( LLOW + repeat - 1 > LHIGH ) {
      repeat = LHIGH - LLOW + 1;
      ku_alfa();
     kprintf( " *** VECTOR/CREATE %s: too many initializers\n", vspec );
    }

    for( i = 0; i < repeat; i++ ) {
      if( vtype[0] == 'I' )
        IQ(LLOW) = ival;
      else
        Q(LLOW) = rval;
      LLOW++;
    }
  }
  return 0;
}
GB*/

/*
 * action routine for /VECTOR/INPUT
 */
/*GB int kxvinput()
{
  char *vspec = ku_getc();
  int  lvspec = strlen( vspec );
  char *vname;
  char *p;
  INTEGER LLOW;
  INTEGER LHIGH;
  int nval;
  int vtype;
  int ilo[3];
  int ihi[3];
  int idx[3];
  int ndim = 1;
  int status = 0;
  int i;

  Kcvect.TVECFL = ku_true( 0 );
  F77_CALL_Cxx(Kuvect,vspec,lvspec,&LLOW,&LHIGH);
  Kcvect.TVECFL = ku_true( 1 );

  if( LLOW == 0 ) {
    ku_alfa();
   kprintf( " *** VECTOR/INPUT: unknown vector %s\n", vspec );
    return -1;
  }

  if( IQUEST(20) != 0 ) {
    ku_alfa();
   kprintf( " *** VECTOR/INPUT: sub-vector %s not implemented\n",
           vspec );
    return -1;
  }

  vtype = IQUEST(14);
  for( i = 0; i < 3; i++ ) {
    idx[i] = ilo[i] = IQUEST(21+i);
    if( (ihi[i] = IQUEST(31+i)) > 1 )
      ndim = i + 1;
  }

  nval = LHIGH - LLOW + 1;

  vname = strdup( vspec );
  if( (p = strchr( vname, '(' )) != NULL )
    *p = '\0';

  ku_getc();
  while( nval > 0 ) {
    const char *value;
    INTEGER ival;
    REAL rval;
    int repeat;
    char *prompt = str2dup( vname, "(" );
    prompt = mstricat( prompt, idx[0] );
    for( i = 1; i < ndim; i++ ) {
      prompt = mstrcat( prompt, "," );
      prompt = mstricat( prompt, idx[i] );
    }
    prompt = mstrcat( prompt, ") " );

    if( (value = ku_getl()) == NULL ) {
      value = ku_proc( prompt, NULL );
      if( value == NULL ) {
        free( prompt );
        status = -1;
        break;
      }
    }

    if( *value == '\0' ) {
      free( prompt );
      repeat = 1;
    }
    else {
      if( vtype != 1 )
        repeat = repeat_value( "VECTOR/INPUT", prompt, value, &ival, NULL );
      else
        repeat = repeat_value( "VECTOR/INPUT", prompt, value, NULL, &rval );

      free( prompt );
      if( repeat < 1 ) {
        status = -1;
        break;
      }

      if( repeat > nval ) {
        repeat = nval;
        ku_alfa();
       kprintf( " *** VECTOR/INPUT: too many initializers for %s\n", vspec );
      }

      for( i = 0; i < repeat; i++ ) {
        if( vtype != 1 )
          IQ(LLOW) = ival;
        else
          Q(LLOW) = rval;
        LLOW++;
      }
    }

    nval -= repeat;
    for( i = 0; i < repeat; i++ ) {
      int k;
      for( k = 0; k < ndim; k++ ) {
        if( ++idx[k] > ihi[k] )
          idx[k] = ilo[k];
        else
          break;
      }
    }
  }

  free( vname );
  return status;
}
GB*/

/*
 * decode a input value [rep*]val where val can be numeric or a vector element
 */
/*GB static int repeat_value( const char *cmd,
                 const char *vname,
                 const char *rep_value,
                 INTEGER *ival,
                 REAL *rval )
{
  int repeat;
  const char *value;
  char *star;
  char *tail;

  while( (star = strchr( rep_value, '*')) != NULL
        && (repeat = atoi( rep_value )) < 1 ) {

    ku_alfa();
   kprintf( " *** %s: invalid repetition factor %s for %s\n",
           cmd, rep_value, vname );
    rep_value = ku_pros( "Reenter initializer or Q to quit:", NULL );
    if( rep_value == NULL || *rep_value == 'q' || *rep_value == 'Q' )
      return -1;              
  }

  if( star != NULL )
    value = star + 1;
  else {
    value = rep_value;
    repeat = 1;
  }

  if( ival != NULL ) {
    *ival = fstrtoi( value, &tail );
    if( *tail != '\0' )
      *ival = fstrtod( value, &tail );
  }

  if( rval != NULL ) {
    *rval = fstrtod( value, &tail );
  }

  if( *tail != '\0' ) {
    INTEGER LLOW;
    INTEGER LHIGH;
    F77_CALL_Cxx(Kuvect,value,strlen(value),&LLOW,&LHIGH);

    if( LLOW == 0 ) {
      ku_alfa();
     kprintf( " *** %s: invalid initializer %s for %s\n", cmd, value, vname );
      value = ku_pros( "Reenter initializer or Q to quit:", NULL );
      if( value == NULL || *value == 'q' || *value == 'Q' )
        return -1;             
      return repeat_value( cmd, vname, value, ival, rval );
    }

    if( LLOW != LHIGH ) {
      ku_alfa();
     kprintf( " *** %s: only first element of %s used as initializer for %s\n",
             cmd, value, vname );
    }
    if( IQUEST(14) != 1 ) {
      if( ival != NULL )
        *ival = IQ(LLOW);
      if( rval != NULL )
        *rval = IQ(LLOW);
    }
    else {
      if( ival != NULL )
        *ival = Q(LLOW);
      if( rval != NULL )
        *rval = Q(LLOW);
    }
  }

  return repeat;
}
GB*/

/*
 * action routine for /VECTOR/PRINT
 */
/*GB int kxvprint()
{
  char *vspec = ku_getc();
  int  lvspec = strlen( vspec );
  int  dense = ku_geti();
  char *vname;
  char *p;
  INTEGER LLOW;
  INTEGER LHIGH;
  int nval;
  int vtype;
  int ilo[3];
  int ihi[3];
  int idx[3];
  char dfm[3][8];
  int ndim = 1;
  int n;
  int i;

  ku_alfa();
  F77_CALL_Cxx(Kuvect,vspec,lvspec,&LLOW,&LHIGH);

  if( LLOW == 0 ) {
   kprintf( " *** VECTOR/PRINT: unknown vector %s\n", vspec );
    return -1;
  }

  vtype = IQUEST(14);
  for( i = 0; i < 3; i++ ) {
    idx[i] = ilo[i] = IQUEST(21+i);
    if( (ihi[i] = IQUEST(31+i)) > 1 )
      ndim = i + 1;
  }
  for( i = 0; i < ndim; i++ ) {
    int m = ihi[i];
    int k = 1;
    while( m >= 10 ) {
      m /= 10;
      k++;
    }
    sprintf( dfm[i], "%s%%%dd", (i == 0) ? "" : ",", k );
  }

  nval = LHIGH - LLOW + 1;

  vname = strupper( strdup( vspec ) );
  if( (p = strchr( vname, '(' )) != NULL )
    *p = '\0';

  for( n = 0; n < nval; n++ ) {
    int same = 0;

    if( dense > 0 ) {
      if( vtype != 1 ) {
        for( i = n + 1; i < nval && IQ(LLOW+i) == IQ(LLOW+n); i++ )
          same++;
      }
      else {
        for( i = n + 1; i < nval && Q(LLOW+i) == Q(LLOW+n); i++ )
          same++;
      }
      if( dense == 1 && same == 1 )
        same = 0;
    }

   kprintf( " %s(", vname );
    for( i = 0; i < ndim; i++ )
     kprintf( dfm[i], idx[i] );
   kprintf( ")" );

    if( same > 0 ) {
      int k;
      for( k = 0; k < same; k++ ) {
        for( i = 0; i < ndim; i++ ) {
          if( ++idx[i] > ihi[i] )
            idx[i] = ilo[i];
          else
            break;
        }
      }
      if( dense == 1 )
       kprintf( " ...\n %s", vname );
      else
       kprintf( ".." );
     kprintf( "(" );
      for( i = 0; i < ndim; i++ )
       kprintf( dfm[i], idx[i] );
     kprintf( ")" );
      n += same;
    }
   kprintf( " = " );

    if( vtype != 1 )
     kprintf( "%d", IQ(LLOW+n) );
    else {
      double dval = Q(LLOW+n);
     kprintf( "%s", strfromd( dval, 0 ) );
    }
   kprintf( "\n" );

    for( i = 0; i < ndim; i++ ) {
      if( ++idx[i] > ihi[i] )
        idx[i] = ilo[i];
      else
        break;
    }
  }

  free( vname );
  return 0;
}
GB*/

/*
 * set user routine to be called when a vector is deleted
 * (COMIS needs to clear its tables in that case)
 */
void F77_ENTRY_E(Kucomv,fun)
/* { */
  kjmpaddr.user_comis_F = fun;
}

/*
 * Because for IBM/370 we cannot use arguments in an indirect function call
 * we have to store the vector name in a buffer and call the user routine
 * which then has to call KUVNAM to retrieve the vector name.
 */

static char deleted_vname[32];
static int  deleted_len_vname;

void F77_ENTRY_C(Kicomv,vname)
/* { */
  if( kjmpaddr.user_comis_F != NULL ) {
    if( len_vname > (sizeof deleted_vname) )
      len_vname = (sizeof deleted_vname);
    strncpy( deleted_vname, vname, len_vname );
    deleted_len_vname = len_vname;
    (*kjmpaddr.user_comis_F)();
  }
}

void F77_ENTRY_C(Kuvnam,vname)
/* { */
  memset( vname, ' ', len_vname );
  if( len_vname > deleted_len_vname )
    len_vname = deleted_len_vname;
  strncpy( vname, deleted_vname, len_vname );
}


/*
 * return the number of leading characters which are valid for a vector name
 */
int len_vector( const char *line )
{
  int len = 0;

  if( !isdigit( line[0] ) ) {

    while( line[len] != '\0'
          && ( isalnum( line[len] )
              || strchr( "_?", line[len] ) != NULL ) ) {
      len++;
    }
  }
  return len;
}


/*
 * return vector address as index into Q():
 */
int ku_vqaddr( const char *vname )
{
  printf("debug : ku_vqaddr : \"%s\"\n",vname);
  /*GB
  INTEGER LLOW;
  INTEGER LHIGH;
  F77_CALL_Cxx(Kuvect,vname,strlen(vname),&LLOW,&LHIGH);
  return LLOW;
  */
  return 0; /*GB*/
}


/*
 * return vector type:
 *  0 : vector name not defined
 *  1 : real vector
 *  2 : integer vector
 */
int ku_vtype( const char *vname )
{
  /*GB
  INTEGER LLOW;
  INTEGER LHIGH;

  F77_CALL_Cxx(Kuvect,vname,strlen(vname),&LLOW,&LHIGH);
  if( LLOW != 0 ) {
    return IQUEST(14);
  }
  GB*/
  if( kjmpaddr.vector_find_C != NULL ) {
    return (*kjmpaddr.vector_find_C)(vname,kjmpaddr.vector_find_C_data);
  } else {
    kprintf("ku_vtype : dummy\n");
    return 0;
  }
}


/*
 * test if vspec specifies a single vector element and return its value
 */
int ku_vvalue( const char *vspec,
              double *value )
{
  /*GB
  INTEGER LLOW;
  INTEGER LHIGH;

  F77_CALL_Cxx(Kuvect,vspec,strlen(vspec),&LLOW,&LHIGH);
  if( LLOW != 0 && LLOW == LHIGH ) {
    if( value != NULL ) {
      if( IQUEST(14) == 1 ) {
        *value = Q(LLOW);
      }
      else if( IQUEST(14) == 2 ) {
        *value = IQ(LLOW);
      }
    }
    return 0;
  }
  return 1;
  GB*/
  if( kjmpaddr.vector_value_C != NULL ) {
    return (*kjmpaddr.vector_value_C)(vspec,value,kjmpaddr.vector_value_C_data);
  } else {
    kprintf("ku_vvalue : dummy\n");
    *value = 0.;
    return 1;
  }
}


/*
 * inquire if file is Fortran opened
 */
/*GB
int ku_inqf( const char *fname )
{
  INTEGER luno;
  F77_CALL_Cx(Kuinqf,fname,strlen(fname),&luno);
  return luno;
}
GB*/

/*
 * Fortran open a file
 */
int ku_open( int lun,
            const char *fname,
            const char *mode )
{
  /*GB
  INTEGER luno = lun;
  INTEGER istat;
  F77_CALL_xCCx(Kuopen,&luno,fname,strlen(fname),mode,strlen(mode),&istat);
  return istat;
  GB*/
  return 1;/*GB*/
}


/*
 * Fortran close a file
 */
int ku_close( int lun )
{
  /*GB
  INTEGER luno = lun;
  INTEGER istat;
  F77_CALL_xCx(Kuclos,&luno," ",1,&istat);
  return istat;
  GB*/
  return 1;/*GB*/
}


/*
 * Read a line from Fortran unit
 */
/*GB
int ku_read( int lun,
            char *line,
            size_t len )
{
  INTEGER luno = lun;
  INTEGER nch;
  F77_CALL_xCx(Kuread,&luno,line,len,&nch);
  if( nch >= 0 )
    line[nch] = '\0';
  return nch;
}
GB*/


/*
 * Write a line to Fortran unit
 */
/*GB
void ku_write( int lun,
              const char *line )
{
  INTEGER luno = lun;
  const char *p = line;
  const char *s;
  while( (s = strchr( p, '\n' )) != NULL ) {
    F77_CALL_xC(Kuwrit,&luno,p,s-p);
    p = s + 1;
  }
  F77_CALL_xC(Kuwrit,&luno,p,strlen(p));
}
GB*/

