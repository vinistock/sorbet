#ifndef SORBET_INSTRUCTIONS_H
#define SORBET_INSTRUCTIONS_H

#include "cfg/LocalRef.h"
#include "core/Context.h"
#include "core/GlobalState.h"
#include "core/LocalVariable.h"
#include "core/NameRef.h"
#include "core/Types.h"
#include <climits>
#include <memory>

namespace sorbet::cfg {
class VariableUseSite final {
public:
    LocalRef variable;
    core::TypePtr type;
    VariableUseSite() = default;
    VariableUseSite(LocalRef local) : variable(local){};
    VariableUseSite(const VariableUseSite &) = delete;
    const VariableUseSite &operator=(const VariableUseSite &rhs) = delete;
    VariableUseSite(VariableUseSite &&) = default;
    VariableUseSite &operator=(VariableUseSite &&rhs) = default;
    std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};

// TODO: convert it to implicitly numbered instead of explicitly bound
// implicitly numbered: result of every instruction can be uniquely referenced
// by its position in a linear array.

// When adding a new subtype, see if you need to add it to fillInBlockArguments
class Instruction {
public:
    virtual ~Instruction() = default;
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const = 0;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const = 0;
    Instruction() = default;
    bool isSynthetic = false;
};

template <class To> To *cast_instruction(Instruction *what) {
    static_assert(!std::is_pointer<To>::value, "To has to be a pointer");
    static_assert(std::is_assignable<Instruction *&, To *>::value,
                  "Ill Formed To, has to be a subclass of Instruction");
    return fast_cast<Instruction, To>(what);
}

template <class To> bool isa_instruction(Instruction *what) {
    return cast_instruction<To>(what) != nullptr;
}

class Ident final : public Instruction {
public:
    LocalRef what;

    Ident(LocalRef what);
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Ident, 16, 8);

class Alias final : public Instruction {
public:
    core::SymbolRef what;
    core::NameRef name;

    Alias(core::SymbolRef what, core::NameRef name = core::NameRef::noName());

    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Alias, 24, 8);

class SolveConstraint final : public Instruction {
public:
    LocalRef send;
    std::shared_ptr<core::SendAndBlockLink> link;
    SolveConstraint(const std::shared_ptr<core::SendAndBlockLink> &link, LocalRef send) : send(send), link(link){};
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(SolveConstraint, 32, 8);

class Send final : public Instruction {
public:
    bool isPrivateOk;
    u2 numPosArgs;
    core::NameRef fun;
    LocalRef recv;
    core::TypePtr recvType;
    core::LocOffsets receiverLoc;
    InlinedVector<VariableUseSite, 2> args;
    InlinedVector<core::LocOffsets, 2> argLocs;
    std::shared_ptr<core::SendAndBlockLink> link;

    Send(LocalRef recv, core::NameRef fun, core::LocOffsets receiverLoc, u2 numPosArgs,
         const InlinedVector<LocalRef, 2> &args, InlinedVector<core::LocOffsets, 2> argLocs, bool isPrivateOk = false,
         const std::shared_ptr<core::SendAndBlockLink> &link = nullptr);

    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Send, 144, 8);

class Return final : public Instruction {
public:
    LocalRef variable;
    core::TypePtr type;

    Return(LocalRef what);
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Return, 32, 8);

class BlockReturn final : public Instruction {
public:
    LocalRef variable;
    core::TypePtr type;
    std::shared_ptr<core::SendAndBlockLink> link;

    BlockReturn(std::shared_ptr<core::SendAndBlockLink> link, LocalRef what);
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(BlockReturn, 48, 8);

class LoadSelf final : public Instruction {
public:
    LocalRef fallback;
    std::shared_ptr<core::SendAndBlockLink> link;
    LoadSelf(std::shared_ptr<core::SendAndBlockLink> link, LocalRef fallback);
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(LoadSelf, 32, 8);

class Literal final : public Instruction {
public:
    core::TypePtr value;

    Literal(const core::TypePtr &value);
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Literal, 32, 8);

class GetCurrentException : public Instruction {
public:
    GetCurrentException() {
        categoryCounterInc("cfg", "GetCurrentException");
    };
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(GetCurrentException, 16, 8);

class LoadArg final : public Instruction {
public:
    u2 argId;
    core::SymbolRef method;

    LoadArg(core::SymbolRef method, u2 argId) : argId(argId), method(method) {
        categoryCounterInc("cfg", "loadarg");
    };

    const core::ArgInfo &argument(const core::GlobalState &gs) const;
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(LoadArg, 16, 8);

class ArgPresent final : public Instruction {
public:
    u2 argId;
    core::SymbolRef method;

    ArgPresent(core::SymbolRef method, u2 argId) : argId(argId), method(method) {
        categoryCounterInc("cfg", "argpresent");
    }

    const core::ArgInfo &argument(const core::GlobalState &gs) const;
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(ArgPresent, 16, 8);

class LoadYieldParams final : public Instruction {
public:
    std::shared_ptr<core::SendAndBlockLink> link;

    LoadYieldParams(const std::shared_ptr<core::SendAndBlockLink> &link) : link(link) {
        categoryCounterInc("cfg", "loadarg");
    };
    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(LoadYieldParams, 32, 8);

class Cast final : public Instruction {
public:
    core::NameRef cast;
    LocalRef variable;
    core::TypePtr resultType; // TODO(froydnj) is this is the result type or the cast type?
    core::TypePtr type;

    Cast(LocalRef value, const core::TypePtr &type, core::NameRef cast) : cast(cast), variable(value), type(type) {}

    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(Cast, 56, 8);

class TAbsurd final : public Instruction {
public:
    LocalRef variable;
    core::TypePtr type;

    TAbsurd(LocalRef what) : variable(what) {
        categoryCounterInc("cfg", "tabsurd");
    }

    virtual std::string toString(const core::GlobalState &gs, const CFG &cfg) const;
    virtual std::string showRaw(const core::GlobalState &gs, const CFG &cfg, int tabs = 0) const;
};
CheckSize(TAbsurd, 32, 8);

} // namespace sorbet::cfg

#endif // SORBET_CFG_H
