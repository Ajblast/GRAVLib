#include "Events/SimpleEventHandleGenerator.h"

GRAVLib::Events::simpleEventHandleGenerator::~simpleEventHandleGenerator() {}

GRAVLib::Events::eventHandle GRAVLib::Events::simpleEventHandleGenerator::generate()
{
    return m_CurrentHandle++;
}
