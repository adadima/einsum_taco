//
// Created by Alexandra Dima on 10/16/21.
//

#include <einsum_taco/ir/type.h>

namespace einsum {
    Datatype::Datatype(Kind kind) : kind(kind) {
    }

    Datatype::Kind Datatype::getKind() const {
        return this->kind;
    }

    bool Datatype::isBool() const {
        return this->getKind() == Kind::Bool;
    }

    bool Datatype::isInt() const {
        return this->getKind() == Kind::Int;
    }

    bool Datatype::isFloat() const {
        return this->getKind() == Kind::Float;
    }

    std::string Datatype::dump() const {
        switch(this->getKind()) {
            case Kind::Bool:
                return "bool";
            case Kind::Int:
                return "int";
            case Kind::Float:
                return "float";
        }
    }

    bool TupleType::isBool() const {
        return false;
    }

    bool TupleType::isInt() const {
        return false;
    }

    bool TupleType::isFloat() const {
        return false;
    }

    std::string TupleType::dump() const {
        std::string types;
        for (int i=0; i < this->tuple.size(); i++) {
            types += this->tuple[i]->dump();
            if (i < this->tuple.size() - 1) {
                types += ", ";
            }
        }
        return "(" + types + ")";
    }

    bool TensorType::isBool() const {
        return false;
    }

    bool TensorType::isInt() const {
        return false;
    }

    bool TensorType::isFloat() const {
        return false;
    }

    bool DimensionType::isFloat() const {
        return false;
    }

    bool DimensionType::isInt() const {
        return false;
    }

    bool DimensionType::isBool() const {
        return false;
    }

    std::string TensorType::dump() const {
        std::string dims;
        for (const auto &dimension : this->dimensions) {
            dims += "[" + dimension->dump() + "]";
        }
        return this->getElementType()->dump() + dims;
    }

    std::string FixedDimension::dump() const {
        return std::to_string(this->value);
    }

    std::string VariableDimension::dump() const {
        return this->varName;
    }

    std::string BinaryExpressionDimension::dump() const {
        return this->left->dump() + " " + this->op->sign + " " + this->right->dump();
    }

    size_t Datatype::getNumBytes() const {
        switch(this->getKind()) {
            case Kind::Bool:
                return sizeof(bool);
            case Kind::Int:
                return sizeof(int);
            case Kind::Float:
                return sizeof(double);
        }
    }

    std::shared_ptr<einsum::DimensionType> TensorType::getDimension(int i) const {
        return this->dimensions[i];
    }

    std::shared_ptr<Datatype> TensorType::getElementType() const {
        return this->type;
    }

    int TensorType::getOrder() const {
        return (int) this->dimensions.size();
    }
}