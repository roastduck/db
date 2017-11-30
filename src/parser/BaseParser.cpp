#include "BaseParser.h"

void BaseParser::createDb(const std::string &name)
{
    tableMgr->createDb(name);
}
