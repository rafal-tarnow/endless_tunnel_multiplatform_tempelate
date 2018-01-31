QMAKE_CFLAGS += -DFT2_BUILD_LIBRARY

INCLUDEPATH += $$PWD/include
#DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/src/autofit/afangles.h \
    $$PWD/src/autofit/afblue.cin \
    $$PWD/src/autofit/afblue.h \
    $$PWD/src/autofit/afblue.hin \
    $$PWD/src/autofit/afcjk.h \
    $$PWD/src/autofit/afcover.h \
    $$PWD/src/autofit/afdummy.h \
    $$PWD/src/autofit/aferrors.h \
    $$PWD/src/autofit/afglobal.h \
    $$PWD/src/autofit/afhints.h \
    $$PWD/src/autofit/afindic.h \
    $$PWD/src/autofit/aflatin.h \
    $$PWD/src/autofit/aflatin2.h \
    $$PWD/src/autofit/afloader.h \
    $$PWD/src/autofit/afmodule.h \
    $$PWD/src/autofit/afpic.h \
    $$PWD/src/autofit/afranges.h \
    $$PWD/src/autofit/afscript.h \
    $$PWD/src/autofit/afshaper.h \
    $$PWD/src/autofit/afstyles.h \
    $$PWD/src/autofit/aftypes.h \
    $$PWD/src/autofit/afwarp.h \
    $$PWD/src/autofit/afwrtsys.h \
    $$PWD/src/base/basepic.h \
    $$PWD/src/base/ftbase.h \
    $$PWD/src/base/md5.h \
    $$PWD/src/bdf/bdf.h \
    $$PWD/src/bdf/bdfdrivr.h \
    $$PWD/src/bdf/bdferror.h \
    $$PWD/src/cache/ftccache.h \
    $$PWD/src/cache/ftccback.h \
    $$PWD/src/cache/ftcerror.h \
    $$PWD/src/cache/ftcglyph.h \
    $$PWD/src/cache/ftcimage.h \
    $$PWD/src/cache/ftcmanag.h \
    $$PWD/src/cache/ftcmru.h \
    $$PWD/src/cache/ftcsbits.h \
    $$PWD/src/cff/cf2arrst.h \
    $$PWD/src/cff/cf2blues.h \
    $$PWD/src/cff/cf2error.h \
    $$PWD/src/cff/cf2fixed.h \
    $$PWD/src/cff/cf2font.h \
    $$PWD/src/cff/cf2ft.h \
    $$PWD/src/cff/cf2glue.h \
    $$PWD/src/cff/cf2hints.h \
    $$PWD/src/cff/cf2intrp.h \
    $$PWD/src/cff/cf2read.h \
    $$PWD/src/cff/cf2stack.h \
    $$PWD/src/cff/cf2types.h \
    $$PWD/src/cff/cffcmap.h \
    $$PWD/src/cff/cffdrivr.h \
    $$PWD/src/cff/cfferrs.h \
    $$PWD/src/cff/cffgload.h \
    $$PWD/src/cff/cffload.h \
    $$PWD/src/cff/cffobjs.h \
    $$PWD/src/cff/cffparse.h \
    $$PWD/src/cff/cffpic.h \
    $$PWD/src/cff/cfftoken.h \
    $$PWD/src/cff/cfftypes.h \
    $$PWD/src/cid/ciderrs.h \
    $$PWD/src/cid/cidgload.h \
    $$PWD/src/cid/cidload.h \
    $$PWD/src/cid/cidobjs.h \
    $$PWD/src/cid/cidparse.h \
    $$PWD/src/cid/cidriver.h \
    $$PWD/src/cid/cidtoken.h \
    $$PWD/src/gxvalid/gxvalid.h \
    $$PWD/src/gxvalid/gxvcommn.h \
    $$PWD/src/gxvalid/gxverror.h \
    $$PWD/src/gxvalid/gxvfeat.h \
    $$PWD/src/gxvalid/gxvmod.h \
    $$PWD/src/gxvalid/gxvmort.h \
    $$PWD/src/gxvalid/gxvmorx.h \
    $$PWD/src/gzip/ftzconf.h \
    $$PWD/src/gzip/infblock.h \
    $$PWD/src/gzip/infcodes.h \
    $$PWD/src/gzip/inffixed.h \
    $$PWD/src/gzip/inftrees.h \
    $$PWD/src/gzip/infutil.h \
    $$PWD/src/gzip/zlib.h \
    $$PWD/src/gzip/zutil.h \
    $$PWD/src/lzw/ftzopen.h \
    $$PWD/src/otvalid/otvalid.h \
    $$PWD/src/otvalid/otvcommn.h \
    $$PWD/src/otvalid/otverror.h \
    $$PWD/src/otvalid/otvgpos.h \
    $$PWD/src/otvalid/otvmod.h \
    $$PWD/src/pcf/pcf.h \
    $$PWD/src/pcf/pcfdrivr.h \
    $$PWD/src/pcf/pcferror.h \
    $$PWD/src/pcf/pcfread.h \
    $$PWD/src/pcf/pcfutil.h \
    $$PWD/src/pfr/pfrcmap.h \
    $$PWD/src/pfr/pfrdrivr.h \
    $$PWD/src/pfr/pfrerror.h \
    $$PWD/src/pfr/pfrgload.h \
    $$PWD/src/pfr/pfrload.h \
    $$PWD/src/pfr/pfrobjs.h \
    $$PWD/src/pfr/pfrsbit.h \
    $$PWD/src/pfr/pfrtypes.h \
    $$PWD/src/psaux/afmparse.h \
    $$PWD/src/psaux/psauxerr.h \
    $$PWD/src/psaux/psauxmod.h \
    $$PWD/src/psaux/psconv.h \
    $$PWD/src/psaux/psobjs.h \
    $$PWD/src/psaux/t1cmap.h \
    $$PWD/src/psaux/t1decode.h \
    $$PWD/src/pshinter/pshalgo.h \
    $$PWD/src/pshinter/pshglob.h \
    $$PWD/src/pshinter/pshmod.h \
    $$PWD/src/pshinter/pshnterr.h \
    $$PWD/src/pshinter/pshpic.h \
    $$PWD/src/pshinter/pshrec.h \
    $$PWD/src/psnames/psmodule.h \
    $$PWD/src/psnames/psnamerr.h \
    $$PWD/src/psnames/pspic.h \
    $$PWD/src/psnames/pstables.h \
    $$PWD/src/raster/ftmisc.h \
    $$PWD/src/raster/ftraster.h \
    $$PWD/src/raster/ftrend1.h \
    $$PWD/src/raster/rasterrs.h \
    $$PWD/src/raster/rastpic.h \
    $$PWD/src/sfnt/pngshim.h \
    $$PWD/src/sfnt/sfdriver.h \
    $$PWD/src/sfnt/sferrors.h \
    $$PWD/src/sfnt/sfntpic.h \
    $$PWD/src/sfnt/sfobjs.h \
    $$PWD/src/sfnt/ttbdf.h \
    $$PWD/src/sfnt/ttcmap.h \
    $$PWD/src/sfnt/ttcmapc.h \
    $$PWD/src/sfnt/ttkern.h \
    $$PWD/src/sfnt/ttload.h \
    $$PWD/src/sfnt/ttmtx.h \
    $$PWD/src/sfnt/ttpost.h \
    $$PWD/src/sfnt/ttsbit.h \
    $$PWD/src/smooth/ftgrays.h \
    $$PWD/src/smooth/ftsmerrs.h \
    $$PWD/src/smooth/ftsmooth.h \
    $$PWD/src/smooth/ftspic.h \
    $$PWD/src/truetype/ttdriver.h \
    $$PWD/src/truetype/tterrors.h \
    $$PWD/src/truetype/ttgload.h \
    $$PWD/src/truetype/ttgxvar.h \
    $$PWD/src/truetype/ttinterp.h \
    $$PWD/src/truetype/ttobjs.h \
    $$PWD/src/truetype/ttpic.h \
    $$PWD/src/truetype/ttpload.h \
    $$PWD/src/truetype/ttsubpix.h \
    $$PWD/src/type1/t1afm.h \
    $$PWD/src/type1/t1driver.h \
    $$PWD/src/type1/t1errors.h \
    $$PWD/src/type1/t1gload.h \
    $$PWD/src/type1/t1load.h \
    $$PWD/src/type1/t1objs.h \
    $$PWD/src/type1/t1parse.h \
    $$PWD/src/type1/t1tokens.h \
    $$PWD/src/type42/t42drivr.h \
    $$PWD/src/type42/t42error.h \
    $$PWD/src/type42/t42objs.h \
    $$PWD/src/type42/t42parse.h \
    $$PWD/src/type42/t42types.h \
    $$PWD/src/winfonts/fnterrs.h \
    $$PWD/src/winfonts/winfnt.h \
    $$PWD/include/freetype/config/ftconfig.h \
    $$PWD/include/freetype/config/ftheader.h \
    $$PWD/include/freetype/config/ftmodule.h \
    $$PWD/include/freetype/config/ftoption.h \
    $$PWD/include/freetype/config/ftstdlib.h \
    $$PWD/include/freetype/internal/services/svbdf.h \
    $$PWD/include/freetype/internal/services/svcid.h \
    $$PWD/include/freetype/internal/services/svfntfmt.h \
    $$PWD/include/freetype/internal/services/svgldict.h \
    $$PWD/include/freetype/internal/services/svgxval.h \
    $$PWD/include/freetype/internal/services/svkern.h \
    $$PWD/include/freetype/internal/services/svmetric.h \
    $$PWD/include/freetype/internal/services/svmm.h \
    $$PWD/include/freetype/internal/services/svotval.h \
    $$PWD/include/freetype/internal/services/svpfr.h \
    $$PWD/include/freetype/internal/services/svpostnm.h \
    $$PWD/include/freetype/internal/services/svprop.h \
    $$PWD/include/freetype/internal/services/svpscmap.h \
    $$PWD/include/freetype/internal/services/svpsinfo.h \
    $$PWD/include/freetype/internal/services/svsfnt.h \
    $$PWD/include/freetype/internal/services/svttcmap.h \
    $$PWD/include/freetype/internal/services/svtteng.h \
    $$PWD/include/freetype/internal/services/svttglyf.h \
    $$PWD/include/freetype/internal/services/svwinfnt.h \
    $$PWD/include/freetype/internal/autohint.h \
    $$PWD/include/freetype/internal/ftcalc.h \
    $$PWD/include/freetype/internal/ftdebug.h \
    $$PWD/include/freetype/internal/ftdriver.h \
    $$PWD/include/freetype/internal/ftgloadr.h \
    $$PWD/include/freetype/internal/fthash.h \
    $$PWD/include/freetype/internal/ftmemory.h \
    $$PWD/include/freetype/internal/ftobjs.h \
    $$PWD/include/freetype/internal/ftpic.h \
    $$PWD/include/freetype/internal/ftrfork.h \
    $$PWD/include/freetype/internal/ftserv.h \
    $$PWD/include/freetype/internal/ftstream.h \
    $$PWD/include/freetype/internal/fttrace.h \
    $$PWD/include/freetype/internal/ftvalid.h \
    $$PWD/include/freetype/internal/internal.h \
    $$PWD/include/freetype/internal/psaux.h \
    $$PWD/include/freetype/internal/pshints.h \
    $$PWD/include/freetype/internal/sfnt.h \
    $$PWD/include/freetype/internal/t1types.h \
    $$PWD/include/freetype/internal/tttypes.h \
    $$PWD/include/freetype/freetype.h \
    $$PWD/include/freetype/ftadvanc.h \
    $$PWD/include/freetype/ftautoh.h \
    $$PWD/include/freetype/ftbbox.h \
    $$PWD/include/freetype/ftbdf.h \
    $$PWD/include/freetype/ftbitmap.h \
    $$PWD/include/freetype/ftbzip2.h \
    $$PWD/include/freetype/ftcache.h \
    $$PWD/include/freetype/ftcffdrv.h \
    $$PWD/include/freetype/ftchapters.h \
    $$PWD/include/freetype/ftcid.h \
    $$PWD/include/freetype/fterrdef.h \
    $$PWD/include/freetype/fterrors.h \
    $$PWD/include/freetype/ftfntfmt.h \
    $$PWD/include/freetype/ftgasp.h \
    $$PWD/include/freetype/ftglyph.h \
    $$PWD/include/freetype/ftgxval.h \
    $$PWD/include/freetype/ftgzip.h \
    $$PWD/include/freetype/ftimage.h \
    $$PWD/include/freetype/ftincrem.h \
    $$PWD/include/freetype/ftlcdfil.h \
    $$PWD/include/freetype/ftlist.h \
    $$PWD/include/freetype/ftlzw.h \
    $$PWD/include/freetype/ftmac.h \
    $$PWD/include/freetype/ftmm.h \
    $$PWD/include/freetype/ftmodapi.h \
    $$PWD/include/freetype/ftmoderr.h \
    $$PWD/include/freetype/ftotval.h \
    $$PWD/include/freetype/ftoutln.h \
    $$PWD/include/freetype/ftpcfdrv.h \
    $$PWD/include/freetype/ftpfr.h \
    $$PWD/include/freetype/ftrender.h \
    $$PWD/include/freetype/ftsizes.h \
    $$PWD/include/freetype/ftsnames.h \
    $$PWD/include/freetype/ftstroke.h \
    $$PWD/include/freetype/ftsynth.h \
    $$PWD/include/freetype/ftsystem.h \
    $$PWD/include/freetype/fttrigon.h \
    $$PWD/include/freetype/ftttdrv.h \
    $$PWD/include/freetype/fttypes.h \
    $$PWD/include/freetype/ftwinfnt.h \
    $$PWD/include/freetype/t1tables.h \
    $$PWD/include/freetype/ttnameid.h \
    $$PWD/include/freetype/tttables.h \
    $$PWD/include/freetype/tttags.h \
    $$PWD/include/freetype/ttunpat.h \
    $$PWD/include/ft2build.h

