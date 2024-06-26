/*
  A4 Size :210 x 297  (8.27 x 11.69 inches)
  So it is this dimension in mm is used as the 
  measurement unit.

   With Shift in print_column: dated 13th Sept. 1995
   with column and row shading and underline
   Ver dated 24-6-96
   Hyphanation included
   'CG(int)' to set gap between columns included
   New GUI Version

 Note: On 21st Jan 2002
   slide is modified to print directly on a printer or to
   make a PostScript File
   ie: slide -oPsFile <slide_input>
   In this case there will not be any screen output
 Note:dated 15th Jan 2018
   Using Kulina Graphics library
   No screen display
   Uses kgInitGph ang kgInitImg
   Can create Postscript output and Png file


*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <kulina.h>
#define BLANK 31
#define TX_CLR 15
#define TX_FONT 0
  static char *TmpDir = NULL;
  static char DUMM_FIL [ 100 ] ;
  static char Z_DU_ZZ [ 100 ] ;
  static char TEMP_FILE [ 100 ] ;
  static char Contents [ 100 ] ;
  static char OutFile [ 100 ] ;
  static char GphFile [ 100 ] ;
  static void *Img = NULL;
  static void *Png = NULL;
  static char Bimg [ 300 ] ;
  static void *Pimg = NULL;
  static void *Pbkimg = NULL;
  static void *Sbkimg = NULL;
  static int PngOut = 0 , PsOut = 0 , Bkgr = 0;
  static char leftstr [ 50 ] = { " " } , rightstr [ 50 ] = { " " };
  static char bleftstr [ 50 ] = { " " } , brightstr [ 50 ] = { " " };
  static char Leftstr [ 50 ] = { " " } , Rightstr [ 50 ] = { " " };
  static char Bleftstr [ 50 ] = { " " } , Brightstr [ 50 ] = { " " };
  static char PngFile [ 100 ] , PsFile [ 100 ] ;
  static int Pon = 0 , BW = 1;
  short numpos = 2 , numtype , numline = 0 , numtop = 1 , numpos_d;
  static int Bk_gr = 0 , Hd_on = 0 , Hd_line = 0 , Hd_max = 0 , Hd_level = 0 , Mk_con = 0 , Con_file = 0;
      
  static char B_file [ 40 ] = { " " };
  static int Scroll = 0 , Sxres = 1376 , Syres = 768 , Frames = 300;
  static int Pxres = 1376 , Pyres = 768;
  static int Entry = 0 , Hypn = 0 , Rjust = 1;
  static int B_entry = 0;
  static double B_xm , B_ym , B_dx , B_dy , E_a , E_b;
  static float Yend;
  static float tw , th , tg;
  static int Columns = 1 , Top_skip = 22 , Para_of = 10 , Lines = 0 , NOTE = 0 , REF = 0 , Col_gap = 10;
      
  static int F_N_no = 0 , R_N_no = 0;
  static float Brk_pt;
  static int L_mar = 35 , R_margin = 195;
  static float G_x = 0. , G_y = 0. , G_sx = 1. , G_sy = 1. , G_shx = 0. , G_shy = 0.;
      
  static int txth , txtw;
  static int txtg , ifac = 24;
  static int SpFac = 24,Font=0,TxtClr=TX_CLR,TxtW=16;;
  long tpattern = 15 , tfill = 1 , tbodr = 1 , tbkgr , tfnt = 0 , tcolor = TX_CLR , tbold = 1 , tangle = 0 , tslant = 0 , tuline = 0;
      
  static long ipattern , ifill , ibodr , ibkgr , ifnt , icolor , ibold , iangle , islant , iuline;
      
  static long pgnum = 1 , pgofs = 0 , ipge;
  static float hfac = 1. , wfac = 1. , gfac = 1. , fac = 1.;
  static short right_margin = 195.0;
  static short bline = 0 , tline = 0;
  static float blinepos , bheadpos;
  static float tlinepos , theadpos;
  static float pagepos;
  static float wxmin , wymin , wxmax , wymax;
  static int Xres , Yres;
  static char rstr [ 17 ] [ 5 ] = { "   i" , "  ii" , " iii" , "  iv" , "   v" , "  vi" , " vii" , "viii" , "  ix" , "   x" , "  xi" , " xii" , "xiii" , " xiv" , "  xv" , " xvi" , "xvii" };
      
  static short nonblk ( char *ln ) ;
  static void scan ( void ) ;
  static void read_txt ( char *flname , float ofs , float yy ) ;
  static int process ( char *flname ) ;
  static void blowup ( void ) ;
  static void settextsetting ( char *txt ) ;
  static void scr_strlngth ( char *line , float *xdsp ) ;
  static char *buf = NULL , line [ 200 ] ;
  static char *Buf = NULL;
  static unsigned char tmenu [ 23 ] = { " Give text file name :" };
  int stripblnk ( char * ) ;
#define MAX1 55
#define MAX2 60
#define FOREVER for(;;)
#define Chk_Alloc(M) {\
  if ( M == NULL ) {\
          printf ( "Error: In MALLOC...\n" ) ;\
          exit ( 0 ) ;\
      }\
  }
#define DRAWAR(x1,y1,x2,y2,v) { \
  float xx , yy;\
      xx = x2;yy = y2;\
      if ( fabs ( ( x1 ) - ( x2 ) ) < 0.6 ) xx = x1; \
      if ( fabs ( ( y1 ) - ( y2 ) ) < 0.6 ) yy = y1; \
      drarrow ( x1 , y1 , xx , yy , v ) ;\
  }
#define skip_blank while((buf[i]==' ')&&(i<max)) i++;
#define copy_word {\
  if ( i < max ) line [ lng++ ] = buf [ i++ ] ;\
      while ( ( i < max ) && ( buf [ i ] > ' ' ) && ( buf [ i ] != '#' ) && \
       ( buf [ i ] != '\\' ) && ( buf [ i-1 ] != '-' ) && \
       ( buf [ i-1 ] != ':' ) ) line [ lng++ ] = buf [ i++ ] ;\
      while ( ( i < max ) && ( ( buf [ i ] == ',' ) || ( buf [ i ] == '.' ) || ( buf [ i ] == ':' ) ) ) \
      line [ lng++ ] = buf [ i++ ] ;\
  }
#define copy_non_word while((buf[i]!=' ')&&(i<max)&& \
   ( ( buf [ i ] < 'A' ) || ( ( buf [ i ] > 'Z' ) && ( buf [ i ] < 'a' ) ) \
   || ( buf [ i ] > 'z' ) ) ) line [ lng++ ] = buf [ i++ ] ;
#define copy_block {\
  while ( ( i < max ) && ! ( ( buf [ i ] == '\\' ) && ( buf [ i+1 ] == 'E' ) ) ) { \
          if ( lng < 150 ) line [ lng++ ] = buf [ i++ ] ;\
      else i++;};i+= 2 ;\
      while ( ( i < max ) && ( ( buf [ i ] == ',' ) || ( buf [ i ] == '.' ) || ( buf [ i ] == ';' ) ) ) \
      line [ lng++ ] = buf [ i++ ] ; \
  }
#define check_limit if(lng==450) {\
  printf ( " Very long string...\n" ) ;\
      line [ 99 ] = '\0';\
      printf ( "%-s\n" , line ) ;\
      exit ( 0 ) ;\
  }
#define copy_punch
#define copy_blanks while((buf[i]==' ')&&(i<max)) line[lng++]=buf[i++];
#define Check_dist if(xdsp<0.) {\
  printf ( " Error : line width < 0" ) ;\
  exit ( 0 ) ;}
#define check_dolor if(line[0]=='$') fprintf(ot,"!g !b");
#define no_advance  fprintf(ot,"$A0\n");
#define advance     fprintf(ot,"$A1\n");
#define Fprintf(fp,Bf) { int i,j;char *bf;\
  i = 0;j = 0;bf = Bf;\
      while ( bf [ i ] != '\0' ) i++;\
      i--;while ( ( bf [ i ] == ' ' ) || ( bf [ i ] == BLANK ) ) i--;bf [ ++i ] = '\0';\
      i = 0;\
      if ( ( bf [ 0 ] == '$' ) ) { Buf [ j++ ] = ' ';}\
      while ( bf [ i ] != '\0' ) {\
          if ( ( bf [ i ] == BLANK ) || ( bf [ i ] == '_' ) ) \
          { Buf [ j++ ] = '!';Buf [ j++ ] = 'g';Buf [ j++ ] = ' ';}\
          else Buf [ j++ ] = bf [ i ] ;\
          i++;\
      }\
      Buf [ j ] = '\0';\
      fprintf ( fp , "%-s \n" , Buf ) ;\
  }
#define process_backslash {\
  switch ( buf [ i+1 ] ) \
      {\
          case 'W' :\
          i+= 2;copy_block; check_limit;\
          break;\
          case 'N' :\
          i+= 2; i += ( process_f_note ( & buf [ i ] , max-i , rmg , ( char * ) "Ft" , & NOTE ) +1 ) ;\
          fprintf ( ot , "$RK%-d \n" , Lines ) ;\
          fprintf ( ot , "$RF%-3.3d %-d \n" , NOTE , Lines ) ;\
          break;\
          case 'R' :\
          i+= 2; i += ( process_r_note ( & buf [ i ] , max-i , rmg , ( char * ) "En" , & REF ) +1 ) ;\
          break;\
          case '#' :\
          case '\\' :\
          default :\
          i++; copy_non_word; copy_word; copy_punch;break;\
      }\
  }
#define process_hash {\
  switch ( buf [ i+1 ] ) \
      {\
          case 'N' :\
          F_N_no++;\
          i+= 2;sprintf ( & line [ lng ] , "!S%-2d!e " , F_N_no ) ;lng+= 6; break;\
          case 'R' :\
          i--;\
          default :\
          i++; copy_non_word; copy_word; copy_punch;break;\
      }\
  }
#define check_slant_nor_bold {\
  int i = 0;\
      while ( ln [ i ] >= ' ' ) {\
          if ( ln [ i ] == '!' ) {\
              switch ( ln [ i+1 ] ) {\
                  case 'f': ifnt = ( ln [ i+2 ] -'0' ) *10 + ( ln [ i+3 ] -'0' ) ;\
                  fprintf ( f23 , "$f%-d\n" , ifnt ) ;\
                  i+= 2;\
                  break;\
                  case 'c': icolor = ( ln [ i+2 ] -'0' ) *10 + ( ln [ i+3 ] -'0' ) ;\
                  fprintf ( f23 , "$tc%-d\n" , icolor ) ;\
                  i+= 2;\
                  break;\
                  case 'I': Slant_on = 1; break;\
                  case 'B': Bold_on = 1; break;\
                  case 'U': Uline_on = 1; break;\
                  case 'O': Oline_on = 1; break;\
                  case 'N': Slant_on = 0; Bold_on = 0;Uline_on = 0;Oline_on = 0;\
                  default : break;\
              }\
              i++;\
          }\
          i++;\
      }\
  }
#define pro_graphic_commands {\
  int fil = 14 , h;\
      switch ( buf [ 2 ] ) { \
          case 'L': \
          fprintf ( tmp , "$GC\n$GW2\n$GL%-d 0 %-d 0\n" , L_mar , R_margin ) ; \
          break; \
          case 'I': { \
              int w , h , sh;\
              char flname [ 200 ] ; \
              sscanf ( buf+3 , "%s%d%d" , flname , & w , & h ) ;\
           /* sh = (R_margin -L_mar -w)/2+L_mar;*/\
              sh = ( rmg -ofs -w ) /2+ofs;\
              fprintf ( tmp , "$GC\n$GI%-s %-d %-d %-d\n" , flname , sh , w , h ) ; \
          } \
          break; \
          break; \
          case 'R': \
          fil = scanint ( ( char * ) & buf [ 3 ] ) ; \
          h = th*hfac*1.25+0.5; \
          fprintf ( tmp , "$GC\n$GR%-d 0 %-d -%-d %-d\n" , L_mar , R_margin , h , fil ) ; \
          break; \
          default: \
          break; \
      } \
  }
#define pro_other_commands {\
  switch ( buf [ 1 ] ) \
      { \
          case 'f':\
          tfnt = scanint ( ( char * ) & buf [ 2 ] ) ; \
	  Font = tfnt; \
          kgTextFont ( Img , ( long ) tfnt ) ;\
          break;\
	  case 't':\
          if(buf[2]=='c') TxtClr =scanint ( ( char * ) & buf [ 3 ] ) ; \
	  break;\
          case 'o': \
          ofs = scanint ( ( char * ) & buf [ 2 ] ) ; \
          if ( L_mar > ofs ) L_mar = ofs; \
          break; \
          case 'm': \
          rmg = scanint ( ( char * ) & buf [ 2 ] ) ; \
          if ( R_margin < rmg ) R_margin = rmg; \
          rmgc = rmg;\
          break; \
          case 'C': \
          if ( buf [ 2 ] == 'G' ) {\
              Col_gap = scanint ( ( char * ) & buf [ 3 ] ) ; \
          }\
          else columns = scanint ( ( char * ) & buf [ 2 ] ) ; \
          if ( columns < 1 ) {columns = 1;rmg = rmgc;} \
          if ( columns > 1 ) { \
              lng = ( rmgc -ofs- ( columns-1 ) *Col_gap ) /columns; \
              rmg = ofs+lng; \
              fprintf ( tmp , "$m%-d\n$X\n" , ( int ) ( rmg ) ) ; \
          } \
          fprintf ( tmp , "$m%-d\n" , ( int ) ( rmg ) ) ; \
          break; \
          case 'p': \
          ipr = buf [ 2 ] -'0'; \
          break; \
          case 's': \
          isfac = scanint ( ( char * ) & buf [ 2 ] ) ; \
	  SpFac = isfac; \
          break; \
          case 'h': \
          ihfac = scanint ( ( char * ) & buf [ 2 ] ) ; \
          hfac = ( ( float ) ihfac ) /12.0; \
          kgTextSize ( Img , th*hfac , tw*wfac , tg*gfac ) ; \
          break; \
          case 'w': \
          iwfac = scanint ( ( char * ) & buf [ 2 ] ) ; \
	  TxtW = iwfac;\
          wfac = ( ( float ) iwfac ) /12.0; \
          kgTextSize ( Img , th*hfac , tw*wfac , tg*gfac ) ; \
          width = ( tw*wfac+tg*gfac ) ; \
          break; \
          case 'g': \
          ifac = scanint ( ( char * ) & buf [ 2 ] ) ; \
          gfac = ( ( float ) ifac ) /1.0; \
          kgTextSize ( Img , th*hfac , tw*wfac , tg*gfac ) ; \
          width = ( tw*wfac+tg*gfac ) ; \
          break; \
          default : break; \
      } \
      if ( buf [ 1 ] != 'o' ) fprintf ( tmp , "%s" , buf ) ;\
      else fprintf ( tmp , "$o%-d\n" , ( int ) ( ofs*10+0.5 ) ) ; \
  }
#define pro_para_list_table {  \
  switch ( buf [ 2 ] ) \
      { \
          case 'P': \
          mkpara ( fp , tmp , ofs , rmg , Para_of , width ) ; \
          break; \
          case 'L': \
          mklist ( fp , tmp , ofs , rmg , width ) ; \
          break; \
          case 'B': \
          mkpara ( fp , tmp , ofs , rmg , 0 , width ) ; \
          break; \
          case 'A':\
          mkdotalign ( fp , tmp , ofs , rmg ) ;\
          break;\
          case 'T': \
          switch ( buf [ 3 ] ) { \
              case '0':Tbl_type = 0;break; \
              case '1':Tbl_type = 1;break; \
              case '2':Tbl_type = 2;break; \
              case '3':Tbl_type = 3;break; \
              case '4':Tbl_type = 4;break; \
              case '5':Tbl_type = 5;break; \
              default :Tbl_type = 0;break; \
          } \
          mktbl ( fp , tmp , ofs , rmg , width ) ; \
          break; \
          case 'O': \
          Para_of = scanint ( ( char * ) & buf [ 3 ] ) ; \
          break; \
          case 'R': \
          R_N_no = 0; \
          fclose ( tmp ) ; \
          copy_refences ( DUMM_FIL ) ;\
          tmp = fopen ( DUMM_FIL , "a" ) ; \
          break; \
          case 'Y': \
          Hypn = scanint ( ( char * ) & buf [ 3 ] ) ; \
          break; \
          case 'J': \
          Rjust = scanint ( ( char * ) & buf [ 3 ] ) ; \
          break; \
          case 'N': \
          F_N_no = scanint ( ( char * ) & buf [ 3 ] ) -1; \
          break; \
          case 'H': \
          { \
              int level = 0;\
              level = scanint ( ( char * ) & buf [ 3 ] ) ; \
              if ( level > Hd_max ) Hd_max = level; \
          /*fprintf(tmp,"$RL4\n");*/\
              fprintf ( tmp , "%s" , buf ) ;\
          } \
          break; \
          case 'E': \
          fprintf ( tmp , "%s" , buf ) ;\
          break; \
          default: \
          return; \
      } \
  }
#define pro_heading {\
  int hdr = 0 , gap = 3;char code;\
      hdr = scanint ( ( char * ) & ln [ 3 ] ) ; \
      if ( hdr > 7 ) hdr = 7;\
      if ( hdr < 0 ) hdr = 0;\
      Hd_level = hdr; \
      if ( hdr < 3 ) gap = txtg+1;\
      else gap = txtg+2;\
      hdr++;hdr*= 2;\
      code = 'c';\
      if ( ln [ 4 ] >= ' ' ) {\
          switch ( ln [ 5 ] ) {\
              case 'c':code = 'c';break;\
              case 'l':code = 'l';break;\
              case 'r':code = 'r';break;\
          }\
      }\
      fprintf ( f23 , "$%-c\n" , code ) ; \
      fprintf ( f23 , "$h%-d\n$w%-d\n$g%-d\n$s%-d\n" , \
      txth+hdr , txtw+hdr , gap , ifac+hdr ) ;\
      fac = ( ( float ) ( ifac+hdr ) ) /24.0;\
  }
#define copy_heading { \
  int level , no;\
      float wd , dsp;\
      level = Hd_max-Hd_level;\
      Con_file = 1;\
      if ( Hd_line == 0 ) {\
          if ( level == 0 ) fprintf ( cfile , "\n" ) ; \
          fprintf ( cfile , "$h14\n$w14\n" ) ; \
          fprintf ( cfile , "$A0\n" ) ; \
          fprintf ( cfile , "$r\n%-d\n$l\n" , pgnum+pgofs ) ;\
      }\
      fprintf ( cfile , "$o%-d\n" , ( L_mar+5*level ) *10 ) ;\
      fprintf ( cfile , "$h%-d\n$w%-d\n" , 14-level , 14-level ) ;\
      Hd_line = 1; \
      kgTextSize ( Img , ( 14.0-level ) *2.81/12. , ( 14.0-level ) *2.81/12. , tg ) ;\
      wd = kgStringLength ( Img , " . . . . . . . . . . . . . . . . . . . ." ) ;\
      wd /= 20.;\
      dsp = kgStringLength ( Img , ln ) ;\
      no = ( 185.-dsp-L_mar-5*level ) / ( wd ) ;\
      fprintf ( cfile , "$A0\n%s\n$m185\n$r\n" , ln ) ; \
      for ( level = 0;level < no;level++ ) fprintf ( cfile , " ." ) ;\
      fprintf ( cfile , "\n$l\n$m195\n" ) ;\
  }
#define Set_background {  \
  int i = 0 , j = 0;\
      while ( * ( ln+3+i ) == ' ' ) i++;\
      if ( * ( ln+3+i ) == '\n' ) Bk_gr = 0;\
      else {\
          while ( * ( ln+3+i ) > ' ' ) B_file [ j++ ] = * ( ln+3+i++ ) ;\
          B_file [ j ] = '\0';\
          fprintf ( f23 , "$RS%-s\n" , B_file ) ;\
          Bk_gr = 1;\
      }\
  }
#define Refix_begin_char {\
  int Notok = 1;\
      if ( b_chr != 0 ) b_chr--;\
      while ( Notok ) {\
          pt = first_adr;\
          if ( b_chr == ( e_chr-1 ) ) break;\
          else b_chr++;\
          while ( ( pt != NULL ) && Notok ) {\
              int pos;\
              chr = & ( pt-> buf [ b_chr ] ) ;\
              if ( *chr != BLANK ) Notok = 0;\
              pt = pt-> adr;\
          }\
      }\
      pt = first_adr;\
  }
#define Set_Defaults \
  pglimit = 268.0/5.62;\
  pglimit_bk = 268.0/5.62;\
  Columns = 1 , Top_skip = 22 , Para_of = 10;\
  Col_shft = 0.;\
  Col_gap = 10;\
  ifac = 24;\
  txth = 12 , txtw = 12 , txtg = 1;\
  nf = 100;\
  Ad_vn = 1;\
  rmg = right_margin;\
  rmgl = right_margin;\
  fac = 1.0;\
  ifnt = TX_FONT;\
  islant = 0;\
  iuline = 0;\
  bold = 0;\
  ipr = 1;\
  ibold = 1;\
  iptn = 15;\
  ipattern = 15;\
  ifill = 1;\
  icolor = TX_CLR;\
  ibodr = 1;\
  ibkgr = 0;\
  set = 'l';\
  ipge = 1;\
  ofs = 35.;\
  yy = 172.;\
  yy = 174.;\
  ofsl = 35.;\
  leftmar = 35.;\
  yyl = 172.;\
  yyl = 174.;\
  iskip = 0;\
  tlinepos = 151.0 , theadpos = 152.5;\
  blinepos = -110.0 , bheadpos = -115.5;\
  pagepos = theadpos;\
  pgnum = 0;\
  pgofs = 0;Finish = 0;Od_Ev = 0;
