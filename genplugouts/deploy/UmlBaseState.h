#ifndef _UMLBASESTATE_H
#define _UMLBASESTATE_H


#include "UmlItem.h"
#include "anItemKind.h"

#include "StateBehavior.h"

class UmlState;
class UmlStateDiagram;

class UmlBaseState : public UmlItem
{
public:
    //  returns a new state named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlState * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseState(void * id, const QByteArray & s) : UmlItem(id, s) {
    }


public:
    // return the entry behavior in OCL
    const QByteArray & entryBehavior();

    // set the entry behavior in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_EntryBehavior(const char * s);

    // return the exit behavior in OCL
    const QByteArray & exitBehavior();

    // set the exit behavior in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_ExitBehavior(const char * s);

    // return the activity in OCL
    const QByteArray & doActivity();

    // set the activity in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DoActivity(const char * s);

#ifdef WITHCPP
    // return the entry behavior in C++
    const QByteArray & cppEntryBehavior();

    // set the entry behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppEntryBehavior(const char * s);

    // return the exit behavior in C++
    const QByteArray & cppExitBehavior();

    // set the exit behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppExitBehavior(const char * s);

    // return the activity in C++
    const QByteArray & cppDoActivity();

    // set the activity in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDoActivity(const char * s);
#endif

#ifdef WITHJAVA
    // return the entry behavior in Java
    const QByteArray & javaEntryBehavior();

    // set the entry behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaEntryBehavior(const char * s);

    // return the exit behavior in Java
    const QByteArray & javaExitBehavior();

    // set the exit behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaExitBehavior(const char * s);

    // return the activity in Java
    const QByteArray & javaDoActivity();

    // set the activity in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaDoActivity(const char * s);
#endif

    // returns the optional associated diagram
    UmlStateDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlStateDiagram * d);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


private:
    StateBehavior _uml;

#ifdef WITHCPP
    StateBehavior _cpp;
#endif

#ifdef WITHJAVA
    StateBehavior _java;
#endif

    UmlStateDiagram * _assoc_diagram;


protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

#endif
