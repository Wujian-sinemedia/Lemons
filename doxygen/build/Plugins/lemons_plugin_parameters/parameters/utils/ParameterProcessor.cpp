
/** @ingroup parameters
 *  @{
 */

/** @ingroup utils
 *  @{
 */

namespace lemons::plugin
{
template < typename SampleType >
ParameterProcessor< SampleType >::ParameterProcessor (ParameterList& listToUse)
    : list (listToUse)
{
}

template < typename SampleType >
void ParameterProcessor< SampleType >::handleMidiMessage (const MidiMessage& m)
{
    list.processMidiMessage (m);
}

template class ParameterProcessor< float >;
template class ParameterProcessor< double >;

}  // namespace lemons::plugin

/** @}*/

/** @}*/
