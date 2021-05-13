

namespace bav
{


/* calls a function synchronously on a background thread */
void callInBackground (std::function< void() > function);



/* calls a function on the message thread after a specified delay */
void delayedLambda (std::function< void() > callback, int msInFuture);



}  // namespace
