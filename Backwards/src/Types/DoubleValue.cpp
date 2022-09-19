/*
BSD 3-Clause License

Copyright (c) 2022, Thomas DiModica
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "Backwards/Types/ValueType.h"

#include "Backwards/Types/DoubleValue.h"
#include "Backwards/Types/StringValue.h"
#include "Backwards/Types/ArrayValue.h"
#include "Backwards/Types/DictionaryValue.h"
#include "Backwards/Types/FunctionValue.h"

#include <cmath>
#include <functional>

namespace Backwards
 {

namespace Types
 {

   DoubleValue::DoubleValue() : value(SlowFloat::SlowFloat())
    {
    }

   DoubleValue::DoubleValue(const SlowFloat::SlowFloat& value) : value(value)
    {
    }

   const std::string& DoubleValue::getTypeName() const
    {
      static const std::string name ("Double");
      return name;
    }

   std::shared_ptr<ValueType> DoubleValue::neg () const
    {
      return std::make_shared<DoubleValue>(-value);
    }

   bool DoubleValue::logical () const
    {
      return (SlowFloat::SlowFloat() == value) ? false : true;
    }

   std::shared_ptr<ValueType> DoubleValue::add (const DoubleValue& lhs) const
    {
      return std::make_shared<DoubleValue>(lhs.value + value);
    }

   std::shared_ptr<ValueType> DoubleValue::sub (const DoubleValue& lhs) const
    {
      return std::make_shared<DoubleValue>(lhs.value - value);
    }

   std::shared_ptr<ValueType> DoubleValue::mul (const DoubleValue& lhs) const
    {
      return std::make_shared<DoubleValue>(lhs.value * value);
    }

   std::shared_ptr<ValueType> DoubleValue::div (const DoubleValue& lhs) const
    {
      return std::make_shared<DoubleValue>(lhs.value / value);
    }

   std::shared_ptr<ValueType> DoubleValue::power (const DoubleValue& lhs) const
    {
      return std::make_shared<DoubleValue>(SlowFloat::SlowFloat(std::pow(static_cast<double>(lhs.value), static_cast<double>(value))));
    }

   bool DoubleValue::greater (const DoubleValue& lhs) const
    {
      return lhs.value > value;
    }

   bool DoubleValue::less (const DoubleValue& lhs) const
    {
      return lhs.value < value;
    }

   bool DoubleValue::geq (const DoubleValue& lhs) const
    {
      return lhs.value >= value;
    }

   bool DoubleValue::leq (const DoubleValue& lhs) const
    {
      return lhs.value <= value;
    }

   bool DoubleValue::equal (const DoubleValue& lhs) const
    {
      return lhs.value == value;
    }

   bool DoubleValue::notEqual (const DoubleValue& lhs) const
    {
      return lhs.value != value;
    }

   IMPLEMENTVISITOR(DoubleValue)

   bool DoubleValue::sort (const DoubleValue& lhs) const
    {
      return lhs.value < value;
    }

   bool DoubleValue::sort (const StringValue&) const
    {
      return false;
    }

   bool DoubleValue::sort (const ArrayValue&) const
    {
      return false;
    }

   bool DoubleValue::sort (const DictionaryValue&) const
    {
      return false;
    }

   bool DoubleValue::sort (const FunctionValue&) const
    {
      return false;
    }

   size_t DoubleValue::hash() const
    {
      return std::hash<double>()(static_cast<double>(value));
    }

 } // namespace Types

 } // namespace Backwards
