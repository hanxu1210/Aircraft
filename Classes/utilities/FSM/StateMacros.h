#ifndef UTILITIES_FSM_STATEMACROS_H__INCLUDED
#define UTILITIES_FSM_STATEMACROS_H__INCLUDED


//------------------------------------------------------------------------
// 
// State accessor.
// 
//------------------------------------------------------------------------
#define	STATE( StateName )\
    (Fsm<FsmOwner>::State)&FsmThisClass::StateName##_State


//------------------------------------------------------------------------
// 
// Declare a state (in the .h).
// 
//------------------------------------------------------------------------
#define	DECLARE_STATE( StateName )\
    E_FsmStateResult StateName##_State(int __functionName)

#define	DECLARE_VIRTUAL_STATE( StateName )\
    virtual DECLARE_STATE(StateName)


//------------------------------------------------------------------------
// 
// Implement a state (in the .cpp).
// 
//------------------------------------------------------------------------
#define	IMPLEMENT_STATE_BEGIN( ClassName, StateName )\
    E_FsmStateResult ClassName::StateName##_State(int __functionName)\
    {\
        E_FsmStateResult __returnCode = FSR_FunctionNotHandled;\
        if (false)

#define	IMPLEMENT_STATE_END\
        \
        return __returnCode;\
    }


//------------------------------------------------------------------------
// 
// Implement the Constructor, Destructor and Update functions of a state.
// 
//------------------------------------------------------------------------
#define	STATE_CONSTRUCTOR_BEGIN    STATE_FUNCTION_BEGIN_INTERNAL( Constructor )
#define	STATE_CONSTRUCTOR_END      STATE_FUNCTION_END

#define	STATE_DESTRUCTOR_BEGIN     STATE_FUNCTION_BEGIN_INTERNAL( Destructor )
#define	STATE_DESTRUCTOR_END       STATE_FUNCTION_END

#define	STATE_UPDATE_BEGIN         STATE_FUNCTION_BEGIN_INTERNAL( Update )
#define	STATE_UPDATE_END           STATE_FUNCTION_END

#define	STATE_FUNCTION_BEGIN_INTERNAL( FunctionName ) \
        } else if (__functionName == Fsm<FsmOwner>::Function_##FunctionName)\
        {\
            __returnCode = FSR_FunctionHandled;

//------------------------------------------------------------------------
// 
// Implement a custom function of a state.
// 
//------------------------------------------------------------------------
#define	STATE_FUNCTION_BEGIN( FunctionName ) \
        } else if (__functionName == FunctionName)\
        {\
            __returnCode = FSR_FunctionHandled;\

#define STATE_FUNCTION_END



//------------------------------------------------------------------------
// 
// Call the parent state of this state for the current function.
//
// Note: The state must be virtual.
//       The super typedef must exists.
// 
//------------------------------------------------------------------------
#define CALL_PARENT_STATE( StateName )\
    if (super::StateName##_State(__functionName) == FSR_FunctionInterrupted)\
    {\
        return FSR_FunctionInterrupted;\
    }


#endif
