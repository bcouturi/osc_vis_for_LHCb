
#
#  Usage :
#    csh> source <osc_vis_path>/Qt-setup.csh
#

#set verbose

set Qt_home=none
if ( "`uname -n | grep lxplus`" != "" ) then
  if ( `uname -m` == "x86_64" ) then
    set Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.4.2/slc4_amd64_gcc34"
  else
    set Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.4.2/slc4_ia32_gcc34"
  endif
else
  if ( `uname` == Darwin ) then
    set Qt_home=/Library/Frameworks
  endif
endif

if ( "${Qt_home}" != "none" ) then

if ( `uname` == "Linux" ) then
  set lib_path="${Qt_home}/lib"
  set lib_curr=`printenv LD_LIBRARY_PATH`
  if ( "${lib_curr}" == "" ) then
    setenv LD_LIBRARY_PATH "${lib_path}"
  else
    if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
      setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${lib_path}"
    endif
  endif
  unset lib_curr
  unset lib_path
endif

if ( `uname` == "Darwin" ) then

  set lib_path="${Qt_home}/Qt3Support.framework:${Qt_home}/QtCore.framework:${Qt_home}/QtGui.framework:${Qt_home}/QtOpenGL.framework:${Qt_home}/QtNetWork.framework:${Qt_home}/QtAssistant.framework:${Qt_home}/QtDBus.framework:${Qt_home}/QtDesigner.framework:${Qt_home}/QtDesignerComponents.framework:${Qt_home}/QtHelp.framework:${Qt_home}/QtScript.framework:${Qt_home}/QtScriptTools.framework:${Qt_home}/QtSql.framework:${Qt_home}/QtSvg.framework:${Qt_home}/QtTest.framework:${Qt_home}/QtWebKit.framework:${Qt_home}/QtXml.framework:${Qt_home}/QtXmlPatterns.framework"

  set lib_curr=`printenv DYLD_LIBRARY_PATH`
  if ( "${lib_curr}" == "" ) then
    setenv DYLD_LIBRARY_PATH "${lib_path}"
  else
    if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
      setenv DYLD_LIBRARY_PATH "${DYLD_LIBRARY_PATH}:${lib_path}"
    endif
  endif
  unset lib_curr
  unset lib_path
endif

set bin_path="${Qt_home}/bin"
set bin_curr=`printenv PATH`
if ( "${bin_curr}" == "" ) then
 setenv PATH "${bin_path}"
else
 if ( `echo "${bin_curr}" | grep "${bin_path}" ` == "" ) then
   setenv PATH "${PATH}:${bin_path}"
 endif
endif
unset bin_curr
unset bin_path

endif

unset Qt_home