#define Reset_Defaults \
  pglimit_bk = pglimit;\
  ifac = 24; \
  pagepos = theadpos;\
  pgnum = 0;\
  pgofs = 0;Finish = 0;Od_Ev = 0;
#define Set_Strings \
  switch ( Od_Ev ) \
  {\
      case 1:\
      if ( ( cpgno%2 ) == 0 ) { \
          Rightstr [ 0 ] = '\0'; \
          Brightstr [ 0 ] = '\0'; \
          copy_string ( leftstr , Leftstr ) ; \
          copy_string ( bleftstr , Bleftstr ) ; \
      } \
      else { \
          Leftstr [ 0 ] = '\0'; \
          Bleftstr [ 0 ] = '\0'; \
          copy_string ( rightstr , Rightstr ) ; \
          copy_string ( brightstr , Brightstr ) ; \
      } \
      break; \
      case 2:\
      if ( ( cpgno%2 ) != 0 ) { \
          Rightstr [ 0 ] = '\0'; \
          Brightstr [ 0 ] = '\0'; \
          copy_string ( leftstr , Leftstr ) ; \
          copy_string ( bleftstr , Bleftstr ) ; \
      } \
      else { \
          Leftstr [ 0 ] = '\0'; \
          Bleftstr [ 0 ] = '\0'; \
          copy_string ( rightstr , Rightstr ) ; \
          copy_string ( brightstr , Brightstr ) ; \
      } \
      break; \
      case 0: \
      copy_string ( leftstr , Leftstr ) ; \
      copy_string ( bleftstr , Bleftstr ) ; \
      copy_string ( rightstr , Rightstr ) ; \
      copy_string ( brightstr , Brightstr ) ; \
      break; \
      default: break;\
  }
#define Set_No_Pos \
  {\
      numpos_d = numpos;\
      if ( ( cpgno%2 ) == 0 ) { \
          if ( numpos == 14 ) numpos_d = 11;\
          if ( numpos == 4 ) numpos_d = 1;\
          if ( numpos == 15 ) numpos_d = 13;\
          if ( numpos == 5 ) numpos_d = 3;\
      }\
      else { \
          if ( numpos == 14 ) numpos_d = 13;\
          if ( numpos == 4 ) numpos_d = 3;\
          if ( numpos == 15 ) numpos_d = 11;\
          if ( numpos == 5 ) numpos_d = 1;\
      } \
  }
#define Default_state(f23) {\
  fprintf ( f23 , "$o%-d\n" , ( int ) ( ofsl*10+0.5 ) ) ;\
      fprintf ( f23 , "$m%-d\n" , rmgl ) ;\
      fprintf ( f23 , "$s%-d\n" , ifac ) ;\
      fprintf ( f23 , "$f%1d\n" , ifnt ) ;\
      fprintf ( f23 , "$i%1d\n" , islant ) ;\
      fprintf ( f23 , "$U%1d\n" , iuline ) ;\
      fprintf ( f23 , "$p%1d\n" , ipr ) ;\
      fprintf ( f23 , "$tp%2.2d\n" , ipattern ) ;\
      fprintf ( f23 , "$td%1d\n" , ibodr ) ;\
      fprintf ( f23 , "$tb%1d\n" , ibold ) ;\
      fprintf ( f23 , "$tk%1d\n" , ibkgr ) ;\
      fprintf ( f23 , "$tf%1d\n" , ifill ) ;\
      fprintf ( f23 , "$tc%1d\n" , icolor ) ;\
      fprintf ( f23 , "$%c\n" , set ) ;\
      if ( bold ) fprintf ( f23 , "$b\n" ) ;\
      else fprintf ( f23 , "$n\n" ) ;\
      fprintf ( f23 , "$h%-d\n$w%-d\n$g%-d\n"\
       , txth , txtw , txtg ) ;\
  }
#define check_page_limit {\
  if ( pl > pglimit ) {\
          NewPage = 1;\
          if ( Foot_note ) {\
              fprintf ( f23 , "$f0\n$U0\n$p1\n$h14\n$w14\n$i0\n$n\n" ) ;\
	      fprintf(f23,"$g1\n$a\n");\
              fprintf ( f23 , "$GC\n$GL%-d -3 %-d -3\n" , \
               ( int ) L_mar+Col_shft* ( col_num-1 ) , ( int ) L_mar+70+\
              Col_shft* ( col_num-1 ) ) ;\
              Foot_note = 0;\
              add_notes ( f23 , ftnotes , ( int ) ( Col_shft* ( col_num-1 ) ) ) ;\
              remove ( ftnotes ) ;\
              if ( col_num != Columns ) {\
                  Default_state(f23);\
                  pglimit = pglimit_bk;\
              }\
          }\
          if ( col_num == Columns ) goto l150;\
          else {\
              fprintf ( f23 , "$o%-d\n" , ( int ) ( ofsl+Col_shft*col_num ) *10 ) ;\
              fprintf ( f23 , "$m%-d\n" , ( int ) ( rmgl+Col_shft* ( col_num ) ) ) ;\
              fprintf ( f23 , "$Y\n" ) ;\
              fprintf ( f23 , "$C%-d\n" , ( int ) ( Col_shft*col_num ) ) ;\
              pl = Brk_pt;\
              col_num++;\
          }\
      }\
  }
#define scan_page_limit {\
  if ( pl > pglimit ) {\
          NewPage = 1;\
          if ( Foot_note ) {\
              Foot_note = 0;\
              if ( col_num != Columns ) {\
                  pglimit = pglimit_bk;\
              }\
          }\
          if ( col_num == Columns ) goto l150;\
          else {\
              pl = Brk_pt;\
              col_num++;\
          }\
      }\
  }
#define list_align_print {  \
  int i = 0 , j; \
      char ch; \
      while ( ( line [ i ] != '\0' ) && ( ! ( ( line [ i ] == '!' ) && ( line [ i+1 ] == '%' ) ) ) ) { \
          i++; \
      } \
      if ( ( i != 0 ) && ( line [ i ] != '\0' ) ) { \
          fprintf ( ot , "$l\n" ) ; \
          no_advance; \
          check_dolor; \
          for ( j = 0;j < i;j++ ) putc ( line [ j ] , ot ) ; \
          fprintf ( ot , "\n$a\n" ) ; \
          ch = line [ i ] ; \
          line [ i ] = '\0'; \
          ofchrs = str_lngth ( line , & dsp ) +1; \
          line [ i ] = ch; \
          i+= 2; \
          pofs = 0.8*width*ofchrs+0.5; \
          fprintf ( ot , "$o%-d\n" , ( ofs+pofs ) *10 ) ; \
          lnlng = rmg - ofs -pofs;\
          check_dolor; \
          scr_strlngth ( & line [ i ] , & xdsp ) ;\
          check_too_low ( & line [ i ] , lnlng , xdsp , wid ) ;\
          fprintf ( ot , "%s\n" , & line [ i ] ) ; \
      } \
      else{ \
          check_dolor; \
          check_too_low ( line , lnlng , xdsp , wid ) ;\
          fprintf ( ot , "%s\n" , line ) ; \
          fprintf ( ot , "$o%-d\n" , ( ofs+pofs ) *10 ) ; \
          lnlng -= pofs; \
      } \
  }
#define list_align_print_one {  \
  int i = 0 , j; \
      char ch; \
      while ( ( line [ i ] != '\0' ) && ( ! ( ( line [ i ] == '!' ) && ( line [ i+1 ] == '%' ) ) ) ) { \
          i++; \
      } \
      if ( ( i != 0 ) && ( line [ i ] != '\0' ) ) { \
          fprintf ( ot , "$l\n" ) ; \
          no_advance; \
          check_dolor; \
          for ( j = 0;j < i;j++ ) putc ( line [ j ] , ot ) ; \
          fprintf ( ot , "\n" ) ; \
          ch = line [ i ] ; \
          line [ i ] = '\0'; \
          ofchrs = str_lngth ( line , & dsp ) +1; \
          line [ i ] = ch; \
          i+= 2; \
          pofs = 0.8*width*ofchrs+0.5; \
          fprintf ( ot , "$o%-d\n" , ( ofs+pofs ) *10 ) ; \
          check_dolor; \
          fprintf ( ot , "%s\n" , & line [ i ] ) ; \
      } \
      else{ \
          check_dolor; \
          fprintf ( ot , "%s\n" , line ) ; \
      } \
      lnlng -= pofs; \
  }
#define check_too_much {\
  if ( Hypn != 1 ) {\
          lng = oldlng;\
          i = oldi;\
          line [ lng ] = '\0';\
          xdsp = oldxdsp;\
          check_never_end ( & line [ lng+1 ] , lnlng ) ;\
      }\
      else {\
          float xdspn;\
          int in , lngn , pos = 3;\
          char ch;\
          xdspn = xdsp;\
          in = i; lngn = lng;\
          lng = oldlng;\
          i = oldi;\
          ch = line [ lng ] ;\
          line [ lng ] = '\0';\
          check_never_end ( & line [ lng+1 ] , lnlng ) ;\
          xdsp = oldxdsp;\
          if ( ( xdsp < lnlng*0.93 ) && ( ( lngn-lng ) > 7 ) ) {\
              pos = 1;\
              xdsp = xdspn;\
              while ( xdsp > lnlng*1.01 ) {\
                  line [ lng ] = ch;\
                  lng = lngn-pos;\
                  while ( line [ lng ] == ' ' ) {pos++; lng = lngn-pos;}\
                  while ( line [ lng ] == '#' ) {\
                      if ( line [ lng+1 ] != 'R' ) {pos+= 2; lng = lngn-pos;}\
                      else {\
                          int i = 3;\
                          pos+= 3;\
                          while ( line [ lng+i ] != '/' ) {i++;pos++;}\
                          pos++;\
                          lng = lngn-pos;\
                      }\
                  }\
                  ch = line [ lng ] ;\
                  line [ lng ] = '\0';\
                  scr_strlngth ( line , & xdsp ) ;\
                  pos++;\
              }\
              line [ lng ] = ch;\
              if ( pos == 4 ) {pos++;lng--;}\
              if ( line [ lng+1 ] == ' ' ) {pos--;++lng;i = in-pos+1;}\
              else {\
                  if ( pos < 5 ) {lng = lngn;i = in;}\
                  else{\
                      i = in-pos+1;\
                      if ( ch != ' ' ) line [ lng++ ] = '-';\
                  }\
              }\
              line [ lng ] = '\0';\
              scr_strlngth ( line , & xdsp ) ;\
          } \
      }\
  }
  static void copy_string ( char *s , char *d ) ;
  typedef struct t_line {
      struct t_line *adr;
      char buf [ 200 ] ;
  } T_L;
  typedef struct r_name {
      long *adr;
      char fn [ 16 ] ;
      char token [ 5 ] ;
      int lines;
  } R_B;
  static int num_of_lines , hed_pos;
  static T_L *First_adr , *Max_line;
  static R_B *First_ref , *Cur_ref;
  static int T_left , T_right , E_chr , Width , Tbl_type = 0 , Tbl_off = 1;
  static float Widthf;
  static FILE *Tv , *Th , *Td;
  static long T_fill = 13;
  static FILE *Ogf;
  static unsigned char T_col_fill [ 100 ] , T_col_num;
  typedef struct position {
      float y;
      struct position *Pr;
      struct position *Nx;
  } G_K;