SOURCES += \
    $$PWD/src/autofit/afangles.c \
    $$PWD/src/autofit/afblue.c \
    $$PWD/src/autofit/afcjk.c \
    $$PWD/src/autofit/afdummy.c \
    $$PWD/src/autofit/afglobal.c \
    $$PWD/src/autofit/afhints.c \
    $$PWD/src/autofit/afindic.c \
    $$PWD/src/autofit/aflatin.c \
    #$$PWD/src/autofit/aflatin2.c \
    $$PWD/src/autofit/afloader.c \
    $$PWD/src/autofit/afmodule.c \
    $$PWD/src/autofit/afpic.c \
    $$PWD/src/autofit/afranges.c \
    $$PWD/src/autofit/afshaper.c \
    $$PWD/src/autofit/afwarp.c \
    $$PWD/src/autofit/autofit.c \
    $$PWD/src/base/basepic.c \
    $$PWD/src/base/ftadvanc.c \
    $$PWD/src/base/ftapi.c \
    $$PWD/src/base/ftbase.c \
    $$PWD/src/base/ftbbox.c \
    $$PWD/src/base/ftbdf.c \
    $$PWD/src/base/ftbitmap.c \
    $$PWD/src/base/ftcalc.c \
    $$PWD/src/base/ftcid.c \
    $$PWD/src/base/ftdbgmem.c \
    $$PWD/src/base/ftdebug.c \
    $$PWD/src/base/ftfntfmt.c \
    $$PWD/src/base/ftfstype.c \
    $$PWD/src/base/ftgasp.c \
    $$PWD/src/base/ftgloadr.c \
    $$PWD/src/base/ftglyph.c \
    $$PWD/src/base/ftgxval.c \
    $$PWD/src/base/fthash.c \
    $$PWD/src/base/ftinit.c \
    $$PWD/src/base/ftlcdfil.c \
    $$PWD/src/base/ftmac.c \
    $$PWD/src/base/ftmm.c \
    $$PWD/src/base/ftobjs.c \
    $$PWD/src/base/ftotval.c \
    $$PWD/src/base/ftoutln.c \
    $$PWD/src/base/ftpatent.c \
    $$PWD/src/base/ftpfr.c \
    $$PWD/src/base/ftpic.c \
    $$PWD/src/base/ftrfork.c \
    $$PWD/src/base/ftsnames.c \
    $$PWD/src/base/ftstream.c \
    $$PWD/src/base/ftstroke.c \
    $$PWD/src/base/ftsynth.c \
    $$PWD/src/base/ftsystem.c \
    $$PWD/src/base/fttrigon.c \
    $$PWD/src/base/fttype1.c \
    $$PWD/src/base/ftutil.c \
    $$PWD/src/base/ftwinfnt.c \
    $$PWD/src/base/md5.c \
