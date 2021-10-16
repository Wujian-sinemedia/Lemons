
/** @ingroup preset_library
 *  @{
 */

/** @ingroup sub_libraries
 *  @{
 */
namespace lemons::plugin::presets
{
FactoryPresetsManager::FactoryPresetsManager (StateBase&    stateToUse,
                                              const String& companyNameToUse,
                                              const String& productNameToUse,
                                              const String& presetFileExtensionToUse)
    : SubLibrary (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse)
{
    refreshPresetList();
}

void FactoryPresetsManager::refreshPresetList()
{
}

}  // namespace lemons::plugin::presets

/** @}*/

/** @}*/
