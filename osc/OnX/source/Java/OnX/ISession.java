/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package OnX;

public class ISession {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected ISession(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ISession obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      OnXJNI.delete_ISession(swigCPtr);
    }
    swigCPtr = 0;
  }

  public String name() {
    return OnXJNI.ISession_name(swigCPtr, this);
  }

  public void addManager(IManager arg0) {
    OnXJNI.ISession_addManager(swigCPtr, this, IManager.getCPtr(arg0), arg0);
  }

  public void removeManager(IManager arg0) {
    OnXJNI.ISession_removeManager(swigCPtr, this, IManager.getCPtr(arg0), arg0);
  }

  public IManager findManager(String aName) {
    long cPtr = OnXJNI.ISession_findManager(swigCPtr, this, aName);
    return (cPtr == 0) ? null : new IManager(cPtr, false);
  }

  public void destroyManager(String aName) {
    OnXJNI.ISession_destroyManager(swigCPtr, this, aName);
  }

  public IWriter printer() {
    return new IWriter(OnXJNI.ISession_printer(swigCPtr, this), false);
  }

  public boolean load(String aFile, String aPackage) {
    return OnXJNI.ISession_load(swigCPtr, this, aFile, aPackage);
  }

  public void redirectOutput(boolean aRedirect) {
    OnXJNI.ISession_redirectOutput(swigCPtr, this, aRedirect);
  }

  public boolean setParameter(String aKey, String aValue) {
    return OnXJNI.ISession_setParameter(swigCPtr, this, aKey, aValue);
  }

  public boolean parameterValue(String aKey, SWIGTYPE_p_std__string aValue) {
    return OnXJNI.ISession_parameterValue__SWIG_0(swigCPtr, this, aKey, SWIGTYPE_p_std__string.getCPtr(aValue));
  }

  public boolean removeParameter(String aKey) {
    return OnXJNI.ISession_removeParameter(swigCPtr, this, aKey);
  }

  public String[] availableParameters() {
  return OnXJNI.ISession_availableParameters(swigCPtr, this);
}

  public void flush() {
    OnXJNI.ISession_flush(swigCPtr, this);
  }

  public void setVerboseLevel(int aVerbosity) {
    OnXJNI.ISession_setVerboseLevel(swigCPtr, this, aVerbosity);
  }

  public int verboseLevel() {
    return OnXJNI.ISession_verboseLevel(swigCPtr, this);
  }

  public long numberOfManagers() {
    return OnXJNI.ISession_numberOfManagers(swigCPtr, this);
  }

  public IManager manager(long arg0) {
    long cPtr = OnXJNI.ISession_manager(swigCPtr, this, arg0);
    return (cPtr == 0) ? null : new IManager(cPtr, false);
  }

  public String[] managerNames() {
  return OnXJNI.ISession_managerNames(swigCPtr, this);
}

  public SWIGTYPE_p_std__vectorTstd__pairTstd__string_std__string_t_t arguments() {
    return new SWIGTYPE_p_std__vectorTstd__pairTstd__string_std__string_t_t(OnXJNI.ISession_arguments(swigCPtr, this), true);
  }

  public String parameterValue(String aKey) {
    return OnXJNI.ISession_parameterValue__SWIG_1(swigCPtr, this, aKey);
  }

  public void setColor(double aR, double aG, double aB) {
    OnXJNI.ISession_setColor__SWIG_0(swigCPtr, this, aR, aG, aB);
  }

  public void setColor(String aColor) {
    OnXJNI.ISession_setColor__SWIG_1(swigCPtr, this, aColor);
  }

  public IProcessor accessorManager() {
    long cPtr = OnXJNI.ISession_accessorManager(swigCPtr, this);
    return (cPtr == 0) ? null : new IProcessor(cPtr, false);
  }

  public IScriptManager scriptManager() {
    long cPtr = OnXJNI.ISession_scriptManager(swigCPtr, this);
    return (cPtr == 0) ? null : new IScriptManager(cPtr, false);
  }

  public IUI ui() {
    long cPtr = OnXJNI.ISession_ui(swigCPtr, this);
    return (cPtr == 0) ? null : new IUI(cPtr, false);
  }

}
