#ifndef UTILITIES_FSM_TRANSITIONMACROS_H__INCLUDED
#define UTILITIES_FSM_TRANSITIONMACROS_H__INCLUDED


//------------------------------------------------------------------------
// 
// Switch to a new state.
// 
//------------------------------------------------------------------------
#define SWITCH_TO_STATE( DestinationStateName )\
    {\
        assert(__functionName != Fsm<FsmOwner>::Function_Constructor && __functionName != Fsm<FsmOwner>::Function_Destructor);\
        GetFsm().SwitchState(STATE(DestinationStateName));\
        return FSR_FunctionInterrupted;\
    }


//------------------------------------------------------------------------
// 
// Switch to the previous state.
// 
//------------------------------------------------------------------------
#define SWITCH_TO_PREVIOUS_STATE()\
    {\
        assert(__functionName != Fsm<FsmOwner>::Function_Constructor && __functionName != Fsm<FsmOwner>::Function_Destructor);\
        GetFsm().SwitchToPreviousState();\
        return FSR_FunctionInterrupted;\
    }


//------------------------------------------------------------------------
// 
// Implement a transitions block.
// 
//------------------------------------------------------------------------
#define STATE_TRANSITION_BEGIN

#define NoTransitionAction ((void)0)

#define TRANSIT_TO_STATE( Expression, DoAction, DestinationStateName )\
    if (Expression)\
    {\
        DoAction;\
        SWITCH_TO_STATE(DestinationStateName);\
    }

#define STATE_TRANSITION_END


#endif
