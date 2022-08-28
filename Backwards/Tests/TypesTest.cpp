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
#include "gtest/gtest.h"

#include "Backwards/Types/ValueType.h"

#include "Backwards/Types/DoubleValue.h"
#include "Backwards/Types/StringValue.h"
#include "Backwards/Types/ArrayValue.h"
#include "Backwards/Types/DictionaryValue.h"
#include "Backwards/Types/FunctionValue.h"

/*
   NOTE : The base cases for add/sub/mul/div for ArrayValue/DictionaryValue in ValueType.cpp are impossible calls.
   Those two values intercept the base call and commute first, so there can never be a type error.
*/

TEST(TypesTests, testDoubles)
 {
   Backwards::Types::DoubleValue defaulted;
   Backwards::Types::DoubleValue low (1.0);
   Backwards::Types::DoubleValue med (5.0);
   Backwards::Types::DoubleValue high (10.0);
   std::shared_ptr<Backwards::Types::ValueType> temp;

   EXPECT_EQ("Double", defaulted.getTypeName());

   EXPECT_FALSE(defaulted.logical());
   EXPECT_TRUE(low.logical());
   EXPECT_TRUE(med.logical());
   EXPECT_TRUE(high.logical());

   temp = low.neg();
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(-1.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.neg();
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(-10.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.add(med);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(15.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.sub(med); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(-5.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.mul(med);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(50.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.div(med); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(0.5, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = med.power(high);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(100000.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.add(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(15.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.sub(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(5.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.mul(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(50.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.div(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

   temp = high.power(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*temp.get()));
   EXPECT_EQ(100000.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(temp)->value);

      // Remember: operations are flipped.
   EXPECT_TRUE(med.greater(high));
   EXPECT_FALSE(med.greater(low));
   EXPECT_FALSE(med.greater(med));

   EXPECT_FALSE(med.less(high));
   EXPECT_TRUE(med.less(low));
   EXPECT_FALSE(med.less(med));

   EXPECT_TRUE(med.geq(high));
   EXPECT_FALSE(med.geq(low));
   EXPECT_TRUE(med.geq(med));

   EXPECT_FALSE(med.leq(high));
   EXPECT_TRUE(med.leq(low));
   EXPECT_TRUE(med.leq(med));

   EXPECT_FALSE(med.equal(high));
   EXPECT_FALSE(med.equal(low));
   EXPECT_TRUE(med.equal(med));

   EXPECT_TRUE(med.notEqual(high));
   EXPECT_TRUE(med.notEqual(low));
   EXPECT_FALSE(med.notEqual(med));

   EXPECT_FALSE(med.sort(high));
   EXPECT_TRUE(med.sort(low));
   EXPECT_FALSE(med.sort(med));

      // Not flipped.
   EXPECT_FALSE(med.greater(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.less(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.geq(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.leq(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.equal(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.notEqual(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.sort(dynamic_cast<Backwards::Types::ValueType&>(med)));

   Backwards::Types::StringValue v1;
   Backwards::Types::ArrayValue v2;
   Backwards::Types::DictionaryValue v3;
   Backwards::Types::FunctionValue v4;

   EXPECT_FALSE(med.sort(v1));
   EXPECT_FALSE(med.sort(v2));
   EXPECT_FALSE(med.sort(v3));
   EXPECT_FALSE(med.sort(v4));

   EXPECT_NE(static_cast<size_t>(low.value), low.hash());
 }

TEST(TypesTests, testStrings)
 {
   Backwards::Types::StringValue defaulted;
   Backwards::Types::StringValue low ("A");
   Backwards::Types::StringValue med ("M");
   Backwards::Types::StringValue high ("Z");
   std::shared_ptr<Backwards::Types::ValueType> temp;

   EXPECT_EQ("String", defaulted.getTypeName());

   EXPECT_THROW(defaulted.logical(), Backwards::Types::TypedOperationException);
   EXPECT_THROW(low.neg(), Backwards::Types::TypedOperationException);

   temp = high.add(med);
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*temp.get()));
   EXPECT_EQ("MZ", std::dynamic_pointer_cast<Backwards::Types::StringValue>(temp)->value);

   temp = high.add(dynamic_cast<Backwards::Types::ValueType&>(med));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*temp.get()));
   EXPECT_EQ("ZM", std::dynamic_pointer_cast<Backwards::Types::StringValue>(temp)->value);

      // Remember: operations are flipped.
   EXPECT_TRUE(med.greater(high));
   EXPECT_FALSE(med.greater(low));
   EXPECT_FALSE(med.greater(med));

   EXPECT_FALSE(med.less(high));
   EXPECT_TRUE(med.less(low));
   EXPECT_FALSE(med.less(med));

   EXPECT_TRUE(med.geq(high));
   EXPECT_FALSE(med.geq(low));
   EXPECT_TRUE(med.geq(med));

   EXPECT_FALSE(med.leq(high));
   EXPECT_TRUE(med.leq(low));
   EXPECT_TRUE(med.leq(med));

   EXPECT_FALSE(med.equal(high));
   EXPECT_FALSE(med.equal(low));
   EXPECT_TRUE(med.equal(med));

   EXPECT_TRUE(med.notEqual(high));
   EXPECT_TRUE(med.notEqual(low));
   EXPECT_FALSE(med.notEqual(med));

   EXPECT_FALSE(med.sort(high));
   EXPECT_TRUE(med.sort(low));
   EXPECT_FALSE(med.sort(med));

      // Not flipped.
   EXPECT_FALSE(med.greater(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.less(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.geq(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.leq(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_TRUE(med.equal(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.notEqual(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.sort(dynamic_cast<Backwards::Types::ValueType&>(med)));

   Backwards::Types::DoubleValue v1;
   Backwards::Types::ArrayValue v2;
   Backwards::Types::DictionaryValue v3;
   Backwards::Types::FunctionValue v4;

   EXPECT_TRUE(med.sort(v1));
   EXPECT_FALSE(med.sort(v2));
   EXPECT_FALSE(med.sort(v3));
   EXPECT_FALSE(med.sort(v4));

   EXPECT_THROW(high.sub(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.mul(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.div(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.power(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);

   EXPECT_NE(0U, low.hash());
 }

class DummyFOH final : public Backwards::Types::FunctionObjectHolder
 {
 };

TEST(TypesTests, testFunctions)
 {
   Backwards::Types::FunctionValue defaulted;
   std::shared_ptr<DummyFOH> l = std::make_shared<DummyFOH>();
   std::shared_ptr<DummyFOH> m = std::make_shared<DummyFOH>();
   std::shared_ptr<DummyFOH> h = std::make_shared<DummyFOH>();
   Backwards::Types::FunctionValue low (l);
   Backwards::Types::FunctionValue med (m);
   Backwards::Types::FunctionValue high (h);

   EXPECT_EQ("Function", defaulted.getTypeName());

   EXPECT_THROW(defaulted.logical(), Backwards::Types::TypedOperationException);
   EXPECT_THROW(defaulted.neg(), Backwards::Types::TypedOperationException);

      // Remember: operations are flipped.
   EXPECT_FALSE(med.equal(high));
   EXPECT_FALSE(med.equal(low));
   EXPECT_TRUE(med.equal(med));

   EXPECT_TRUE(med.notEqual(high));
   EXPECT_TRUE(med.notEqual(low));
   EXPECT_FALSE(med.notEqual(med));

   EXPECT_TRUE(med.sort(high) | high.sort(med));
   EXPECT_TRUE(med.sort(low) | low.sort(med));
   EXPECT_FALSE(med.sort(med));

      // Not flipped.
   EXPECT_TRUE(med.equal(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.notEqual(dynamic_cast<Backwards::Types::ValueType&>(med)));
   EXPECT_FALSE(med.sort(dynamic_cast<Backwards::Types::ValueType&>(med)));

   Backwards::Types::DoubleValue v1;
   Backwards::Types::StringValue v2;
   Backwards::Types::ArrayValue v3;
   Backwards::Types::DictionaryValue v4;

   EXPECT_TRUE(med.sort(v1));
   EXPECT_TRUE(med.sort(v2));
   EXPECT_TRUE(med.sort(v3));
   EXPECT_TRUE(med.sort(v4));

   EXPECT_THROW(high.add(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.sub(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.mul(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.div(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.power(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);

   EXPECT_THROW(high.greater(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.less(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.geq(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(high.leq(dynamic_cast<Backwards::Types::ValueType&>(med)), Backwards::Types::TypedOperationException);

   EXPECT_NE(0U, low.hash());
 }

TEST(TypesTests, testArrays)
 {
   Backwards::Types::ArrayValue defaulted;
   Backwards::Types::ArrayValue one;
   Backwards::Types::ArrayValue two;
   Backwards::Types::ArrayValue three;
   Backwards::Types::ArrayValue four;
   Backwards::Types::ArrayValue five;
   std::shared_ptr<Backwards::Types::ValueType> temp;

   one.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(2.0));
   two.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(4.0));
   three.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(2.0));
   three.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(4.0));
   four.value.emplace_back(std::make_shared<Backwards::Types::StringValue>("A"));
   five.value.emplace_back(std::make_shared<Backwards::Types::StringValue>("B"));

   EXPECT_EQ("Array", defaulted.getTypeName());

   EXPECT_THROW(defaulted.logical(), Backwards::Types::TypedOperationException);

   temp = one.neg();
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(-2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

    /*
      I hate myself for doing this: { 4 } + { 2 } = { { 6 } }
    */
   temp = one.add(two);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.sub(two); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.mul(two);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.div(two); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.add(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.sub(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(-2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.mul(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.div(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ(0.5, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = four.add(five); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ("BA", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = four.add(dynamic_cast<Backwards::Types::ValueType&>(five));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get()));
   EXPECT_EQ("AB", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   EXPECT_FALSE(one.equal(two));
   EXPECT_FALSE(two.equal(one));
   EXPECT_TRUE(one.equal(one));

   EXPECT_FALSE(one.equal(four));
   EXPECT_FALSE(four.equal(one));

   EXPECT_TRUE(one.notEqual(two));
   EXPECT_TRUE(two.notEqual(one));
   EXPECT_FALSE(one.notEqual(one));

   EXPECT_FALSE(one.sort(two));
   EXPECT_TRUE(two.sort(one));
   EXPECT_FALSE(one.sort(one));

   EXPECT_FALSE(one.sort(three));
   EXPECT_TRUE(three.sort(one));

   EXPECT_TRUE(one.equal(dynamic_cast<Backwards::Types::ValueType&>(one)));
   EXPECT_FALSE(one.notEqual(dynamic_cast<Backwards::Types::ValueType&>(one)));
   EXPECT_FALSE(one.sort(dynamic_cast<Backwards::Types::ValueType&>(one)));

   Backwards::Types::DoubleValue v1;
   Backwards::Types::StringValue v2;
   Backwards::Types::DictionaryValue v3;
   Backwards::Types::FunctionValue v4;

   EXPECT_TRUE(one.sort(v1));
   EXPECT_TRUE(one.sort(v2));
   EXPECT_FALSE(one.sort(v3));
   EXPECT_FALSE(one.sort(v4));

   EXPECT_THROW(two.power(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.greater(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.less(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.geq(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.leq(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);

   EXPECT_NE(0U, one.hash());

   Backwards::Types::DictionaryValue six;
   six.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(4.0)));

   temp = one.add(six);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.sub(six);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.mul(six);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.div(six);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0];
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);
 }

TEST(TypesTests, testDictionaries)
 {
   Backwards::Types::DictionaryValue defaulted;
   Backwards::Types::DictionaryValue one;
   Backwards::Types::DictionaryValue two;
   Backwards::Types::DictionaryValue three;
   Backwards::Types::DictionaryValue four;
   Backwards::Types::DictionaryValue five;
   Backwards::Types::DictionaryValue seven;
   std::shared_ptr<Backwards::Types::ValueType> temp;

   one.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(2.0)));
   two.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("B"), std::make_shared<Backwards::Types::DoubleValue>(4.0)));
   three.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(2.0)));
   three.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("B"), std::make_shared<Backwards::Types::DoubleValue>(4.0)));
   four.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::StringValue>("A")));
   five.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("B"), std::make_shared<Backwards::Types::StringValue>("B")));
   seven.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(4.0)));

   EXPECT_EQ("Dictionary", defaulted.getTypeName());

   EXPECT_THROW(defaulted.logical(), Backwards::Types::TypedOperationException);

   temp = one.neg();
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get()));
   EXPECT_EQ(-2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

    /*
      I hate myself for doing this: { "B" : 4 } + { "A" : 2 } = { "A" : { "B" : 6 } }
    */
   temp = one.add(two);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.sub(two); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.mul(two);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.div(two); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.add(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.sub(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(-2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.mul(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = one.div(dynamic_cast<Backwards::Types::ValueType&>(two));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ(0.5, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = four.add(five); // Is flipped.
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ("BA", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   temp = four.add(dynamic_cast<Backwards::Types::ValueType&>(five));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get()));
   EXPECT_EQ("B", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   EXPECT_EQ("AB", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second)->value);

   EXPECT_FALSE(one.equal(two));
   EXPECT_FALSE(two.equal(one));
   EXPECT_TRUE(one.equal(one));

   EXPECT_FALSE(one.equal(four));
   EXPECT_FALSE(four.equal(one));

   EXPECT_TRUE(one.notEqual(two));
   EXPECT_TRUE(two.notEqual(one));
   EXPECT_FALSE(one.notEqual(one));

   EXPECT_FALSE(one.sort(two));
   EXPECT_TRUE(two.sort(one));
   EXPECT_FALSE(one.sort(one));

   EXPECT_FALSE(one.sort(three));
   EXPECT_TRUE(three.sort(one));

   EXPECT_FALSE(one.sort(seven));
   EXPECT_TRUE(seven.sort(one));

   EXPECT_TRUE(one.equal(dynamic_cast<Backwards::Types::ValueType&>(one)));
   EXPECT_FALSE(one.notEqual(dynamic_cast<Backwards::Types::ValueType&>(one)));
   EXPECT_FALSE(one.sort(dynamic_cast<Backwards::Types::ValueType&>(one)));

   Backwards::Types::DoubleValue v1;
   Backwards::Types::StringValue v2;
   Backwards::Types::ArrayValue v3;
   Backwards::Types::FunctionValue v4;

   EXPECT_TRUE(one.sort(v1));
   EXPECT_TRUE(one.sort(v2));
   EXPECT_TRUE(one.sort(v3));
   EXPECT_FALSE(one.sort(v4));

   EXPECT_THROW(two.power(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.greater(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.less(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.geq(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(two.leq(dynamic_cast<Backwards::Types::ValueType&>(one)), Backwards::Types::TypedOperationException);

   EXPECT_NE(0U, one.hash());

   Backwards::Types::ArrayValue six;
   six.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(4.0));

   temp = one.add(six);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ(6.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.sub(six);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.mul(six);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ(8.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);

   temp = one.div(six);
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*temp.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first.get())));
   EXPECT_EQ("A", std::dynamic_pointer_cast<Backwards::Types::StringValue>(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->first)->value);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second.get())));
   temp = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(temp)->value.begin()->second;
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value.size());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0].get())));
   EXPECT_EQ(2.0, std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(temp)->value[0])->value);
 }

TEST(TypesTests, testBadOperations)
 {
   Backwards::Types::DoubleValue v1;
   Backwards::Types::StringValue v2;
   Backwards::Types::ArrayValue v3;
   Backwards::Types::DictionaryValue v4;
   Backwards::Types::FunctionValue v5;

   EXPECT_THROW(v1.add(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.add(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.add(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.add(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.add(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.add(v2), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.sub(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.sub(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.sub(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.sub(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.sub(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.sub(v2), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.mul(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.mul(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.mul(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.mul(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.mul(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.mul(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.mul(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.mul(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.div(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.div(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.div(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.div(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.div(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.div(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.div(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.div(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.power(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.power(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.power(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.power(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.power(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.power(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.power(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.power(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.power(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.power(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.power(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.power(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.power(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.power(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.power(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.power(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.power(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.power(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.power(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.power(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.equal(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.equal(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.equal(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.equal(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.equal(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.equal(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.equal(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.equal(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.equal(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.equal(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.equal(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.equal(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.equal(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.equal(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.equal(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.equal(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.equal(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.equal(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.equal(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.equal(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.notEqual(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.notEqual(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.notEqual(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.notEqual(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.notEqual(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.notEqual(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.notEqual(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.notEqual(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.notEqual(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.notEqual(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.notEqual(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.notEqual(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.notEqual(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.notEqual(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.notEqual(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.notEqual(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.notEqual(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.notEqual(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.notEqual(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.notEqual(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.less(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.less(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.less(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.less(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.less(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.less(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.less(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.less(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.less(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.less(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.less(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.less(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.less(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.less(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.less(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.less(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.less(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.less(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.less(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.less(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.greater(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.greater(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.greater(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.greater(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.greater(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.greater(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.greater(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.greater(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.greater(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.greater(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.greater(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.greater(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.greater(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.greater(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.greater(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.greater(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.greater(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.greater(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.greater(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.greater(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.leq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.leq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.leq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.leq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.leq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.leq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.leq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.leq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.leq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.leq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.leq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.leq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.leq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.leq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.leq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.leq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.leq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.leq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.leq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.leq(v4), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v1.geq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.geq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.geq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v1.geq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v2.geq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.geq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.geq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v2.geq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v3.geq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.geq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.geq(v4), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v3.geq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v4.geq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.geq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.geq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v4.geq(v5), Backwards::Types::TypedOperationException);

   EXPECT_THROW(v5.geq(v1), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.geq(v2), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.geq(v3), Backwards::Types::TypedOperationException);
   EXPECT_THROW(v5.geq(v4), Backwards::Types::TypedOperationException);

   try
    {
      (void) v1.add(v2);
      FAIL() << "Didn't throw exception.";
    }
   catch(Backwards::Types::TypedOperationException& e)
    {
      EXPECT_STREQ("Error adding Double to String", e.what());
    }
 }

TEST(TypesTests, checkCertainCase) // Make sure iteration isn't broken for sorting.
 {
   Backwards::Types::ArrayValue one;
   Backwards::Types::ArrayValue two;

   one.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(2.0));
   one.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(4.0));

   two.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(2.0));
   two.value.emplace_back(std::make_shared<Backwards::Types::DoubleValue>(6.0));

   EXPECT_FALSE(one.sort(two));
   EXPECT_TRUE(two.sort(one));

   Backwards::Types::DictionaryValue tree;
   Backwards::Types::DictionaryValue four;

   tree.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(2.0)));
   tree.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("B"), std::make_shared<Backwards::Types::DoubleValue>(4.0)));

   four.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("A"), std::make_shared<Backwards::Types::DoubleValue>(2.0)));
   four.value.insert(std::make_pair(std::make_shared<Backwards::Types::StringValue>("B"), std::make_shared<Backwards::Types::DoubleValue>(6.0)));

   EXPECT_FALSE(tree.sort(four));
   EXPECT_TRUE(four.sort(tree));
 }
