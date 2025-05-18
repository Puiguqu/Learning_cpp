#include <iostream> // For standard I/O (cout, endl)
#include <string>   // For using std::string
#include <vector>   // For using std::vector (though not directly used in final FSM, good for general C++)
#include <map>      // For std::map, used for transition table and state actions
#include <functional> // For std::function, to store callable actions
#include <stdexcept>  // For std::runtime_error (optional, for more robust error handling)

// --- Configuration ---

// 1. Define States for the FSM
// Using 'enum class' provides type safety and better scoping compared to plain 'enum'.
enum class State {
    START,
    STATE_A,
    STATE_B,
    ACCEPT, // An example of an accepting/final state
    REJECT, // An example of a rejecting/error state
    // Add more states as needed
    INVALID // A special state to indicate an undefined or error condition internally
};

// Helper function to convert State enum to a printable string.
// This is useful for debugging and logging.
std::string stateToString(State s) {
    switch (s) {
        case State::START:   return "START";
        case State::STATE_A: return "STATE_A";
        case State::STATE_B: return "STATE_B";
        case State::ACCEPT:  return "ACCEPT";
        case State::REJECT:  return "REJECT";
        case State::INVALID: return "INVALID";
        default:             return "UNKNOWN_STATE";
    }
}

// 2. Define Input Alphabet
// For this example, we'll use 'char' as input symbols.
// You could also use 'int', 'std::string', or another 'enum class' for more complex inputs.
using Input = char;

// 3. Define Actions (Optional)
// Actions are functions that can be executed upon entering a state or during a transition.
// 'std::function<void()>' allows us to store any callable that takes no arguments and returns void
// (e.g., free functions, lambdas, bound member functions).
using Action = std::function<void()>;

// --- FSM Core Components ---

// Structure to hold information about a transition.
// This includes the state to transition to and an optional action to perform during the transition.
struct TransitionInfo {
    State nextState;
    Action action; // Action to perform specifically for this transition (Mealy-like behavior)

    // Constructor for TransitionInfo
    TransitionInfo(State next, Action act = nullptr)
        : nextState(next), action(std::move(act)) {}
};

// The Finite State Machine Class
class FiniteStateMachine {
public:
    // Constructor: Initializes the FSM with a starting state.
    FiniteStateMachine(State initialState)
        : currentState_(initialState), initialState_(initialState) {
        std::cout << "FSM Initialized. Initial state: " << stateToString(currentState_) << std::endl;
        // Optionally, perform an action upon entering the initial state right away.
        performStateEntryAction(currentState_);
    }

