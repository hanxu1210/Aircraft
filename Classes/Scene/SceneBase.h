#ifndef _TEST_BASIC_H_
#define _TEST_BASIC_H_

USING_NS_CC;
using namespace std;

class SceneBase : public Scene
{
public: 
    SceneBase(bool bPortrait = false);
    virtual void onEnter();

    virtual void runThisTest() = 0;
};

typedef Layer* (*NEWTESTFUNC)();
#define TESTLAYER_CREATE_FUNC(className) \
static Layer* create##className() \
{ return new className(); }

#define CF(className) create##className

#endif