#    $$PWD/src/bdf/bdf.c \
#    $$PWD/src/bdf/bdfdrivr.c \
#    $$PWD/src/bdf/bdflib.c \
    $$PWD/src/bzip2/ftbzip2.c \
    $$PWD/src/cache/ftcache.c \
    $$PWD/src/cache/ftcbasic.c \
    $$PWD/src/cache/ftccache.c \
    $$PWD/src/cache/ftccmap.c \
    $$PWD/src/cache/ftcglyph.c \
    $$PWD/src/cache/ftcimage.c \
    $$PWD/src/cache/ftcmanag.c \
    $$PWD/src/cache/ftcmru.c \
    $$PWD/src/cache/ftcsbits.c \
#    $$PWD/src/cff/cf2arrst.c \
#    $$PWD/src/cff/cf2blues.c \
#    $$PWD/src/cff/cf2error.c \
#    $$PWD/src/cff/cf2font.c \
#    $$PWD/src/cff/cf2ft.c \
#    $$PWD/src/cff/cf2hints.c \
#    $$PWD/src/cff/cf2intrp.c \
#    $$PWD/src/cff/cf2read.c \
#    $$PWD/src/cff/cf2stack.c \
#    $$PWD/src/cff/cff.c \
#    $$PWD/src/cff/cffcmap.c \
#    $$PWD/src/cff/cffdrivr.c \
#    $$PWD/src/cff/cffgload.c \
#    $$PWD/src/cff/cffload.c \
#    $$PWD/src/cff/cffobjs.c \
#    $$PWD/src/cff/cffparse.c \
#    $$PWD/src/cff/cffpic.c \
#    $$PWD/src/cid/cidgload.c \
#    $$PWD/src/cid/cidload.c \
#    $$PWD/src/cid/cidobjs.c \
#    $$PWD/src/cid/cidparse.c \
#    $$PWD/src/cid/cidriver.c \
#    $$PWD/src/cid/type1cid.c \
#    $$PWD/src/gxvalid/gxvalid.c \
#    $$PWD/src/gxvalid/gxvbsln.c \
#    $$PWD/src/gxvalid/gxvcommn.c \
#    $$PWD/src/gxvalid/gxvfeat.c \
#    $$PWD/src/gxvalid/gxvfgen.c \
#    $$PWD/src/gxvalid/gxvjust.c \
#    $$PWD/src/gxvalid/gxvkern.c \
#    $$PWD/src/gxvalid/gxvlcar.c \
#    $$PWD/src/gxvalid/gxvmod.c \
#    $$PWD/src/gxvalid/gxvmort.c \
#    $$PWD/src/gxvalid/gxvmort0.c \
#    $$PWD/src/gxvalid/gxvmort1.c \
#    $$PWD/src/gxvalid/gxvmort2.c \
#    $$PWD/src/gxvalid/gxvmort4.c \
#    $$PWD/src/gxvalid/gxvmort5.c \
#    $$PWD/src/gxvalid/gxvmorx.c \
#    $$PWD/src/gxvalid/gxvmorx0.c \
#    $$PWD/src/gxvalid/gxvmorx1.c \
#    $$PWD/src/gxvalid/gxvmorx2.c \
#    $$PWD/src/gxvalid/gxvmorx4.c \
#    $$PWD/src/gxvalid/gxvmorx5.c \
#    $$PWD/src/gxvalid/gxvopbd.c \
#    $$PWD/src/gxvalid/gxvprop.c \
#    $$PWD/src/gxvalid/gxvtrak.c \
    $$PWD/src/gzip/adler32.c \
    $$PWD/src/gzip/ftgzip.c \
    $$PWD/src/gzip/infblock.c \
    $$PWD/src/gzip/infcodes.c \
    $$PWD/src/gzip/inflate.c \
    $$PWD/src/gzip/inftrees.c \
