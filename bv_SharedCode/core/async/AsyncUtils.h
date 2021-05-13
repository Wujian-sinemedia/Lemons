
namespace bav
{


/* calls a function synchronously on a background thread */

void callInBackground (std::function< void() > function);


/* calls a function synchronously on a background thread with a single argument */

template <typename ArgumentType>
void callInBackground (std::function< void (ArgumentType) > function, ArgumentType argument);



/*===================================================================================================*/


/* calls a function on the message thread after a specified delay */

void callDelayed (std::function< void() > callback, int msInFuture);


/* calls a function on the message thread after a specified delay */

template<typename ArgumentType>
void callDelayed (std::function< void (ArgumentType) > function, ArgumentType argument, int msInFuture);


}  // namespace

