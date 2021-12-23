/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

namespace lemons::plugin
{

ProcessorAttributes ProcessorAttributes::fromProjectDefines()
{
    ProcessorAttributes attributes;
    
#ifdef JucePlugin_WantsMidiInput
    attributes.acceptsMidi = JucePlugin_WantsMidiInput;
#endif
    
#ifdef JucePlugin_ProducesMidiOutput
    attributes.producesMidi = JucePlugin_ProducesMidiOutput;
#endif
    
#ifdef JucePlugin_IsMidiEffect
    attributes.isMidiEffect = JucePlugin_IsMidiEffect;
#endif
    
#ifdef JucePlugin_Name
    attributes.name = JucePlugin_Name;
#endif

    return attributes;
}

}