#    $$PWD/src/gzip/infutil.c \
    $$PWD/src/gzip/zutil.c \
    $$PWD/src/lzw/ftlzw.c \
    $$PWD/src/lzw/ftzopen.c \
    $$PWD/src/otvalid/otvalid.c \
    $$PWD/src/otvalid/otvbase.c \
    $$PWD/src/otvalid/otvcommn.c \
    $$PWD/src/otvalid/otvgdef.c \
    $$PWD/src/otvalid/otvgpos.c \
    $$PWD/src/otvalid/otvgsub.c \
    $$PWD/src/otvalid/otvjstf.c \
    $$PWD/src/otvalid/otvmath.c \
    $$PWD/src/otvalid/otvmod.c \
#    $$PWD/src/pcf/pcf.c \
#    $$PWD/src/pcf/pcfdrivr.c \
#    $$PWD/src/pcf/pcfread.c \
#    $$PWD/src/pcf/pcfutil.c \
#    $$PWD/src/pfr/pfr.c \
#    $$PWD/src/pfr/pfrcmap.c \
#    $$PWD/src/pfr/pfrdrivr.c \
#    $$PWD/src/pfr/pfrgload.c \
#    $$PWD/src/pfr/pfrload.c \
#    $$PWD/src/pfr/pfrobjs.c \
#    $$PWD/src/pfr/pfrsbit.c \
    $$PWD/src/psaux/afmparse.c \
    $$PWD/src/psaux/psaux.c \
    $$PWD/src/psaux/psauxmod.c \
    $$PWD/src/psaux/psconv.c \
    $$PWD/src/psaux/psobjs.c \
    $$PWD/src/psaux/t1cmap.c \
    $$PWD/src/psaux/t1decode.c \
    $$PWD/src/pshinter/pshalgo.c \
    $$PWD/src/pshinter/pshglob.c \
    $$PWD/src/pshinter/pshinter.c \
    $$PWD/src/pshinter/pshmod.c \
    $$PWD/src/pshinter/pshpic.c \
    $$PWD/src/pshinter/pshrec.c \
    $$PWD/src/psnames/psmodule.c \
    $$PWD/src/psnames/psnames.c \
    $$PWD/src/psnames/pspic.c \
    $$PWD/src/raster/ftraster.c \
    $$PWD/src/raster/ftrend1.c \
    $$PWD/src/raster/raster.c \
    $$PWD/src/raster/rastpic.c \
    $$PWD/src/sfnt/pngshim.c \
    $$PWD/src/sfnt/sfdriver.c \
    $$PWD/src/sfnt/sfnt.c \
    $$PWD/src/sfnt/sfntpic.c \
    $$PWD/src/sfnt/sfobjs.c \
    $$PWD/src/sfnt/ttbdf.c \
    $$PWD/src/sfnt/ttcmap.c \
    $$PWD/src/sfnt/ttkern.c \
    $$PWD/src/sfnt/ttload.c \
    $$PWD/src/sfnt/ttmtx.c \
    $$PWD/src/sfnt/ttpost.c \
    $$PWD/src/sfnt/ttsbit.c \
    $$PWD/src/smooth/ftgrays.c \
    $$PWD/src/smooth/ftsmooth.c \
    $$PWD/src/smooth/ftspic.c \
    $$PWD/src/smooth/smooth.c \
    $$PWD/src/truetype/truetype.c \
    $$PWD/src/truetype/ttdriver.c \
    $$PWD/src/truetype/ttgload.c \
    $$PWD/src/truetype/ttgxvar.c \
    $$PWD/src/truetype/ttinterp.c \
    $$PWD/src/truetype/ttobjs.c \
    $$PWD/src/truetype/ttpic.c \
    $$PWD/src/truetype/ttpload.c \
    $$PWD/src/truetype/ttsubpix.c \
