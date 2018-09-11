#ifndef UTILITIES_FSM_FSM_H__INCLUDED
#define UTILITIES_FSM_FSM_H__INCLUDED


//------------------------------------------------------------------------
///
/// Return type of a state to indicate what happen in it.
///
//------------------------------------------------------------------------
enum E_FsmStateResult
{
    // The function doesn't exist, so it is not handled.
    FSR_FunctionNotHandled,

    // The function exist and have been handled.
    FSR_FunctionHandled,

    // The function is handled but it have been interrupted by a switch state.
    FSR_FunctionInterrupted
};


//------------------------------------------------------------------------
/// 
/// Fsm : Finite State Machine.
///
/// A state is a member function of class.  All the states of the Fsm must
/// belong to the same class.
///
/// A state is defined by a constructor, a destructor and an update function.
/// When you switch between states, the destructor of the current is called
/// before the constructor of the next.
///
/// A function of a state is only defined by an integer value passed to the
/// state. So, you can call your own function on the current state.
///
/// A set of macros are provided to ease the use of the Fsm inside a class.
///
//------------------------------------------------------------------------
template < typename T >
class Fsm
{
public:
    enum E_Function
    {
        Function_Update         = 10000,
        Function_Constructor    = 10001,
        Function_Destructor     = 10002
    };

    typedef E_FsmStateResult (T::*State)(unsigned functionName);

public:
    Fsm();

    void Init(T * object, State initialState);
   
    void Update();

    void SetCurrentState(State newState);

    void SwitchState(State nextState);

    void SwitchToPreviousState();

    void UnsetToState(State nextState);

    void UnsetToPreviousState();

    E_FsmStateResult CallCurrentState(unsigned functionName);

    bool    IsCurrentState(State state) const;   
    bool    IsPreviousState(State state) const;  

    T *     GetObject();                 
    State   GetCurrentState();           
    State   GetPreviousState();       

private:
    T *     m_object;
    State   m_currentState;
    State   m_previousState;
};

#include "Fsm.hpp"

#endif
