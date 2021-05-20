
namespace bav
{
/* Calls a function synchronously on a background thread */

void callInBackground (std::function< void() > function);


/* Calls a function with a single argument synchronously on a background thread */

template < typename ArgumentType >
void callInBackground (std::function< void (ArgumentType) > function,
                       ArgumentType                         argument);


/*===================================================================================================*/


/* Calls a function asynchronously on the message thread */

void callOnMessageThread (std::function< void() > function);


/* Calls a function with a single argument asynchronously on the message thread */

template < typename ArgumentType >
void callOnMessageThread (std::function< void (ArgumentType) > function,
                          ArgumentType                         argument);


/*===================================================================================================*/


/* Calls a function on the message thread after a specified delay */

void callDelayed (std::function< void() > callback, int msInFuture);


/* Calls a function with a single argument on the message thread after a specified delay */

template < typename ArgumentType >
void callDelayed (std::function< void (ArgumentType) > function,
                  ArgumentType                         argument,
                  int                                  msInFuture);


/*===================================================================================================*/


/* Calls a function on a background thread after a specified delay */

void callInBackgroundDelayed (std::function< void() > function, int msInFuture);


/* Calls a function with a single argument on a background thread after a specified delay */

template < typename ArgumentType >
void callInBackgroundDelayed (std::function< void (ArgumentType) > function,
                              ArgumentType                         argument,
                              int                                  msInFuture);


}  // namespace bav