    // Adds a transition rule to the FSM.
    // 'fromState': The state from which the transition originates.
    // 'inputSymbol': The input that triggers this transition.
    // 'toState': The state to which the FSM transitions.
    // 'transitionAction': An optional action to execute when this specific transition occurs.
    void addTransition(State fromState, Input inputSymbol, State toState, Action transitionAction = nullptr) {
        transitions_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(fromState, inputSymbol),
            std::forward_as_tuple(toState, std::move(transitionAction))
        );
        std::cout << "Added transition: " << stateToString(fromState)
                  << " --(" << inputSymbol << ")--> " << stateToString(toState) << std::endl;
    }

    // Adds an action to be performed upon entering a specific state (Moore-like behavior).
    // 'state': The state for which to add the entry action.
    // 'entryAction': The action to perform when 'state' is entered.
    void addStateEntryAction(State state, Action entryAction) {
        stateEntryActions_[state] = std::move(entryAction);
        std::cout << "Added entry action for state: " << stateToString(state) << std::endl;
    }

    // Processes a single input symbol.
    // 'inputSymbol': The input to process.
    void processInput(Input inputSymbol) {
        std::cout << "\nProcessing input: '" << inputSymbol << "' from current state: " << stateToString(currentState_) << std::endl;

        // Create a key to look up in the transitions_ map.
        std::pair<State, Input> transitionKey = {currentState_, inputSymbol};
        auto it = transitions_.find(transitionKey);

        if (it != transitions_.end()) {
            // Transition is defined
            const TransitionInfo& transInfo = it->second; // Get the transition details
            State previousState = currentState_;
            currentState_ = transInfo.nextState; // Update current state

            std::cout << "Transitioning: " << stateToString(previousState)
                      << " --(" << inputSymbol << ")--> " << stateToString(currentState_) << std::endl;

            // 1. Execute transition-specific action (if any)
            if (transInfo.action) {
                std::cout << "Executing transition-specific action..." << std::endl;
                transInfo.action();
            }

            // 2. Execute new state's entry action (if any)
            performStateEntryAction(currentState_);

        } else {
            // No transition defined for the current state and input
            std::cout << "No transition defined for state " << stateToString(currentState_)
                      << " with input '" << inputSymbol << "'. Staying in current state (or transitioning to a REJECT state if defined)." << std::endl;
            // Optional: Transition to a REJECT or error state if desired.
            // For example, if you have a catch-all transition from currentState_ for any other input:
            // auto catchAllIt = transitions_.find({currentState_, '*'}); // Assuming '*' is a wildcard input
            // if (catchAllIt != transitions_.end()) { /* handle wildcard */ }
            // else { currentState_ = State::REJECT; performStateEntryAction(currentState_); }
        }
    }

    // Processes a sequence of input symbols.
    // 'inputSequence': A string where each character is an input symbol.
    void processInputSequence(const std::string& inputSequence) {
        std::cout << "\n--- Processing input sequence: \"" << inputSequence << "\" ---" << std::endl;
        for (char symbol : inputSequence) {
            processInput(symbol);
            // Optional: Stop processing if a terminal state (like ACCEPT or REJECT) is reached
            // and further input is not meaningful for this FSM design.
            if (currentState_ == State::ACCEPT || currentState_ == State::REJECT) {
                std::cout << "Reached a terminal state (" << stateToString(currentState_) << "). Further inputs in sequence might be ignored by design." << std::endl;
                // break; // Uncomment if you want to stop processing the sequence
            }
        }
        std::cout << "--- Finished processing sequence ---" << std::endl;
    }

    // Returns the current state of the FSM.
    State getCurrentState() const {
        return currentState_;
    }

    // Resets the FSM to its initial state.
    // Also performs the entry action for the initial state.
    void reset() {
        std::cout << "\n--- FSM Reset ---" << std::endl;
        currentState_ = initialState_;
        std::cout << "FSM reset to initial state: " << stateToString(currentState_) << std::endl;
        performStateEntryAction(currentState_); // Perform entry action of the initial state
    }

private:
    State currentState_; // The current state of the FSM
    State initialState_; // The state the FSM starts in or resets to

    // Transition Table:
    // Maps a pair of (CurrentState, InputSymbol) to TransitionInfo (NextState + optional Action).
    std::map<std::pair<State, Input>, TransitionInfo> transitions_;

    // State Entry Actions:
    // Maps a State to an Action to be performed upon entering that state.
    std::map<State, Action> stateEntryActions_;

    // Helper function to execute the entry action for a given state, if one exists.
    void performStateEntryAction(State state) {
        auto actionIt = stateEntryActions_.find(state);
        if (actionIt != stateEntryActions_.end() && actionIt->second) { // Check if action exists and is not null
            std::cout << "Executing entry action for state " << stateToString(state) << "..." << std::endl;
            actionIt->second(); // Call the action
        }
    }
};

// --- Example Usage ---

// Define some action functions
void actionOnEnterStart() {
    std::cout << "Action: Welcome! FSM is at the START." << std::endl;
}

void actionOnEnterStateA() {
    std::cout << "Action: Entered STATE_A. Performing task A..." << std::endl;
}

void actionOnEnterStateB() {
    std::cout << "Action: Entered STATE_B. Performing task B..." << std::endl;
}

void actionOnEnterAccept() {
    std::cout << "Action: Entered ACCEPT state. String accepted!" << std::endl;
}

void actionOnEnterReject() {
    std::cout << "Action: Entered REJECT state. String rejected or error." << std::endl;
}

void specificTransitionAction_A_to_B() {
    std::cout << "TransitionAction: Special processing for A --'1'--> B." << std::endl;
}


