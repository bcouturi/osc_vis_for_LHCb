#ifndef BatchLab_XML_Manager_h
#define BatchLab_XML_Manager_h

//Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Store/IStoreManager.h>

#include <Slash/Core/ISession.h>
#include <BatchLab/XML/Tree.h>

namespace BatchLab {

namespace XML {

class Manager 
:public virtual Slash::Core::IManager
,public virtual Slash::Store::IStoreManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::XML::Manager)
    else INLIB_IF_CAST(Slash::Store::IStoreManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //Slash::Store::IStoreManager
  virtual Slash::Store::ITree* createTree(const std::string& aName,
                                          bool aReadOnly = false,
                                          bool aCreateNew = false,
                                          const std::string& aOptions = ""){
    XML::Tree* tree = 
      new XML::Tree(fSession,aName,aReadOnly,aCreateNew,aOptions,
                    *this,fSession.verboseLevel());
    if(!tree) return 0;
    if(!tree->isValid()) {
      delete tree;
      return 0;
    }
    fSession.addManager(tree);
    return tree;
  }


  virtual Slash::Store::IObject* createObject(
 Slash::Store::ITree& //aTree
,const std::string& //aClass
,const std::string& //aPath
,const std::string& //aName
,const std::string& //aTitle
,const std::vector<std::string>& //aArgs
,const std::string& = "" //aOptions
  ){
    std::ostream& out = fSession.cout();
    out << "BatchLab::XML::Manager::createObject :" 
        << " dummy."
        << std::endl;
    return 0;
  }

  virtual Slash::Store::IObject* createObject(
 Slash::Store::ITree& //aTree
,const std::string& //aClass
,const std::string& //aPath
,const std::string& //aName
,const std::string& //aTitle
,const std::vector<Slash::Store::IStoreManager::Range>& //aArgs
,const std::string& = "" //aOptions
  ){
    std::ostream& out = fSession.cout();
    out << "BatchLab::XML::Manager::createObject :" 
        << " dummy."
        << std::endl;
    return 0;
  }
public:
  Manager(Slash::Core::ISession& aSession)
  :fName("XML_Manager"),fSession(aSession){}
  virtual ~Manager(){}
private:
  std::string fName;
  Slash::Core::ISession& fSession;
};

} //XML

} //BatchLab

#endif


