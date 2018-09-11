#ifndef UTILITIES_FSM_FSMMACROS_H__INCLUDED
#define UTILITIES_FSM_FSMMACROS_H__INCLUDED


//------------------------------------------------------------------------
// 
// Declare a Finite State Machine (Fsm) in a class.
//
// Note: Must be call before all state and superstate.
//       And only in the base class.
// 
//------------------------------------------------------------------------
#define DECLARE_FSM( ThisClassName )\
    typedef ThisClassName FsmOwner;\
    typedef ThisClassName FsmThisClass;\
    const Fsm<FsmOwner>& GetFsm() const {return __fsm;}\
    Fsm<FsmOwner>& GetFsm() {return __fsm;}\
    Fsm<FsmOwner> __fsm


//------------------------------------------------------------------------
// 
// Overload a Fsm in a derived class.
//
// Note: Must be call before all state and superstate.
//       The base class must contains the Fsm.
// 
//------------------------------------------------------------------------
#define OVERLOAD_FSM( ThisClassName )\
    typedef ThisClassName FsmThisClass


//------------------------------------------------------------------------
// 
// Initialize the Fsm with an initial state.
// 
//------------------------------------------------------------------------
#define INIT_FSM( InitialStateName )\
    GetFsm().Init(this, STATE(InitialStateName))


//------------------------------------------------------------------------
// 
// Update the Fsm.
// 
//------------------------------------------------------------------------
#define UPDATE_FSM()\
    GetFsm().Update()


#endif
