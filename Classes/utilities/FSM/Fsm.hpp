#ifndef UTILITIES_FSM_FSM_HPP__INCLUDED
#define UTILITIES_FSM_FSM_HPP__INCLUDED

template < typename T >
Fsm< T >::Fsm() 
    : m_object(NULL), m_currentState(NULL), m_previousState(NULL)
{
}

//------------------------------------------------------------------------
// 
// Set the object and the initial state.
// You must call this function only one time and before any other
// function of the Fsm.
//
//------------------------------------------------------------------------
template < typename T >
void Fsm< T >::Init(T * object, State initialState)
{
    //assert( m_object == NULL );
    assert( object );
    m_object = object;

    assert( initialState );
    m_previousState = initialState;
    m_currentState = initialState;
    CallCurrentState(Function_Constructor);
}

//------------------------------------------------------------------------
// 
// Update of the current state.
// 
//------------------------------------------------------------------------
template < typename T >
void Fsm< T >::Update()
{
    CallCurrentState(Function_Update);
}

//------------------------------------------------------------------------
// 
// Set a new state without passing by the destructor of the current state.
// 
//------------------------------------------------------------------------
template < typename T >
void Fsm< T >::SetCurrentState(State newState)
{
    m_previousState = m_currentState;
    m_currentState = newState;
    CallCurrentState(Function_Constructor);
}

//------------------------------------------------------------------------
// 
// Switch the current state to a new state.
// 
//------------------------------------------------------------------------
template < typename T >
void Fsm< T >::SwitchState(State nextState)
{
    CallCurrentState(Function_Destructor);
    m_previousState = m_currentState;
    m_currentState = nextState;
    CallCurrentState(Function_Constructor);
}

//------------------------------------------------------------------------
// 
// Switch the current state to the previous state.
// 
//------------------------------------------------------------------------
/*template < typename T >
void Fsm< T >::SwitchToPreviousState()
{
    CallCurrentState(Function_Destructor);
    Gear::Swap(m_currentState, m_previousState);
    CallCurrentState(Function_Constructor);
}*/

//------------------------------------------------------------------------
// 
// Call the destructor of the current state and set to a new state
// without passing by its constructor.
// 
//------------------------------------------------------------------------
template < typename T >
void Fsm< T >::UnsetToState(State nextState)
{
    CallCurrentState(Function_Destructor);
    m_previousState = m_currentState;
    m_currentState = nextState;
}

//------------------------------------------------------------------------
// 
// Call the destructor of the current state and set to the previous state
// without passing by its constructor.
// 
//------------------------------------------------------------------------
/*template < typename T >
void Fsm< T >::UnsetToPreviousState()
{
    CallCurrentState(Function_Destructor);
    Gear::Swap(m_currentState, m_previousState);
}*/

//------------------------------------------------------------------------
// 
// Call a specific function of the current state.
// 
//------------------------------------------------------------------------
template < typename T >
E_FsmStateResult Fsm< T >::CallCurrentState(unsigned functionName)
{
    assert(m_object && m_currentState);
    return (m_object->*m_currentState)(functionName);
}

//------------------------------------------------------------------------
// 
// Test equality with the current state or previous state.
// 
//------------------------------------------------------------------------
template < typename T >
inline bool Fsm< T >::IsCurrentState(State state) const   
{
    return m_currentState == state;
}
    
template < typename T >
inline bool Fsm< T >::IsPreviousState(State state) const
{
    return m_previousState == state;
}

template < typename T >
inline T * Fsm< T >::GetObject()                     
{
    return m_object;
}
    
template < typename T >
inline typename Fsm< T >::State Fsm< T >::GetCurrentState()               
{
    return m_currentState;
}
    
template < typename T >
inline typename Fsm< T >::State Fsm< T >::GetPreviousState()              
{
    return m_previousState;
}


#endif
