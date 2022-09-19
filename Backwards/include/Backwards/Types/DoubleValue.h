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
#ifndef BACKWARDS_TYPES_DOUBLEVALUE_H
#define BACKWARDS_TYPES_DOUBLEVALUE_H

#include "SlowFloat.h"
#include "Backwards/Types/ValueType.h"

namespace Backwards
 {

namespace Types
 {

   class DoubleValue final : public ValueType
    {

   public:
      SlowFloat::SlowFloat value;

      DoubleValue();
      DoubleValue(const SlowFloat::SlowFloat& value);

      const std::string& getTypeName() const;

      std::shared_ptr<ValueType> neg() const;
      bool logical() const;

      std::shared_ptr<ValueType> add (const DoubleValue& lhs) const;
      std::shared_ptr<ValueType> sub (const DoubleValue& lhs) const;
      std::shared_ptr<ValueType> mul (const DoubleValue& lhs) const;
      std::shared_ptr<ValueType> div (const DoubleValue& lhs) const;
      std::shared_ptr<ValueType> power (const DoubleValue& lhs) const;

      bool greater (const DoubleValue& lhs) const;
      bool less (const DoubleValue& lhs) const;
      bool geq (const DoubleValue& lhs) const;
      bool leq (const DoubleValue& lhs) const;
      bool equal (const DoubleValue& lhs) const;
      bool notEqual (const DoubleValue& lhs) const;

      DECLAREVISITOR

    };

 } // namespace Types

 } // namespace Backwards

#endif /* BACKWARDS_TYPES_DOUBLEVALUE_H */