int Write_State(FILE *fp) {
      fprintf ( fp , "$s%-d\n" , ifac ) ;
      fprintf ( fp , "$f%1d\n" , ifnt ) ;
      fprintf ( fp , "$i%1d\n" , islant ) ;
      fprintf ( fp , "$U%1d\n" , iuline ) ;
      fprintf ( fp , "$tp%2.2d\n" , ipattern ) ;
      fprintf ( fp , "$td%1d\n" , ibodr ) ;
      fprintf ( fp , "$tb%1d\n" , ibold ) ;
      fprintf ( fp , "$tk%1d\n" , ibkgr ) ;
      fprintf ( fp , "$tf%1d\n" , ifill ) ;
      fprintf ( fp , "$tc%1d\n" , icolor ) ;
      fprintf ( fp , "$h%-d\n$w%-d\n$g%-d\n"
       , txth , txtw , txtg ) ;
	return 1;
}
  static int strchars ( char *title ) {
      float gj;
      int ngp , n , i , j , k;
      G_K *FB_P = NULL , *O_P = NULL , *D_P = NULL;
      ngp = 1;
      j = 0;
      while ( title [ j ] != '\0' ) j++;
      if ( j == 0 ) {
          return ( 0 ) ;
      }
      gj = -1;
      i = 0;
      while ( i < j ) {
          if ( title [ i ] != '!' ) {
              gj += 1.;
              } else {
              i = i + 1;
              switch ( title [ i ] ) {
                  case 's':
                  case 'S':
                  break;
                  case 'e':
                  break;
                  case 'b':
                  gj = gj - 1;
                  break;
                  case 'g':
                  break;
                  case 'r':
                  if ( O_P != NULL ) {
                      gj = O_P-> y;
                      D_P = O_P;
                      O_P = O_P-> Pr;
                      free ( D_P ) ;
                      if ( O_P == NULL ) FB_P = NULL;
                  }
                  break;
                  case 'k':
                  if ( FB_P == NULL ) {
                      FB_P = ( G_K * ) malloc ( ( int ) sizeof ( G_K ) ) ;
                      O_P = FB_P;
                      O_P-> Nx = NULL;
                      O_P-> Pr = NULL;
                      } else {
                      O_P-> Nx = ( G_K * ) malloc ( ( int ) sizeof ( G_K ) ) ;
                      O_P-> Nx-> Pr = O_P;
                      O_P = O_P-> Nx;
                      O_P-> Nx = NULL;
                  }
                   ( O_P-> y ) = gj;
                  break;
                  case '!':
                  gj = gj + 1;
                  break;
                  case '%':
                  break;
                  case 'f':
                  i += 2;
		/*gj = gj-2; */
                  break;
                  case 'c':
                  i += 2;
		/*gj = gj-2; */
                  break;
                  case 'z':
                  i = i + 2;
		/*gj = gj-2; */
                  break;
                  case 'h':
                  i = i + 2;
		/*gj = gj-2; */
                  break;
                  case 'w':
                  i = i + 2;
		/*gj = gj-2; */
                  break;
                  default:
                  break;
              }
          }
          i = i + 1;
      }
      ngp = gj + 0.1;
      O_P = FB_P;
      while ( O_P != NULL ) {
          D_P = O_P;
          O_P = O_P-> Pr;
          free ( D_P ) ;
      }
      return ( ngp ) ;
  }
  static File *clean_up_open ( char *flname ) {
      File *fp;
      fp = Fopen ( flname ) ;
      if ( fp != NULL ) rmv_right_blnk ( fp ) ;
      return fp;
  }
  static void clean_up ( char *flname ) {
      int i , j;
      FILE *fp;
      struct ln {
          char *bf;
          struct ln *nx;
      } *LN , *st , *tmp;
      char buf [ 500 ] ;
      LN = NULL;
      if ( ( fp = fopen ( flname , "r" ) ) == NULL ) return;
      while ( fgets ( buf , 500 , fp ) != NULL ) {
          i = 0;
          while ( buf [ i ] != '\n' ) i++;
          j = i - 1;
          while ( ( j > 0 ) && ( buf [ j ] == ' ' ) ) j--;
          buf [ ++j ] = '\n';
          buf [ ++j ] = '\0';
          if ( LN == NULL ) {
              LN = ( struct ln * ) malloc ( sizeof ( struct ln ) ) ;
              Chk_Alloc ( LN ) ;
              st = LN;
              LN-> nx = NULL;
              LN-> bf = ( char * ) malloc ( j + 1 ) ;
              Chk_Alloc ( LN-> bf ) ;
              strcpy ( LN-> bf , buf ) ;
              } else {
              LN-> nx = ( struct ln * ) malloc ( sizeof ( struct ln ) ) ;
              Chk_Alloc ( LN-> nx ) ;
              LN = LN-> nx;
              LN-> nx = NULL;
              LN-> bf = ( char * ) malloc ( j + 1 ) ;
              Chk_Alloc ( LN-> bf ) ;
              strcpy ( LN-> bf , buf ) ;
          }
      }
      LN = st;
      fclose ( fp ) ;
      fp = fopen ( flname , "w" ) ;
      while ( LN != NULL ) {
          tmp = LN-> nx;
          fprintf ( fp , "%s" , LN-> bf ) ;
          free ( LN-> bf ) ;
          free ( LN ) ;
          LN = tmp;
      }
      fclose ( fp ) ;
  }
  static int find_break_point ( char *buf , int start , int max ) {
      int i;
      i = start;
      if ( start >= max ) return ( start ) ;
      if ( buf [ i ] == '\0' ) return ( i ) ;
      else {
          while ( ( buf [ i ] != BLANK ) && ( buf [ i ] != '\0' ) ) i++;
          return ( i ) ;
      }
  }
  static int check_break_point ( char *buf , int brek , int max ) {
      if ( brek > max ) return ( 1 ) ;
      if ( ( buf [ brek ] == BLANK ) || ( buf [ brek ] == '\0' ) ) return ( 1 ) ;
      else return ( 0 ) ;
  }
  static int check_all_lines ( T_L * ptr , int b_chr , int e_chr , int brek ) {
      int ret;
      T_L *tptr;
      tptr = ptr;
      while ( tptr != NULL ) {
          ret = check_break_point ( ( tptr-> buf ) , brek , e_chr ) ;
          if ( ret == 0 ) return ( 0 ) ;
          tptr = tptr-> adr;
      }
      return ( 1 ) ;
  }
  static int str_lngth ( char *chr , float *dsp ) {
      int i = 0 , j = 0 , k = 0;
      while ( * ( chr + i ) != '\0' ) i++;
      i--;
      while ( ( * ( chr + i ) == BLANK ) && ( i >= 0 ) ) i--;
      i++;
      for ( j = 0; j < i; j++ ) {
          if ( * ( chr + j ) == BLANK ) {
              Buf [ k++ ] = '!';
              Buf [ k++ ] = 'g';
              Buf [ k++ ] = ' ';
          } else Buf [ k++ ] = * ( chr + j ) ;
      }
      Buf [ k ] = '\0';
      *dsp = kgStringLength ( Img , Buf ) ;
      return strchars ( Buf ) ;
  }
  static void Str_lngth ( char *chr , float *dsp ) {
      int i = 0 , j = 0 , k = 0;
      while ( * ( chr + i ) != '\0' ) i++;
      for ( j = 0; j < i; j++ ) {
          if ( * ( chr + j ) == BLANK ) {
              Buf [ k++ ] = '!';
              Buf [ k++ ] = 'g';
              Buf [ k++ ] = ' ';
          } else Buf [ k++ ] = * ( chr + j ) ;
      }
      Buf [ k ] = '\0';
      *dsp = kgStringLength ( Img , Buf ) ;
      return;
  }
  static void Title_lngth ( char *chr , float *dsp ) {
      int i = 0 , j = 0 , k = 0;
      while ( * ( chr + i ) != '\0' ) i++;
      while ( ( * ( chr + i - 1 ) == BLANK ) || ( * ( chr + i - 1 ) == ' ' ) ) {
          i--;
      }
      for ( j = 0; j < i; j++ ) {
          if ( * ( chr + j ) == BLANK ) {
              Buf [ k++ ] = '!';
              Buf [ k++ ] = 'g';
              Buf [ k++ ] = ' ';
          } else Buf [ k++ ] = * ( chr + j ) ;
      }
      Buf [ k ] = '\0';
      *dsp = kgStringLength ( Img , Buf ) ;
      return;
  }
  static int find_offset ( int b_chr ) {
      T_L *pt;
      char *chr , ch;
      int i = 0 , ln = 0 , lngth = 0;
      float dsp;
      if ( b_chr <= 0 ) return ( T_left ) ;
      pt = First_adr;
      while ( pt != NULL ) {
          chr = pt-> buf;
          for ( i = 0; i < b_chr; i++ ) {
              if ( chr [ i ] == 127 ) buf [ i ] = BLANK;
              else buf [ i ] = chr [ i ] ;
          }
          buf [ b_chr ] = '\0';
/* Coreections made on 4-6-96 
    i=b_chr-1;
    while((i>=0)&&(buf[i]==BLANK))i--;
    buf[i+1]='\0';
    end corrections   */
          Str_lngth ( buf , & dsp ) ;
          ln = dsp + 0.5;
          if ( ln > lngth ) lngth = ln;
          pt = pt-> adr;
      }
      return ( lngth + T_left + Width ) ;
  }
  static int find_max_offset ( int b_chr ) {
      T_L *pt;
      char *chr , ch;
      int i = 0 , ln = 0 , lngth = 0;
      float dsp;
      pt = First_adr;
      while ( pt != NULL ) {
          chr = pt-> buf;
          i = b_chr;
          while ( ( i > 0 ) && ( ( chr [ i ] == BLANK ) 
              || ( chr [ i ] == '\0' ) ) ) i--;
          ch = chr [ i + 1 ] ;
          chr [ i + 1 ] = '\0';
          str_lngth ( chr , & dsp ) ;
          ln = dsp + 1.0;
          if ( ln > lngth ) {
              lngth = ln;
              Max_line = pt;
          }
          chr [ i + 1 ] = ch;
          pt = pt-> adr;
      }
      return ( lngth ) ;
  }
  static int check_top_lines ( int b_chr , int e_chr ) {
      T_L *pt;
      char *chr;
      int i = 0 , lngth = 0;
      pt = First_adr;
      while ( ( pt != NULL ) && ( i < hed_pos ) ) {
          i++;
          chr = pt-> buf;
          if ( b_chr > 0 ) {
              if ( ( chr [ b_chr ] != 127 ) || ( chr [ e_chr ] != 127 ) ) lngth = i;
          } else if ( chr [ e_chr ] != 127 ) lngth = i;
          pt = pt-> adr;
      }
      return ( hed_pos - lngth ) ;
  }
  static void Add_blanks ( T_L * first_adr , int at_chr , int ext_c ) {
      T_L *pt;
      char *chr;
      int j , i = 0 , lngth = 0 , nxt_brk , OK = 1;
      pt = first_adr;
      while ( ( pt != NULL ) ) {
          chr = pt-> buf;
          for ( i = E_chr; i > at_chr; i-- ) {
              chr [ i + ext_c ] = chr [ i ] ;
          }
          chr [ E_chr + 1 + ext_c ] = '\0';
          for ( j = 1; j <= ext_c; j++ ) {
              chr [ at_chr + j ] = BLANK;
          }
          pt = pt-> adr;
      }
      pt = First_adr;
      while ( pt != first_adr ) {
          chr = pt-> buf;
          i = at_chr;
          while ( ( i <= E_chr ) && ( OK ) ) {
              if ( chr [ i ] == 127 ) OK = 0;
              else i++;
          }
          nxt_brk = i;
          for ( i = E_chr; i >= nxt_brk; i-- ) {
              chr [ i + ext_c ] = chr [ i ] ;
          }
          chr [ E_chr + 1 + ext_c ] = '\0';
          for ( j = 0; j < ext_c; j++ ) {
              chr [ nxt_brk + j ] = BLANK;
          }
          pt = pt-> adr;
      }
      return;
  }
  static int print_column ( FILE * fp , T_L * first_adr , int b_chr , int e_chr , int h_end , int rmg ) 
      {
      int i , off_set , R_mar;
      T_L *pt;
      int y_post , y_posb , ENTRY = 1 , Pos , Right = 1 , y_sh_t , ext_c;
      float Max_ln , max_ln , Shift , shift;
      float col_wid;
      char *chr;
      char ch , ch1;
      Max_ln = Width;
      pt = first_adr;
      off_set = find_offset ( b_chr - 1 ) ;
      R_mar = find_offset ( e_chr ) ;
      if ( R_mar > T_right ) {
          T_right = R_mar + Width;
          return ( -1 ) ;
      }
      if ( e_chr == E_chr ) R_mar = T_right;
      col_wid = R_mar - off_set;
      y_post = ( hed_pos ) * 4 + 1;
      y_posb = ( num_of_lines ) * 4 + 1;
      if ( T_col_fill [ T_col_num ] != 0 ) {
          y_sh_t = -y_post + 4 * check_top_lines ( b_chr - 1 , e_chr ) ;
          fprintf ( Ogf , "$GR %-d %-d %-d %-d %-d \n" , off_set , y_sh_t , R_mar , -y_posb , T_col_fill [ T_col_num ] ) ;
              
      }
      T_col_num++;
      fprintf ( Tv , "%-d %-d %-d \n" , off_set , y_post , y_posb ) ;
      if ( b_chr < 0 ) printf ( "b_chr < 0\n" ) ;
      Refix_begin_char;
      while ( pt != NULL ) {
          int pos;
          if ( b_chr < 0 ) exit ( 0 ) ;
          chr = & ( pt-> buf [ b_chr ] ) ;
          for ( pos = 0; pos < ( e_chr - b_chr + 1 ) ; pos++ ) {
              if ( * ( chr + pos ) == 8 ) { /* introduced for debuging */
                  printf ( "%s\n" , chr ) ;
              }
              buf [ pos ] = * ( chr + pos ) ;
          }
          buf [ e_chr - b_chr + 1 ] = '\0';
          i = 0;
          while ( * ( buf + i ) != '\0' ) {
              i++;
          }
          i--;
          while ( ( * ( buf + i ) == BLANK ) && ( i > 0 ) ) {
              i--;
          }
          if ( ( i != 0 ) && ENTRY ) {
              * ( buf + i + 1 ) = '\0';
              Pos = i;
              ENTRY = 0;
              Str_lngth ( buf , & Max_ln ) ;
              } else if ( ( i != 0 ) ) {
              if ( i != Pos ) Right = 0;
              * ( buf + i + 1 ) = '\0';
              Str_lngth ( buf , & max_ln ) ;
              if ( max_ln > Max_ln ) Max_ln = max_ln;
          }
          pt = pt-> adr;
      }
      Shift = ( col_wid - Max_ln ) ;
      if ( Shift <= 0 ) {
          ext_c = -Shift / ( float ) Widthf + 1.0;
          Add_blanks ( first_adr , e_chr , ext_c ) ;
          E_chr += ext_c;
          T_right += ( ext_c * Width ) ;
          return ( -1 ) ;
      }
      Shift /= 2.;
      for ( i = 0; i < hed_pos; i++ ) fprintf ( fp , "\n" ) ;
      Shift += 0.5;
      if ( Right ) {
          pt = first_adr;
          while ( pt != NULL ) {
              chr = & ( pt-> buf [ b_chr ] ) ;
              for ( i = 0; i < ( e_chr - b_chr + 1 ) ; i++ ) buf [ i ] = * ( chr + i ) ;
              buf [ e_chr - b_chr + 1 ] = '\0';
              str_lngth ( buf , & max_ln ) ;
              shift = ( Max_ln - max_ln ) ;
              fprintf ( fp , "$o%-d\n" , ( int ) ( ( off_set + Shift + shift ) * 10. + 0.5 ) ) ;
                  
              Fprintf ( fp , buf ) ;
              pt = pt-> adr;
          }
          } else {
          fprintf ( fp , "$o%-d\n" , ( int ) ( ( off_set + Shift ) * 10.0 + 0.5 ) ) ;
          pt = first_adr;
          while ( pt != NULL ) {
              chr = & ( pt-> buf [ b_chr ] ) ;
              for ( i = 0; i < ( e_chr - b_chr + 1 ) ; i++ ) buf [ i ] = * ( chr + i ) ;
              buf [ e_chr - b_chr + 1 ] = '\0';
              Fprintf ( fp , buf ) ;
              pt = pt-> adr;
          }
      }
/*
    if(Right){
     fprintf(fp,"$l\n$m%-d\n",rmg);
    }
*/
      fprintf ( fp , "$A-%-d\n\n" , num_of_lines ) ;
      return ( 1 ) ;
  }
  static int print_one_column_old ( FILE * fp , T_L * first_adr , int b_chr , int e_chr , int h_end ) 
      {
      int i , off_set , R_mar;
      int y_pos , ext_c;
      T_L *pt;
      char *chr;
      float lngth , col_wid , Shift;
      char ch;
      pt = first_adr;
      off_set = find_offset ( b_chr - 1 ) ;
      R_mar = find_offset ( e_chr ) ;
      if ( R_mar > T_right ) {
          T_right = R_mar + Width;
          return ( -1 ) ;
      }
      if ( e_chr == E_chr ) R_mar = T_right;
      col_wid = R_mar - off_set;
      y_pos = ( hed_pos - h_end + 1 ) * 4 + 1;
      fprintf ( Tv , "%-d %d %-d \n" , off_set , y_pos - 4 , y_pos ) ;
      fprintf ( Th , "%-d %-d %-d \n" , y_pos , off_set , R_mar ) ;
      fprintf ( fp , "$o%-d\n" , ( int ) ( ( off_set + 
          ( 3. * Width ) / 4 ) * 10 + 0.5 ) ) ;
      for ( i = 0; i < ( hed_pos - h_end ) ; i++ ) fprintf ( fp , "\n" ) ;
      if ( pt != NULL ) {
          chr = & ( pt-> buf [ b_chr ] ) ;
          ch = pt-> buf [ e_chr ] ;
          pt-> buf [ e_chr ] = '\0';
          fprintf ( fp , "$A-%-d\n" , ( hed_pos - h_end ) ) ;
          i = 0;
          while ( * ( chr + i ) != '\0' ) {
              * ( buf + i ) = * ( chr + i ) ;
              i++;
          }
          buf [ i ] = '\0';
          Str_lngth ( buf , & lngth ) ;
          Fprintf ( fp , buf ) ;
          pt-> buf [ e_chr ] = ch;
      }
      Shift = ( col_wid - lngth ) ;
      if ( Shift <= 0 ) {
          ext_c = -Shift / ( float ) Widthf + 1.0;
          Add_blanks ( first_adr , e_chr , ext_c ) ;
          E_chr += ext_c;
          T_right += ( ext_c * Width ) ;
          return ( -1 ) ;
      }
      first_adr-> buf [ e_chr ] = 127;
      return ( 1 ) ;
  }
  static int print_one_column ( FILE * fp , T_L * first_adr , int b_chr , int e_chr , int h_end ) 
      {
      int i , off_set , R_mar , Ofset;
      int y_pos , ext_c;
      T_L *pt;
      char *chr;
      float lngth = 0.0 , col_wid , Shift;
      char ch , *Bf;
      pt = first_adr;
      off_set = find_offset ( b_chr - 1 ) ;
      R_mar = find_offset ( e_chr ) ;
      if ( R_mar > T_right ) {
          T_right = R_mar + Width;
          return ( -1 ) ;
      }
      if ( e_chr == E_chr ) R_mar = T_right;
      col_wid = R_mar - off_set;
      Shift = col_wid;
      y_pos = ( hed_pos - h_end + 1 ) * 4 + 1;
      fprintf ( Tv , "%-d %d %-d \n" , off_set , y_pos - 4 , y_pos ) ;
      fprintf ( Th , "%-d %-d %-d \n" , y_pos , off_set , R_mar ) ;
      for ( i = 0; i < ( hed_pos - h_end ) ; i++ ) fprintf ( fp , "\n" ) ;
      if ( pt != NULL ) {
          chr = & ( pt-> buf [ b_chr ] ) ;
          ch = pt-> buf [ e_chr ] ;
          pt-> buf [ e_chr ] = '\0';
          fprintf ( fp , "$A-%-d\n" , ( hed_pos - h_end ) ) ;
          i = 0;
          while ( * ( chr + i ) != '\0' ) {
              * ( buf + i ) = * ( chr + i ) ;
              i++;
          }
          buf [ i ] = '\0';
          i = 0;
          while ( ( buf [ i ] == BLANK ) || ( buf [ i ] == ' ' ) ) i++;
          Title_lngth ( & buf [ i ] , & lngth ) ;
/*   Width is subtracted from col_wid to accomodate the left offset */
          Shift = ( col_wid - lngth - Width ) ;
          Ofset = Shift * 0.5 + 3. * Width / 4. + 0.5;
          fprintf ( fp , "$o%-d\n" , ( int ) ( ( Ofset + off_set ) * 10. + 0.5 ) ) ;
          Bf = & buf [ i ] ;
          Fprintf ( fp , Bf ) ;
          pt-> buf [ e_chr ] = ch;
      }
      if ( Shift <= 0 ) {
          ext_c = -Shift / ( float ) Widthf + 1.0;
          Add_blanks ( first_adr , e_chr , ext_c ) ;
          E_chr += ext_c;
          T_right += ( ext_c * Width ) ;
          return ( -1 ) ;
      }
      first_adr-> buf [ e_chr ] = 127;
      return ( 1 ) ;
  }
  static int find_column ( FILE * fp , T_L * first_adr , int b_chr , int e_chr , int h_end , int rmg ) 
      {
      int brk_f , nb_chr , b_chr_d;
      char *chpt;
      T_L *pt;
      if ( first_adr == NULL ) return ( e_chr + 1 ) ;
      chpt = ( first_adr-> buf ) ;
      brk_f = b_chr;
      while ( ( brk_f < e_chr ) && ( check_all_lines 
          ( first_adr , b_chr , e_chr , brk_f ) > 0 ) ) {
          brk_f++;
      }
      b_chr_d = brk_f;
      brk_f = find_break_point ( chpt , b_chr_d , e_chr ) ;
      while ( ( brk_f < e_chr ) && ( check_all_lines ( first_adr , b_chr , e_chr , brk_f ) == 0 ) ) brk_f++;
          
      if ( ( brk_f >= e_chr ) && ( h_end == 0 ) ) {
          if ( print_column ( fp , first_adr , b_chr , e_chr , h_end , rmg ) < 0 ) return 
              ( -1 ) ;
          return ( e_chr + 1 ) ;
          } else {
          while ( ( brk_f < e_chr ) && ( check_all_lines ( first_adr , b_chr , e_chr , brk_f ) > 0 ) ) brk_f++;
              
          if ( brk_f < e_chr ) brk_f--;
          if ( h_end == 0 ) {
              if ( print_column ( fp , first_adr , b_chr , brk_f , h_end , rmg ) < 0 ) return 
                  ( -1 ) ;
              return ( brk_f + 1 ) ;
              } else {
              if ( print_one_column ( fp , first_adr , b_chr , brk_f , h_end ) < 0 ) return 
                  ( -1 ) ;
              while ( ( nb_chr = find_column ( fp , ( T_L * ) first_adr-> adr , b_chr , brk_f , h_end - 1 , rmg ) ) < brk_f ) 
                  {
                  if ( nb_chr < 0 ) {
                      return ( -1 ) ;
                  }
                  b_chr = nb_chr;
              }
          }
      }
  }
  static int get_line ( FILE * fp , char *buf ) {
      int i;
      char ch;
      i = 0;
      while ( ( ch = getc ( fp ) ) != '\n' ) {
          if ( ch == EOF ) return ( -1 ) ;
          * ( buf + i ) = ch;
          i++;
          if ( i > 149 ) i = 149;
      }
      * ( buf + i ) = '\0';
      return ( i ) ;
  }
  static void copy_table_text ( FILE * Fp , FILE * ot ) {
      while ( get_line ( Fp , buf ) >= 0 ) {
          fprintf ( ot , "%s\n" , buf ) ;
      }
  }
  static void expand_tab ( char *buf , char *obuf ) {
      int j = 0 , k = 0;
      for ( ;; ) {
          while ( * ( buf + j ) >= ' ' ) {
              obuf [ k++ ] = buf [ j++ ] ;
          };
          if ( buf [ j ] == '\t' ) {
              int l;
              l = ( k / 8 + 1 ) * 8;
              while ( k < l ) obuf [ k++ ] = ' ';
              j++;
              } else {
              obuf [ k++ ] = '\n';
              obuf [ k++ ] = '\0';
              break;
          }
      }
  }
  static void process_tab ( char *buf , char *obuf ) {
      int j = 0 , k = 0;
      for ( ;; ) {
          while ( * ( buf + j ) >= ' ' ) {
              obuf [ k++ ] = buf [ j++ ] ;
          };
          if ( buf [ j ] == '\t' ) {
              int l;
              l = ( k / 8 + 1 ) * 8;
              while ( k < l ) obuf [ k++ ] = ' ';
              j++;
              } else {
              obuf [ k ] = '\0';
              break;
          }
      }
  }
  static T_L *get_tbl ( int *n_lines , int *h_end , File * fp ) {
      int i , ln , j , entry = 0;
      long fil = 14;
      T_L *new_adr;
      T_L *pt , *first_adr;
      first_adr = ( T_L * ) malloc ( sizeof ( T_L ) ) ;
      if ( first_adr == NULL ) {
          printf ( " Error : Im mem allocation\n" ) ;
          exit ( 0 ) ;
      }
      pt = first_adr;
      pt-> adr = NULL;
      i = 0;
      *n_lines = 0;
      *h_end = 0;
      while ( Get_line ( Buf , fp ) >= 0 ) {
          process_tab ( Buf , buf ) ;
          if ( buf [ 0 ] == '$' ) {
              switch ( buf [ 1 ] ) {
                  case 'E':
                  if ( buf [ 2 ] == 'H' ) {
                      *h_end = i;
                  }
                  if ( buf [ 2 ] == 'T' ) {
                      switch ( buf [ 3 ] ) {
                          case '0':
                          Tbl_off = 1;
                          break;
                          case '1':
                          Tbl_off = 0;
                          break;
                          default:
                          Tbl_off = 1;
                          break;
                      }
                      return ( first_adr ) ;
                  }
                  break;
                  case 'D':
                  switch ( buf [ 2 ] ) {
                      case 'L':
                      fprintf ( Td , "D%-d\n" , i ) ;
                      break;
                      default:
                      break;
                  }
                  break;
                  case 'F':
                  switch ( buf [ 2 ] ) {
                      case 'R':
                      fprintf ( Td , "R%-d \n" , i ) ;
                      break;
                      case 'C':
                      if ( sscanf ( & buf [ 3 ] , "%d" , & fil ) > 0 ) {
                          T_col_fill [ fil - 1 ] = T_fill;
                      }
                      break;
                      case 'P':
                      if ( sscanf ( & buf [ 3 ] , "%d" , & fil ) > 0 ) {
                          T_fill = fil % 16;
                      }
                      break;
                      default:
                      break;
                  }
                  break;
                  default:
                  break;
              }
              } else {
              i++;
              if ( entry == 0 ) {
                  j = 0;
                  pt-> buf [ 0 ] = ' ';
                  while ( ( buf [ j ] != '\0' ) && ( j < 198 ) ) {
                      pt-> buf [ j + 1 ] = buf [ j ] ;
                      j++;
                  }
                  pt-> buf [ j + 1 ] = '\0';
                  entry = 1;
                  } else {
                  new_adr = ( T_L * ) malloc ( sizeof ( T_L ) ) ;
                  if ( new_adr == NULL ) {
                      printf ( " Error : In mem allocation\n" ) ;
                      exit ( 0 ) ;
                  }
                  pt-> adr = new_adr;
                  pt = new_adr;
                  pt-> adr = NULL;
                  j = 0;
                  pt-> buf [ 0 ] = ' ';
                  while ( ( buf [ j ] != '\0' ) && ( j < 198 ) ) {
                      pt-> buf [ j + 1 ] = buf [ j ] ;
                      j++;
                  }
                  pt-> buf [ j + 1 ] = '\0';
                  if ( i > 200 ) {
                      printf ( " Error: too many lines in table...\n" ) ;
                      exit ( 0 ) ;
                  }
              }
          }
          *n_lines = i;
      }
      return ( first_adr ) ;
  }
  static void clear_tbl_mem ( T_L * first_adr ) {
      T_L *pt , *n_adr;
      pt = first_adr;
      while ( pt != NULL ) {
          n_adr = pt-> adr;
          free ( pt ) ;
          pt = n_adr;
      }
  }
  static int maximum_lngth ( T_L * first_adr ) {
      T_L *pt;
      int i , ln;
      char *chr;
      ln = 0;
      pt = first_adr;
      if ( pt == NULL ) return ( 0 ) ;
      while ( pt != NULL ) {
          i = 0;
          chr = pt-> buf;
          while ( * ( chr + i ) != '\0' ) i++;
          if ( i > ln ) ln = i;
          pt = pt-> adr;
      }
      pt = first_adr;
      while ( pt != NULL ) {
          i = 0;
          chr = pt-> buf;
          while ( * ( chr + i ) != '\0' ) i++;
          while ( i < ln ) {
              * ( chr + i ) = ' ';
              i++;
          }
          * ( chr + ln ) = '\0';
          pt = pt-> adr;
      }
      return ( ln ) ;
  }
  static void process_hor_lines ( FILE * fp ) {
      FILE *in , *ot;
      int i , y , x1 , x2 , yd , x1d , x2d;
      char fln1 [ 15 ] , fln2 [ 15 ] ;
      i = -1;
      for ( ;; ) {
          i = ( i + 1 ) % 2;
          sprintf ( fln1 , "Tbl_Draw.Ho%1.1d\0" , i ) ;
          sprintf ( fln2 , "Tbl_Draw.Ho%1.1d\0" , ( ( i + 1 ) % 2 ) ) ;
          in = fopen ( fln1 , "r" ) ;
          if ( in == NULL ) break;
          ot = fopen ( fln2 , "w" ) ;
          if ( fscanf ( in , "%d %d %d" , & y , & x1 , & x2 ) <= 0 ) break;
          while ( fscanf ( in , "%d %d %d" , & yd , & x1d , & x2d ) > 0 ) {
              if ( ( x1 == x1d ) && ( x2 == x2d ) ) {
                  if ( yd > y ) y = yd;
              } else fprintf ( ot , "%-d %-d %-d \n" , yd , x1d , x2d ) ;
          }
          fclose ( in ) ;
          fclose ( ot ) ;
          fprintf ( fp , "$GL %-d %-d %-d %-d \n" , x1 , -y , x2 , -y ) ;
      }
      fclose ( in ) ;
      fclose ( ot ) ;
  }
  static void process_ver_lines ( FILE * fp ) {
      FILE *in , *ot;
      int i , x , y1 , y2 , xd , y1d , y2d;
      char fln1 [ 15 ] , fln2 [ 15 ] ;
      i = -1;
      for ( ;; ) {
          i = ( i + 1 ) % 2;
          sprintf ( fln1 , "Tbl_Draw.Ve%1.1d\0" , i ) ;
          sprintf ( fln2 , "Tbl_Draw.Ve%1.1d\0" , ( ( i + 1 ) % 2 ) ) ;
          in = fopen ( fln1 , "r" ) ;
          if ( in == NULL ) break;
          ot = fopen ( fln2 , "w" ) ;
          if ( fscanf ( in , "%d %d %d" , & x , & y1 , & y2 ) <= 0 ) break;
          while ( fscanf ( in , "%d %d %d" , & xd , & y1d , & y2d ) > 0 ) {
              if ( ( x == xd ) ) {
                  if ( y1d < y1 ) y1 = y1d;
                  if ( y2d > y2 ) y2 = y2d;
              } else fprintf ( ot , "%-d %-d %-d \n" , xd , y1d , y2d ) ;
          }
          fclose ( in ) ;
          fclose ( ot ) ;
          fprintf ( fp , "$GL %-d %-d %-d %-d \n" , x , -y1 , x , -y2 ) ;
      }
      fclose ( in ) ;
      fclose ( ot ) ;
  }
  static void process_draw_lines ( FILE * fp ) {
      FILE *in , *ot;
      long fil;
      int i , y , x1 , x2 , yd , x1d , x2d;
      char ch , ch1;
      in = fopen ( "Tbl_Draw.HoR" , "r" ) ;
      if ( in == NULL ) return;
      while ( ( ch = getc ( in ) ) != EOF ) {
          switch ( ch ) {
              case 'D':
              if ( fscanf ( in , "%d" , & yd ) > 0 ) {
                  y = yd * 4 + 1;
                  fprintf ( fp , "$GL %-d %-d %-d %-d \n" , T_left , -y , T_right , -y ) ;
              }
              break;
              case 'R':
              if ( fscanf ( in , "%d" , & yd ) > 0 ) {
                  y = yd * 4 + 1;
                  fprintf ( fp , "$GR %-d %-d %-d %-d %-d \n" , T_left , -y , T_right , -y - 4 , T_fill ) ;
                      
              }
              break;
              default:
              break;
          }
      }
      fclose ( in ) ;
  }
  static int remove_blanks ( void ) {
      int n_b_pos = 1 , i , j , OK = 1;
      T_L *pt;
      char *chr;
      while ( OK ) {
          pt = First_adr;
          while ( pt != NULL ) {
              OK = 0;
              if ( pt-> buf [ n_b_pos ] != ' ' ) break;
              OK = 1;
              pt = pt-> adr;
          }
          if ( OK ) n_b_pos++;
      }
      pt = First_adr;
      while ( pt != NULL ) {
          i = n_b_pos;
          j = 1;
          chr = pt-> buf;
          while ( * ( chr + i ) != '\0' ) {
              * ( chr + j ) = * ( chr + i ) ;
              if ( * ( chr + j ) == ' ' ) * ( chr + j ) = BLANK;
              i++;
              j++;
          }
          * ( chr + j ) = '\0';
          pt = pt-> adr;
      }
      return ( n_b_pos - 1 ) ;
  }
  static void Clean_up ( ) {
      int n_b_pos = 1 , i , j , OK = 1;
      T_L *pt;
      char *chr;
      pt = First_adr;
      while ( pt != NULL ) {
          chr = pt-> buf;
          i = 0;
          while ( * ( chr + i ) != '\0' ) {
              if ( chr [ i ] == 127 ) chr [ i ] = BLANK;
              i++;
          }
          pt = pt-> adr;
      }
  }
  static void mktbl ( File * fp , FILE * ot , int ofs , int rmg , float width ) 
      {
      int n_lines , h_end , e_chr , st_p , nst_p , i;
      int y_posh , y_post , y_posb , y_posd , maxln;
      int Err = 1;
      FILE *Fp;
      T_L *first_ptr;
      float wd;
      for ( i = 0; i < 100; i++ ) T_col_fill [ i ] = 0;
      fprintf ( ot , "$U0\n" ) ;
      Td = fopen ( "Tbl_Draw.HoR" , "w" ) ;
      first_ptr = get_tbl ( & n_lines , & h_end , fp ) ;
      fprintf ( ot , "$l\n$GC\n" ) ;
      First_adr = first_ptr;
      num_of_lines = n_lines;
      hed_pos = h_end;
      e_chr = maximum_lngth ( first_ptr ) ;
      e_chr -= remove_blanks ( ) ;
      E_chr = e_chr;
      maxln = find_max_offset ( E_chr ) ;
      wd = kgStringLength ( Img , ( char * ) "!g " ) ;
      Width = wd + 0.5;
      Widthf = wd;
      fclose ( Td ) ;
      if ( Tbl_off ) {
          T_left = ( rmg - ofs - maxln - 2 * Width ) / 2 + ofs;
          T_right = T_left + maxln + 2 * Width;
      }
      while ( Err ) {
          T_col_num = 0;
          e_chr = E_chr;
          Err = 0;
          st_p = 0;
          Fp = fopen ( "Tbl_Txt.DuM" , "w" ) ;
          Ogf = fopen ( "Tbl_Draw.VeR" , "w" ) ;
          Tv = fopen ( "Tbl_Draw.Ve0" , "w" ) ;
          Th = fopen ( "Tbl_Draw.Ho0" , "w" ) ;
          while ( ( Err == 0 ) && ( ( nst_p = find_column ( Fp , first_ptr , st_p , e_chr , h_end , rmg ) ) < e_chr ) ) 
              {
              if ( nst_p < 0 ) Err = 1;
              st_p = nst_p;
          }
          fclose ( Tv ) ;
          fclose ( Th ) ;
          if ( Err ) {
              fclose ( Ogf ) ;
              fclose ( Fp ) ;
              Clean_up ( ) ;
              maxln = ( T_right - T_left ) ;
              T_left = ( rmg - ofs - maxln ) / 2 + ofs;
              T_right = T_left + maxln;
          }
      }
      fclose ( Ogf ) ;
      Ogf = fopen ( "Tbl_Draw.VeR" , "r" ) ;
      copy_table_text ( Ogf , ot ) ;
      fclose ( Ogf ) ;
      process_draw_lines ( ot ) ;
      if ( Tbl_type < 2 ) process_ver_lines ( ot ) ;
      if ( Tbl_type < 3 ) process_hor_lines ( ot ) ;
/*
*/
      remove ( "Tbl_Draw.Ve0" ) ;
      remove ( "Tbl_Draw.Ve1" ) ;
      remove ( "Tbl_Draw.Ho0" ) ;
      remove ( "Tbl_Draw.Ho1" ) ;
      remove ( "Tbl_Draw.HoR" ) ;
      y_post = -1;
      y_posh = - ( hed_pos * 4 + 1 ) ;
      y_posb = - ( num_of_lines * 4 + 1 ) ;
      if ( Tbl_type < 5 ) {
          fprintf ( ot , "$GW2\n" ) ;
          if ( hed_pos != 0 ) fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_left , y_posh , T_right , y_posh ) ;
              
          fprintf ( ot , "$GW3\n" ) ;
          fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_left , y_post , T_right , y_post ) ;
          fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_left , y_posb , T_right , y_posb ) ;
      }
      if ( Tbl_type < 4 ) {
          fprintf ( ot , "$GW3\n" ) ;
          fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_left , y_post , T_left , y_posb ) ;
          fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_right , y_post , T_right , y_posb ) ;
              
      }
      if ( ( Tbl_type == 1 ) || ( Tbl_type == 2 ) ) {
          fprintf ( ot , "$GW1\n$GY1\n" ) ;
          y_posd = y_posh;
          for ( i = hed_pos; i < num_of_lines - 1; i++ ) {
              y_posd -= 4;
              fprintf ( ot , "$GL%-d %-d %-d %-d\n" , T_left , y_posd , T_right , y_posd ) ;
          }
          fprintf ( ot , "$GY0\n" ) ;
      }
      fclose ( Fp ) ;
      Fp = fopen ( "Tbl_Txt.DuM" , "r" ) ;
      copy_table_text ( Fp , ot ) ;
      fclose ( Fp ) ;
      fprintf ( ot , "$GW1\n$A%-d\n\n" , num_of_lines ) ;
      fprintf ( ot , "$GF 1.0 1.0\n$GS 0. 0.\n$GE\n" ) ;
      fprintf ( ot , "$o%-d\n" , ( int ) ( ofs * 10 + 0.5 ) ) ;
      clear_tbl_mem ( first_ptr ) ;
      remove ( "Tbl_Txt.DuM" ) ;
      remove ( "Tbl_Draw.VeR" ) ;
  }
  static void check_never_end ( char *l , float lnlng ) {
      float dist;
      dist = kgStringLength ( Img , l ) ;
      if ( dist > lnlng ) {
          printf ( " Very long word...\n" ) ;
          printf ( " %s \n" , l ) ;
          exit ( 0 ) ;
      }
      return;
  }
  static void check_too_low ( char *l , float lnlng , float dist , float wd ) {
      char buf [ 200 ] ;
      float cdist;
      int b_pos = 0 , i , k = 0 , j , n_blk = 0 , E_blnk , a_b = 0 , chk_pt;
      if ( ! Rjust ) return;
      i = 0;
      while ( l [ i ] != '\0' ) {
          if ( l [ i ] == ' ' ) n_blk++;
          i++;
      }
      if ( n_blk == 0 ) n_blk = 1;
      if ( dist > ( lnlng * 0.97 ) ) return;
      E_blnk = ( int ) ( ( lnlng * 0.97 - dist ) / wd + 0.5 ) ;
      a_b = E_blnk / n_blk;
      chk_pt = E_blnk - n_blk * a_b;
      a_b++;
      i = 0;
      j = 0;
      while ( l [ i ] != '\0' ) {
          if ( l [ i ] == ' ' ) {
              b_pos++;
              if ( b_pos > chk_pt ) {
                  for ( k = 0; k <= ( a_b - 1 ) ; k++ ) buf [ j++ ] = ' ';
                  } else {
                  for ( k = 0; k <= a_b; k++ ) buf [ j++ ] = ' ';
              }
              } else {
              buf [ j++ ] = l [ i ] ;
          }
          i++;
      }
      buf [ j ] = '\0';
      i = 0;
      while ( buf [ i ] != '\0' ) {
          l [ i ] = buf [ i ] ;
          i++;
      }
      l [ i ] = '\0';
      return;
  }
  static int process_f_note ( char *bf , int last , int rmg , char *name , int *NOTE ) 
      {
      FILE *ot;
      char *buf;
      char flname [ 16 ] ;
      char line [ 500 ] ;
      int pofs , ofchrs = 3 , ofs , eql = 1;
      int ch , i = 0 , max , max0 , lng , lngmax , entry = 0 , oldi , oldlng , j , k;
          
      float lnlng , xdsp , oldxdsp , dsp , width,Fac;
      width = tw * 0.75 + tg;
      buf = bf;
      Lines = 0;
      Fac = 10.0/TxtW;
      *NOTE += 1;
      sprintf ( flname , "%-s_Note.%-3.3d\0" , name , *NOTE ) ;
      while ( ( eql != 0 ) && ( i < last ) ) {
          if ( buf [ i ] == '\\' ) {
              switch ( buf [ i + 1 ] ) {
                  case 'E':
                  i++;
                  eql--;
                  break;
                  case 'W':
                  i++;
                  eql++;
                  break;
                  case '\\':
                  break;
                  default:
                  printf ( " Wrong string in footnote\n" ) ;
                  buf [ 30 ] = '\0';
                  printf ( "$s...\n" , buf ) ;
                  exit ( 0 ) ;
              }
          }
          i++;
      }
      if ( i <= last ) i -= 2;
      if ( i < 1 ) return ( 0 ) ;
      ot = fopen ( flname , "w" ) ;
      pofs = width * ofchrs + 0.5;
      ofs = L_mar;
      max = i;
      max0 = max + 1;
      j = 0;
      i = 0;
      while ( i < max ) {
          if ( ( buf [ i ] == '#' ) && ( buf [ i + 1 ] == 'N' ) ) {
              sprintf ( line , "!S%-2d!e!%c\0" , *NOTE , '%' ) ;
              i += 2;
              k = 0;
              while ( line [ k ] != '\0' ) Buf [ j++ ] = line [ k++ ] ;
              } else {
              Buf [ j ] = buf [ i ] ;
              i++;
              j++;
          }
      }
      max = j;
      buf = Buf;
      i = 0;
      oldi = 0;
      lngmax = MAX1;
      if ( max < lngmax ) lngmax = max;
      lnlng = rmg - ofs;
      if ( lnlng < 40.0 ) {
          printf ( " Error: line width too narrow..\n" ) ;
          exit ( 0 ) ;
      }
      lng = 0;
      oldlng = 0;
      xdsp = 0.;
      oldxdsp = 0.;
      skip_blank;
      if ( lng >= lngmax ) {
          fprintf ( ot , "\n" ) ;
          return ( 0 ) ;
      };
      fprintf ( ot , "$o0\n$l\n" ) ;
      while ( ( xdsp < ( lnlng ) ) && ( i < max ) ) {
          oldi = i;
          oldlng = lng;
          oldxdsp = xdsp;
          copy_blanks;
          switch ( buf [ i ] ) {
              case '\\':
              i += 2;
              copy_block;
              check_limit;
              break;
              default:
              copy_non_word;
              copy_word;
              copy_punch;
              break;
          }
          line [ lng ] = '\0';
          xdsp = kgStringLength ( Img , line )*Fac ;
      }
      if ( xdsp > ( lnlng * 1.01 ) ) check_too_much;
      lngmax = MAX2;
      while ( i < max ) {
          if ( entry ) {
              check_dolor;
               ( Lines ) += 1;
              fprintf ( ot , "%s\n" , line ) ;
              } else {
              int i = 0 , j;
              char ch;
              while ( ( line [ i ] != '\0' ) && ( ! ( ( line [ i ] == '!' ) && 
                  ( line [ i + 1 ] == '%' ) ) ) ) {
                  i++;
              }
              if ( ( i != 0 ) && ( line [ i ] != '\0' ) ) {
                  fprintf ( ot , "$l\n" ) ;
                  no_advance;
                  check_dolor;
                  for ( j = 0; j < i; j++ ) putc ( line [ j ] , ot ) ;
                  fprintf ( ot , "\n$l\n" ) ;
                  ch = line [ i ] ;
                  line [ i ] = '\0';
                  ofchrs = strchars ( line ) + 1;
                  dsp = kgStringLength ( Img , line )*Fac ;
                  line [ i ] = ch;
                  i += 2;
                  pofs = width * ofchrs + 0.5;
                  fprintf ( ot , "$o%-d\n" , ( int ) ( pofs * 10 + 0.5 ) ) ;
                  check_dolor;
                   ( Lines ) += 1;
                  fprintf ( ot , "%s\n" , & line [ i ] ) ;
                  } else {
                  check_dolor;
                   ( Lines ) += 1;
                  fprintf ( ot , "%s\n" , line ) ;
                  fprintf ( ot , "$o%-d\n" , ( int ) ( pofs * 10 + 0.5 ) ) ;
              }
              lnlng -= pofs;
              entry = 1;
          }
          lng = 0;
          oldlng = 0;
          xdsp = 0.;
          oldxdsp = 0.;
          skip_blank;
          while ( ( xdsp < lnlng ) && ( i < max ) ) {
              oldi = i;
              oldlng = lng;
              oldxdsp = xdsp;
              copy_blanks;
              switch ( buf [ i ] ) {
                  case '\\':
                  i += 2;
                  copy_block;
                  check_limit;
                  break;
                  default:
                  copy_non_word;
                  copy_word;
                  copy_punch;
                  break;
              }
              line [ lng ] = '\0';
              xdsp = kgStringLength ( Img , line )*Fac ;
              Check_dist;
          }
          if ( ( xdsp > ( lnlng * 1.01 ) ) && ( lng != oldlng ) ) {
              check_too_much;
          }
          line [ lng ] = '\0';
      }
      if ( xdsp < lnlng - 2. ) fprintf ( ot , "$l\n" ) ;
      check_dolor;
       ( Lines ) += 1;
      fprintf ( ot , "%s\n" , line ) ;
      fclose ( ot ) ;
      return ( max0 ) ;
  }
  static int copy_token ( char *buf , char *d ) {
      int i , j;
      i = 0;
      while ( ( i < 5 ) && ( buf [ i ] != '/' ) ) {
          * ( d + i ) = buf [ i ] ;
          i++;
      }
      j = i + 2;
      if ( i < 5 ) while ( i < 5 ) * ( d + i++ ) = ' ';
      else {
          if ( buf [ i ] != '/' ) {
              printf ( "Refenece name more than 5 chars..\n" ) ;
              buf [ 6 ] = '\0';
              printf ( "%s...\n" , buf ) ;
              exit ( 0 ) ;
          }
      }
      return ( j ) ;
  }
  static int process_r_note ( char *bf , int last , int rmg , char *name , int *NOTE ) 
      {
      FILE *ot;
      char flname [ 16 ] ;
      char line [ 500 ] ;
      char *buf;
      int eql = 1;
      int pofs , ofchrs = 3 , ofs;
      int ch , i = 0 , max , max0 , lng , lngmax , entry = 0 , oldi , oldlng , j , k;
          
      float lnlng , xdsp , oldxdsp , dsp , wd;
      static float width;
      R_B *R_str , *ptr;
      if ( Entry == 0 ) {
          width = tw * wfac + tg * gfac;
          Entry = 1;
      }
      buf = bf;
      Lines = 0;
      *NOTE += 1;
      wd = kgStringLength ( Img , " " ) ;
      sprintf ( flname , "%-s_Note.%-3.3d\0" , name , *NOTE ) ;
      while ( ( eql != 0 ) && ( i < last ) ) {
          if ( buf [ i ] == '\\' ) {
              switch ( buf [ i + 1 ] ) {
                  case 'E':
                  i++;
                  eql--;
                  break;
                  case 'W':
                  i++;
                  eql++;
                  break;
                  case '\\':
                  break;
                  default:
                  printf ( " Wrong string in reference\n" ) ;
                  buf [ 30 ] = '\0';
                  printf ( "$s...\n" , buf ) ;
                  exit ( 0 ) ;
              }
          }
          i++;
      }
      if ( i <= last ) i -= 2;
      if ( i < 1 ) return ( 0 ) ;
      if ( buf [ 0 ] != '/' ) {
          printf ( "Reference name not given\n" ) ;
          buf [ 20 ] = '\0';
          printf ( "%s......\n" , buf ) ;
          exit ( 0 ) ;
      }
      R_str = ( R_B * ) malloc ( sizeof ( R_B ) ) ;
      if ( R_str == NULL ) {
          printf ( "Error: In /Ref/ .. Memory Allocation..\n" ) ;
          exit ( 0 ) ;
      }
      if ( Cur_ref != NULL ) {
          Cur_ref-> adr = ( long * ) R_str;
      } else First_ref = R_str;
      Cur_ref = R_str;
      R_str-> adr = NULL;
      copy_string ( flname , R_str-> fn ) ;
      max = i;
      max0 = max + 1;
      i = copy_token ( buf + 1 , R_str-> token ) ;
      ot = fopen ( flname , "w" ) ;
      if ( ot == NULL ) {
          printf ( "Error: In /Ref/ .. Opening FIle..\n" ) ;
          exit ( 0 ) ;
      }
      pofs = width * ofchrs + 0.5;
      ofs = L_mar;
      copy_string ( ( char * ) "1.  !%" , Buf ) ;
      j = 6;
      while ( i < max ) {
          Buf [ j ] = buf [ i ] ;
          i++;
          j++;
      }
      max = j;
      buf = Buf;
      i = 0;
      oldi = 0;
      lngmax = MAX1;
      if ( max < lngmax ) lngmax = max;
      lnlng = rmg - ofs;
      if ( lnlng < 40.0 ) {
          printf ( " Error: line width too narrow..\n" ) ;
          exit ( 0 ) ;
      }
      lng = 0;
      oldlng = 0;
      xdsp = 0.;
      oldxdsp = 0.;
      skip_blank;
      if ( lng >= lngmax ) {
          fprintf ( ot , "\n" ) ;
          return ( 0 ) ;
      };
      fprintf ( ot , "$o0\n$a\n" ) ;
      while ( ( xdsp < ( lnlng ) ) && ( i < max ) ) {
          oldi = i;
          oldlng = lng;
          oldxdsp = xdsp;
          copy_blanks;
          switch ( buf [ i ] ) {
              case '\\':
              i += 2;
              copy_block;
              check_limit;
              break;
              default:
              copy_non_word;
              copy_word;
              copy_punch;
              break;
          }
          line [ lng ] = '\0';
          xdsp = kgStringLength ( Img , line ) ;
      }
      if ( xdsp > ( lnlng * 1.01 ) ) {
          check_too_much;
      }
      {
          int i = 0 , j;
          char ch;
          while ( ( line [ i ] != '\0' ) && ( ! ( ( line [ i ] == '!' ) && 
              ( line [ i + 1 ] == '%' ) ) ) ) {
              i++;
          }
          if ( ( i != 0 ) && ( line [ i ] != '\0' ) ) {
              fprintf ( ot , "$l\n" ) ;
              no_advance;
              check_dolor;
              for ( j = 0; j < i; j++ ) putc ( line [ j ] , ot ) ;
              fprintf ( ot , "\n$a\n" ) ;
              ch = line [ i ] ;
              line [ i ] = '\0';
              ofchrs = strchars ( line ) ;
              dsp = kgStringLength ( Img , line ) ;
              line [ i ] = ch;
              i += 2;
              pofs = width * ofchrs + 0.5;
              fprintf ( ot , "$o%-d\n" , ( int ) ( pofs * 10 + 0.5 ) ) ;
              j = 0;
              while ( line [ i ] != '\0' ) line [ j++ ] = line [ i++ ] ;
              line [ j ] = '\0';
              xdsp = kgStringLength ( Img , line ) ;
              } else {
              printf ( "Error: %s\n" , line ) ;
              exit ( 0 ) ;
          }
          lnlng -= pofs;
          entry = 0;
      }
      lngmax = MAX2;
      while ( i < max ) {
          check_dolor;
           ( Lines ) += 1;
          check_too_low ( line , lnlng , xdsp , wd ) ;
          fprintf ( ot , "%s\n" , line ) ;
          lng = 0;
          oldlng = 0;
          xdsp = 0.;
          oldxdsp = 0.;
          skip_blank;
          while ( ( xdsp < lnlng ) && ( i < max ) ) {
              oldi = i;
              oldlng = lng;
              oldxdsp = xdsp;
              copy_blanks;
              switch ( buf [ i ] ) {
                  case '\\':
                  i += 2;
                  copy_block;
                  check_limit;
                  break;
                  default:
                  copy_non_word;
                  copy_word;
                  copy_punch;
                  break;
              }
              line [ lng ] = '\0';
              xdsp = kgStringLength ( Img , line ) ;
              Check_dist;
          }
          if ( ( xdsp > ( lnlng * 1.01 ) ) && ( lng != oldlng ) ) {
              check_too_much;
          }
          line [ lng ] = '\0';
          entry = 1;
      }
      if ( xdsp < lnlng - 2. ) fprintf ( ot , "$l\n" ) ;
      check_dolor;
      Lines++;
      R_str-> lines = Lines;
      fprintf ( ot , "%s\n" , line ) ;
      fclose ( ot ) ;
      return ( max0 ) ;
  }
  static void scr_strlngth ( char *line , float *xdsp ) {
      char bf [ 250 ] ;
      int i = 0 , j = 0 , k;
      while ( line [ i ] != '\0' ) {
          if ( ( line [ i ] == '#' ) && ( line [ i + 1 ] == 'R' ) ) {
              bf [ j++ ] = ' ';
              bf [ j++ ] = ' ';
              i += 3;
              k = i + 6;
              while ( ( line [ i ] != '/' ) && ( i < k ) ) i++;
          } else bf [ j++ ] = line [ i++ ] ;
      }
      bf [ j ] = '\0';
      *xdsp = kgStringLength ( Img , bf ) ;
  }
  static void append_para ( FILE * tmp , FILE * ot , int lines ) {
      int count = 0;
      char bf [ 500 ] ;
      if ( lines > 1 ) {
          while ( fgets ( bf , 500 , tmp ) != NULL ) {
              if ( bf [ 0 ] == '$' ) fprintf ( ot , "%s" , bf ) ;
              else {
                  count++;
                  fprintf ( ot , "%s" , bf ) ;
                  if ( count == lines - 2 ) fprintf ( ot , "$RL2\n" ) ;
              }
          }
          } else {
          if ( lines == 1 ) fprintf ( ot , "$RL3\n" ) ;
          while ( fgets ( bf , 500 , tmp ) != NULL ) {
              fprintf ( ot , "%s" , bf ) ;
          }
      }
  }
  static void get_strlngth ( char *line , float *xdsp ) {
      int i = 0 , j = 0 , k;
      while ( line [ i ] != '\0' ) {
          if ( ( line [ i ] == '#' ) && ( line [ i + 1 ] == 'R' ) ) {
              Buf [ j++ ] = ' ';
              Buf [ j++ ] = ' ';
              i += 3;
              k = i + 6;
              while ( ( line [ i ] != '/' ) && ( i < k ) ) i++;
              } else {
              if ( ( line [ i ] == '\\' ) && ( ( line [ i + 1 ] == 'R' ) 
                  || ( line [ i ] == 'N' ) ) ) {
                  k = 1;
                  i += 2;
                  while ( k != 0 ) {
                      while ( ( line [ i ] == '\\' ) && ( line [ i + 1 ] == 'E' ) ) {
                          k--;
                          i += 2;
                      }
                      if ( ( line [ i ] == '\\' ) ) {
                          k++;
                          i += 2;
                      } else i++;
                  }
              } else Buf [ j++ ] = line [ i++ ] ;
          }
      }
      Buf [ j ] = '\0';
      kgStringLength ( Img , Buf ) ;
  }
  static int Not_blnk_line ( char *l ) {
      int i = 0;
      while ( l [ i ] >= ' ' ) {
          if ( l [ i ] != ' ' ) return ( 1 ) ;
          i++;
      }
      return ( 0 ) ;
  }
  static void mkpara ( File * fp , FILE * ot , int ofs , int rmg , int pofs , float width ) 
      {
      long ch , i = 0 , max , lng , lngmax , entry = 0 , oldi , oldlng , e_lines , chk_line;
          
      float lnlng , xdsp , oldxdsp , t_lngth , wid;
      FILE *tmp;
      if ( ( ch = Getc ( fp ) ) == '$' ) {
          Ungetc ( ch , fp ) ;
          return;
      }
      FOREVER {
          if ( ch == EOF ) break;
          while ( ch < ' ' ) {
              if ( ( ch = Getc ( fp ) ) == EOF ) goto jmp;
              if ( ch == '$' ) {
                  Ungetc ( ch , fp ) ;
                  goto jmp;
              }
              if ( ( i > 1 ) && ( buf [ i - 1 ] != ' ' ) && ( ( buf [ i - 2 ] != '\\' ) 
                  || ( buf [ i - 1 ] != 'E' ) ) ) buf [ i++ ] = ' ';
          }
          buf [ i++ ] = ch;
          ch = Getc ( fp ) ;
      }
      jmp:
      if ( i < 1 ) return;
      wid = kgStringLength ( Img , buf ) ;
      max = i;
      buf [ i + 1 ] = '\0';
      i = 0;
      oldi = 0;
      lngmax = MAX1;
      if ( max < lngmax ) lngmax = max;
      lnlng = rmg - ofs - 5 - pofs;
      if ( lnlng < 40.0 ) {
          printf ( " Error: line width too narrow..\n" ) ;
          exit ( 0 ) ;
      }
      e_lines = 0;
      lng = 0;
      oldlng = 0;
      xdsp = 0.;
      oldxdsp = 0.;
      skip_blank;
      if ( lng >= lngmax ) {
          fprintf ( ot , "\n" ) ;
          return;
      };
      tmp = fopen ( "Para_Z.ZzZ" , "w" ) ;
      fprintf ( tmp , "$RL2\n$o%-d\n$a\n" , ( int ) 
          ( ( ofs + pofs ) * 10 + 0.5 ) ) ;
      while ( ( xdsp < ( lnlng ) ) && ( i < max ) ) {
          oldi = i;
          oldlng = lng;
          oldxdsp = xdsp;
          copy_blanks;
          switch ( buf [ i ] ) {
              case '\\':
              process_backslash;
              break;
              case '#':
              process_hash;
              break;
              default:
              copy_non_word;
              copy_word;
              copy_punch;
              break;
          }
          line [ lng ] = '\0';
          scr_strlngth ( line , & xdsp ) ;
	/*Check_dist; */
      }
      if ( xdsp > ( lnlng * 1.01 ) ) {
          check_too_much;
      }
      lngmax = MAX2;
      while ( i < max ) {
          e_lines++;
          check_too_low ( line , lnlng , xdsp , wid ) ;
          if ( line [ 0 ] == '$' ) fprintf ( ot , "!g !b%s\n" , line ) ;
          else fprintf ( tmp , "%s\n" , line ) ;
          if ( entry == 0 ) {
              fprintf ( tmp , "$o%-d\n" , ( int ) ( ofs * 10 + 0.5 ) ) ;
              lnlng += pofs;
              entry = 1;
          }
          lng = 0;
          oldlng = 0;
          xdsp = 0.;
          skip_blank;
          while ( ( xdsp < lnlng ) && ( i < max ) ) {
              oldi = i;
              oldlng = lng;
              oldxdsp = xdsp;
              copy_blanks;
              switch ( buf [ i ] ) {
                  case '\\':
                  process_backslash;
                  break;
                  case '#':
                  process_hash;
                  break;
                  default:
                  copy_non_word;
                  copy_word;
                  copy_punch;
                  break;
              }
              line [ lng ] = '\0';
              scr_strlngth ( line , & xdsp ) ;
              Check_dist;
          }
          if ( ( xdsp > ( lnlng * 1.01 ) ) && ( lng != oldlng ) ) {
              check_too_much;
          }
          line [ lng ] = '\0';
      }
      if ( Not_blnk_line ( line ) ) {
          e_lines++;
          if ( xdsp < lnlng - 2. ) fprintf ( tmp , "$l\n" ) ;
          if ( line [ 0 ] == '$' ) fprintf ( tmp , "!g !b%s\n" , line ) ;
          else fprintf ( tmp , "%s\n" , line ) ;
      }
      if ( entry == 0 ) fprintf ( tmp , "$o%-d\n" , ( int ) ( ofs * 10 + 0.5 ) ) ;
      fclose ( tmp ) ;
      tmp = fopen ( "Para_Z.ZzZ" , "r" ) ;
      append_para ( tmp , ot , e_lines ) ;
      fclose ( tmp ) ;
      remove ( "Para_Z.ZzZ" ) ;
  }
  static void mklist ( File * fp , FILE * ot , int ofs , int rmg , float width ) 
      {
      int pofs , ofchrs = 3;
      int ch , i = 0 , max , lng , lngmax , entry = 0 , oldi , oldlng;
      float lnlng , xdsp , oldxdsp , dsp , wid;
      if ( ( ch = Getc ( fp ) ) == '$' ) {
          Ungetc ( ch , fp ) ;
          return;
      }
      FOREVER {
          if ( ch == EOF ) break;
          while ( ch < ' ' ) {
              if ( ( ch = Getc ( fp ) ) == EOF ) goto jmp;
              if ( ch == '$' ) {
                  Ungetc ( ch , fp ) ;
                  goto jmp;
              }
              if ( ( i > 1 ) && ( buf [ i - 1 ] != ' ' ) && ( ( buf [ i - 2 ] != '\\' ) 
                  || ( buf [ i - 1 ] != 'E' ) ) ) buf [ i++ ] = ' ';
          }
          buf [ i++ ] = ch;
          ch = Getc ( fp ) ;
      }
      jmp:
      if ( i < 1 ) return;
      wid = kgStringLength ( Img , buf ) ;
      pofs = width * ofchrs + 0.5;
      max = i;
      i = 0;
      oldi = 0;
      lngmax = MAX1;
      if ( max < lngmax ) lngmax = max;
      lnlng = rmg - ofs;
      if ( lnlng < 40.0 ) {
          printf ( " Error: line width too narrow..\n" ) ;
          exit ( 0 ) ;
      }
      lng = 0;
      oldlng = 0;
      xdsp = 0.;
      oldxdsp = 0.;
      skip_blank;
      if ( lng >= lngmax ) {
          fprintf ( ot , "\n" ) ;
          return;
      };
      fprintf ( ot , "$o%-d\n$a\n" , ( int ) ( ofs * 10 + 0.5 ) ) ;
      while ( ( xdsp < ( lnlng ) ) && ( i < max ) ) {
          oldi = i;
          oldlng = lng;
          oldxdsp = xdsp;
          copy_blanks;
          switch ( buf [ i ] ) {
              case '\\':
              process_backslash;
              break;
              case '#':
              process_hash;
              break;
              default:
              copy_non_word;
              copy_word;
              copy_punch;
              break;
          }
          line [ lng ] = '\0';
          scr_strlngth ( line , & xdsp ) ;
      }
      if ( xdsp > ( lnlng * 1.01 ) ) {
          check_too_much;
      }
      lngmax = MAX2;
      while ( i < max ) {
          if ( entry ) {
              check_dolor;
              check_too_low ( line , lnlng , xdsp , wid ) ;
              fprintf ( ot , "%s\n" , line ) ;
              } else {
              list_align_print;
              entry = 1;
          }
          lng = 0;
          oldlng = 0;
          xdsp = 0.;
          oldxdsp = 0.;
          skip_blank;
          while ( ( xdsp < lnlng ) && ( i < max ) ) {
              oldi = i;
              oldlng = lng;
              oldxdsp = xdsp;
              copy_blanks;
              switch ( buf [ i ] ) {
                  case '\\':
                  process_backslash;
                  break;
                  case '#':
                  process_hash;
                  break;
                  default:
                  copy_non_word;
                  copy_word;
                  copy_punch;
                  break;
              }
              line [ lng ] = '\0';
              scr_strlngth ( line , & xdsp ) ;
              Check_dist;
          }
          if ( ( xdsp > ( lnlng * 1.01 ) ) && ( lng != oldlng ) ) {
              check_too_much;
          }
          line [ lng ] = '\0';
      }
      if ( entry == 1 ) {
          if ( xdsp < lnlng - 2. ) fprintf ( ot , "$l\n" ) ;
          check_dolor;
          fprintf ( ot , "%s\n" , line ) ;
          } else {
          if ( xdsp < lnlng - 2. ) fprintf ( ot , "$l\n" ) ;
          check_dolor;
          list_align_print_one;
      }
      fprintf ( ot , "$o%-d\n" , ( int ) ( ofs * 10 + 0.5 ) ) ;
  }
