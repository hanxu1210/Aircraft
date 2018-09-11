#ifndef UTILITIES_FSM_TRANSITIONCLUSTERMACROS_H__INCLUDED
#define UTILITIES_FSM_TRANSITIONCLUSTERMACROS_H__INCLUDED


//------------------------------------------------------------------------
// 
// Declare a transition cluster (in the .h).
// 
//------------------------------------------------------------------------
#define	DECLARE_TRANSITION_CLUSTER( ClusterName )\
    E_FsmStateResult ClusterName##_TransitionCluster(int __functionName)

#define	DECLARE_VIRTUAL_TRANSITION_CLUSTER( ClusterName )\
    virtual DECLARE_TRANSITION_CLUSTER( ClusterName )


//------------------------------------------------------------------------
// 
// Implement a transition cluster (in the .cpp).
// 
//------------------------------------------------------------------------
#define	IMPLEMENT_TRANSITION_CLUSTER_BEGIN( ClassName, ClusterName )\
    E_FsmStateResult ClassName::ClusterName##_TransitionCluster(int __functionName)\
    {

#define	IMPLEMENT_TRANSITION_CLUSTER_END\
        return FSR_FunctionHandled;\
    }


//------------------------------------------------------------------------
// 
// Call a transition cluster inside a state.
// 
//------------------------------------------------------------------------
#define CALL_TRANSITION_CLUSTER( ClusterName )\
    assert(__functionName != Fsm<FsmOwner>::Function_Constructor && __functionName != Fsm<FsmOwner>::Function_Destructor );\
    if (ClusterName##_TransitionCluster(__functionName) == FSR_FunctionInterrupted)\
    {\
        return FSR_FunctionInterrupted;\
    }


//------------------------------------------------------------------------
// 
// Call the parent transition cluster.
//
// Note: The cluster must be virtual.
//       The super typedef must exists.
// 
//------------------------------------------------------------------------
#define CALL_PARENT_TRANSITION_CLUSTER( ClusterName )\
    if (super::ClusterName##_TransitionCluster(__functionName) == FSR_FunctionInterrupted)\
    {\
        return FSR_FunctionInterrupted;\
    }


#endif
