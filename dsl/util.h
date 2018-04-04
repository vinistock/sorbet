#ifndef SRUBY_DSL_UTIL_H
#define SRUBY_DSL_UTIL_H

#include "ast/ast.h"
#include "core/Names.h"
#include <memory>

namespace ruby_typer {
namespace dsl {
class ASTUtil {
public:
    static std::unique_ptr<ast::Expression> dupType(ast::Expression *orig);
    static std::unique_ptr<ast::Expression> getHashValue(core::MutableContext ctx, ast::Hash *hash, core::NameRef name);

    ASTUtil() = delete;
};
} // namespace dsl
} // namespace ruby_typer
#endif
