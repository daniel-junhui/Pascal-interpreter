// Copyright 2023 Zhu Junhui

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "meta.h"
#include "ValueAST.h"
#include "NonValueAST.h"

namespace Pascal {
class Visitor : public ValueASTVisitor, public NonValueASTVisitor {};

class Checker : public ValueASTChecker, public NonValueASTChecker {};
}  // namespace Pascal
