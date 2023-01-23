#pragma once

#include <pl/patterns/pattern.hpp>

namespace pl::ptrn {

    class PatternSigned : public Pattern {
    public:
        PatternSigned(core::Evaluator *evaluator, u64 offset, size_t size)
            : Pattern(evaluator, offset, size) { }

        [[nodiscard]] std::unique_ptr<Pattern> clone() const override {
            return std::unique_ptr<Pattern>(new PatternSigned(*this));
        }

        [[nodiscard]] core::Token::Literal getValue() const override {
            i128 data = 0;
            this->getEvaluator()->readData(this->getOffset(), &data, this->getSize(), this->getSection());
            data = hlp::changeEndianess(data, this->getSize(), this->getEndian());

            return hlp::signExtend(this->getSize() * 8, data);
        }

        [[nodiscard]] std::string getFormattedName() const override {
            return this->getTypeName();
        }

        [[nodiscard]] bool operator==(const Pattern &other) const override { return areCommonPropertiesEqual<decltype(*this)>(other); }

        void accept(PatternVisitor &v) override {
            v.visit(*this);
        }

        std::string getFormattedValue() override {
            auto data = this->getValue().toSigned();
            auto size = this->getSize();

            return this->formatDisplayValue(fmt::format("{:d} (0x{:0{}X})", data, u128(data) & hlp::bitmask(8 * size), size * 2), this->getValue());
        }

        [[nodiscard]] std::string toString() const override {
            auto value = this->getValue();
            auto result = fmt::format("{:d}", value.toSigned());

            return this->formatDisplayValue(result, value);
        }
    };

}