#    $$PWD/src/type1/t1afm.c \
#    $$PWD/src/type1/t1driver.c \
#    $$PWD/src/type1/t1gload.c \
#    $$PWD/src/type1/t1load.c \
#    $$PWD/src/type1/t1objs.c \
#    $$PWD/src/type1/t1parse.c \
#    $$PWD/src/type1/type1.c \
#    $$PWD/src/type42/t42drivr.c \
#    $$PWD/src/type42/t42objs.c \
#    $$PWD/src/type42/t42parse.c \
#    $$PWD/src/type42/type42.c \
#    $$PWD/src/winfonts/winfnt.c

#DISTFILES += \
#    $$PWD/src/base/Jamfile \
#    $$PWD/src/bdf/Jamfile \
#    $$PWD/src/bzip2/Jamfile \
#    $$PWD/src/cache/Jamfile \
#    $$PWD/src/cff/Jamfile \
#    $$PWD/src/cid/Jamfile \
#    $$PWD/src/gxvalid/Jamfile \
#    $$PWD/src/gzip/Jamfile \
#    $$PWD/src/lzw/Jamfile \
#    $$PWD/src/otvalid/Jamfile \
#    $$PWD/src/pcf/Jamfile \
#    $$PWD/src/pfr/Jamfile \
#    $$PWD/src/psaux/Jamfile \
#    $$PWD/src/pshinter/Jamfile \
#    $$PWD/src/psnames/Jamfile \
#    $$PWD/src/raster/Jamfile \
#    $$PWD/src/sfnt/Jamfile \
#    $$PWD/src/smooth/Jamfile \
#    $$PWD/src/truetype/Jamfile \
#    $$PWD/src/type1/Jamfile \
#    $$PWD/src/type42/Jamfile \
#    $$PWD/src/winfonts/Jamfile \
#    $$PWD/src/Jamfile \
#    $$PWD/src/autofit/module.mk \
#    $$PWD/src/base/rules.mk \
#    $$PWD/src/bdf/module.mk \
#    $$PWD/src/bdf/rules.mk \
#    $$PWD/src/bzip2/rules.mk \
#    $$PWD/src/cache/rules.mk \
#    $$PWD/src/cff/module.mk \
#    $$PWD/src/cff/rules.mk \
#    $$PWD/src/cid/module.mk \
#    $$PWD/src/cid/rules.mk \
#    $$PWD/src/gxvalid/module.mk \
#    $$PWD/src/gxvalid/rules.mk \
#    $$PWD/src/gzip/rules.mk \
#    $$PWD/src/lzw/rules.mk \
#    $$PWD/src/otvalid/module.mk \
#    $$PWD/src/otvalid/rules.mk \
#    $$PWD/src/pcf/module.mk \
#    $$PWD/src/pcf/rules.mk \
#    $$PWD/src/pfr/module.mk \
#    $$PWD/src/pfr/rules.mk \
#    $$PWD/src/psaux/module.mk \
#    $$PWD/src/psaux/rules.mk \
#    $$PWD/src/pshinter/module.mk \
#    $$PWD/src/pshinter/rules.mk \
#    $$PWD/src/psnames/module.mk \
#    $$PWD/src/psnames/rules.mk \
#    $$PWD/src/raster/module.mk \
#    $$PWD/src/raster/rules.mk \
#    $$PWD/src/sfnt/module.mk \
#    $$PWD/src/sfnt/rules.mk \
#    $$PWD/src/smooth/module.mk \
#    $$PWD/src/smooth/rules.mk \
#    $$PWD/src/truetype/module.mk \
#    $$PWD/src/truetype/rules.mk \
#    $$PWD/src/type1/module.mk \
#    $$PWD/src/type1/rules.mk \
#    $$PWD/src/type42/module.mk \
#    $$PWD/src/type42/rules.mk \
#    $$PWD/src/winfonts/module.mk \
#    $$PWD/src/winfonts/rules.mk \
#    $$PWD/src/bdf/README \
#    $$PWD/src/gxvalid/README \
#    $$PWD/src/pcf/README