/*
   Making dot aligned text is not tested very well
   just implemented without proper analysis
   it may work!!! Let us pray
*/
  static int make_dot_adjusted_line ( FILE * cfile , char *ln , int ofs , int rmg ) 
      {
      int no , level , i , rmg1;
      float wd , dsp , rln;
    /*  txtsize((txth)*2.81/12.,(txtw)*2.81/12.,txtg); */
      wd = kgStringLength ( Img , " . . . . . . . . . . . . . . . . . . . ." ) ;
      wd /= 20.;
      i = 0;
      while ( ln [ i ] >= ' ' ) {
          if ( ( ln [ i ] == '|' ) && ( ln [ i + 1 ] == '|' ) ) {
              ln [ i++ ] = '\0';
              ln [ i ] = ' ';
              break;
          }
          i++;
      }
      dsp = kgStringLength ( Img , ln ) ;
      rln = kgStringLength ( Img , ln + i ) ;
      no = ( rmg - ofs - dsp - rln ) / ( wd ) + 0.5;
      rmg1 = ( int ) ( rln / wd + 0.5 ) * wd;
      rmg1 = rmg - rmg1;
      fprintf ( cfile , "$l\n$A0\n%s\n$m%-d\n$r\n$A0\n" , ln , rmg1 ) ;
      for ( level = 1; level < no; level++ ) {
          fprintf ( cfile , " ." ) ;
      }
      fprintf ( cfile , "\n$m%-d\n%s" , rmg , ln + i ) ;
      fprintf ( cfile , "$l\n" ) ;
      return 1;
  }
  static int mkdotalign ( File * fp , FILE * ot , int ofs , int rmg ) {
      while ( Get_line ( buf , fp ) > 0 ) {
          if ( ( buf [ 0 ] == '$' ) ) {
              if ( ( buf [ 1 ] == 'S' ) && ( buf [ 2 ] == 'E' ) ) return 1;
              else {
                  printf ( "ERROR:Wrong Command inside Dot aligned block\n" ) ;
                  exit ( 0 ) ;
              }
          }
          make_dot_adjusted_line ( ot , buf , ofs , rmg ) ;
      }
      return 1;
  }
  static int scanint ( char *ch ) {
      int v = 0;
      int i = 0 , sign = 1;
      while ( * ( ch + i ) == ' ' ) i++;
      if ( * ( ch + i ) == '-' ) {
          sign = -1;
          i++;
      };
      while ( ( * ( ch + i ) >= '0' ) && ( * ( ch + i ) <= '9' ) ) {
          v = v * 10 + * ( ch + i ) - '0';
          i++;
      }
      return ( v * sign ) ;
  }
  static void copy_string ( char *s , char *d ) {
      int i = 0;
      while ( * ( s + i ) != '\0' ) {
          * ( d + i ) = * ( s + i ) ;
          i++;
      }
      * ( d + i ) = '\0';
  }
  static void cp ( char *inf , char *outf ) {
      int in , out , bytes;
      char buf [ 1024 ] ;
      in = open ( inf , O_RDONLY , S_IREAD | S_IWRITE ) ;
      if ( in <= 0 ) return;
      out = open ( outf , O_RDWR | O_TRUNC | O_CREAT , S_IREAD | S_IWRITE ) ;
      while ( ( bytes = read ( in , buf , 1024 ) ) == 1024 ) write 
          ( out , buf , 1024 ) ;
      write ( out , buf , bytes ) ;
      close ( in ) ;
      close ( out ) ;
  }
  static void append ( char *inf , char *outf ) {
      int in , out , bytes;
      char buf [ 1024 ] ;
      in = open ( inf , O_RDONLY , S_IREAD | S_IWRITE ) ;
      if ( in <= 0 ) return;
      out = open ( outf , O_RDWR | O_CREAT | O_APPEND , S_IREAD | S_IWRITE ) ;
      while ( ( bytes = read ( in , buf , 1024 ) ) == 1024 ) write 
          ( out , buf , 1024 ) ;
      write ( out , buf , bytes ) ;
      close ( in ) ;
      close ( out ) ;
  }
  static void add_notes ( FILE * ot , char *outf , int Shft ) {
      int bytes = 470 , lmar , ofs1 , ofs2;
      FILE *in;
      char buf [ 500 ] ;
      in = fopen ( outf , "r" ) ;
      if ( in == NULL ) return;
      lmar = L_mar + Shft;
      ofs1 = lmar;
      fprintf ( ot , "$h12\n$w12\n$s18\n$a\n" ) ;
      fprintf ( ot , "\n$o%-d\n" , ( int ) ( lmar * 10 + 0.5 ) ) ;
      while ( fgets ( buf , bytes , in ) != NULL ) {
          if ( buf [ 0 ] == '$' ) {
              switch ( buf [ 1 ] ) {
                  case 'o':
                  ofs2 = scanint ( ( char * ) buf + 2 ) ;
                  fprintf ( ot , "$o%-d\n" , ( lmar * 10 + ofs2 ) ) ;
                  break;
                  default:
                  fprintf ( ot , "%s" , buf ) ;
                  break;
              }
          } else fprintf ( ot , "%s" , buf ) ;
      }
      fclose ( in ) ;
  }
  static void add_refns ( FILE * ot , char *outf , int ref , int lines ) {
      int bytes = 470 , lmar , ofs1 , ofs2 , entry = 0 , k , ln = 0 , lnchk;
      char token [ 6 ] ;
      FILE *in;
      char buf [ 500 ] ;
      in = fopen ( outf , "r" ) ;
      if ( in == NULL ) return;
      lmar = L_mar;
      ofs1 = lmar;
    /*fprintf(ot,"$h12\n$w12\n$s24\n"); */
      fprintf ( ot , "\n$o%-d\n" , ( int ) ( lmar * 10 + 0.5 ) ) ;
      if ( lines < 4 ) fprintf ( ot , "$RL%-d\n" , lines ) ;
      else fprintf ( ot , "$RL2\n" ) ;
      lnchk = lines - 2;
      while ( fgets ( buf , bytes , in ) != NULL ) {
          if ( buf [ 0 ] == '$' ) {
              switch ( buf [ 1 ] ) {
                  case 'o':
                  ofs2 = scanint ( ( char * ) buf + 2 ) ;
                  fprintf ( ot , "$o%-d\n" , ( lmar * 10 + ofs2 ) ) ;
                  break;
                  default:
                  fprintf ( ot , "%s" , buf ) ;
                  break;
              }
              } else {
              if ( entry == 0 ) {
                  sprintf ( token , "%3d.\0" , ref ) ;
                  k = 0;
                  while ( token [ k ] != '\0' ) k++;
                  k--;
                  while ( token [ k ] == ' ' ) k--;
                  token [ ++k ] = '\0';
                  k = 0;
                  while ( token [ k ] != '\0' ) {
                      buf [ k ] = token [ k ] ;
                      k++;
                  }
              }
              ln++;
              if ( ( ln > 2 ) && ( ln == lnchk ) ) fprintf ( ot , "$RL2\n" ) ;
              fprintf ( ot , "%s" , buf ) ;
              entry = 1;
          }
      }
      fclose ( in ) ;
  }
  static int get_block ( FILE * fp ) {
      char buf [ 500 ] ;
      FILE *tmp;
      static char code = 'l';
      static int h = 12 , w = 12 , g = 1 , s = 22;
      tmp = fopen ( TEMP_FILE , "w" ) ;
      if ( fgets ( buf , 490 , fp ) == NULL ) {
          fclose ( tmp ) ;
          return ( 0 ) ;
      }
      fprintf ( tmp , "$h%-d\n$w%-d\n$g%-d\n$s%-d\n$%c\n" , h , w , g , s , code ) ;
      while ( 1 ) {
          if ( buf [ 0 ] == '#' ) break;
          if ( buf [ 0 ] == '$' ) switch ( buf [ 1 ] ) {
              case 'h':
              h = scanint ( buf + 2 ) ;
              break;
              case 'w':
              w = scanint ( buf + 2 ) ;
	      TxtW = w;
              break;
              case 'g':
              g = scanint ( buf + 2 ) ;
              break;
              case 's':
              s = scanint ( buf + 2 ) ;
              break;
              case 'l':
              case 'r':
              case 'a':
              case 'c':
              code = buf [ 1 ] ;
              default:
              break;
          }
          fprintf ( tmp , "%s" , buf ) ;
          if ( fgets ( buf , 490 , fp ) == NULL ) {
              break;
          }
      }
      fclose ( tmp ) ;
      return ( 1 ) ;
  }
  static void storetextsetting ( char *txt ) {
      char ch;
      ch = *txt;
      switch ( ch ) {
          case 'p':
          ipattern = scanint ( ++txt ) ;
          break;
          case 'f':
          ifill = scanint ( ++txt ) ;
          break;
          case 'b':
          ibold = scanint ( ++txt ) ;
          break;
          case 'd':
          ibodr = scanint ( ++txt ) ;
          break;
          case 'c':
          icolor = scanint ( ++txt ) ;
	  TxtClr = icolor;
          break;
          case 'k':
          ibkgr = scanint ( ++txt ) ;
          break;
          case 'a':
          iangle = scanint ( ++txt ) ;
          break;
          default:
          remove ( Z_DU_ZZ ) ;
          remove ( TEMP_FILE ) ;
          printf ( " Wrong code: %c \n" , txt [ 1 ] ) ;
          exit ( 0 ) ;
      }
  }
  static void settextsetting ( char *txt ) {
      char ch;
      ch = *txt;
      switch ( ch ) {
          case 'p':
          tpattern = scanint ( ++txt ) ;
//                txtpattern(tpattern);
          break;
          case 'f':
          tfill = scanint ( ++txt ) ;
//                txtfill(tfill);
          break;
          case 'b':
          tbold = scanint ( ++txt ) ;
//                set_txtbold(tbold);
          break;
          case 'd':
          tbodr = scanint ( ++txt ) ;
//                txtbodr(tbodr);
          break;
          case 'c':
          tcolor = scanint ( ++txt ) ;
	  TxtClr = tcolor;
          kgTextColor ( Img , tcolor ) ;
          break;
          case 'k':
          tbkgr = scanint ( ++txt ) ;
//                txtbkgr(tbkgr);
          break;
          case 'a':
          tangle = scanint ( ++txt ) ;
          kgTextAngle ( Img , ( float ) tangle ) ;
          break;
          default:
          remove ( Z_DU_ZZ ) ;
          remove ( TEMP_FILE ) ;
          printf ( " Wrong code: %c \n" , txt [ 1 ] ) ;
          exit ( 0 ) ;
      }
  }
  static void graphic_ops ( char *pt , float yshift , float yfac ) {
      char ch;
      float x1 , y1 , x2 , y2 , w , h;
      long val , fil;
      char buff [ 120 ] ;
      ch = pt [ 0 ] ;
      switch ( ch ) {
          case 'M':
          if ( sscanf ( pt + 1 , "%f %f" , & x1 , & y1 ) > 0 ) {
              G_x = ( x1 * G_sx + G_shx ) , G_y = ( y1 * G_sy + G_shy ) ;
          }
          break;
          case 'D':
          if ( sscanf ( pt + 1 , "%f %f" , & x2 , & y2 ) > 0 ) {
              kgMove2f ( Img , G_x , G_y ) ;
              x2 = ( x2 * G_sx + G_shx ) , y2 = ( y2 * G_sy + G_shy ) ;
              kgDraw2f ( Img , x2 , y2 ) ;
              G_x = x2 , G_y = y2;
          }
          break;
          case 'I':
          if ( sscanf ( pt + 1 , "%s%f%f%f" , buf+2 , & x1 , & w , & h ) > 0 ) {
              buf [ 0 ] = '#' , buf [ 1 ] = '#';
//            printf("%s : %f %f %f %f\n",buf,x1,y1,x2,y2);
              kgDrawImage ( Img , buf , x1 , yshift-h , x1+w , yshift ) ;
          }
          break;
          case 'L':
          if ( sscanf ( pt + 1 , "%f %f %f %f" , & x1 , & y1 , & x2 , & y2 ) > 0 ) {
              x1 = ( x1 * G_sx + G_shx ) , y1 = ( y1 * G_sy + G_shy ) ;
              x2 = ( x2 * G_sx + G_shx ) , y2 = ( y2 * G_sy + G_shy ) ;
              kgMove2f ( Img , x1 , y1 ) ;
              kgDraw2f ( Img , x2 , y2 ) ;
              G_x = x2 , G_y = y2;
          }
          break;
          case 'R':
          if ( sscanf ( pt + 1 , "%f %f %f %f %ld" , & x1 , & y1 , & x2 , & y2 , & fil ) > 0 ) 
              {
              x1 = ( x1 * G_sx + G_shx ) , y1 = ( y1 * G_sy + G_shy ) ;
              x2 = ( x2 * G_sx + G_shx ) , y2 = ( y2 * G_sy + G_shy ) ;
//              box_fill(x1,y1,x2,y2,fil,0);
          }
          break;
          case 'Y':
          if ( sscanf ( pt + 1 , "%ld" , & val ) > 0 ) {
              kgLineStyle ( Img , val ) ;
          }
          break;
          case 'W':
          if ( sscanf ( pt + 1 , "%ld" , & val ) > 0 ) {
              kgLineWidth ( Img , val ) ;
          }
          break;
          case 'F':
          sscanf ( pt + 1 , "%f %f" , & G_sx , & G_sy ) ;
          break;
          case 'S':
          sscanf ( pt + 1 , "%f %f" , & G_shx , & G_shy ) ;
          break;
          case 'C':
          G_shy = yshift + yfac;
          G_sy = 0.25 * yfac;
          break;
          default:
          break;
      }
  }
  static void read_txt ( char *flname , float ofs , float yy ) {
      char *buf;
      static char *txt;
      int ch;
      FILE *f15;
      float fac , space , hfac , wfac , gfac , sfac , ww , wg , rmg , xx , xdsp , gap , xlngth , blngth;
          
      float rmgc;
      float yybgn , YY , u_offset , Brk_yy;
      short icent , i , j , k , iofs , n , ngp , ipr , uline , ifac;
      short Ad_vn = 1 , colnum = 1;
      txt = Buf;
      l1:
      stripblnk ( ( char * ) flname ) ;
      f15 = fopen ( flname , "r" ) ;
      if ( f15 == NULL ) { /* printf ( "Error:\n" ) ; */
          goto l1100;
      };
      space = 2.0;
      icent = 0;
      buf = & ( txt [ 2 ] ) ;
      kgTextFont ( Img , tfnt ) ;
      kgLineColor ( Img , 15 ) ;
      kgTextColor ( Img , tcolor ) ;
      kgTextAngle ( Img , 0. ) ;
      G_shx = 0.;
      YY = -1000.;
      hfac = 1.0;
      wfac = 1.0;
      gfac = 1.0;
      sfac = 1.0;
      kgTextSize ( Img , th * hfac , tw * wfac , tg * gfac ) ;
      yybgn = 174. - sfac * space * th - Top_skip;
      yy = yy - sfac * space * th;
      ww = tw * wfac;
      wg = tg * gfac;
      rmg = right_margin;
      xx = 0.;
      Brk_yy = yy;
      l10:
      i = 0;
      ch = fgetc ( f15 ) ;
      while ( ( ch != EOF ) && ( ch != '\n' ) ) {
          txt [ i ] = ch;
          ch = fgetc ( f15 ) ;
          i++;
      }
      if ( ch == EOF ) goto l1100;
      if ( i == 0 ) txt [ i++ ] = ' ';
      while ( ( txt [ i - 1 ] == ' ' ) && ( i > 1 ) ) i--;
      txt [ i ] = '\0';
      if ( ( ch != EOF ) && ( i > 0 ) ) {
          if ( txt [ 0 ] == '$' ) {
              switch ( txt [ 1 ] ) {
                  case 'h':
                  ifac = scanint ( ( char * ) & txt [ 2 ] ) ;
                  hfac = ( ( float ) ifac ) / 12.0;
                  break;
                  case 'w':
                  ifac = scanint ( ( char * ) & txt [ 2 ] ) ;
	          TxtW = ifac;
                  wfac = ( ( float ) ifac ) / 12.0;
                  break;
                  case 'g':
                  ifac = scanint ( ( char * ) & txt [ 2 ] ) ;
                  gfac = ( ( float ) ifac ) ;
                  break;
                  case 's':
                  ifac = scanint ( ( char * ) & txt [ 2 ] ) ;
                  sfac = ( ( float ) ifac ) / 24.0;
                  break;
                  case 'c':
                  icent = 1;
                  break;
                  case 'l':
                  icent = 0;
                  break;
                  case 'r':
                  icent = 2;
                  break;
                  case 'a':
                  if ( Rjust ) icent = 3;
                  else icent = 0;
                  break;
                  case 'C':
                  G_shx = ( float ) scanint ( ( char * ) & txt [ 2 ] ) ;
                  break;
                  case 'M':
                  sscanf ( & txt [ 2 ] , "%f" , & YY ) ;
                  break;
                  case 'X':
                  Brk_yy = yy;
                  break;
                  case 'Y':
                  yy = Brk_yy;
                  break;
                  case 'D':
                  kgMove2f ( Img , ofs , YY ) ;
                  kgDraw2f ( Img , rmg , YY ) ;
                  YY = -1000.;
                  break;
                  case 'R':
                  switch ( txt [ 2 ] ) {
                      case 'G':
                      break;
                      case 'S':
                      break;
                      case 'C':
                      break;
                      default:
                      break;
                  }
                  break;
                  case 'b':
                  break;
                  case 'n':
                  break;
                  case 'f':
                  tfnt = scanint ( ( char * ) & txt [ 2 ] ) ;
                  kgTextFont ( Img , ( long ) tfnt ) ;
                  break;
                  case 'i':
                  break;
                  case 'U':
                  tuline = scanint ( ( char * ) & txt [ 2 ] ) ;
                  if ( tuline == 0 ) kgLineWidth ( Img , 0L ) ;
                  else kgLineWidth ( Img , 2L ) ;
                  break;
                  case 'p':
                  ipr = txt [ 2 ] - '0';
                  break;
                  case 't':
                  settextsetting ( ( char * ) & txt [ 2 ] ) ;
                  break;
                  case 'o':
                  ofs = scanint ( ( char * ) & txt [ 2 ] ) / 10.0;
                  break;
                  case 'm':
                  rmg = scanint ( ( char * ) & txt [ 2 ] ) ;
                  break;
                  case 'P':
                  break;
                  case 'A':
                  Ad_vn = scanint ( ( char * ) & txt [ 2 ] ) ;
                  break;
                  case 'G':
                  graphic_ops ( ( char * ) & txt [ 2 ] , yy , sfac * space * th ) ;
                  break;
                  default:
                  remove ( Z_DU_ZZ ) ;
                  remove ( TEMP_FILE ) ;
                  printf ( " Wrong code: %c \n" , txt [ 1 ] ) ;
                  exit ( 0 ) ;
              }
              kgTextSize ( Img , th * hfac , tw * wfac , tg * gfac ) ;
              ww = tw * wfac;
              wg = tg * gfac;
              } else {
              kgTextSize ( Img , th * hfac , tw * wfac , tg * gfac ) ;
              u_offset = th * hfac * 0.2;
              xdsp = 0.;
              if ( icent != 0 ) {
                  ngp = strchars ( txt ) ;
                  xdsp = kgStringLength ( Img , txt ) ;
                  xdsp = ( rmg - ofs - xdsp ) * 0.5;
              };
              if ( icent == 2 ) xdsp = 2.0 * xdsp;
              if ( icent == 3 ) {
                  xdsp = 2.0 * xdsp;
                  gap = tg * gfac;
                  if ( ngp >= 1 ) gap = gap + ( xdsp / ( ngp ) ) ;
                  kgTextSize ( Img , th * hfac , tw * wfac , gap ) ;
                  xdsp = 0;
              };
              if ( YY > -127. ) {
                  kgMove2f ( Img , xdsp + ofs , YY ) ;
                  YY = -1000.;
                  } else {
                  switch ( tuline ) {
                      case 0:
                      break;
                      case 3:
                      kgLineStyle ( Img , 1 ) ;
                      case 2:
                      kgMove2f ( Img , ofs , yy - 3 * u_offset ) ;
                      kgDraw2f ( Img , rmg , yy - 3 * u_offset ) ;
                      kgLineStyle ( Img , 0 ) ;
                      break;
                      default:
                      kgMove2f ( Img , xdsp + ofs , yy - 2 * u_offset ) ;
                      xlngth = kgStringLength ( Img , txt ) ;
                      blngth = kgStringLength ( Img , " " ) ;
                      if ( xlngth > blngth ) kgDraw2f ( Img , xdsp + ofs + xlngth , yy - 2 * u_offset ) ;
                          
                      break;
                  }
                  kgMove2f ( Img , xdsp + ofs , yy ) ;
                  yy = yy - sfac * space * th * Ad_vn;
                  Ad_vn = 1;
              }
              kgWriteText ( Img , txt ) ;
          };
      }
      Yend = yy;
      goto l10;
      l1100:
      if ( f15 != NULL ) fclose ( f15 ) ;
      return;
  }
  static void readhdr ( char code , char *str ) {
      int i = 0;
      switch ( code ) {
          case 'l':
/*
             sscanf(str,"%s\0",leftstr);
*/
          i = 0;
          while ( ( str [ i ] >= ' ' ) && ( i < 49 ) ) {
              leftstr [ i ] = str [ i ] ;
              i++;
          }
          leftstr [ i ] = '\0';
          break;
          case 'r':
/*
             sscanf(str,"%s\0",rightstr);
*/
          i = 0;
          while ( ( str [ i ] >= ' ' ) && ( i < 49 ) ) {
              rightstr [ i ] = str [ i ] ;
              i++;
          }
          rightstr [ i ] = '\0';
          break;
          case 'd':
          tline = scanint ( ( char * ) str ) ;
          break;
/*
      case 't':
             linepos=151.0,headpos=152.5;
             numpos=1;
             break;
      case 'b':
             linepos=-110.0,headpos=-115.5;
             numpos=0;
             break;
*/
      }
  }
  static void readftr ( char code , char *str ) {
      int i = 0;
      switch ( code ) {
          case 'l':
          i = 0;
          while ( ( str [ i ] >= ' ' ) && ( i < 49 ) ) {
              bleftstr [ i ] = str [ i ] ;
              i++;
          }
          bleftstr [ i ] = '\0';
          break;
          case 'r':
          i = 0;
          while ( ( str [ i ] >= ' ' ) && ( i < 49 ) ) {
              brightstr [ i ] = str [ i ] ;
              i++;
          }
          brightstr [ i ] = '\0';
          break;
          case 'd':
          bline = scanint ( ( char * ) str ) ;
          break;
      }
  }
  static void readnum ( char code , char *str ) {
      char subcode;
      int ofst;
      switch ( code ) {
          case 'a':
          numtype = 1;
          break;
          case 'r':
          numtype = 2;
          break;
          case 'n':
          numtype = 0;
          break;
          case 'P':
          ofst = scanint ( str ) ;
          theadpos = 170 - ofst;
          tlinepos = theadpos - 1.5;
          bheadpos = -127.0 + ofst;
          blinepos = bheadpos + 5.5;
          if ( pagepos > 0. ) pagepos = theadpos;
          else pagepos = bheadpos;
          break;
          case 'T':
          numtop = 1;
          pagepos = theadpos;
          subcode = *str;
          switch ( subcode ) {
              case 'l':
              numpos = 11;
              break;
              case 'r':
              numpos = 13;
              break;
              case 'm':
              numpos = 12;
              break;
              case 'o':
              numpos = 14;
              break;
              case 'e':
              numpos = 15;
              break;
          }
          break;
          case 'B':
          numtop = 0;
          pagepos = bheadpos;
          subcode = *str;
          switch ( subcode ) {
              case 'l':
              numpos = 1;
              break;
              case 'r':
              numpos = 3;
              break;
              case 'm':
              numpos = 2;
              break;
              case 'o':
              numpos = 4;
              break;
              case 'e':
              numpos = 5;
              break;
          }
          break;
/*
      case 'l':
             numline=scanint((char *)str);
             break;
*/
          case 'p':
          ipge = scanint ( ( char * ) str ) ;
          break;
          case 's':
          pgnum = 1;
          break;
          case 'o':
          pgofs = scanint ( ( char * ) str ) ;
          break;
      }
  }
  static int comp_ok ( char *s , char *d ) {
      int i = 0;
      while ( i < 5 ) {
          if ( * ( s + i ) != * ( d + i ) ) return ( 0 ) ;
          i++;
      }
      return ( 1 ) ;
  }
  static int get_ref ( char *buf , char *token ) {
      int i = 0 , j = 0 , k = 0;
      char *tk;
      R_B *pt;
      pt = First_ref;
      if ( pt == NULL ) {
          printf ( "Ref. not given..\n" ) ;
          * ( buf + 10 ) = '\0';
          printf ( "%s...\n" , buf ) ;
          exit ( 0 ) ;
      }
      j = 0;
      if ( buf [ j ] != '/' ) {
          printf ( "Wrong Ref...\n" ) ;
          * ( buf + 30 ) = '\0';
          printf ( "%s...\n" , buf ) ;
          exit ( 0 ) ;
      }
      j++;
      while ( ( j < 6 ) && ( buf [ j ] != '/' ) ) {
          token [ k++ ] = buf [ j++ ] ;
      }
      if ( buf [ j ] != '/' ) {
          printf ( "Wrong Ref...\n" ) ;
          * ( buf + 30 ) = '\0';
          printf ( "%s...\n" , buf ) ;
          exit ( 0 ) ;
      }
      j++;
      while ( k < 5 ) token [ k++ ] = ' ';
      while ( pt != NULL ) {
          i++;
          tk = pt-> token;
          if ( comp_ok ( tk , token ) ) {
              sprintf ( token , "%-d\0" , i ) ;
              k = 0;
              while ( token [ k ] != '\0' ) k++;
              k--;
              while ( token [ k ] == ' ' ) {
                  token [ k ] = '\0';
                  k--;
              };
              return ( j ) ;
              } else {
              pt = ( R_B * ) pt-> adr;
          }
      }
      printf ( "Wrong Ref...\n" ) ;
      * ( buf + 20 ) = '\0';
      printf ( "No such Ref: %s...\n" , buf ) ;
      exit ( 0 ) ;
  }
  static long ref_code ( char *t ) {
      int i = 0;
      long code = 0;
      unsigned char ch;
      for ( i = 0; i < 5; i++ ) {
          ch = * ( t + i ) ;
          if ( ch >= 'a' ) ch = ch - 'a' + 'A';
          ch -= 32;
          code = code * 100 + ch;
      }
      return ( code ) ;
  }
  static void exchange_str ( char *s , char *d , int n ) {
      char ch;
      int i;
      for ( i = 0; i < n; i++ ) {
          ch = * ( s + i ) ;
          * ( s + i ) = * ( d + i ) ;
          * ( d + i ) = ch;
      }
  }
  static void sort_ref ( void ) {
      R_B *ptr , *ptr1 , *tmp;
      long *t_ptr;
      long code , code1;
      int i = 0;
      ptr = First_ref;
      while ( ptr != NULL ) {
          tmp = ptr;
          code = ref_code ( ptr-> token ) ;
          ptr1 = ( R_B * ) ptr-> adr;
          while ( ptr1 != NULL ) {
              code1 = ref_code ( ptr1-> token ) ;
              if ( code > code1 ) {
                  code = code1;
                  tmp = ptr1;
              }
              ptr1 = ( R_B * ) ptr1-> adr;
          }
          if ( ptr != tmp ) {
              int ln;
              exchange_str ( ptr-> fn , tmp-> fn , 16 ) ;
              exchange_str ( ptr-> token , tmp-> token , 5 ) ;
              ln = ptr-> lines;
              ptr-> lines = tmp-> lines;
              tmp-> lines = ln;
          }
          ptr = ( R_B * ) ptr-> adr;
      }
  }
  static void copy_refences ( char *fl ) {
      int nchr , i , j , k;
      char token [ 6 ] ;
      FILE *tmp , *tmp1 , *tmp2;
      R_B *ptr;
      long *t_ptr;
      remove ( TEMP_FILE ) ;
      if ( First_ref == NULL ) return;
      ptr = First_ref;
      sort_ref ( ) ;
      tmp = fopen ( fl , "r" ) ;
      tmp1 = fopen ( TEMP_FILE , "w" ) ;
      if ( tmp != NULL ) {
          while ( ( fgets ( buf , 500 , tmp ) ) != NULL ) {
              nchr = 0;
              while ( buf [ nchr ] >= ' ' ) nchr++;
              i = 0 , j = 0;
              if ( ( buf [ i ] == '#' ) ) {
                  if ( buf [ i + 1 ] == 'R' ) {
                      i += get_ref ( & buf [ i + 2 ] , token ) + 2;
                      k = 0;
                      while ( token [ k ] != '\0' ) Buf [ j++ ] = token [ k++ ] ;
                      } else {
                      printf ( "Wrong #..\n" ) ;
                      buf [ 10 ] = '\0';
                      printf ( "%s...\n" , buf ) ;
                      exit ( 0 ) ;
                  }
              }
              while ( i < nchr ) {
                  if ( ( buf [ i ] == '#' ) && ( buf [ i - 1 ] != '\\' ) && 
                      ( buf [ i + 1 ] == 'R' ) ) {
                      i += get_ref ( & buf [ i + 2 ] , token ) + 2;
                      k = 0;
                      while ( token [ k ] != '\0' ) Buf [ j++ ] = token [ k++ ] ;
                  } else Buf [ j++ ] = buf [ i++ ] ;
              }
              Buf [ j ] = '\0';
              fprintf ( tmp1 , "%-s\n" , Buf ) ;
          }
          fclose ( tmp ) ;
          fclose ( tmp1 ) ;
          cp ( ( char * ) TEMP_FILE , fl ) ;
          tmp = fopen ( fl , "a" ) ;
          i = 0;
          while ( ptr != NULL ) {
              i++;
              add_refns ( tmp , ptr-> fn , i , ptr-> lines ) ;
              remove ( ptr-> fn ) ;
              ptr = ( R_B * ) ptr-> adr;
          }
          ptr = First_ref;
          while ( ptr != NULL ) {
              t_ptr = ptr-> adr;
              free ( ptr ) ;
              ptr = ( R_B * ) t_ptr;
          }
          First_ref = NULL;
          fclose ( tmp ) ;
      }
  }
  static void preprocess ( File * fp ) {
      FILE *tmp , *tmp1;
      float width , space = 2.1 , sp;
    /*char buf[200]; */
      float sfac = 1.0;
      int i , ofs = 35 , columns = 1 , lng , ifac = 1 , ihfac = 12 , iwfac = 12 , isfac = 24 , tfnt = 0;
          
      int rmg = 195 , ipr = 0;
      int rmgc;
      First_ref = NULL;
      Cur_ref = First_ref;
      width = ( tw * wfac + tg * gfac ) ;
      tmp = fopen ( DUMM_FIL , "w" ) ;
    /*while( fgets(buf,200,fp)>0){ */
      while ( Get_line ( buf , fp ) > 0 ) {
          if ( ( buf [ 0 ] == '$' ) ) {
              switch ( buf [ 1 ] ) {
                  case 'S':
                  pro_para_list_table;
                  break;
                  case 'D':
                  pro_graphic_commands;
                  break;
                  case 'M':
                  if ( buf [ 2 ] == 'C' ) Mk_con = 1;
                  break;
                  default:
                  pro_other_commands;
                  break;
              }
              } else {
              expand_tab ( buf , Buf ) ;
              fprintf ( tmp , "%s" , Buf ) ;
          }
      }
      Fclose ( fp ) ;
      fclose ( tmp ) ;
      remove ( Z_DU_ZZ ) ;
      copy_refences ( DUMM_FIL ) ;
      if ( Mk_con ) {
          scan ( ) ;
          cp ( Contents , Z_DU_ZZ ) ;
      }
      append ( DUMM_FIL , Z_DU_ZZ ) ;
      remove ( DUMM_FIL ) ;
  }
  static int Not_Blank ( char *ln ) {
      int i = 0;
      while ( ln [ i ] == ' ' ) i++;
      if ( ln [ i ] > ' ' ) return ( 1 ) ;
      return ( 0 ) ;
  }
  static void scan ( void ) {
      int ch;
      FILE *cfile , *f22;
      char ln [ 500 ] , buf [ 3 ] , set;
      short item , n , nf , ipr , iptn , iskip , np , i , err = 0 , bold , j , col_num;
          
      long stpage = 1 , leftmar , cpgno , rmgl , rmg , Finish , Od_Ev , rmgc;
      short Ad_vn , Slant_on , Bold_on , Uline_on , Oline_on;
      float fac , pl , ofs , yy , ofsl , yyl;
      float pglimit , bspace;
      float w [ 4 ] , wxl , wyl , wxu , wyu;
/*
      int txth,txtw;
      int txtg,ifac=24;
*/
      float pglimit_bk;
      short Col_shft = 0;
      int Foot_note = 0;
      int Hd_on = 0 , Hd_line = 0;
      float Brk_pt;
      int NewPage = 0;
      cfile = fopen ( Contents , "w" ) ;
//      Write_State(cfile);
      fprintf ( cfile , "$NBm\n$Nr\n" ) ;
      fprintf ( cfile , "$s%-d\n",SpFac);
      fprintf ( cfile , "$f%-d\n",Font);
      fprintf ( cfile , "$tc%-d\n",TxtClr);
      fprintf ( cfile , "$LE275\n");
      fprintf ( cfile , "$C1\n$c\n$g5\n!z32CONTENTS\n\n$l\n$g1\n" ) ;
      Set_Defaults;
      f22 = fopen ( DUMM_FIL , "r" ) ;
      stpage = 0;
      Col_shft = ( rmgl - ofsl ) + Col_gap;
      l10:
      pgnum++;
      pglimit = pglimit_bk;
      np += 1;
      pl = 0;
      col_num = 1;
      Brk_pt = pl;
      l100:
      i = 0;
      ln [ i ] = ' ';
      ch = fgetc ( f22 ) ;
      while ( ( ch != EOF ) && ( ch != '\n' ) ) {
          ln [ i ] = ch;
          ch = fgetc ( f22 ) ;
          i++;
      }
      if ( ch == EOF ) {
          Finish = 1;
          goto l150;
      }
      while ( ( ln [ i - 1 ] == ' ' ) && ( i > 0 ) ) i--;
      ln [ i ] = '\0';
      if ( ln [ 0 ] == '$' ) {
          switch ( ln [ 1 ] ) {
              case 'S':
              {
                  switch ( ln [ 2 ] ) {
                      case 'H':
                      Hd_on = 1;
                      {
                          int hdr = 0;
                          hdr = scanint ( ( char * ) & ln [ 3 ] ) ;
                          if ( hdr > 7 ) hdr = 7;
                          if ( hdr < 0 ) hdr = 0;
                          Hd_level = hdr;
                          hdr++;
                          hdr *= 2;
                          fac = ( ( float ) ( ifac + hdr ) ) / 24.0;
                      }
                      break;
                      case 'E':
                      Hd_on = 0;
                      Hd_line = 0;
                      fac = ( ( float ) ( ifac ) ) / 24.0;
                      default:
                      break;
                  }
              }
              goto l100;
              case 'P':
              if ( pl != 0. ) {
                  if ( col_num == Columns ) goto l150;
                  else {
                      pglimit = pl - fac * Ad_vn * 0.8;
                      pl = Brk_pt;
                      col_num++;
                  }
              }
              goto l100;
              case 'L':
              switch ( ln [ 2 ] ) {
                  case 'E':
                  pglimit = ( float ) scanint ( ( char * ) & ln [ 3 ] ) ;
                  pglimit -= Top_skip;
                  pglimit = ( pglimit / 5.62 ) ;
                  pglimit_bk = pglimit;
                  goto l100;
                  case 'S':
                  pglimit = ( pglimit * 5.62 ) + Top_skip;
                  Top_skip = scanint ( ( char * ) & ln [ 3 ] ) ;
                  pglimit -= Top_skip;
                  pglimit = ( pglimit / 5.62 ) ;
                  pglimit_bk = pglimit;
                  default:
                  goto l100;
                  break;
              }
              break;
              case 's':
              ifac = scanint ( ( char * ) & ln [ 2 ] ) ;
              fac = ( ( float ) ifac ) / 24.0;
              break;
              case 'W':
              Col_gap = scanint ( ( char * ) & ln [ 2 ] ) ;
              Col_shft = ( rmgl - ofsl ) + Col_gap;
              goto l100;
              case 'C':
              if ( ln [ 2 ] == 'G' ) {
                  Col_gap = scanint ( ( char * ) & ln [ 3 ] ) ;
                  Col_shft = ( rmgl - ofsl ) + Col_gap;
              }
              Columns = scanint ( ( char * ) & ln [ 2 ] ) ;
              goto l100;
              break;
              case 'N':
              readnum ( ln [ 2 ] , & ln [ 3 ] ) ;
              goto l100;
              case 'A':
              Ad_vn = scanint ( ( char * ) & ln [ 2 ] ) ;
              break;
              case 'X':
              Brk_pt = pl;
              break;
              case 'Y':
              pl = Brk_pt;
              break;
              case 'R':
              switch ( ln [ 2 ] ) {
                  case 'F':
                  {
                      int note , lines;
                      note = scanint ( ( char * ) ( & ln [ 3 ] ) ) ;
                      lines = scanint ( ( char * ) ( & ln [ 7 ] ) ) ;
                      pglimit -= ( lines ) * 0.75;
                      if ( Foot_note == 0 ) pglimit -= 0.75;
                  }
                  goto l100;
                  break;
                  case 'K':
                  {
                      int lines;
                      float lngth;
                      lines = scanint ( ( char * ) ( & ln [ 3 ] ) ) ;
                      lngth = ( lines ) * 0.75;
                      if ( Foot_note == 0 ) lngth += 0.75;
                      if ( pl + lngth > pglimit ) pglimit -= lngth;
                  }
                  goto l100;
                  break;
                  case 'L':
                  {
                      int lines;
                      float lngth;
                      lines = scanint ( ( char * ) ( & ln [ 3 ] ) ) ;
                      lngth = ( lines ) * fac;
                      if ( pl + lngth > pglimit ) pglimit -= lngth;
                      scan_page_limit;
                  }
                  goto l100;
                  break;
                  case 'B':
                  goto l100;
                  break;
                  default:
                  break;
              }
              break;
              case 'G':
              switch ( ln [ 2 ] ) {
                  case 'C':
                  pglimit_bk = pglimit;
                  pglimit = 1000;
                  break;
                  case 'E':
                  pglimit = pglimit_bk;
                  break;
                  default:
                  break;
              }
              break;
          }
          } else {
          if ( Hd_on ) copy_heading;
          if ( NewPage ) {
              if ( Not_Blank ( ln ) ) {
                  pl = pl + fac * Ad_vn;
                  NewPage = 0;
              }
              } else {
              pl = pl + fac * Ad_vn;
          }
          Ad_vn = 1;
          scan_page_limit;
      }
      goto l100;
      l150:
      if ( ! Finish ) goto l10;
      fclose ( f22 ) ;
      fprintf ( cfile , "$P\n$h12\n$w12\n$s24\n$o350\n$Nn\n$Ns\n" ) ;
      fclose ( cfile ) ;
      Dwritefile(Dreadfile(Contents),"Contents");
      if ( Con_file == 0 ) remove ( Contents ) ;
      return;
  }
  static short nonblk ( char *ln ) {
      int n = 0;
      while ( * ( ln + n ) != '\0' ) n++;
      n--;
      while ( ( n >= 0 ) && ( * ( ln + n ) == ' ' ) ) n--;
      n++;
      * ( ln + n ) = '\0';
      return ( n ) ;
  }
  static void Reset_for_tp ( float *xp , float *yp ) {
      xp [ 0 ] = 0. , xp [ 1 ] = 297.0 , yp [ 0 ] = -127. , yp [ 1 ] = 170.;
      Yend = 170;
//    kgUserFrame(Img, 0., -117., 373., 163.);
    //kgUserFrame(Img, 0., -127., 396., 170.);
      kgUserFrame ( Img , 0. , -127. , 297. , 170. ) ;
      kgTextColor ( Img , tcolor ) ;
      kgTextAngle ( Img , 0. ) ;
      kgTextSize ( Img , th , tw , tg ) ;
      kgTextFont ( Img , tfnt ) ;
      B_entry = 0;
      right_margin = 195.0;
      Bk_gr = 0 , Hd_on = 0 , Hd_line = 0 , Hd_max = 0 , Hd_level = 0 , Mk_con = 0 , Con_file = 0;
          
      Entry = 0 , Hypn = 0 , Rjust = 1;
      Columns = 1 , Lines = 0 , NOTE = 0 , REF = 0;
      F_N_no = 0 , R_N_no = 0;
      L_mar = 35 , R_margin = 195;
      G_x = 0. , G_y = 0. , G_sx = 1. , G_sy = 1. , G_shx = 0. , G_shy = 0.;
  }
  static int MakePngImage ( void *Bimg , float wxl , float wyl , float wxu , float wyu ) 
      {
      float ylow;
      float fact;
      float yup , ydown;
      ylow = wyl;
      fact = ( float ) Pyres/ ( float ) Pxres;
      if ( Yend < ylow ) ylow = Yend;
//    printf("Yend= %f wyl = %f\n",Yend,wyl);
      ydown = wyu - ( wxu-wxl ) *fact;
      yup = wyu;
      kgUserFrame ( Img , wxl , ylow , wxu , wyu ) ;
      kgBackupGph ( Img , GphFile ) ;
      Pimg = kgInitImage ( Pxres , Pyres , 4 ) ;
      kgUserFrame ( Pimg , wxl , ydown , wxu , yup ) ;
//    kgDrawImage(Pimg,"##picture1.jpg",wxl,ydown,wxu-50,yup-50);
      kgImportGphFile ( Pimg , GphFile , wxl , ylow , wxu , wyu ) ;
//    kgUserFrame(Pimg, wxl, ylow, wxu, wyu);
//    kgImportGphFile(Pimg, GphFile, wxl, ylow, wxu, wyu);
//    Png = kgGetResizedImage(Pimg);
      Png = kgGetSharpImage ( Pimg ) ;
      kgCloseImage ( Pimg ) ;
      if ( Bimg ) {
          void *tmp;
//          tmp=kgGetImageCopy(NULL,Sbkimg);
          tmp = kgCopyImage ( Bimg ) ;
          kgMergeImages ( tmp , Png , 0 , 0 ) ;
          kgFreeImage ( Png ) ;
          Png = tmp;
      }
      kgWriteImage ( Png , PngFile ) ;
      kgFreeImage ( Png ) ;
  }
  static int MakeScrollFrames ( void *Bimg , float wxl , float wyl , float wxu , float wyu ) 
      {
      int j;
      float dx , fact;
      float ylow , yup , ydown;
      char PngFrame [ 50 ] ;
      ylow = wyl;
      fact = ( float ) Syres/ ( float ) Sxres;
      if ( Yend < ylow ) ylow = Yend;
      ylow = Yend;
      if ( Frames < 0 ) Frames = 1;
      kgCleanDir ( "Frames" ) ;
      mkdir ( "Frames" , 0744 ) ;
      dx = ( wyu-ylow ) /Frames;
      kgUserFrame ( Img , wxl , ylow , wxu , wyu ) ;
      kgBackupGph ( Img , GphFile ) ;
      ydown = wyu - ( wxu-wxl ) *fact;
//    ydown = wyl;
      yup = wyu;
      for ( j = 0;j < Frames;j++ ) {
          Pimg = kgInitImage ( Sxres , Syres , 4 ) ;
          kgUserFrame ( Pimg , wxl , ydown , wxu , yup ) ;
//        if(Bkgr) kgDrawImage(Pimg,Bimg,wxl,ydown,wxu,yup);
          kgImportGphFile ( Pimg , GphFile , wxl , ylow , wxu , wyu ) ;
//       Png = kgGetResizedImage(Pimg);
          Png = kgGetSharpImage ( Pimg ) ;
          kgCloseImage ( Pimg ) ;
          if ( Bimg ) {
              void *tmp;
//          tmp=kgGetImageCopy(NULL,Sbkimg);
              tmp = kgCopyImage ( Bimg ) ;
              kgMergeImages ( tmp , Png , 0 , 0 ) ;
              kgFreeImage ( Png ) ;
              Png = tmp;
          }
          sprintf ( PngFrame , "Frames/Frame%-4.4d.png" , j+1 ) ;
          printf ( "%s\r" , PngFrame ) ;
          fflush ( stdout ) ;
          kgWriteImage ( Png , PngFrame ) ;
          kgFreeImage ( Png ) ;
          yup -= dx;
          ydown -= dx;
      }
      printf ( "\n" ) ;
  }
  static int print_process ( char *flname , int Ponly ) {
      static char mag [ 16 ] = { "            " } , pageno [ 5 ] = { "1  " };
      int ch;
      int RIGHT_MAR;
      File *f21;
      FILE *f22 , *f23 = NULL , *cfile;
      char ln [ 500 ] , buf [ 3 ] , set;
//    static char tempf[10] = { "TEMP_FILE" };
      short item , n , nf , ipr , iptn , iskip , np , i , err = 0 , bold , j , col_num;
          
      long stpage = 1 , endpage = 9999;
      short leftmar , cpgno , rmgl , rmg , rmgc , Finish , Od_Ev;
      short Ad_vn , Slant_on , Bold_on , Uline_on , Oline_on;
      float fac , pl , ofs , yy , ofsl , yyl;
      float pglimit , bspace;
      float w [ 4 ] , wxl , wyl , wxu , wyu;
/*
      int txth,txtw;
      int txtg,ifac=24;
*/
      float pglimit_bk;
      short Col_shft = 0;
      char ftnotes [ 16 ] = { "Foot_NoT.ZzZ" };
      int Foot_note = 0;
      Set_Defaults;
      stripblnk ( ( char * ) flname ) ;
      f21 = clean_up_open ( flname ) ;
    /*f22=fopen(flname,"r"); */
      pglimit_bk = pglimit;
      remove ( ftnotes ) ;
      if ( f21 != NULL ) {
          int NewPage = 0;
          kgTextSize ( Img , th , tw , tg ) ;
          preprocess ( f21 ) ;
	  /*
	   * Need a relook on the following line
	   * to check whether everything need to
	   * be re initialized
	  */
//          Reset_Defaults;
          ifac = 24;
          pgnum = 0;
          pgofs = 0;
          f22 = fopen ( Z_DU_ZZ , "r" ) ;
          if ( f22 == NULL ) {
              return ( 1 ) ;
          }
          Col_shft = ( rmgl - ofsl ) + Col_gap;
          Slant_on = 0;
          Bold_on = 0;
          Uline_on = 0;
          Oline_on = 0;
          l10:
          RIGHT_MAR = 0;
          nf = nf + 1;;
          pgnum++;
          pglimit = pglimit_bk;
          kgClearView ( Img ) ;
          kgClearBuffer ( Img ) ;
          f23 = fopen ( TEMP_FILE , "w" ) ;
          np = nf - 100;
          Foot_note = 0;
          if ( np < stpage ) iskip = 1;
	/*fprintf(f23,"\n\n\n\n"); */
          Default_state(f23);
          if ( Bk_gr ) {
              fprintf ( f23 , "$RS%-s\n" , B_file ) ;
          }
          pl = 0;
          col_num = 1;
          Brk_pt = pl;
          l100:
          i = 0;
          ln [ i ] = ' ';
          ch = fgetc ( f22 ) ;
          while ( ( ch != EOF ) && ( ch != '\n' ) ) {
              ln [ i ] = ch;
              ch = fgetc ( f22 ) ;
              i++;
          }
          if ( ch == EOF ) {
              Finish = 1;
              goto l150;
          }
          while ( ( ln [ i - 1 ] == ' ' ) && ( i > 0 ) ) i--;
          ln [ i ] = '\0';
          if ( ln [ 0 ] == '$' ) {
              switch ( ln [ 1 ] ) {
                  case 'S':
                  {
                      switch ( ln [ 2 ] ) {
                          case 'H':
                          Hd_on = 1;
                          pro_heading;
                          break;
                          case 'E':
                          Hd_on = 0;
                          Hd_line = 0;
                          fprintf ( f23 , "$l\n$h%-d\n$w%-d\n$g%-d\n$s%-d\n" , txth , txtw , txtg , ifac ) ;
                              
                          fac = ( ( float ) ifac ) / 24.0;
                          default:
                          break;
                      }
                  }
                  goto l100;
                  break;
                  case 'P':
                  if ( pl != 0. ) {
                      if ( col_num == Columns ) goto l150;
                      else {
                          pglimit = pl - fac * Ad_vn * 0.8;
                          fprintf ( f23 , "$o%-d\n" , ( int ) ( ( ofsl + Col_shft * col_num ) * 10 + 0.5 ) ) ;
                              
                          fprintf ( f23 , "$m%-d\n" , ( int ) ( rmgl + Col_shft * ( col_num ) ) ) ;
                          fprintf ( f23 , "$Y\n" ) ;
                          fprintf ( f23 , "$C%-d\n" , Col_shft * col_num ) ;
                          pl = Brk_pt;
                          col_num++;
                      }
                  }
                  goto l100;
                  break;
                  case 'L':
                  switch ( ln [ 2 ] ) {
                      case 'E':
                      pglimit = ( float ) scanint ( ( char * ) & ln [ 3 ] ) ;
                      pglimit -= Top_skip;
                      pglimit = ( pglimit / 5.62 ) ;
                      pglimit_bk = pglimit;
                      goto l100;
                      break;
                      case 'S':
                      pglimit = ( pglimit * 5.62 ) + Top_skip;
                      Top_skip = scanint ( ( char * ) & ln [ 3 ] ) ;
                      pglimit -= Top_skip;
                      pglimit = ( pglimit / 5.62 ) ;
                      pglimit_bk = pglimit;
                      default:
                      goto l100;
                      break;
                  }
                  break;
                  case 'H':
                  readhdr ( ln [ 2 ] , & ln [ 3 ] ) ;
                  goto l100;
                  break;
                  case 'B':
                  readftr ( ln [ 2 ] , & ln [ 3 ] ) ;
                  goto l100;
                  break;
                  case 'N':
                  readnum ( ln [ 2 ] , & ln [ 3 ] ) ;
                  goto l100;
                  break;
                  case 'E':
                  Od_Ev = scanint ( ( char * ) & ln [ 2 ] ) ;
                  goto l100;
                  case 's':
                  ifac = scanint ( ( char * ) & ln [ 2 ] ) ;
                  fac = ( ( float ) ifac ) / 24.0;
                  break;
                  case 'h':
                  txth = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'w':
                  txtw = scanint ( ( char * ) & ln [ 2 ] ) ;
		  TxtW = txtw;
                  break;
                  case 'g':
                  txtg = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'f':
                  ifnt = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'i':
                  islant = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'U':
                  iuline = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'o':
                  ofsl = scanint ( ( char * ) & ln [ 2 ] ) / 10.0;
                  if ( col_num > 1 ) {
                      ofsl += ( ( col_num - 1 ) * Col_shft ) ;
                      fprintf ( f23 , "$o%-d\n" , ( int ) ( ofsl * 10. + 0.5 ) ) ;
                      ofsl -= ( ( col_num - 1 ) * Col_shft ) ;
                      goto l100;
                  }
                  break;
                  case 'm':
                  rmgl = scanint ( ( char * ) & ln [ 2 ] ) ;
                  if ( RIGHT_MAR < rmgl ) RIGHT_MAR = rmgl;
                  if ( col_num > 1 ) {
                      rmgl += ( ( col_num - 1 ) * Col_shft ) ;
                      fprintf ( f23 , "$m%-d\n" , ( int ) ( rmgl ) ) ;
                      rmgl -= ( ( col_num - 1 ) * Col_shft ) ;
                      goto l100;
                  }
                  Col_shft = ( rmgl - ofsl ) + Col_gap;
                  break;
                  case 'C':
                  if ( ln [ 2 ] == 'G' ) {
                      Col_gap = scanint ( ( char * ) & ln [ 3 ] ) ;
                      Col_shft = ( rmgl - ofsl ) + Col_gap;
                  } else Columns = scanint ( ( char * ) & ln [ 2 ] ) ;
                  goto l100;
                  break;
                  case 'p':
                  ipr = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 't':
                  storetextsetting ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'b':
                  bold = 1;
                  break;
                  case 'n':
                  bold = 0;
                  break;
                  case 'r':
                  case 'l':
                  case 'c':
                  case 'a':
                  set = ln [ 1 ] ;
                  break;
                  case 'A':
                  Ad_vn = scanint ( ( char * ) & ln [ 2 ] ) ;
                  break;
                  case 'X':
                  Brk_pt = pl;
                  break;
                  case 'Y':
                  pl = Brk_pt;
                  break;
                  case 'R':
                  switch ( ln [ 2 ] ) {
                      case 'F':
                      {
                          int note , lines;
                          char fname [ 16 ] ;
                          note = scanint ( ( char * ) ( & ln [ 3 ] ) ) ;
                          lines = scanint ( ( char * ) ( & ln [ 7 ] ) ) ;
                          pglimit -= ( lines ) * 0.75;
                          if ( Foot_note == 0 ) pglimit -= 0.75;
                          Foot_note = 1;
                          sprintf ( fname , "Ft_Note.%-3.3d\0" , note ) ;
                          append ( fname , ftnotes ) ;
                          remove ( fname ) ;
                      }
                      goto l100;
                      break;
                      case 'K':
                      {
                          int lines;
                          float lngth;
                          lines = scanint ( ( char * ) ( & ln [ 3 ] ) ) ;
                          lngth = ( lines ) * 0.75;
                          if ( Foot_note == 0 ) lngth += 0.75;
                          if ( pl + lngth > pglimit ) pglimit -= lngth;
                      }
                      goto l100;
                      break;
                      case 'L':
                      {
                          int lines;
                          float lngth;
                          lines = scanint ( ( char * ) ( & ln [ 3 ] ) ) - 1;
                          lngth = ( lines ) * fac;
                          if ( pl + lngth > pglimit ) pglimit -= lngth;
                          check_page_limit;
                      }
                      goto l100;
                      break;
                      case 'B':
                      Set_background;
                      goto l100;
                      break;
                      default:
                      break;
                  }
                  break;
                  case 'G':
                  switch ( ln [ 2 ] ) {
                      case 'C':
                      pglimit_bk = pglimit;
                      pglimit = 1000;
                      break;
                      case 'E':
                      pglimit = pglimit_bk;
                      break;
                      default:
                      break;
                  }
                  break;
              }
              n = nonblk ( ln ) ;
              fprintf ( f23 , "%s\n" , ln ) ;
              } else {
              n = nonblk ( ln ) ;
              if ( Slant_on ) fprintf ( f23 , "!I" ) ;
              if ( Bold_on ) fprintf ( f23 , "!B" ) ;
              if ( Uline_on ) fprintf ( f23 , "!U" ) ;
              if ( Oline_on ) fprintf ( f23 , "!O" ) ;
              if ( NewPage ) {
                  if ( Not_Blank ( ln ) ) {
                      fprintf ( f23 , "%s\n" , ln ) ;
                      pl = pl + fac * Ad_vn;
                      NewPage = 0;
                  }
                  } else {
                  fprintf ( f23 , "%s\n" , ln ) ;
                  pl = pl + fac * Ad_vn;
              }
              check_slant_nor_bold;
              Ad_vn = 1;
              if ( RIGHT_MAR < rmgl ) RIGHT_MAR = rmgl;
              check_page_limit;
          }
          goto l100;
          l150:
          fprintf ( f23 , "$f0\n$U0\n$p1\n$h12\n$w12\n$i0\n$n\n" ) ;
          if ( Foot_note ) {
              fprintf ( f23 , "$GC\n$GL%-d -3 %-d -3\n" , ( int ) L_mar + Col_shft * 
                  ( col_num - 1 ) , ( int ) L_mar + 70 + Col_shft * ( col_num - 1 ) ) ;
              Foot_note = 0;
              add_notes ( f23 , ftnotes , ( int ) ( Col_shft * ( col_num - 1 ) ) ) ;
              remove ( ftnotes ) ;
          }
          cpgno = pgnum + pgofs;
          Set_Strings;
          Set_No_Pos;
          switch ( numtype ) {
              case 0:
              pageno [ 0 ] = '\0';
              break;
              case 1:
              sprintf ( pageno , "%4d\0" , cpgno ) ;
              break;
              case 2:
              pageno [ 0 ] = rstr [ cpgno - 1 ] [ 0 ] ;
              pageno [ 1 ] = rstr [ cpgno - 1 ] [ 1 ] ;
              pageno [ 2 ] = rstr [ cpgno - 1 ] [ 2 ] ;
              pageno [ 3 ] = rstr [ cpgno - 1 ] [ 3 ] ;
              pageno [ 4 ] = '\0';
              break;
          }
          if ( numtype == 0 ) numpos_d = 0;
          if ( ( numpos_d != 3 ) && ( numpos_d != 13 ) ) {
              i = 0;
              while ( pageno [ i ] == ' ' ) i++;
              for ( j = i; j <= 4; j++ ) pageno [ j - i ] = pageno [ j ] ;
          }
          if ( ( ipge != 0 ) ) {
              fprintf ( f23 , "$o%-d\n" , L_mar * 10 ) ;
              if ( tline ) fprintf ( f23 , "$M%6.1f\n$D\n" , tlinepos ) ;
              fprintf ( f23 , "$M%6.1f\n" , theadpos ) ;
              fprintf ( f23 , "$l\n$h12\n$w12\n$g2\n$n\n$f0\n$p1\n" ) ;
              if ( numpos_d != 11 ) fprintf ( f23 , "%s\n" , Leftstr ) ;
              else {
                  fprintf ( f23 , "%s\n" , pageno ) ;
                  fprintf ( f23 , "$M%6.1f\n" , theadpos ) ;
                  fprintf ( f23 , "     %s\n" , Leftstr ) ;
              }
              fprintf ( f23 , "$M%6.1f\n$r\n" , theadpos ) ;
              if ( numpos_d != 13 ) fprintf ( f23 , "%s\n" , Rightstr ) ;
              else fprintf ( f23 , "%s%4s\n" , Rightstr , pageno ) ;
              if ( bline ) fprintf ( f23 , "$M%6.1f\n$D\n" , blinepos ) ;
              fprintf ( f23 , "$M%6.1f\n" , bheadpos ) ;
              if ( numpos_d != 1 ) fprintf ( f23 , "$l\n%s\n" , Bleftstr ) ;
              else {
                  fprintf ( f23 , "$l\n%s\n" , pageno ) ;
                  fprintf ( f23 , "$M%6.1f\n" , bheadpos ) ;
                  fprintf ( f23 , "    %s\n" , Bleftstr ) ;
              }
              fprintf ( f23 , "$M%6.1f\n$r\n" , bheadpos ) ;
              if ( numpos_d != 3 ) fprintf ( f23 , "%s\n" , Brightstr ) ;
              else fprintf ( f23 , "%s%4s\n" , Brightstr , pageno ) ;
              fprintf ( f23 , "$M%6.1f\n" , pagepos ) ;
              if ( ( numpos_d == 2 ) || ( numpos_d == 12 ) ) fprintf 
                  ( f23 , "$c\n%s\n" , pageno ) ;
          }
          fclose ( f23 ) ;
          if ( iskip <= 0 ) {
              read_txt ( TEMP_FILE , ofs , yy - Top_skip ) ;
              iskip = 0;
              } else {
              iskip--;
          }
          ofs = ofsl;
          yy = yyl;
          rmg = rmgl;
          if ( np < stpage ) goto l10;
#if 0
          kgViewport ( Img , 0. , 0. , 1. , 1. ) ;
#endif
          kgGetWindow ( Img , w , w + 1 , w + 2 , w + 3 ) ;
//        kgHardCopy(Img,"txt.ps");
          if ( RIGHT_MAR < 220 ) {
//        kgHardCopy(Img,"txt.ps");
              wxl = w [ 0 ] , wyl = w [ 1 ] ;
//`	    wxu = wxl + (w[2] - w[0]) * 0.5625;
//	    wxu = wxl + (w[2] - w[0]) * 0.5303;
              wxu = wxl + ( w [ 2 ] - w [ 0 ] ) * 0.70707;
              wyu = w [ 3 ] ;
              if ( PsOut == 1 ) {
                  kgUserFrame ( Img , wxl , wyl , wxu , wyu ) ;
                  remove ( OutFile ) ;
                  kgA4Copy ( Img , OutFile ) ;
                  sprintf ( ln , "cat %s >> %s" , OutFile , PsFile ) ;
                  system ( ln ) ;
              }
              } else {
              wxl = w [ 0 ] , wxu = w [ 2 ] ;
              wyl = w [ 3 ] - ( w [ 3 ] -w [ 1 ] ) *0.70707;
              wyu = w [ 3 ] ;
              if ( PsOut ) {
                  kgUserFrame ( Img , wxl , wyl , wxu , wyu ) ;
                  remove ( OutFile ) ;
                  kgLandscapeCopy ( Img , OutFile ) ;
                  sprintf ( ln , "cat %s >> %s" , OutFile , PsFile ) ;
                  system ( ln ) ;
              }
          }
          if ( PngOut ) {
              MakePngImage ( Pbkimg , wxl , wyl , wxu , wyu ) ;
          }
          if ( Scroll ) {
              MakeScrollFrames ( Sbkimg , wxl , wyl , wxu , wyu ) ;
          }
          kgUserFrame ( Img , w [ 0 ] , w [ 1 ] , w [ 2 ] , w [ 3 ] ) ;
          if ( ! Finish && ( np < endpage ) ) goto l10;
          else {
              remove ( Z_DU_ZZ ) ;
              remove ( TEMP_FILE ) ;
              return ( 0 ) ;
          }
      }
  }
  static void get_cur_dir ( char *path , char *flname ) {
      int i = 0 , l;
      char ch , dir [ 100 ] ;
      l = strlen ( path ) ;
      i = l - 1;
      while ( ( i >= 0 ) && ( path [ i ] != '/' ) ) i--;
      if ( i <= 0 ) strcpy ( dir , "./" ) ;
      else {
          ch = path [ i ] ;
          path [ i ] = '\0';
          strcpy ( dir , path ) ;
          path [ i ] = ch;
      }
      strcpy ( flname , path + i + 1 ) ;
      chdir ( dir ) ;
      return;
  }
  static int PrintUsage ( char *name ) {
      fprintf ( stderr , "Usage :\n" "%-s [options] <input file> \n" "Options:\n" " -o[PsFile] : create postscript output\n" "               default output output.ps\n" " -p<[xresxyres:]png_file> : create Png image\n" "         xres Image resolution x direction\n" "              default 1376\n" "         yres Image resolution y direction\n" "              default 768\n" " -s<[xresxyres]:no_of_frames> : create Scroll Images\n" "         xres Image resolution x direction\n" "              default 1376\n" "         yres Image resolution y direction\n" "              default 768\n" "         no_of_frames  no of required frames\n" "                       ie FPSxseconds, default 300\n" "            png frames are created under 'FRAMES'\n" " -k<background_image> : Image file for background\n" "                        if required\n" , name ) ;
          
      fprintf ( stderr , "Note :\n" " when using -s or -p options, make sure that \n" " it is a single page text, one can use \n" " the $LE<length> command to set a large value,\n" " if needed\n" ) ;
          
      return 1;
  }
  static void ProcessTextDoc ( int argc , char *argv [ ] ) {
      int i , j , INPUT;
      static float xp [ 2 ] = { 0. , 297. } , yp [ 2 ] = { -127.0 , 170.0 };
      char ch , flname [ 30 ] , *pt , ch1;
      char Line [ 300 ] ;
      if ( TmpDir == NULL ) {
          TmpDir = kgMakeTmpDir ( ) ;
          sprintf ( DUMM_FIL , "%-s/DUMM_FIL" , TmpDir ) ;
          sprintf ( Z_DU_ZZ , "%-s/Z_DU_ZZ" , TmpDir ) ;
          sprintf ( TEMP_FILE , "%-s/TEMP_FILE" , TmpDir ) ;
          sprintf ( Contents , "%-s/Contents" , TmpDir ) ;
          sprintf ( OutFile , "%-s/OutFile" , TmpDir ) ;
          sprintf ( GphFile , "%-s/GphFile" , TmpDir ) ;
      }
      if ( buf == NULL ) {
          buf = ( char * ) malloc ( sizeof ( char ) * 50000L ) ;
          if ( buf == NULL ) {
              printf ( " Memory allocation error \n" ) ;
              exit ( 0 ) ;
          }
      }
      Buf = buf + 40000L;
      tw = 2.81;
      th = 2.81;
      tg = 2.81 / 12.0;
      ifnt = TX_FONT;
      islant = 0;
      iuline = 0;
      tpattern = 15;
      tbodr = 1;
      tfill = 1;
      tbold = 1;
      tcolor = TX_CLR;
      tangle = 0.0;
      tbkgr = 0;
      Scroll = 0;
      PngOut = 0;
      PsOut = 0;
      Bkgr = 0;
      Pbkimg = NULL;
      Sbkimg = NULL;
      Sxres = 1376;
      Syres = 768;
      Frames = 300;
      strcpy ( PsFile , "output.ps" ) ;
      if ( argc == 1 ) {
          PrintUsage ( argv [ 0 ] ) ;
          return ;
      }
      INPUT = 0;
      i = 1;
      while ( i < argc ) {
          ch = *argv [ i ] ;
          if ( ch == '-' ) {
              if ( * ( argv [ i ] + 1 ) == 'p' ) {
                  pt = argv [ i ] + 2;
                  if ( *pt <= ' ' ) {
                      i++;
                      if ( i >= argc ) break;
                      pt = argv [ i ] ;
                  }
                  strcpy ( PngFile , pt ) ;
//		if (PngFile[0] == '\0') strcpy(PngFile, "Page.png");
                  Pon = 1;
                  PngOut = 1;
                  strcpy ( Line , pt ) ;
                  j = 0;
                  while ( Line [ j ] != '\0' ) {
                      if ( ( Line [ j ] == 'x' ) || ( Line [ j ] == 'X' ) ) Line [ j ] = ' ';
                      if ( Line [ j ] == ':' ) {
                          Line [ j ] = '\0';
                          strcpy ( PngFile , Line+j+1 ) ;
                      }
                      j++;
                  }
                  if ( Line [ 0 ] > ' ' ) sscanf ( Line , "%d%d" , & Pxres , & Pyres ) ;
              }
              if ( * ( argv [ i ] + 1 ) == 'o' ) {
                  pt = argv [ i ] + 2;
                  if ( *pt <= ' ' ) {
                      i++;
                      if ( i >= argc ) break;
                      pt = argv [ i ] ;
                  }
                  strcpy ( PsFile , pt ) ;
                  if ( PsFile [ 0 ] == '\0' ) strcpy ( PsFile , "output.ps" ) ;
                  Pon = 2;
                  PsOut = 1;
              }
              if ( * ( argv [ i ] + 1 ) == 's' ) {
             /* Create Scroll frames */
                  pt = argv [ i ] + 2;
                  if ( *pt <= ' ' ) {
                      i++;
                      if ( i >= argc ) break;
                      pt = argv [ i ] ;
                  }
                  Scroll = 1;
                  strcpy ( Line , pt ) ;
                  j = 0;
                  while ( Line [ j ] != '\0' ) {
                      if ( ( Line [ j ] == 'x' ) || ( Line [ j ] == 'X' ) 
                          || ( Line [ j ] == ':' ) ) Line [ j ] = ' ';
                      j++;
                  }
                  if ( Line [ 0 ] > ' ' ) sscanf ( Line , "%d%d%d" , & Sxres , & Syres , & Frames ) ;
                      
              }
              if ( * ( argv [ i ] + 1 ) == 'k' ) {
                  pt = argv [ i ] + 2;
                  if ( *pt <= ' ' ) {
                      i++;
                      if ( i >= argc ) break;
                      pt = argv [ i ] ;
                  }
                  strcpy ( Line , pt ) ;
                  if ( Line [ 0 ] > ' ' ) {
                      strcpy ( Bimg , "##" ) ;
                      strcat ( Bimg , Line ) ;
                      printf ( "Bimg = %s\n" , Bimg ) ;
                      Bkgr = 1;
                  }
              }
              if ( * ( argv [ i ] + 1 ) == 'c' ) {
                  BW = 0;
              }
              if ( * ( argv [ i ] + 1 ) == 'h' ) {
                  PrintUsage ( argv [ 0 ] ) ;
                  return ;
              }
          }
	/*else strcpy(flname,argv[i]); */
          else{
              get_cur_dir ( argv [ i ] , flname ) ;
              INPUT = 1;
          }
          i++;
      }
      if ( ! INPUT ) {
          PrintUsage ( argv [ 0 ] ) ;
          return ;
      }
      if ( ( Scroll == 0 ) && ( PngOut == 0 ) ) PsOut = 1;
      if ( Bkgr ) {
          if ( PngOut ) {
              void *tmp;
              tmp = kgGetImage ( Bimg+2 ) ;
              Pbkimg = kgChangeSizeImage ( tmp , Pxres , Pyres ) ;
              kgFreeImage ( tmp ) ;
          }
          if ( Scroll ) {
              void *tmp;
              tmp = kgGetImage ( Bimg+2 ) ;
              Sbkimg = kgChangeSizeImage ( tmp , Sxres , Syres ) ;
              kgFreeImage ( tmp ) ;
          }
      }
      if ( PsOut ) remove ( PsFile ) ;
      Img = kgInitGph ( 960 , 720 ) ;
//   Img = kgInitImage(960,720,1);
//    kgUserFrame(Img, 0., -117., 373., 163.);
//    kgUserFrame(Img, 0., -117., 373., 180.);
      kgUserFrame ( Img , 0. , -127. , 297. , 170. ) ;
//    kgUserFrame(Img, 0., -127., 396., 170.);
      Reset_for_tp ( xp , yp ) ;
      print_process ( flname , 1 ) ;
      kgCloseGph ( Img ) ;
      kgCleanDir ( TmpDir ) ;
      free ( TmpDir ) ;
      TmpDir = NULL;
      return;
  }
  int main ( int argc , char *argv [ ] ) {
      ProcessTextDoc ( argc , argv ) ;
  }