int main() {
    // 1. Create an FSM instance with an initial state.
    FiniteStateMachine fsm(State::START);

    // 2. Define state-entry actions (Moore-like behavior).
    fsm.addStateEntryAction(State::START, actionOnEnterStart);
    fsm.addStateEntryAction(State::STATE_A, actionOnEnterStateA);
    fsm.addStateEntryAction(State::STATE_B, actionOnEnterStateB);
    fsm.addStateEntryAction(State::ACCEPT, actionOnEnterAccept);
    fsm.addStateEntryAction(State::REJECT, actionOnEnterReject);
    // Example of a lambda for an action
    fsm.addStateEntryAction(State::INVALID, [](){ std::cout << "Action: Entered INVALID state! This shouldn't happen in normal flow." << std::endl; });


    // 3. Define transitions.
    // Format: addTransition(FROM_STATE, INPUT_SYMBOL, TO_STATE, optional_TRANSITION_ACTION);

    // Transitions from START state
    fsm.addTransition(State::START, 'a', State::STATE_A);
    fsm.addTransition(State::START, 'b', State::REJECT); // Example: 'b' from START leads to REJECT

    // Transitions from STATE_A
    fsm.addTransition(State::STATE_A, 'a', State::STATE_A); // Loop on 'a'
    fsm.addTransition(State::STATE_A, 'b', State::STATE_B, specificTransitionAction_A_to_B); // Transition with a specific action

    // Transitions from STATE_B
    fsm.addTransition(State::STATE_B, 'a', State::STATE_A);
    fsm.addTransition(State::STATE_B, 'b', State::ACCEPT); // 'b' leads to ACCEPT state
    fsm.addTransition(State::STATE_B, 'c', State::REJECT); // 'c' leads to REJECT

    // Transitions from ACCEPT (e.g., stay in accept or reset)
    fsm.addTransition(State::ACCEPT, 'a', State::ACCEPT); // Stay in accept
    fsm.addTransition(State::ACCEPT, 'b', State::ACCEPT); // Stay in accept

    // REJECT state usually has no outgoing transitions for valid inputs, or loops on itself.
    fsm.addTransition(State::REJECT, 'a', State::REJECT);
    fsm.addTransition(State::REJECT, 'b', State::REJECT);


    // 4. Process input sequences.
    std::cout << "\n\n======== FSM SIMULATION START ========" << std::endl;

    std::cout << "\n--- Test Case 1: Sequence 'aab' (should go START -> A -> A -> B) ---" << std::endl;
    fsm.processInputSequence("aab");
    std::cout << "Final state after 'aab': " << stateToString(fsm.getCurrentState()) << std::endl; // Expected: STATE_B

    fsm.reset(); // Reset FSM to initial state for the next test

    std::cout << "\n--- Test Case 2: Sequence 'aabba' (should reach ACCEPT) ---" << std::endl;
    fsm.processInputSequence("aabba");
    std::cout << "Final state after 'aabba': " << stateToString(fsm.getCurrentState()) << std::endl; // Expected: ACCEPT

    fsm.reset();

    std::cout << "\n--- Test Case 3: Sequence 'b' (should reach REJECT from START) ---" << std::endl;
    fsm.processInputSequence("b");
    std::cout << "Final state after 'b': " << stateToString(fsm.getCurrentState()) << std::endl; // Expected: REJECT

    fsm.reset();

    std::cout << "\n--- Test Case 4: Sequence 'axb' (undefined transition 'x' from STATE_A) ---" << std::endl;
    fsm.processInputSequence("axb");
    std::cout << "Final state after 'axb': " << stateToString(fsm.getCurrentState()) << std::endl; // Expected: STATE_A (or REJECT if you modify behavior for undefined)

    fsm.reset();

    std::cout << "\n--- Test Case 5: Sequence 'aabc' (should reach REJECT from STATE_B) ---" << std::endl;
    fsm.processInputSequence("aabc");
    std::cout << "Final state after 'aabc': " << stateToString(fsm.getCurrentState()) << std::endl; // Expected: REJECT

    std::cout << "\n\n======== FSM SIMULATION END ========" << std::endl;

    return 0;
}

