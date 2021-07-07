#pragma once

namespace bav::TreeReflectorHelpers
{

String makePropertyNameForElement (const String& propertyName, int& index);

int getNumElementsOfType (const String& propertyName, const ValueTree& tree);

}
